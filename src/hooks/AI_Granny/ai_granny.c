#include "hooks/AI_Granny/ai_granny.h"
#include <stdio.h>
#include <stdlib.h>
#include "MinHook.h"
#include "core/core.h"
#include "granny/unityengine/typedefs.h"
#include "runtime_constants.h"

AI_Granny_Start orig_ai_granny_start = NULL;
AI_Granny_FixedUpdate orig_fixed_update = NULL;

void* granny_ai_ptr = NULL;
void* granny_ai_transform = NULL;

// detour function for ai granny
// looking back at this i think intercepted is better because i literally named it intercepted
static void intercepted_ai_granny_fixed_update(void *current_granny_ai_ptr) {
	if (granny_ai_ptr != current_granny_ai_ptr)
	{
		printf("Resolved Granny's AI: 0x%llX\n", (uint64_t)current_granny_ai_ptr);
		granny_ai_ptr = current_granny_ai_ptr;
	}
	
	return orig_fixed_update(current_granny_ai_ptr);
}

static void intercepted_ai_granny_start(void *current_granny_ai_ptr) {
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
	granny_ai_transform = ((UnityEngine_GameObject_Get_Transform)(game_assembly_base + UNITYENGINE_GAMEOBJECT_GET_TRANSFORM))(((UnityEngine_Component_Get_GameObject)(game_assembly_base + UNITYENGINE_COMPONENT_GET_GAMEOBJECT))(current_granny_ai_ptr, NULL), NULL);
	printf("Resolved Granny's Transform: 0x%llX", (uint64_t)granny_ai_transform);
	
	return orig_ai_granny_start(current_granny_ai_ptr);
}

// right here i think making it so that only one function accesses granny_ai_ptr is much
// more safer so i just made this function
// 0x0 if not found
void* get_ai_granny_ptr() {
	return granny_ai_ptr;
}

void* get_ai_granny_transform() {
	return granny_ai_transform;
}

// encapsulation
void invalidate_ai_granny_ptr() {
	granny_ai_ptr = NULL;
	granny_ai_transform = NULL;
}

bool ai_granny_hook_install() {
	void* granny_ai_fixed_update = (game_assembly_base + GRANNY_AI_FIXED_UPDATE_RVA);
	void* granny_ai_start = (game_assembly_base + GRANNY_AI_START);
	if (MH_CreateHook((LPVOID)granny_ai_fixed_update, (LPVOID)intercepted_ai_granny_fixed_update, (LPVOID*)&orig_fixed_update) != MH_OK)
		return false;
	
	if (MH_CreateHook((LPVOID)granny_ai_start, (LPVOID)intercepted_ai_granny_start, (LPVOID*)&orig_ai_granny_start) != MH_OK)
		return false;
	
	if (MH_EnableHook((LPVOID)granny_ai_fixed_update) != MH_OK)
		return false;
	
	if (MH_EnableHook((LPVOID)granny_ai_start) != MH_OK)
		return false;
	
	
	return true;
}