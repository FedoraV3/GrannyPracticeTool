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

UnityEngine_Vector3_o last_granny_teleport = {0};

// any unity function is required to be called on the main thread of unity game!
void get_granny_position(UnityEngine_Vector3_o *out_pos) {
	// yeah we have snapshot of the last granny update instance so lets just
	// deep copy because the position will be changed in the next update
	out_pos->fields.x = curr_granny_ai->transform_pos.fields.x;
	out_pos->fields.y = curr_granny_ai->transform_pos.fields.y;
	out_pos->fields.z = curr_granny_ai->transform_pos.fields.z;
}

void teleport_granny_to_position(UnityEngine_Vector3_o *pos) {
	void* granny_ai = get_ai_granny_transform();
	if (granny_ai == NULL) {
		return;
	}

	get_granny_position(&last_granny_teleport);
	((UnityEngine_Transform_Set_Position)(game_assembly_base + UNITYENGINE_TRANSFOM_SET_POSITION))(granny_ai, pos, NULL);
}
