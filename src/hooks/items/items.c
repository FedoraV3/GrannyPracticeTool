#include "hooks/items/items.h"
#include "MinHook.h"
#include "core/core.h"
#include "events/handler.h"
#include "granny/item_spawn.h"
#include "granny/unityengine/typedefs.h"
#include "runtime_constants.h"

ObjectsManager_Start orig_objects_manager_start = NULL;
SeedManager_GeneratePlacement orig_seed_manager_generate_placement = NULL;
PickRay_Update orig_pick_ray_update = NULL;

static void __fastcall intercepted_objects_manager_start(void* objects_manager, void* method) {
	orig_objects_manager_start(objects_manager, method);
	item_locations_mark_dirty();
}

static void __fastcall intercepted_seed_manager_generate_placement(void* seed_manager, void* method) {
	orig_seed_manager_generate_placement(seed_manager, method);
	item_locations_mark_dirty();
}

static void __fastcall intercepted_pick_ray_update(void* pick_ray, void* method) {
	item_spawn_tick(*(void**)((uint8_t*)pick_ray + PICKRAY_INVENTORY));
	handle_events();

	orig_pick_ray_update(pick_ray, method);
}

static bool install_hook(uintptr_t rva, LPVOID detour, LPVOID* original) {
	return MH_CreateHook((LPVOID)(game_assembly_base + rva), detour, original) == MH_OK
		&& MH_EnableHook((LPVOID)(game_assembly_base + rva)) == MH_OK;
}

bool items_hook_install(void) {
	return install_hook(OBJECTS_MANAGER_START, (LPVOID)intercepted_objects_manager_start, (LPVOID*)&orig_objects_manager_start)
		&& install_hook(SEED_MANAGER_GENERATE_PLACEMENT, (LPVOID)intercepted_seed_manager_generate_placement, (LPVOID*)&orig_seed_manager_generate_placement)
		&& install_hook(PICKRAY_UPDATE, (LPVOID)intercepted_pick_ray_update, (LPVOID*)&orig_pick_ray_update);
}
