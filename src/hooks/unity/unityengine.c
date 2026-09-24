#include "hooks/unity/unityengine.h"
#include "MinHook.h"
#include "core/core.h"
#include "granny/unityengine/typedefs.h"
#include "hooks/AI_Granny/ai_granny.h"
#include "runtime_constants.h"
#include <stdio.h>
#include <wchar.h>

UnityEngine_SceneManagement_SceneManager_LoadScene orig_ue_scenemanager_load_scene = NULL;

// typedef int64_t (__fastcall *UnityEngine_SceneManagement_SceneManager_LoadScene)(uint16_t* scene, void* method);
static int64_t __fastcall detour_ue_scenemanager_load_scene(System_String_o *scene_name, void* method) {
	if (wcscmp(&scene_name->fields.first_char, L"Menu") == 0) {
		fprintf(stderr, "AI Granny Invalidated\n");
		invalidate_ai_granny_ptr();
	}
		
	return orig_ue_scenemanager_load_scene(scene_name, method);
}

bool unityengine_install() {
	if (MH_CreateHook((LPVOID)(game_assembly_base + UNITYENGINE_SCENE_MANAGEMENT_SCENEMANAGER_LOADSCENE), detour_ue_scenemanager_load_scene, (LPVOID*)&orig_ue_scenemanager_load_scene) != MH_OK)
		return false;
	
	if (MH_EnableHook((LPVOID)(game_assembly_base + UNITYENGINE_SCENE_MANAGEMENT_SCENEMANAGER_LOADSCENE)) != MH_OK)
		return false;
	
	return true;
}