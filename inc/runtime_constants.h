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
#define UNITYENGINE_GAMEOBJECT_GET_TRANSFORM 0x722220
#define UNITYENGINE_COMPONENT_GET_GAMEOBJECT 0x71f0c0
#define UNITYENGINE_COMPONENT_GET_TRANSFORM 0x71f170
// these are the real get/set_position, not the *_Injected icall thunks right above them
// (0x748ee0 / 0x7496f0) which take (transform, vec*) with no return buffer
#define UNITYENGINE_TRANSFORM_GET_POSITION 0x748f30
#define UNITYENGINE_TRANSFOM_SET_POSITION 0x749740
#define UNITYENGINE_SCENE_MANAGEMENT_SCENEMANAGER_LOADSCENE 0x7436d0
#define IL2CPP_THREAD_ATTACH 0xB0940
#define IL2CPP_THREAD_DETACH 0xB0960
#define IL2CPP_DOMAIN_GET 0xAFC00
#define IL2CPP_STRING_NEW 0xb0910

// dont know why they put the player position in ai granny
#define AI_GRANNY_AGENT 0xA8

#define UNITYENGINE_BEHAVIOUR_GET_ENABLED 0x71e990
#define UNITYENGINE_BEHAVIOUR_SET_ENABLED 0x71ea10
#define UNITYENGINE_COLLIDER_SET_ENABLED 0x76b200
#define UNITYENGINE_NAVMESHAGENT_WARP 0x6e7f00
#define UNITYENGINE_NAVMESH_SAMPLE_POSITION 0x6e9930
#define UNITYENGINE_OBJECT_OP_IMPLICIT 0x728b10
#define UNITYENGINE_OBJECT_DESTROY 0x727830

#define MOBILE_FPS_UPDATE 0x22f250
#define MOBILE_FPS_CHARACTER_CONTROLLER 0x28

#define SEED_MANAGER_GENERATE_PLACEMENT 0x251000
#define OBJECTS_MANAGER_START 0x230040
#define ITEM_SEED_DATA_ITEM_NAME 0x20
#define UNITYENGINE_OBJECT_FIND_OBJECTS_OF_TYPE 0x727c10

#define IL2CPP_DOMAIN_ASSEMBLY_OPEN 0xafbf0
#define IL2CPP_ASSEMBLY_GET_IMAGE 0xaf820
#define IL2CPP_CLASS_FROM_NAME 0xaf890
#define IL2CPP_CLASS_GET_TYPE 0xafa00
#define IL2CPP_TYPE_GET_OBJECT 0xb0b60
#define IL2CPP_GCHANDLE_NEW 0xaffd0
#define IL2CPP_GCHANDLE_GET_TARGET 0xaffc0
#define IL2CPP_GCHANDLE_FREE 0xaffb0
#define IL2CPP_LIST_ITEMS 0x10
#define IL2CPP_LIST_SIZE 0x18

#define PICKRAY_UPDATE 0x239570
#define PICKRAY_INVENTORY 0x4e8
#define INVENTORY_ITEMS_PREFAB 0x30
#define INVENTORY_ITEM_DEFS 0x20
#define ITEM_DEFS_ITEM_NAME 0x10
#define ITEM_DEFS_TEXT_HIGHLIGHTED 0x28
#define ITEM_DEFS_ITEM_INT 0x30

#define UNITYENGINE_OBJECT_INSTANTIATE 0x2cffc0
#define UNITYENGINE_GAMEOBJECT_GET_COMPONENT 0x2b9a00
#define METHOD_GAMEOBJECT_GET_COMPONENT_ITEMSPAWN 0xc36b80
#define METHOD_OBJECT_INSTANTIATE_GAMEOBJECT 0xc3e768
#define IL2CPP_INIT_RUNTIME_METADATA 0x138670

#define IL2CPP_ARRAY_MAX_LENGTH 0x18
#define IL2CPP_ARRAY_ITEMS 0x20

#define ITEM_CROSSBOW 1
#define ITEM_PLIER 2
#define ITEM_BATTERY 3
#define ITEM_GAS 4
#define ITEM_SEED 5
#define ITEM_BOOK 6
#define ITEM_WINCH 7
#define ITEM_CARBATTERY 8
#define ITEM_CARKEY 9
#define ITEM_CUTTER 10
#define ITEM_CODE 11
#define ITEM_BATON 12
#define ITEM_ECKEY 13
#define ITEM_HAMMER 14
#define ITEM_PADLOCK 15
#define ITEM_MAS 16
#define ITEM_KUGG1 17
#define ITEM_KUGG2 18
#define ITEM_MEAT 19
#define ITEM_MELON 20
#define ITEM_SPRAY 21
#define ITEM_PLANK 22
#define ITEM_PLAYH 23
#define ITEM_REMOTE 24
#define ITEM_DATA 25
#define ITEM_RUSTY 26
#define ITEM_SAFE 27
#define ITEM_SCREW 28
#define ITEM_SHOTGUN 29
#define ITEM_SHOTGUN2 30
#define ITEM_SP1 31
#define ITEM_SP2 32
#define ITEM_SP3 33
#define ITEM_SPARK 34
#define ITEM_SPECIAL 35
#define ITEM_SPIDER 36
#define ITEM_SYRINGE 37
#define ITEM_T1 38
#define ITEM_T2 39
#define ITEM_T3 40
#define ITEM_T4 41
#define ITEM_TEDDY 42
#define ITEM_TEXT 43
#define ITEM_TOPP 44
#define ITEM_WP 45
#define ITEM_VAS 46
#define ITEM_VAS2 47
#define ITEM_VAS3 48
#define ITEM_WHEEL 49
#define ITEM_STICK 50
#define ITEM_WRENCH 51
#define ITEM_RAT 52
#define ITEM_ORNAMENTFREEZE 53
#define ITEM_ORNAMENTBOMB 54
#define ITEM_FUSE 55
#define ITEM_FIRST ITEM_CROSSBOW
#define ITEM_LAST ITEM_FUSE

#ifdef __cplusplus
extern "C" {
#endif

// func prototypes
typedef void (__fastcall *AI_Granny_Start)(void *granny_ai_ptr, const void* method);
typedef void (__fastcall *AI_Granny_FixedUpdate)(void *granny_ai_ptr, const void* method);


extern const char* const item_names[ITEM_LAST + 1];
extern const uint16_t itemspawn_item_offsets[ITEM_LAST + 1];

extern void* idle_animation;
extern void* walk_anim_animation;
extern void* jumpscare_player_animation;
extern void* under_bed_animation;
extern void* tranquilized_animation;
extern void* search_animation;
extern void* pepper_animation;
extern void* frozen_animation;

int resolve_granny_ai_animation_addresses(void* granny_ai_addr);

#ifdef __cplusplus
}
#endif

#endif /* INC_RUNTIME_CONSTANTS_H_ */
