#include "hooks/gui/gui.h"
#include "hooks/gui/directx/resolve_directx_present.h"
#include "hooks/gui/wndproc/resolve_wndproc.h"
#include "MinHook.h"
#include <d3d11.h>
#include <mutex>

#include "imgui.h"
#include "imgui_internal.h"

#include "cimgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "events/handler.h"
#include "granny/unityengine/structs.h"
#include "hooks/AI_Granny/ai_granny.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

IDXGISwapChain_Present original_present = nullptr;
IDXGISwapChain_ResizeBuffers original_resize_buffers = nullptr;
WNDPROC original_wndproc = nullptr;

IDXGISwapChain* d3d11_swap_chain = nullptr;
ID3D11DeviceContext* d3d11_dev_ctx = nullptr;
ID3D11Device* d3d11_dev = nullptr;
ID3D11RenderTargetView* d3d11_rtv = nullptr;

uint8_t is_gui_open = 1;
static bool imgui_ready = false;

// wndproc runs on the game's main thread and present runs on unity's render thread,
// both touch the imgui context so everything imgui goes through this lock.
// recursive because imgui's handler calls ReleaseCapture() on mouse up, which re-enters
// our wndproc on the same thread with WM_CAPTURECHANGED while we still hold it
static std::recursive_mutex imgui_mutex;

static LRESULT CALLBACK intercepted_wnd_proc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
) {
	{
		std::lock_guard<std::recursive_mutex> lock(imgui_mutex);

		if (uMsg == WM_KEYDOWN && wParam == VK_INSERT && !(lParam & (1 << 30))) {
			is_gui_open = !is_gui_open;
		}

		if (is_gui_open && imgui_ready) {
			if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) {
				return 1;
			}

			if (igGetIO_Nil()->WantCaptureMouse && uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST) {
				return 1;
			}
		}
	}

	// never call into the game while holding the lock
	return original_wndproc(hwnd, uMsg, wParam, lParam);
}

static bool create_render_target(IDXGISwapChain* pSwapChain) {
	if (d3d11_rtv != nullptr) { return true; }
	if (d3d11_dev == nullptr) { return false; }

	ID3D11Texture2D* back_buffer = nullptr;
	if (FAILED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer)))) {
		return false;
	}

	const HRESULT hr = d3d11_dev->CreateRenderTargetView(back_buffer, nullptr, &d3d11_rtv);
	back_buffer->Release();

	return SUCCEEDED(hr);
}

static void release_render_target() {
	if (d3d11_rtv != nullptr) {
		d3d11_rtv->Release();
		d3d11_rtv = nullptr;
	}
}

static bool init_imgui(IDXGISwapChain* pSwapChain) {
	if (FAILED(pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&d3d11_dev)))) {
		d3d11_dev = nullptr;
		return false;
	}

	d3d11_swap_chain = pSwapChain;
	d3d11_dev->GetImmediateContext(&d3d11_dev_ctx);

	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	pSwapChain->GetDesc(&swap_chain_desc);

	igCreateContext(nullptr);
	igGetIO_Nil()->IniFilename = nullptr;

	if (!ImGui_ImplWin32_Init(swap_chain_desc.OutputWindow)) { return false; }
	if (!ImGui_ImplDX11_Init(d3d11_dev, d3d11_dev_ctx)) { return false; }

	imgui_ready = true;
	return true;
}

void render_frame() {
	igBegin("Granny Legacy Practice", nullptr, 0);
	igTextUnformatted("hello world waoikejhr42iuevyb4237iu4v6by", nullptr);
	if (igButton("Set As Granny TP", ImVec2_c{ 0, 0})) {
		if (get_ai_granny_transform() != nullptr) {
			UnityEngine_Vector3_o pos = curr_granny_ai->transform_pos;
			pos.fields.x -= 10;
			pos.fields.z -= 10;

			queue_new_event(GRANNY_SET_POS, GRANNY, &pos, sizeof(pos));
		}
	}
	igEnd();
}

static HRESULT __stdcall intercepted_idxgiswapchain_present(
	IDXGISwapChain* pSwapChain,
	UINT SyncInterval,
	UINT Flags
) {
	{
		std::lock_guard<std::recursive_mutex> lock(imgui_mutex);

		if (d3d11_dev == nullptr) {
			init_imgui(pSwapChain);
		}

		if (imgui_ready && is_gui_open && create_render_target(pSwapChain)) {
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			igNewFrame();

			render_frame();

			igRender();

			ID3D11RenderTargetView* old_rtv = nullptr;
			ID3D11DepthStencilView* old_dsv = nullptr;
			d3d11_dev_ctx->OMGetRenderTargets(1, &old_rtv, &old_dsv);
			d3d11_dev_ctx->OMSetRenderTargets(1, &d3d11_rtv, nullptr);

			ImGui_ImplDX11_RenderDrawData(igGetDrawData());

			d3d11_dev_ctx->OMSetRenderTargets(1, &old_rtv, old_dsv);
			if (old_rtv != nullptr) { old_rtv->Release(); }
			if (old_dsv != nullptr) { old_dsv->Release(); }
		}
	}

	// unlock before presenting, dxgi can SendMessage to the game window inside Present
	// and wait on the main thread, which might be waiting on our lock in the wndproc
	return original_present(pSwapChain, SyncInterval, Flags);
}

static HRESULT __stdcall intercepted_idxgiswapchain_resize_buffers(
	IDXGISwapChain* pSwapChain,
	UINT BufferCount,
	UINT Width,
	UINT Height,
	DXGI_FORMAT NewFormat,
	UINT SwapChainFlags
) {
	{
		std::lock_guard<std::recursive_mutex> lock(imgui_mutex);
		release_render_target();
	}

	return original_resize_buffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

bool gui_install() {
	uint64_t directx_present_addr = resolve_directx_address();
	uint64_t wndproc_addr = resolve_wndproc_address();
	uint64_t directx_resize_buffers_addr = resolve_directx_resize_buffers_address();

	if (MH_CreateHook((LPVOID)directx_present_addr, (LPVOID)intercepted_idxgiswapchain_present, (LPVOID*)&original_present) != MH_OK) {
		return false;
	}

	if (MH_CreateHook((LPVOID)wndproc_addr, (LPVOID)intercepted_wnd_proc, (LPVOID*)&original_wndproc) != MH_OK) {
		return false;
	}

	if (MH_CreateHook((LPVOID)directx_resize_buffers_addr, (LPVOID)intercepted_idxgiswapchain_resize_buffers, (LPVOID*)&original_resize_buffers) != MH_OK) {
		return false;
	}

	if (MH_EnableHook((LPVOID)directx_present_addr) != MH_OK) {
		return false;
	}

	if (MH_EnableHook((LPVOID)wndproc_addr) != MH_OK) {
		return false;
	}

	if (MH_EnableHook((LPVOID)directx_resize_buffers_addr) != MH_OK) {
		return false;
	}

	return true;
}
