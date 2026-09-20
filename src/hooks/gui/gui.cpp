/*
 * gui.c
 *
 *  Created on: Sep 20, 2026
 *      Author: ir0n1c
 */


#include "hooks/gui/gui.h"
#include "granny/core/core.h"
#include "hooks/gui/directx/resolve_directx_present.h"
#include "hooks/gui/wndproc/resolve_wndproc.h"
extern "C" {
#include "lib/nuklear/nuklear_d3d11.h"
}
#include "lib/nuklear/nuklear.h"
#include "MinHook.h"
#include <stdio.h>
#include <d3d11.h>


// always be paranoid about memory since you never know what happens in there
IDXGISwapChain_Present original_present = nullptr;
IDXGISwapChain_ResizeBuffers original_resize_buffers = nullptr;
WNDPROC original_wndproc = nullptr;

IDXGISwapChain* d3d11_swap_chain = nullptr;
ID3D11DeviceContext* d3d11_dev_ctx = nullptr;
ID3D11Device* d3d11_dev = nullptr;
ID3D11RenderTargetView* d3d11_rtv = nullptr;

uint8_t is_gui_open = 1;

static LRESULT CALLBACK intercepted_wnd_proc(
	HWND hwnd, 
	UINT uMsg, 
	WPARAM wParam, 
	LPARAM lParam
) {
	if (is_gui_open && nuklear_ctx) {
		nk_d3d11_handle_event(hwnd, uMsg, wParam,lParam);
	}
	return original_wndproc(hwnd, uMsg, wParam, lParam);
}

