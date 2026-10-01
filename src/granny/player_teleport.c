//
// Created by ir0n1c on 9/29/2026.
//

#include "granny/player_teleport.h"

#include "core/core.h"
#include "runtime_constants.h"
#include "granny/unityengine/typedefs.h"
#include "granny/unityengine/ue_object.h"

#include <Windows.h>

static uint32_t player_controller_handle = 0;

static SRWLOCK player_pos_lock = SRWLOCK_INIT;
static UnityEngine_Vector3_o player_pos;
static bool has_player_pos = false;

static void* get_live_controller(void) {
	return ue_handle_alive_target(player_controller_handle);
}

static void* get_controller_transform(void* controller) {
	return ((UnityEngine_Component_Get_Transform)(game_assembly_base + UNITYENGINE_COMPONENT_GET_TRANSFORM))(controller, NULL);
}

void set_player_controller(void* character_controller) {
	ue_handle_set(&player_controller_handle, character_controller);

	if (character_controller == NULL) {
		AcquireSRWLockExclusive(&player_pos_lock);
		has_player_pos = false;
		ReleaseSRWLockExclusive(&player_pos_lock);
	}
}

void update_player_position(void) {
	void* controller = get_live_controller();
	if (controller == NULL)
		return;

	void* transform = get_controller_transform(controller);
	if (transform == NULL)
		return;

	UnityEngine_Vector3_o pos;
	((UnityEngine_Transform_Get_Position)(game_assembly_base + UNITYENGINE_TRANSFORM_GET_POSITION))(&pos, transform, NULL);

	AcquireSRWLockExclusive(&player_pos_lock);
	player_pos = pos;
	has_player_pos = true;
	ReleaseSRWLockExclusive(&player_pos_lock);
}

// safe to call
bool get_player_position(UnityEngine_Vector3_o *out_pos) {
	AcquireSRWLockShared(&player_pos_lock);
	bool ok = has_player_pos;
	if (ok)
		*out_pos = player_pos;
	ReleaseSRWLockShared(&player_pos_lock);

	return ok;
}

void teleport_player_to_position(UnityEngine_Vector3_o *pos) {
	void* controller = get_live_controller();
	if (controller == NULL)
		return;

	void* transform = get_controller_transform(controller);
	if (transform == NULL)
		return;

	((UnityEngine_Collider_Set_Enabled)(game_assembly_base + UNITYENGINE_COLLIDER_SET_ENABLED))(controller, false, NULL);
	((UnityEngine_Transform_Set_Position)(game_assembly_base + UNITYENGINE_TRANSFOM_SET_POSITION))(transform, pos, NULL);
	((UnityEngine_Collider_Set_Enabled)(game_assembly_base + UNITYENGINE_COLLIDER_SET_ENABLED))(controller, true, NULL);
}
