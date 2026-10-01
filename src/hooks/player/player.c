#include "hooks/player/player.h"
#include "MinHook.h"
#include "core/core.h"
#include "events/handler.h"
#include "granny/player_teleport.h"
#include "granny/unityengine/typedefs.h"
#include "runtime_constants.h"

MobileFPS_Update orig_mobile_fps_update = NULL;

static void __fastcall intercepted_mobile_fps_update(void* mobile_fps, void* method) {
	set_player_controller(*(void**)((uint8_t*)mobile_fps + MOBILE_FPS_CHARACTER_CONTROLLER));
	update_player_position();
	handle_events();

	orig_mobile_fps_update(mobile_fps, method);
}

bool player_hook_install(void) {
	return MH_CreateHook((LPVOID)(game_assembly_base + MOBILE_FPS_UPDATE), (LPVOID)intercepted_mobile_fps_update, (LPVOID*)&orig_mobile_fps_update) == MH_OK
		&& MH_EnableHook((LPVOID)(game_assembly_base + MOBILE_FPS_UPDATE)) == MH_OK;
}
