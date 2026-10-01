/*
 * runtime_constants.c
 *
 *  Created on: Sep 20, 2026
 *      Author: ir0n1c
 */

#include "runtime_constants.h"
#include "granny/unityengine/structs.h"

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

const char* const item_names[ITEM_LAST + 1] = {
	[0] = NULL,
	[ITEM_CROSSBOW] = "crossbow",
	[ITEM_PLIER] = "plier",
	[ITEM_BATTERY] = "battery",
	[ITEM_GAS] = "gas",
	[ITEM_SEED] = "seed",
	[ITEM_BOOK] = "book",
	[ITEM_WINCH] = "winch",
	[ITEM_CARBATTERY] = "carbattery",
	[ITEM_CARKEY] = "carkey",
	[ITEM_CUTTER] = "cutter",
	[ITEM_CODE] = "code",
	[ITEM_BATON] = "baton",
	[ITEM_ECKEY] = "eckey",
	[ITEM_HAMMER] = "hammer",
	[ITEM_PADLOCK] = "padlock",
	[ITEM_MAS] = "mas",
	[ITEM_KUGG1] = "kugg1",
	[ITEM_KUGG2] = "kugg2",
	[ITEM_MEAT] = "meat",
	[ITEM_MELON] = "melon",
	[ITEM_SPRAY] = "spray",
	[ITEM_PLANK] = "plank",
	[ITEM_PLAYH] = "playh",
	[ITEM_REMOTE] = "remote",
	[ITEM_DATA] = "data",
	[ITEM_RUSTY] = "rusty",
	[ITEM_SAFE] = "safe",
	[ITEM_SCREW] = "screw",
	[ITEM_SHOTGUN] = "shotgun",
	[ITEM_SHOTGUN2] = "shotgun2",
	[ITEM_SP1] = "sp1",
	[ITEM_SP2] = "sp2",
	[ITEM_SP3] = "sp3",
	[ITEM_SPARK] = "spark",
	[ITEM_SPECIAL] = "special",
	[ITEM_SPIDER] = "spider",
	[ITEM_SYRINGE] = "syringe",
	[ITEM_T1] = "t1",
	[ITEM_T2] = "t2",
	[ITEM_T3] = "t3",
	[ITEM_T4] = "t4",
	[ITEM_TEDDY] = "teddy",
	[ITEM_TEXT] = "text",
	[ITEM_TOPP] = "topp",
	[ITEM_WP] = "wp",
	[ITEM_VAS] = "vas",
	[ITEM_VAS2] = "vas2",
	[ITEM_VAS3] = "vas3",
	[ITEM_WHEEL] = "wheel",
	[ITEM_STICK] = "stick",
	[ITEM_WRENCH] = "wrench",
	[ITEM_RAT] = "rat",
	[ITEM_ORNAMENTFREEZE] = "ornamentfreeze",
	[ITEM_ORNAMENTBOMB] = "ornamentbomb",
	[ITEM_FUSE] = "fuse",
};

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

