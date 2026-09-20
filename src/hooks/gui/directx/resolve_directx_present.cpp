/*
 * resolve_directx_present.cpp
 *
 *  Created on: Sep 20, 2026
 *      Author: ir0n1c
 */

#include <windows.h>
#include <d3d11.h>

#include "hooks/gui/directx/resolve_directx_present.h"

namespace {
	constexpr size_t kPresentVtableIndex = 8;
	constexpr size_t kResizeBuffersVtableIndex = 13;
	
	constexpr wchar_t kDummyClassName[] = L"GrannyPracticeDxProbe";
	
	class DummyWindow {
	public:
		DummyWindow() {
			WNDCLASSEXW wc = {};
			wc.cbSize        = sizeof(wc);
			wc.lpfnWndProc   = DefWindowProcW;
			wc.hInstance     = GetModuleHandleW(nullptr);
			wc.lpszClassName = kDummyClassName;
	
			instance_ = wc.hInstance;
			atom_     = RegisterClassExW(&wc);
	
			hwnd_ = CreateWindowExW(0, kDummyClassName, L"", WS_OVERLAPPEDWINDOW,
			                        0, 0, 2, 2, nullptr, nullptr, instance_, nullptr);
		}
	
		~DummyWindow() {
			if (hwnd_ != nullptr) { DestroyWindow(hwnd_); }
			if (atom_ != 0) { UnregisterClassW(kDummyClassName, instance_); }
		}
	
		DummyWindow(const DummyWindow&)            = delete;
		DummyWindow& operator=(const DummyWindow&) = delete;
	
		HWND get() const { return hwnd_; }
	
	private:
		HWND      hwnd_     = nullptr;
		HINSTANCE instance_ = nullptr;
		ATOM      atom_     = 0;
	};
}

namespace {
	uint64_t cached_present = 0;
	uint64_t cached_resize_buffers = 0;
}

static bool resolve_swap_chain_vtable(void) {
	if (cached_present != 0 && cached_resize_buffers != 0) { return true; }

	// dummy window
	DummyWindow window;
	if (window.get() == nullptr) { return false; }

	
	// configuration for d3d11 so that we can call CreateD3D11 properly
	DXGI_SWAP_CHAIN_DESC desc  = {};
	desc.BufferCount           = 1;
	desc.BufferDesc.Width      = 2;
	desc.BufferDesc.Height     = 2;
	desc.BufferDesc.Format     = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator   = 0;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow          = window.get();
	desc.SampleDesc.Count      = 1;
	desc.SampleDesc.Quality    = 0;
	desc.Windowed              = TRUE;
	desc.SwapEffect            = DXGI_SWAP_EFFECT_DISCARD;

	const D3D_FEATURE_LEVEL feature_levels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};


	const D3D_DRIVER_TYPE driver_types[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	for (D3D_DRIVER_TYPE driver_type : driver_types) {
		IDXGISwapChain*      swap_chain = nullptr;
		ID3D11Device*        device     = nullptr;
		ID3D11DeviceContext* context    = nullptr;

		const HRESULT hr = D3D11CreateDeviceAndSwapChain(
			nullptr,                                        
			driver_type,
			nullptr,                                        
			0,                                              
			feature_levels,
			static_cast<UINT>(ARRAYSIZE(feature_levels)),
			D3D11_SDK_VERSION,
			&desc,
			&swap_chain,
			&device,
			nullptr,                                        
			&context
		);

		if (FAILED(hr) || swap_chain == nullptr) {
			if (context != nullptr)    { context->Release(); }
			if (device != nullptr)     { device->Release(); }
			if (swap_chain != nullptr) { swap_chain->Release(); }
			continue;
		}

		void** vtable = *reinterpret_cast<void***>(swap_chain);
		cached_present = reinterpret_cast<uint64_t>(vtable[kPresentVtableIndex]);
		cached_resize_buffers = reinterpret_cast<uint64_t>(vtable[kResizeBuffersVtableIndex]);

		if (context != nullptr) { context->Release(); }
		if (device != nullptr)  { device->Release(); }
		swap_chain->Release();

		return true;
	}

	return false;
}

extern "C" uint64_t resolve_directx_address(void) {
	if (!resolve_swap_chain_vtable()) { return 0; }
	return cached_present;
}

extern "C" uint64_t resolve_directx_resize_buffers_address(void) {
	if (!resolve_swap_chain_vtable()) { return 0; }
	return cached_resize_buffers;
}
