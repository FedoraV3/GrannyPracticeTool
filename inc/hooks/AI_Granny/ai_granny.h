#ifndef AI_GRANNY_H
#define AI_GRANNY_H

#include "granny/unityengine/structs.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct granny_ai_info {
	void* granny_ai_ptr;
	uint32_t granny_ai_handle;
	UnityEngine_Vector3_o transform_pos;
} granny_ai_info;

#ifdef __cplusplus
extern "C" {
#endif

extern granny_ai_info *curr_granny_ai;

bool ai_granny_hook_install();
void invalidate_ai_granny_ptr();
void* get_ai_granny(void);

#ifdef __cplusplus
}
#endif

#endif
