#include "hooks/AI_Granny/ai_granny.h"
#include <stdlib.h>
#include "MinHook.h"
#include "granny/core/core.h"
#include "runtime_constants.h"

typedef void (*AI_Granny_FixedUpdate)(void *granny_ai_ptr);

AI_Granny_Start orig_ai_granny_start = NULL;
AI_Granny_FixedUpdate orig_fixed_update = NULL;
void* granny_ai_ptr = NULL;

// detour function for ai granny
// looking back at this i think intercepted is better because i literally named it intercepted
static void intercepted_ai_granny_fixed_update(void *current_granny_ai_ptr) {
	if (granny_ai_ptr != current_granny_ai_ptr)
		granny_ai_ptr = current_granny_ai_ptr;

	// good time to call the resolve_granny_ai_addresses() function
	if (!granny_ai_animations_resolved) {
		resolve_granny_ai_animation_addresses(current_granny_ai_ptr);
	}
	
	return orig_fixed_update(current_granny_ai_ptr);
}

// right here i think making it so that only one function accesses granny_ai_ptr is much
// more safer so i just made this function
// 0x0 if not found
void* get_ai_granny_ptr() {
	return granny_ai_ptr;
}

bool ai_granny_hook_install() {
	void* granny_ai_fixed_update = (game_assembly_base + GRANNY_AI_FIXED_UPDATE_RVA);
	if (MH_CreateHook((LPVOID)granny_ai_fixed_update, (LPVOID)intercepted_ai_granny_fixed_update, (LPVOID*)&orig_fixed_update) != MH_OK)
		return false;
	
	if (MH_EnableHook((LPVOID)granny_ai_fixed_update) != MH_OK)
		return false;
	
	
	return true;
}