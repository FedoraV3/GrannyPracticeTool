/*
 * resolve_wndproc.h
 *
 *  Created on: Sep 20, 2026
 *      Author: ir0n1c
 */


#ifndef RESOLVE_WNDPROC_H
#define RESOLVE_WNDPROC_H

#include <Windows.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

HWND resolve_game_window(void);
uint64_t resolve_wndproc_address(void);

#ifdef __cplusplus
}
#endif

#endif
