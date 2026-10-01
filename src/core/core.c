#include "core/core.h"
#include "MinHook.h"
#include "hooks/AI_Granny/ai_granny.h"
#include "hooks/gui/gui.h"
#include "hooks/items/items.h"
#include "hooks/player/player.h"
#include "hooks/unity/unityengine.h"
#include "runtime_constants.h"

#include <inttypes.h>
#include <locale.h>

uint8_t * game_assembly_base = NULL;

// basically the most important part of the program, its what starts the heart of the program
int core_init() {
	setlocale(LC_ALL, "");
	game_assembly_base = (uint8_t*)GetModuleHandleW(L"GameAssembly.dll");
	
	if (MH_Initialize() != MH_OK) { goto FAIL; }
	if (!unityengine_install()) { goto FAIL; }
	if (!ai_granny_hook_install()) { goto FAIL; }
	if (!items_hook_install()) { goto FAIL; }
	if (!player_hook_install()) { goto FAIL; }
	if (!gui_install()) { goto FAIL; }
	
	return 1;

	FAIL:
	MessageBoxA(NULL, "Hooking has failed!", "Granny Practice Tool", MB_OK | MB_ICONERROR | MB_TOPMOST);
	MH_Uninitialize();
	TerminateProcess(GetCurrentProcess(), 1);
	return 0;
}