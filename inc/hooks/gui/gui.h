/*
 * gui.h
 *
 *  Created on: Sep 20, 2026
 *      Author: ir0n1c
 */

#ifndef INC_HOOKS_GUI_GUI_H_
#define INC_HOOKS_GUI_GUI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <d3d11.h>
#include <stdbool.h>
#include <inttypes.h>

#include "hooks/gui/directx/resolve_directx_present.h"


extern uint8_t is_gui_open;
extern IDXGISwapChain* d3d11_swap_chain;
extern ID3D11DeviceContext* d3d11_dev_ctx;
extern ID3D11Device* d3d11_dev;

extern IDXGISwapChain_Present original_present;
extern IDXGISwapChain_ResizeBuffers original_resize_buffers;
extern WNDPROC original_wndproc;

bool gui_install();
void render_frame();

#ifdef __cplusplus
}
#endif

#endif /* INC_HOOKS_GUI_GUI_H_ */