const uint16_t itemspawn_item_offsets[ITEM_LAST + 1] = {
	[0] = 0,
	[ITEM_CROSSBOW] = offsetof(ItemSpawn_o, fields.crossbow),
	[ITEM_PLIER] = offsetof(ItemSpawn_o, fields.plier),
	[ITEM_BATTERY] = offsetof(ItemSpawn_o, fields.battery),
	[ITEM_GAS] = offsetof(ItemSpawn_o, fields.gas),
	[ITEM_SEED] = offsetof(ItemSpawn_o, fields.seed),
	[ITEM_BOOK] = offsetof(ItemSpawn_o, fields.book),
	[ITEM_WINCH] = offsetof(ItemSpawn_o, fields.winch),
	[ITEM_CARBATTERY] = offsetof(ItemSpawn_o, fields.carbattery),
	[ITEM_CARKEY] = offsetof(ItemSpawn_o, fields.carkey),
	[ITEM_CUTTER] = offsetof(ItemSpawn_o, fields.cutter),
	[ITEM_CODE] = offsetof(ItemSpawn_o, fields.code),
	[ITEM_BATON] = offsetof(ItemSpawn_o, fields.baton),
	[ITEM_ECKEY] = offsetof(ItemSpawn_o, fields.eckey),
	[ITEM_HAMMER] = offsetof(ItemSpawn_o, fields.hammer),
	[ITEM_PADLOCK] = offsetof(ItemSpawn_o, fields.padlock),
	[ITEM_MAS] = offsetof(ItemSpawn_o, fields.mas),
	[ITEM_KUGG1] = offsetof(ItemSpawn_o, fields.kugg1),
	[ITEM_KUGG2] = offsetof(ItemSpawn_o, fields.kugg2),
	[ITEM_MEAT] = offsetof(ItemSpawn_o, fields.meat),
	[ITEM_MELON] = offsetof(ItemSpawn_o, fields.melon),
	[ITEM_SPRAY] = offsetof(ItemSpawn_o, fields.spray),
	[ITEM_PLANK] = offsetof(ItemSpawn_o, fields.plank),
	[ITEM_PLAYH] = offsetof(ItemSpawn_o, fields.playh),
	[ITEM_REMOTE] = offsetof(ItemSpawn_o, fields.remote),
	[ITEM_DATA] = offsetof(ItemSpawn_o, fields.data),
	[ITEM_RUSTY] = offsetof(ItemSpawn_o, fields.rusty),
	[ITEM_SAFE] = offsetof(ItemSpawn_o, fields.safe),
	[ITEM_SCREW] = offsetof(ItemSpawn_o, fields.screw),
	[ITEM_SHOTGUN] = offsetof(ItemSpawn_o, fields.shotgun),
	[ITEM_SHOTGUN2] = offsetof(ItemSpawn_o, fields.shotgun2),
	[ITEM_SP1] = offsetof(ItemSpawn_o, fields.sp1),
	[ITEM_SP2] = offsetof(ItemSpawn_o, fields.sp2),
	[ITEM_SP3] = offsetof(ItemSpawn_o, fields.sp3),
	[ITEM_SPARK] = offsetof(ItemSpawn_o, fields.spark),
	[ITEM_SPECIAL] = offsetof(ItemSpawn_o, fields.special),
	[ITEM_SPIDER] = offsetof(ItemSpawn_o, fields.spider),
	[ITEM_SYRINGE] = offsetof(ItemSpawn_o, fields.syringe),
	[ITEM_T1] = offsetof(ItemSpawn_o, fields.t1),
	[ITEM_T2] = offsetof(ItemSpawn_o, fields.t2),
	[ITEM_T3] = offsetof(ItemSpawn_o, fields.t3),
	[ITEM_T4] = offsetof(ItemSpawn_o, fields.t4),
	[ITEM_TEDDY] = offsetof(ItemSpawn_o, fields.teddy),
	[ITEM_TEXT] = offsetof(ItemSpawn_o, fields.text),
	[ITEM_TOPP] = offsetof(ItemSpawn_o, fields.topp),
	[ITEM_WP] = offsetof(ItemSpawn_o, fields.wp),
	[ITEM_VAS] = offsetof(ItemSpawn_o, fields.vas),
	[ITEM_VAS2] = offsetof(ItemSpawn_o, fields.vas2),
	[ITEM_VAS3] = offsetof(ItemSpawn_o, fields.vas3),
	[ITEM_WHEEL] = offsetof(ItemSpawn_o, fields.wheel),
	[ITEM_STICK] = offsetof(ItemSpawn_o, fields.stick),
	[ITEM_WRENCH] = offsetof(ItemSpawn_o, fields.wrench),
	[ITEM_RAT] = offsetof(ItemSpawn_o, fields.rat),
	[ITEM_ORNAMENTFREEZE] = offsetof(ItemSpawn_o, fields.ornamentfreeze),
	[ITEM_ORNAMENTBOMB] = offsetof(ItemSpawn_o, fields.ornamentbomb),
	[ITEM_FUSE] = offsetof(ItemSpawn_o, fields.fuse),
};
