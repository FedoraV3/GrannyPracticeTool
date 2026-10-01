#include "hooks/gui/gui.h"
#include "hooks/gui/directx/resolve_directx_present.h"
#include "hooks/gui/wndproc/resolve_wndproc.h"
#include "MinHook.h"
#include <d3d11.h>
#include <mutex>
#include <stdio.h>
#include <string.h>

#include "imgui.h"
#include "imgui_internal.h"

#include "cimgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "config/data.h"
#include "config/loader.h"
#include "config/saver.h"
#include "config/writer/writer.h"
#include "events/handler.h"
#include "granny/player_teleport.h"
#include "granny/item_spawn.h"
#include "granny/unityengine/structs.h"
#include "runtime_constants.h"

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

static item_location gui_item_locations[MAX_ITEM_LOCATIONS];
static int gui_item_selected[MAX_ITEM_LOCATIONS];
static int gui_item_location_count = 0;
static long gui_locations_version = -1;
static long gui_selections_version = -1;

static void sync_item_rows() {
	long locations_version = item_locations_version();
	long selections_version = item_selections_version();

	bool locations_changed = locations_version != gui_locations_version;
	if (locations_changed) {
		gui_item_location_count = copy_item_locations(gui_item_locations, MAX_ITEM_LOCATIONS);
		gui_locations_version = locations_version;
	}

	if (locations_changed || selections_version != gui_selections_version) {
		for (int i = 0; i < gui_item_location_count; i++) {
			gui_item_selected[i] = get_item_selection(gui_item_locations[i].key);
		}
		gui_selections_version = selections_version;
	}
}

static void select_item(int row, int item) {
	if (set_item_selection(gui_item_locations[row].key, item)) {
		gui_item_selected[row] = item;
	}
}

static void render_items_tab() {
	bool in_game = has_item_inventory();
	sync_item_rows();

	if (igButton("Spawn Selected Items", ImVec2_c{0,0})) {
		if (in_game) {
			queue_new_event(ITEMS_SPAWN_SELECTED, nullptr, 0);
		}
	}

	igSameLine(0, -1);
	if (igButton("Clear Selections", ImVec2_c{0,0})) {
		clear_item_selections();
	}

	igSameLine(0, -1);
	if (igButton("Refresh Locations", ImVec2_c{0,0})) {
		if (in_game) {
			queue_new_event(ITEM_LOCATIONS_REFRESH, nullptr, 0);
		}
	}

	igSeparator();

	if (gui_item_location_count == 0) {
		igTextDisabled("No item locations found, press Refresh Locations in game");
		return;
	}

	if (!igBeginChild_Str("item_locations", ImVec2_c{0,0}, 0, 0)) {
		igEndChild();
		return;
	}

	for (int i = 0; i < gui_item_location_count; i++) {
		const item_location* loc = &gui_item_locations[i];
		int selected = gui_item_selected[i];

		igPushID_Int(i);

		igTextUnformatted(loc->label, nullptr);
		igSameLine(260, -1);
		igSetNextItemWidth(200);

		if (igBeginCombo("##item", get_item_display_name(selected), 0)) {
			if (igSelectable_Bool("None", selected == 0, 0, ImVec2_c{0,0})) {
				select_item(i, 0);
			}

			for (int item = ITEM_FIRST; item <= ITEM_LAST; item++) {
				if (igSelectable_Bool(get_item_display_name(item), selected == item, 0, ImVec2_c{0,0})) {
					select_item(i, item);
				}
				if (selected == item) {
					igSetItemDefaultFocus();
				}
			}

			igEndCombo();
		}

		igSameLine(0, -1);
		if (igButton("Spawn", ImVec2_c{0,0})) {
			if (in_game && selected != 0) {
				item_spawn_args args = { loc->pos, selected };
				queue_new_event(ITEM_SPAWN_AT, &args, sizeof(args));
			}
		}

		igPopID();
	}

	igEndChild();
}

#define MAX_LISTED_CFGS 64

