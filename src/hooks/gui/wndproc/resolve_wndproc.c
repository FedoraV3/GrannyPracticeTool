/*
 * resolve_wndproc.c
 *
 *  Created on: Sep 20, 2026
 *      Author: ir0n1c
 */


#include "hooks/gui/wndproc/resolve_wndproc.h"

// we get injected before the game has finished creating its window so the
// first lookup has to wait for it instead of giving up immediately
#define WINDOW_WAIT_TIMEOUT_MS  10000
#define WINDOW_POLL_INTERVAL_MS 50

struct window_search {
	DWORD process_id;
	HWND  hwnd;
};

static BOOL CALLBACK pick_process_window(HWND hwnd, LPARAM lparam) {
	struct window_search *search = (struct window_search *)lparam;

	DWORD process_id = 0;
	GetWindowThreadProcessId(hwnd, &process_id);
	if (process_id != search->process_id) { return TRUE; }

	if (GetWindow(hwnd, GW_OWNER) != NULL) { return TRUE; }
	if (!IsWindowVisible(hwnd)) { return TRUE; }

	RECT rect;
	if (!GetClientRect(hwnd, &rect)) { return TRUE; }
	if (rect.right - rect.left <= 0 || rect.bottom - rect.top <= 0) { return TRUE; }

	search->hwnd = hwnd;
	return FALSE;
}

HWND resolve_game_window(void) {
	static HWND cached = NULL;
	if (cached != NULL && IsWindow(cached)) { return cached; }
	cached = NULL;

	struct window_search search;
	search.process_id = GetCurrentProcessId();
	search.hwnd       = NULL;

	for (DWORD waited = 0; waited <= WINDOW_WAIT_TIMEOUT_MS; waited += WINDOW_POLL_INTERVAL_MS) {
		EnumWindows(pick_process_window, (LPARAM)&search);
		if (search.hwnd != NULL) {
			cached = search.hwnd;
			return cached;
		}

		Sleep(WINDOW_POLL_INTERVAL_MS);
	}

	return NULL;
}

uint64_t resolve_wndproc_address(void) {
	HWND hwnd = resolve_game_window();
	if (hwnd == NULL) { return 0; }

	LONG_PTR wnd_proc = IsWindowUnicode(hwnd)
		? GetWindowLongPtrW(hwnd, GWLP_WNDPROC)
		: GetWindowLongPtrA(hwnd, GWLP_WNDPROC);

	return (uint64_t)wnd_proc;
}
