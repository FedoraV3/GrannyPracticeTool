#include "hooks/AI_Granny/ai_granny.h"
#include "MinHook.h"
#include "core/core.h"
#include "events/handler.h"
#include "granny/granny_teleport.h"
#include "granny/unityengine/typedefs.h"
#include "runtime_constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AI_Granny_Start orig_ai_granny_start = NULL;
AI_Granny_FixedUpdate orig_fixed_update = NULL;

// calloc allocs just enough so that all members can be filled in
granny_ai_info *curr_granny_ai = NULL;

// small function
static void invalidate_curr_granny_ai() {
	memset(curr_granny_ai, 0, sizeof(granny_ai_info));
}

// detour function for ai granny
// looking back at this i think intercepted is better because i literally named it intercepted
static void intercepted_ai_granny_fixed_update(void *current_granny_ai_ptr, const void *method) {
	if (curr_granny_ai->granny_ai_ptr != current_granny_ai_ptr)
		curr_granny_ai->granny_ai_ptr = current_granny_ai_ptr;

	// impossible for current_granny_ai_ptr to be NULL
	// update the curr_granny_ai struct here (no need for ai ptr and ai transform
	// -since that is dealt with already)
	if (curr_granny_ai->granny_ai_transform != NULL)
		get_granny_position(&curr_granny_ai->transform_pos);


	// update granny current state
	granny_handle_events();

	orig_fixed_update(current_granny_ai_ptr, method);
}

static void intercepted_ai_granny_start(void *current_granny_ai_ptr, const void *method) {
	// good time to call the resolve_granny_ai_addresses() function
	if (!granny_ai_animations_resolved) {
		resolve_granny_ai_animation_addresses(current_granny_ai_ptr);
	}
	
	/*
	#define UNITYENGINE_GAMEOBJECT_GET_TRANSFORM 0x722220
	#define UNITYENGINE_COMPONENT_GET_GAMEOBJECT 0x71f0c0
	*/
	
	// get granny's transform
	// get gameobject then get transform
	curr_granny_ai->granny_ai_transform = ((UnityEngine_Component_Get_Transform)(game_assembly_base + UNITYENGINE_COMPONENT_GET_TRANSFORM))(current_granny_ai_ptr, NULL);
	orig_ai_granny_start(current_granny_ai_ptr, method);
}

// right here i think making it so that only one function accesses granny_ai_ptr is much
// more safer so i just made this function
// 0x0 if not found
void* get_ai_granny_ptr() {
	return curr_granny_ai->granny_ai_ptr;
}

void* get_ai_granny_transform() {
	return curr_granny_ai->granny_ai_transform;
}

// encapsulation (no longer)
void invalidate_ai_granny_ptr() {
	// 0 out the entire granny ai struct
	invalidate_curr_granny_ai();
	// questioning whether i should calloc and keep it for the lifetime of the program
	// yeah lets do it

	granny_ai_animations_resolved = false;
}

bool ai_granny_hook_install() {
	curr_granny_ai = calloc(sizeof(granny_ai_info), 1);
	if (curr_granny_ai == NULL)
		return false;

	if (MH_CreateHook((game_assembly_base + GRANNY_AI_FIXED_UPDATE_RVA), (LPVOID)intercepted_ai_granny_fixed_update, (LPVOID*)&orig_fixed_update) != MH_OK)
		return false;
	
	if (MH_CreateHook((game_assembly_base + GRANNY_AI_START), (LPVOID)intercepted_ai_granny_start, (LPVOID*)&orig_ai_granny_start) != MH_OK)
		return false;
	
	if (MH_EnableHook((game_assembly_base + GRANNY_AI_FIXED_UPDATE_RVA)) != MH_OK)
		return false;
	
	if (MH_EnableHook((game_assembly_base + GRANNY_AI_START)) != MH_OK)
		return false;

	return true;
}
