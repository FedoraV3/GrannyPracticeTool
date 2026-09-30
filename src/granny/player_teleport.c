//
// Created by ir0n1c on 9/29/2026.
//

#include "../../inc/granny/player_teleport.h"

#include "../../inc/hooks/AI_Granny/ai_granny.h"
#include "hooks/AI_Granny/ai_granny.h"
#include "core/core.h"
#include "runtime_constants.h"
#include "granny/unityengine/ue_vector3.h"

// safe to call
void get_player_position(UnityEngine_Vector3_o *out_pos) {
	out_pos->fields.x = curr_granny_ai->player_transform_pos.fields.x;
	out_pos->fields.y = curr_granny_ai->player_transform_pos.fields.y;
	out_pos->fields.z = curr_granny_ai->player_transform_pos.fields.z;
}

void teleport_player_to_position(UnityEngine_Vector3_o *pos) {
	if (curr_granny_ai->player_transform == NULL)
		return;

	((UnityEngine_Transform_Set_Position)(game_assembly_base + UNITYENGINE_TRANSFOM_SET_POSITION))(curr_granny_ai->player_transform, pos, NULL);
}