//
// Created by ir0n1c on 9/27/2026.
//

#ifndef GRANNYPRACTICETOOL_HANDLER_H
#define GRANNYPRACTICETOOL_HANDLER_H

#include <stdbool.h>
#include <stddef.h>

typedef enum EVENTS {
	ALL_SET_POS = 0x00,
	ITEM_SPAWN_AT = 0x01,
	ITEMS_SPAWN_SELECTED = 0x02,
	ITEM_LOCATIONS_REFRESH = 0x03,
	APPLY_SETUP = 0x04,
} EVENTS;

#define EVENT_ARG_BUF_SIZE 32
#define EVENT_QUEUE_SIZE 16

#ifdef __cplusplus
extern "C" {
#endif

bool queue_new_event(EVENTS event,
					 const void *args,
					 size_t args_size);
void handle_events(void);

#ifdef __cplusplus
}
#endif

#endif // GRANNYPRACTICETOOL_HANDLER_H
