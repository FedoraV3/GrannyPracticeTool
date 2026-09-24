/*
 * runtime_constants.h
 *
 *  Created on: Sep 20, 2026
 *      Author: ir0n1c
 */

#ifndef INC_RUNTIME_CONSTANTS_H_
#define INC_RUNTIME_CONSTANTS_H_

#include <inttypes.h>
#include <stdbool.h>

/*
	public AnimationClip Idle; // 0x30
	public AnimationClip WalkAnim; // 0x38
	public AnimationClip JumpscarePlayer; // 0x40
	public AnimationClip UnderBed; // 0x48
	public AnimationClip Tranquilized; // 0x50
	public AnimationClip Search; // 0x58
	public AnimationClip Pepper; // 0x60
	public AnimationClip Frozen; // 0x68
*/

/* crude way to track state */
/* i dont know if this is reliable but if i imagine the assembly, well its better */
extern bool granny_ai_animations_resolved;

#define idle_animation_offset 0x30
#define walk_anim_animation_offset 0x38
#define jumpscare_player_animation_offset 0x40
#define under_bed_animation_offset 0x48
#define tranquilized_animation_offset 0x50
#define search_animation_offset 0x58
#define pepper_animation_offset 0x60
#define frozen_animation_offset 0x68

// offsets
#define GAMEOVERS_GAMEOVERSTART 0x2197D0
#define GRANNY_AI_START 0x1C2090
#define GRANNY_AI_FIXED_UPDATE_RVA 0x1bde60
#define UNITYENGINE_THIS_GET_NAME 0x728990
#define UNITYENGINE_SCENE_MANAGEMENT_SCENEMANAGER_LOADSCENE 0x7436d0
#define IL2CPP_THREAD_ATTACH 0xB0940
#define IL2CPP_THREAD_DETACH 0xB0960
#define IL2CPP_DOMAIN_GET 0xAFC00

// func prototypes
typedef int64_t (__fastcall *AI_Granny_Start)(void* granny_ai_ptr);

extern void* idle_animation;
extern void* walk_anim_animation;
extern void* jumpscare_player_animation;
extern void* under_bed_animation;
extern void* tranquilized_animation;
extern void* search_animation;
extern void* pepper_animation;
extern void* frozen_animation;

int resolve_granny_ai_animation_addresses(void* granny_ai_addr);

#endif /* INC_RUNTIME_CONSTANTS_H_ */
