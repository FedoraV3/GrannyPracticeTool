/*
 * resolve_directx_present.h
 *
 *  Created on: Sep 20, 2026
 *      Author: ir0n1c
 */

#ifndef INC_HOOKS_GUI_DIRECTX_RESOLVE_DIRECTX_PRESENT_H_
#define INC_HOOKS_GUI_DIRECTX_RESOLVE_DIRECTX_PRESENT_H_

#include <Windows.h>
#include <d3d11.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef HRESULT(__stdcall* IDXGISwapChain_Present)(
    IDXGISwapChain* pSwapChain,
    UINT SyncInterval,
    UINT Flags
);



uint64_t resolve_directx_address(void);

#ifdef __cplusplus
}
#endif
#endif 