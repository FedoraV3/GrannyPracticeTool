/*
 * granny_teleport.c
 *
 *  Created on: Sep 25, 2026
 *      Author: ir0n1c
 */

#include "granny/granny_teleport.h"
#include "core/core.h"
#include "granny/unityengine/structs.h"
#include "granny/unityengine/typedefs.h"
#include "hooks/AI_Granny/ai_granny.h"
#include "runtime_constants.h"
#include <stddef.h>

UnityEngine_Vector3_o last_granny_teleport = {0};

bool get_granny_position(UnityEngine_Vector3_o *out_pos) {
	void* granny_transform = get_ai_granny_transform();
	if (granny_transform == NULL || out_pos == NULL) {
		return false;
	}

	// get_position returns a 12 byte Vector3 by value, so the game writes it into a buffer
	// the caller owns (hidden return pointer in rcx), it has to point at real memory
//	typedef UnityEngine_Vector3_o* (__fastcall *UnityEngine_Transform_Get_Position)(UnityEngine_Vector3_o *vec_buf, void* transform, void* method);
	((UnityEngine_Transform_Get_Position)(game_assembly_base + UNITYENGINE_TRANSFORM_GET_POSITION))(out_pos, granny_transform, NULL);

	return true;
}

void teleport_granny_to_position(UnityEngine_Vector3_o *pos) {
	void* granny_ai = get_ai_granny_transform();
	if (granny_ai == NULL) {
		return;
	}
	
	((UnityEngine_Transform_Set_Position)(game_assembly_base + UNITYENGINE_TRANSFOM_SET_POSITION))(granny_ai, pos, NULL);
}