#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_D3D11_IMPLEMENTATION


#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include <MinHook.h>

#include "hooks/AI_Granny/ai_granny.h"
#include "core/core.h"
#include "lib/nuklear/nuklear_d3d11.h"
#include "hooks/gui/directx/resolve_directx_present.h"
#include "hooks/gui/gui.h"
#include "hooks/gui/wndproc/resolve_wndproc.h"

DWORD WINAPI CheatMain(LPVOID arg) {
	(void)arg;
	
	if (core_init() != 1)
		return 0;
	
	// just allocconsole for testing for now
	// come back to this lets just wrap this around DEBUG
	AllocConsole();
		
	FILE *stream;
		
	freopen_s(&stream, "CONIN$", "r", stdin);
	freopen_s(&stream, "CONOUT$", "w", stdout);
	freopen_s(&stream, "CONOUT$", "w", stderr);
		
	fprintf(stderr, "DirectX Present Address: 0x%llX\n", (unsigned long long)resolve_directx_address());
	fprintf(stderr, "WndProc address: 0x%llX\n", (unsigned long long)resolve_wndproc_address());
		
	fprintf(stderr, "Attempting to hook GUI into game\n");
		
	return 1;
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpvReserved
) {
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
        CreateThread(0, 
					 0, 
					 (LPTHREAD_START_ROUTINE)CheatMain, 
					 hinstDLL, 
					 0, 
					 0);
		break;

		case DLL_PROCESS_DETACH:
		if (lpvReserved == NULL) {
			MH_Uninitialize();
		}
		break;

		default:
		break;
    }
    return TRUE;
}
