#include <Windows.h>

#include <MinHook.h>

#include "core/core.h"

DWORD WINAPI CheatMain(LPVOID arg) {
	(void)arg;

	return core_init() == 1;
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
