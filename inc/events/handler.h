//
// Created by ir0n1c on 9/27/2026.
//

#ifndef GRANNYPRACTICETOOL_HANDLER_H
#define GRANNYPRACTICETOOL_HANDLER_H

#include <stdbool.h>
#include <stddef.h>

typedef enum EVENTS {
	GRANNY_SET_POS = 0x00,
	PLAYER_SET_POS = 0x01,
	ALL_SET_POS = 0x02,
} EVENTS;

typedef enum EVENT_TYPE {
	GRANNY = 0x00,
	MAIN_THREAD = 0x01,
} EVENT_TYPE;

#define EVENT_ARG_BUF_SIZE 32

#ifdef __cplusplus
extern "C" {
#endif

bool queue_new_event(EVENTS event,
					 EVENT_TYPE e_type,
					 const void *args,
					 size_t args_size);
void granny_handle_events();
void main_tr_handle_events();

#ifdef __cplusplus
}
#endif

#endif // GRANNYPRACTICETOOL_HANDLER_H