struct d3d11_pipeline_state {
	ID3D11InputLayout* input_layout;
	ID3D11Buffer* vertex_buffer;
	UINT vertex_stride;
	UINT vertex_offset;
	ID3D11Buffer* index_buffer;
	DXGI_FORMAT index_format;
	UINT index_offset;
	D3D11_PRIMITIVE_TOPOLOGY topology;
	ID3D11VertexShader* vertex_shader;
	ID3D11Buffer* vertex_const_buffer;
	ID3D11PixelShader* pixel_shader;
	ID3D11SamplerState* sampler;
	ID3D11ShaderResourceView* shader_resource;
	ID3D11BlendState* blend_state;
	FLOAT blend_factor[4];
	UINT sample_mask;
	ID3D11RasterizerState* rasterizer_state;
	ID3D11RenderTargetView* render_target;
	ID3D11DepthStencilView* depth_stencil;
	D3D11_VIEWPORT viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	UINT viewport_count;
	D3D11_RECT scissors[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	UINT scissor_count;
};

static void save_pipeline_state(d3d11_pipeline_state* state) {
	state->viewport_count = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	state->scissor_count = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;

	d3d11_dev_ctx->IAGetInputLayout(&state->input_layout);
	d3d11_dev_ctx->IAGetVertexBuffers(0, 1, &state->vertex_buffer, &state->vertex_stride, &state->vertex_offset);
	d3d11_dev_ctx->IAGetIndexBuffer(&state->index_buffer, &state->index_format, &state->index_offset);
	d3d11_dev_ctx->IAGetPrimitiveTopology(&state->topology);
	d3d11_dev_ctx->VSGetShader(&state->vertex_shader, nullptr, nullptr);
	d3d11_dev_ctx->VSGetConstantBuffers(0, 1, &state->vertex_const_buffer);
	d3d11_dev_ctx->PSGetShader(&state->pixel_shader, nullptr, nullptr);
	d3d11_dev_ctx->PSGetSamplers(0, 1, &state->sampler);
	d3d11_dev_ctx->PSGetShaderResources(0, 1, &state->shader_resource);
	d3d11_dev_ctx->OMGetBlendState(&state->blend_state, state->blend_factor, &state->sample_mask);
	d3d11_dev_ctx->OMGetRenderTargets(1, &state->render_target, &state->depth_stencil);
	d3d11_dev_ctx->RSGetState(&state->rasterizer_state);
	d3d11_dev_ctx->RSGetViewports(&state->viewport_count, state->viewports);
	d3d11_dev_ctx->RSGetScissorRects(&state->scissor_count, state->scissors);
}

static void restore_pipeline_state(d3d11_pipeline_state* state) {
	d3d11_dev_ctx->IASetInputLayout(state->input_layout);
	d3d11_dev_ctx->IASetVertexBuffers(0, 1, &state->vertex_buffer, &state->vertex_stride, &state->vertex_offset);
	d3d11_dev_ctx->IASetIndexBuffer(state->index_buffer, state->index_format, state->index_offset);
	d3d11_dev_ctx->IASetPrimitiveTopology(state->topology);
	d3d11_dev_ctx->VSSetShader(state->vertex_shader, nullptr, 0);
	d3d11_dev_ctx->VSSetConstantBuffers(0, 1, &state->vertex_const_buffer);
	d3d11_dev_ctx->PSSetShader(state->pixel_shader, nullptr, 0);
	d3d11_dev_ctx->PSSetSamplers(0, 1, &state->sampler);
	d3d11_dev_ctx->PSSetShaderResources(0, 1, &state->shader_resource);
	d3d11_dev_ctx->OMSetBlendState(state->blend_state, state->blend_factor, state->sample_mask);
	d3d11_dev_ctx->OMSetRenderTargets(1, &state->render_target, state->depth_stencil);
	d3d11_dev_ctx->RSSetState(state->rasterizer_state);
	d3d11_dev_ctx->RSSetViewports(state->viewport_count, state->viewports);
	d3d11_dev_ctx->RSSetScissorRects(state->scissor_count, state->scissors);

	if (state->input_layout != nullptr)        { state->input_layout->Release(); }
	if (state->vertex_buffer != nullptr)       { state->vertex_buffer->Release(); }
	if (state->index_buffer != nullptr)        { state->index_buffer->Release(); }
	if (state->vertex_shader != nullptr)       { state->vertex_shader->Release(); }
	if (state->vertex_const_buffer != nullptr) { state->vertex_const_buffer->Release(); }
	if (state->pixel_shader != nullptr)        { state->pixel_shader->Release(); }
	if (state->sampler != nullptr)             { state->sampler->Release(); }
	if (state->shader_resource != nullptr)     { state->shader_resource->Release(); }
	if (state->blend_state != nullptr)         { state->blend_state->Release(); }
	if (state->rasterizer_state != nullptr)    { state->rasterizer_state->Release(); }
	if (state->render_target != nullptr)       { state->render_target->Release(); }
	if (state->depth_stencil != nullptr)       { state->depth_stencil->Release(); }
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

void render_frame() {
	if (nk_begin(nuklear_ctx, "Granny Legacy Test", nk_rect(50, 50, 220, 220),
	    NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_TITLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_SCALABLE)) {
		
		if (nk_button_label(nuklear_ctx, "Test button")) {
			printf("Hello world!\n");
		}
			
	}
	
	nk_end(nuklear_ctx);
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
		d3d11_dev->GetImmediateContext(&d3d11_dev_ctx);
		
		// this is a pretty good time to make the ui here since this condition will be run once
		// 64kb to max vertex buffer, 128kb for the index buffer
		DXGI_SWAP_CHAIN_DESC swap_chain_desc;
		pSwapChain->GetDesc(&swap_chain_desc);
		
		nuklear_ctx = nk_d3d11_init(d3d11_dev, swap_chain_desc.BufferDesc.Width, swap_chain_desc.BufferDesc.Height, 64000, 128000);
		// font and grab backbuffer
		struct nk_font_atlas *atlas;
		nk_d3d11_font_stash_begin(&atlas);
		nk_d3d11_font_stash_end();
		
		nk_input_begin(nuklear_ctx);
	}
	
	create_render_target(pSwapChain);
	
	if (is_gui_open && nuklear_ctx != nullptr && d3d11_rtv != nullptr) {
		nk_input_end(nuklear_ctx);
		
		render_frame();
		
		// save the last recorded state
		d3d11_pipeline_state state;
		save_pipeline_state(&state);
		
		d3d11_dev_ctx->OMSetRenderTargets(1, &d3d11_rtv, nullptr);
		nk_d3d11_render(d3d11_dev_ctx, NK_ANTI_ALIASING_ON);
		
		// then we restore it over here, thats basically what the long functions do,
		// they just save state, as it uses my global and its kind of unsafe since its possible
		// that it could be nullptr but thats close to impossible right? right?
		restore_pipeline_state(&state);
		
		nk_input_begin(nuklear_ctx);
	}
		
	return original_present(pSwapChain, SyncInterval, Flags);
}

// so that we can resize the window and not instant break the program because of resizing
static HRESULT __stdcall intercepted_idxgiswapchain_resize_buffers(
	IDXGISwapChain* pSwapChain,
	UINT BufferCount,
	UINT Width,
	UINT Height,
	DXGI_FORMAT NewFormat,
	UINT SwapChainFlags
) {
	release_render_target();

	const HRESULT hr = original_resize_buffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
	if (FAILED(hr)) { return hr; }

	if (d3d11_dev_ctx != nullptr && nuklear_ctx != nullptr) {
		DXGI_SWAP_CHAIN_DESC swap_chain_desc;
		pSwapChain->GetDesc(&swap_chain_desc);
		nk_d3d11_resize(d3d11_dev_ctx, swap_chain_desc.BufferDesc.Width, swap_chain_desc.BufferDesc.Height);
	}

	return hr;
}

bool gui_install() {
	// fuck this was so painful
	uint64_t directx_present_addr = resolve_directx_address();
	uint64_t wndproc_addr = resolve_wndproc_address();
	uint64_t directx_resize_buffers_addr = resolve_directx_resize_buffers_address();
	
	// minhook hooking
	if (MH_CreateHook((LPVOID)directx_present_addr, (LPVOID)intercepted_idxgiswapchain_present, (LPVOID*)&original_present) != MH_OK) {
		return false;
	}
	
	if (MH_CreateHook((LPVOID)wndproc_addr, (LPVOID)intercepted_wnd_proc, (LPVOID*)&original_wndproc) != MH_OK) {
		return false;
	}
	
	if (MH_CreateHook((LPVOID)directx_resize_buffers_addr, (LPVOID)intercepted_idxgiswapchain_resize_buffers, (LPVOID*)&original_resize_buffers) != MH_OK) {
		return false;
	}
	
	// now let us enable the hooks
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

