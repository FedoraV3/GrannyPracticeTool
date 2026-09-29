//
// Created by ir0n1c on 9/29/2026.
//

#include "../../../inc/config/writer/writer.h"

#include "config/parser/parser.h"

#include <Windows.h>
#include <direct.h>
#include <shlobj.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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

bool save_to_file(void) {
	const char* dir = init_path();
	if (!dir)
		return false;

	char file[MAX_PATH];
	int n = snprintf(file, sizeof(file), "%s\\config.json", dir);
	if (n < 0 || n >= (int)sizeof(file))
		return false;

	char* json = create_cfg_json();
	if (!json)
		return false;

	HANDLE h = CreateFileA(file, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE) {
		free(json);
		return false;
	}

	DWORD len = (DWORD)strlen(json);
	DWORD written = 0;
	BOOL ok = WriteFile(h, json, len, &written, NULL);

	CloseHandle(h);
	free(json);

	return ok && written == len;
}