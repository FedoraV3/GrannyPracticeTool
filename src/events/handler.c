//
// Created by ir0n1c on 9/27/2026.
//

#include "events/handler.h"

#include "granny/granny_teleport.h"
#include "granny/unityengine/structs.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>

typedef struct EV_DATA {
	EVENTS event;
	EVENT_TYPE e_type;
	uint8_t args[EVENT_ARG_BUF_SIZE];
} EVENT_DATA;

static SRWLOCK event_lock = SRWLOCK_INIT;
static EVENT_DATA pending_event;
static bool has_pending_event = false;

bool queue_new_event(EVENTS event,
					 EVENT_TYPE e_type,
					 const void *args,
					 size_t args_size) {
	if (args_size > EVENT_ARG_BUF_SIZE || (args_size > 0 && args == NULL))
		return false;

	AcquireSRWLockExclusive(&event_lock);

	pending_event.event = event;
	pending_event.e_type = e_type;

	memset(pending_event.args, 0, sizeof(pending_event.args));
	if (args_size > 0)
		memcpy(pending_event.args, args, args_size);

	has_pending_event = true;

	ReleaseSRWLockExclusive(&event_lock);
	return true;
}

static bool take_event(EVENT_TYPE e_type, EVENT_DATA *out) {
	bool taken = false;

	AcquireSRWLockExclusive(&event_lock);
	if (has_pending_event && pending_event.e_type == e_type) {
		*out = pending_event;
		has_pending_event = false;
		taken = true;
	}
	ReleaseSRWLockExclusive(&event_lock);

	return taken;
}

// make sure this only runs on fixedupdate ai_granny! or else......... instant dereference dangling ptr
void granny_handle_events() {
	EVENT_DATA ev;
	if (!take_event(GRANNY, &ev))
		return;

	switch (ev.event) {
		case GRANNY_SET_POS: {
			UnityEngine_Vector3_o pos;
			memcpy(&pos, ev.args, sizeof(pos));
			teleport_granny_to_position(&pos);
			break;
		}

		case PLAYER_SET_POS: {
			// TODO: add this
			break;
		}
	}
}

void main_tr_handle_events() {
	EVENT_DATA ev;
	if (!take_event(MAIN_THREAD, &ev))
		return;
}
