/*
 * runtime_constants.c
 *
 *  Created on: Sep 20, 2026
 *      Author: ir0n1c
 */

#include "runtime_constants.h"

#include <stddef.h>

bool granny_ai_animations_resolved = false;

void* idle_animation = NULL;
void* walk_anim_animation = NULL;
void* jumpscare_player_animation = NULL;
void* under_bed_animation = NULL;
void* tranquilized_animation = NULL;
void* search_animation = NULL;
void* pepper_animation = NULL;
void* frozen_animation = NULL;

int resolve_granny_ai_animation_addresses(void* granny_ai_addr) {
	if (granny_ai_animations_resolved)
		return 1;

	idle_animation = *(void**)((uint8_t*)granny_ai_addr + idle_animation_offset);
	walk_anim_animation = *(void**)((uint8_t*)granny_ai_addr + walk_anim_animation_offset);
	jumpscare_player_animation = *(void**)((uint8_t*)granny_ai_addr + jumpscare_player_animation_offset);
	under_bed_animation = *(void**)((uint8_t*)granny_ai_addr + under_bed_animation_offset);
	tranquilized_animation = *(void**)((uint8_t*)granny_ai_addr + tranquilized_animation_offset);
	search_animation = *(void**)((uint8_t*)granny_ai_addr + search_animation_offset);
	pepper_animation = *(void**)((uint8_t*)granny_ai_addr + pepper_animation_offset);
	frozen_animation = *(void**)((uint8_t*)granny_ai_addr + frozen_animation_offset);

	granny_ai_animations_resolved = true;
	return 1;
}
