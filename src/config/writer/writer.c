//
// Created by ir0n1c on 9/29/2026.
//

#include "config/writer/writer.h"

#include <Windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static const char* path = "%localappdata%\\GrannyPracticeTool";
static char g_path[MAX_PATH];

const char* init_path(void) {
	DWORD len = ExpandEnvironmentStringsA(path, g_path, MAX_PATH);
	if (len == 0 || len > MAX_PATH)
		return NULL;

	if (!CreateDirectoryA(g_path, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
		return NULL;

	return g_path;
}

static bool is_reserved_device_name(const char* cfg_name) {
	static const char* const reserved[] = { "CON", "PRN", "AUX", "NUL" };

	size_t base_len = strcspn(cfg_name, ".");
	while (base_len > 0 && cfg_name[base_len - 1] == ' ')
		base_len--;

	for (size_t i = 0; i < sizeof(reserved) / sizeof(reserved[0]); i++) {
		if (base_len == 3 && _strnicmp(cfg_name, reserved[i], 3) == 0)
			return true;
	}

	return base_len == 4
		&& (_strnicmp(cfg_name, "COM", 3) == 0 || _strnicmp(cfg_name, "LPT", 3) == 0)
		&& cfg_name[3] >= '1' && cfg_name[3] <= '9';
}

bool is_valid_cfg_name(const char* cfg_name) {
	if (!cfg_name || !*cfg_name)
		return false;

	size_t len = strlen(cfg_name);
	if (len >= CFG_NAME_SIZE || cfg_name[len - 1] == '.' || cfg_name[len - 1] == ' ')
		return false;

	for (const char* c = cfg_name; *c; c++) {
		if ((unsigned char)*c < 0x20 || strchr("\\/:*?\"<>|", *c))
			return false;
	}

	return !is_reserved_device_name(cfg_name);
}

bool build_cfg_file_path(const char* cfg_name, char* out, size_t out_size) {
	if (!is_valid_cfg_name(cfg_name))
		return false;

	const char* dir = init_path();
	if (!dir)
		return false;

	int n = snprintf(out, out_size, "%s\\%s" CFG_EXTENSION, dir, cfg_name);
	return n > 0 && (size_t)n < out_size;
}

int list_cfgs(char (*names)[CFG_NAME_SIZE], int max) {
	const char* dir = init_path();
	if (!dir)
		return 0;

	char pattern[MAX_PATH];
	int n = snprintf(pattern, sizeof(pattern), "%s\\*" CFG_EXTENSION, dir);
	if (n < 0 || n >= (int)sizeof(pattern))
		return 0;

	WIN32_FIND_DATAA fd;
	HANDLE h = FindFirstFileA(pattern, &fd);
	if (h == INVALID_HANDLE_VALUE)
		return 0;

	int count = 0;
	const size_t ext_len = sizeof(CFG_EXTENSION) - 1;
	do {
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;

		size_t len = strlen(fd.cFileName);
		if (len <= ext_len || _stricmp(fd.cFileName + len - ext_len, CFG_EXTENSION) != 0)
			continue;

		size_t name_len = len - ext_len;
		if (name_len >= CFG_NAME_SIZE)
			continue;

		memcpy(names[count], fd.cFileName, name_len);
		names[count][name_len] = '\0';
		count++;
	} while (count < max && FindNextFileA(h, &fd));

	FindClose(h);
	return count;
}
