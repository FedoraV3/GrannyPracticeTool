//
// Created by ir0n1c on 9/29/2026.
//

#include "config/data.h"
#include "runtime_constants.h"

#include <Windows.h>
#include <string.h>

const char cfg_pwd[] = "59d7724ee0f9585440ce2c26d3d0d68a";

UnityEngine_Vector3_o player_tp_pos = {0};
UnityEngine_Vector3_o granny_tp_pos = {0};

static SRWLOCK item_selection_lock = SRWLOCK_INIT;
static item_selection item_selections[MAX_ITEM_SELECTIONS];
static int item_selection_count = 0;
static volatile LONG selections_version = 0;

static int find_item_selection(const char* location) {
	for (int i = 0; i < item_selection_count; i++) {
		if (strcmp(item_selections[i].location, location) == 0)
			return i;
	}
	return -1;
}

int get_item_selection(const char* location) {
	if (!location)
		return 0;

	AcquireSRWLockShared(&item_selection_lock);
	int i = find_item_selection(location);
	int item = i >= 0 ? item_selections[i].item : 0;
	ReleaseSRWLockShared(&item_selection_lock);

	return item;
}

bool set_item_selection(const char* location, int item) {
	if (!location || !*location || strlen(location) >= ITEM_LOCATION_NAME_SIZE)
		return false;

	if (item < ITEM_FIRST || item > ITEM_LAST)
		item = 0;

	bool ok = true;
	AcquireSRWLockExclusive(&item_selection_lock);

	int i = find_item_selection(location);
	if (item == 0) {
		if (i >= 0)
			item_selections[i] = item_selections[--item_selection_count];
	} else if (i >= 0) {
		item_selections[i].item = item;
	} else if (item_selection_count < MAX_ITEM_SELECTIONS) {
		strcpy_s(item_selections[item_selection_count].location, ITEM_LOCATION_NAME_SIZE, location);
		item_selections[item_selection_count].item = item;
		item_selection_count++;
	} else {
		ok = false;
	}

	if (ok)
		InterlockedIncrement(&selections_version);

	ReleaseSRWLockExclusive(&item_selection_lock);
	return ok;
}

void clear_item_selections(void) {
	AcquireSRWLockExclusive(&item_selection_lock);
	item_selection_count = 0;
	InterlockedIncrement(&selections_version);
	ReleaseSRWLockExclusive(&item_selection_lock);
}

int copy_item_selections(item_selection* out, int max) {
	AcquireSRWLockShared(&item_selection_lock);
	int count = item_selection_count < max ? item_selection_count : max;
	memcpy(out, item_selections, sizeof(item_selection) * count);
	ReleaseSRWLockShared(&item_selection_lock);

	return count;
}

void replace_item_selections(const item_selection* selections, int count) {
	if (count > MAX_ITEM_SELECTIONS)
		count = MAX_ITEM_SELECTIONS;

	AcquireSRWLockExclusive(&item_selection_lock);
	memcpy(item_selections, selections, sizeof(item_selection) * count);
	item_selection_count = count;
	InterlockedIncrement(&selections_version);
	ReleaseSRWLockExclusive(&item_selection_lock);
}

long item_selections_version(void) {
	return InterlockedCompareExchange(&selections_version, 0, 0);
}
