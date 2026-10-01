//
// Created by ir0n1c on 9/27/2026.
//

#include "events/handler.h"

#include "granny/granny_teleport.h"
#include "granny/item_spawn.h"
#include "granny/player_teleport.h"
#include "granny/unityengine/structs.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>

typedef struct EV_DATA {
	EVENTS event;
	uint8_t args[EVENT_ARG_BUF_SIZE];
} EVENT_DATA;

static SRWLOCK event_lock = SRWLOCK_INIT;
static EVENT_DATA event_queue[EVENT_QUEUE_SIZE];
static size_t event_head = 0;
static size_t event_count = 0;

bool queue_new_event(EVENTS event,
					 const void *args,
					 size_t args_size) {
	if (args_size > EVENT_ARG_BUF_SIZE || (args_size > 0 && args == NULL))
		return false;

	AcquireSRWLockExclusive(&event_lock);

	bool queued = event_count < EVENT_QUEUE_SIZE;
	if (queued) {
		EVENT_DATA *ev = &event_queue[(event_head + event_count) % EVENT_QUEUE_SIZE];
		ev->event = event;

		memset(ev->args, 0, sizeof(ev->args));
		if (args_size > 0)
			memcpy(ev->args, args, args_size);

		event_count++;
	}

	ReleaseSRWLockExclusive(&event_lock);
	return queued;
}

static bool take_event(EVENT_DATA *out) {
	bool taken = false;

	AcquireSRWLockExclusive(&event_lock);
	if (event_count > 0) {
		*out = event_queue[event_head];
		event_head = (event_head + 1) % EVENT_QUEUE_SIZE;
		event_count--;
		taken = true;
	}
	ReleaseSRWLockExclusive(&event_lock);

	return taken;
}

static bool is_zero_pos(const UnityEngine_Vector3_o *pos) {
	return pos->fields.x == 0.0f && pos->fields.y == 0.0f && pos->fields.z == 0.0f;
}

static void teleport_to_positions(const uint8_t *args) {
	UnityEngine_Vector3_o pos[2];
	memcpy(pos, args, sizeof(pos));

	if (!is_zero_pos(&pos[0]))
		teleport_granny_to_position(&pos[0]);
	if (!is_zero_pos(&pos[1]))
		teleport_player_to_position(&pos[1]);
}

void handle_events(void) {
	EVENT_DATA ev;
	while (take_event(&ev)) {
		switch (ev.event) {
			// args are granny pos then player pos, a pos of 0,0,0 is skipped
			case ALL_SET_POS: {
				teleport_to_positions(ev.args);
				break;
			}

			case ITEM_SPAWN_AT: {
				item_spawn_args args;
				memcpy(&args, ev.args, sizeof(args));
				spawn_item_at(args.item, &args.pos);
				break;
			}

			case ITEMS_SPAWN_SELECTED: {
				spawn_selected_items();
				break;
			}

			case APPLY_SETUP: {
				teleport_to_positions(ev.args);
				spawn_selected_items();
				break;
			}

			case ITEM_LOCATIONS_REFRESH: {
				item_locations_refresh();
				break;
			}
		}
	}
}