static char cfg_name[CFG_NAME_SIZE] = "default";
static char cfg_list[MAX_LISTED_CFGS][CFG_NAME_SIZE];
static int cfg_list_count = 0;
static bool cfg_list_loaded = false;
static char cfg_status[128] = "";

static void refresh_cfg_list() {
	cfg_list_count = list_cfgs(cfg_list, MAX_LISTED_CFGS);
	cfg_list_loaded = true;
}

static void render_config_tab() {
	if (!cfg_list_loaded) {
		refresh_cfg_list();
	}

	igSetNextItemWidth(200);
	igInputText("Name", cfg_name, sizeof(cfg_name), 0, nullptr, nullptr);

	if (igButton("Save", ImVec2_c{0,0})) {
		if (save_cfg(cfg_name)) {
			snprintf(cfg_status, sizeof(cfg_status), "Saved %s" CFG_EXTENSION, cfg_name);
			refresh_cfg_list();
		} else {
			snprintf(cfg_status, sizeof(cfg_status), "Failed to save %s" CFG_EXTENSION, cfg_name);
		}
	}

	igSameLine(0, -1);
	if (igButton("Load", ImVec2_c{0,0})) {
		if (load_cfg(cfg_name)) {
			snprintf(cfg_status, sizeof(cfg_status), "Loaded %s" CFG_EXTENSION, cfg_name);
		} else {
			snprintf(cfg_status, sizeof(cfg_status), "Failed to load %s" CFG_EXTENSION, cfg_name);
		}
	}

	igSameLine(0, -1);
	if (igButton("Refresh", ImVec2_c{0,0})) {
		refresh_cfg_list();
	}

	if (cfg_status[0] != '\0') {
		igTextUnformatted(cfg_status, nullptr);
	}

	igSeparator();

	if (cfg_list_count == 0) {
		igTextDisabled("No configs found");
		return;
	}

	for (int i = 0; i < cfg_list_count; i++) {
		if (igSelectable_Bool(cfg_list[i], strcmp(cfg_list[i], cfg_name) == 0, 0, ImVec2_c{0,0})) {
			strcpy_s(cfg_name, sizeof(cfg_name), cfg_list[i]);
		}
	}
}

static char teleport_status[128] = "";

static void set_tp_from_player(UnityEngine_Vector3_o* target) {
	UnityEngine_Vector3_o pos;
	if (get_player_position(&pos)) {
		*target = pos;
		teleport_status[0] = '\0';
	} else {
		snprintf(teleport_status, sizeof(teleport_status), "Player position not available yet");
	}
}

static void render_teleport_tab() {
	if (igButton("Set Granny TP At Player Position", ImVec2_c{0,0})) {
		set_tp_from_player(&granny_tp_pos);
	}

	if (igButton("Set Player TP At Player Position", ImVec2_c{0,0})) {
		set_tp_from_player(&player_tp_pos);
	}

	if (igButton("Teleport And Spawn Items", ImVec2_c{0,0})) {
		UnityEngine_Vector3_o pos[2] = { granny_tp_pos, player_tp_pos };

		if (has_item_inventory()) {
			bool queued = queue_new_event(APPLY_SETUP, pos, sizeof(pos));
			snprintf(teleport_status, sizeof(teleport_status), "%s", queued ? "" : "Too many pending actions, try again");
		} else {
			bool queued = queue_new_event(ALL_SET_POS, pos, sizeof(pos));
			snprintf(teleport_status, sizeof(teleport_status), "%s", queued ? "Items not spawned: level not ready" : "Too many pending actions, try again");
		}
	}

	if (teleport_status[0] != '\0') {
		igTextUnformatted(teleport_status, nullptr);
	}
}

void render_frame() {
	igBegin("Granny Legacy Practice", nullptr, 0);

	if (igBeginTabBar("tabs", 0)) {
		if (igBeginTabItem("Teleport", nullptr, 0)) {
			render_teleport_tab();
			igEndTabItem();
		}

		if (igBeginTabItem("Items", nullptr, 0)) {
			render_items_tab();
			igEndTabItem();
		}

		if (igBeginTabItem("Config", nullptr, 0)) {
			render_config_tab();
			igEndTabItem();
		}

		igEndTabBar();
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
