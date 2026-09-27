#include "core/core.h"
#include "MinHook.h"
#include "events/handler.h"
#include "granny/unityengine/typedefs.h"
#include "hooks/AI_Granny/ai_granny.h"
#include "hooks/gui/gui.h"
#include "hooks/unity/unityengine.h"
#include "runtime_constants.h"

#include <inttypes.h>
#include <locale.h>
#include <stdio.h>

uint8_t * game_assembly_base = NULL;

// basically the most important part of the program, its what starts the heart of the program
int core_init() {
	setlocale(LC_ALL, "");
	game_assembly_base = (uint8_t*)GetModuleHandleW(L"GameAssembly.dll");
	
	if (MH_Initialize() != MH_OK) { goto FAIL; }
	if (!unityengine_install()) { goto FAIL; }
	if (!ai_granny_hook_install()) { goto FAIL; }
	if (!gui_install()) { goto FAIL; }
	
	// attach our thread to il2cpp thread
	/*
		typedef void* (*il2cpp_domain_get_t)(void);
		typedef void* (*il2cpp_thread_attach_t)(void* domain);
	*/
	
	// to save memory on variables just do it directly (wow so long)
	((il2cpp_thread_attach_t)(game_assembly_base + IL2CPP_THREAD_ATTACH))(((il2cpp_domain_get_t)(game_assembly_base + IL2CPP_DOMAIN_GET))());
	return 1;

	FAIL:
	MessageBoxA(NULL, "Hooking has failed!", "Granny Practice Tool", MB_OK | MB_ICONERROR | MB_TOPMOST);
	MH_Uninitialize();
	TerminateProcess(GetCurrentProcess(), 1);
	return 0;
}