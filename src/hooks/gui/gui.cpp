/*
 * gui.c
 *
 *  Created on: Sep 20, 2026
 *      Author: ir0n1c
 */


#include "hooks/gui/gui.h"
#include "hooks/gui/directx/resolve_directx_present.h"
#include "hooks/gui/wndproc/resolve_wndproc.h"
#include "lib/nuklear/nuklear.h"
#include "MinHook.h"
#include <d3d11.h>

// always be paranoid about memory since you never know what happens in there
IDXGISwapChain_Present original_present = nullptr;
WNDPROC original_wndproc = nullptr;

IDXGISwapChain* d3d11_swap_chain = nullptr;
ID3D11DeviceContext* d3d11_dev_ctx = nullptr;
ID3D11Device* d3d11_dev = nullptr;

uint8_t is_gui_open = 1;

// same calling convention or else, like in assembly how the parameters get pushed get fucked, the order is wrong
static void CALLBACK poll_for_inputs(nk_context *ctx) {
	
}

static LRESULT CALLBACK intercepted_wnd_proc(
	HWND hwnd, 
	UINT uMsg, 
	WPARAM wParam, 
	LPARAM lParam
) {
	return original_wndproc(hwnd, uMsg, wParam, lParam);
}

/*
typedef HRESULT(__stdcall* IDXGISwapChain_Present)(
    IDXGISwapChain* pSwapChain,
    UINT SyncInterval,
    UINT Flags
);
*/
static HRESULT __stdcall intercepted_idxgiswapchain_present(
	IDXGISwapChain* pSwapChain,
	UINT SyncInterval,
	UINT Flags
) {	
	if (d3d11_swap_chain == nullptr && d3d11_dev == nullptr) {
		// grab d3d11 dev from t he swap chain
		d3d11_swap_chain = pSwapChain;
		// easy
		pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&d3d11_dev));
	
		// this is a pretty good time to make the ui here since this condition will be run once
	}
	
	return original_present(pSwapChain, SyncInterval, Flags);
}

bool gui_install() {
	// fuck this was so painful
	uint64_t directx_present_addr = resolve_directx_address();
	uint64_t wndproc_addr = resolve_wndproc_address();
	
	// minhook hooking
	if (MH_CreateHook((LPVOID)directx_present_addr, (LPVOID)intercepted_idxgiswapchain_present, (LPVOID*)&original_present) != MH_OK) {
		return false;
	}
	
	if (MH_CreateHook((LPVOID)wndproc_addr, (LPVOID)intercepted_wnd_proc, (LPVOID*)&original_wndproc) != MH_OK) {
		return false;
	}
	
	// now let us enable the hooks
	if (MH_EnableHook((LPVOID)directx_present_addr) != MH_OK) {
		return false;
	}
	
	if (MH_EnableHook((LPVOID)wndproc_addr) != MH_OK) {
		return false;
	}
	
	return true;
}

void render_frame() {
	
}