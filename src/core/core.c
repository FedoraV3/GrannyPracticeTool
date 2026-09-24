#include "core/core.h"
#include "granny/unityengine/typedefs.h"
#include "runtime_constants.h"
#include "MinHook.h"
#include "hooks/gui/gui.h"
#include "hooks/AI_Granny/ai_granny.h"
#include "hooks/unity/unityengine.h"
#include <locale.h>
#include <inttypes.h>

void* game_assembly_base = NULL;
// i wonder if this is compatible for other windows since this is a font variable
// or nuklear can handle this part for me
struct nk_user_font *nuklear_font;
struct nk_context *nuklear_ctx;

// basically the most important part of the program, its what starts the heart of the program
int core_init() {
	setlocale(LC_ALL, "");
	game_assembly_base = (void*)GetModuleHandleW(L"GameAssembly.dll");
	
	if (MH_Initialize() != MH_OK) { return 0; }
	if (!gui_install()) { return 0; }
	if (!unityengine_install()) { return 0; }
	if (!ai_granny_hook_install()) { return 0; }
	
	// attach our thread to il2cpp thread
	/*
		typedef void* (*il2cpp_domain_get_t)(void);
		typedef void* (*il2cpp_thread_attach_t)(void* domain);
	*/
	
	// to save memory on variables just do it directly (wow so long)
	((il2cpp_thread_attach_t)game_assembly_base + IL2CPP_THREAD_ATTACH)(((il2cpp_domain_get_t)game_assembly_base + IL2CPP_DOMAIN_GET)());
	return 1;
}