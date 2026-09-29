//
// Created by ir0n1c on 9/29/2026.
//

#include "config/saver.h"
#include "config/data.h"
#include "config/writer/writer.h"
#include "cJSON.h"

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// builds the json from the values in data.h, pwd is always the first object
static char* build_cfg_json(void) {
	cJSON* root = cJSON_CreateObject();
	if (!root)
		return NULL;

	if (!cJSON_AddStringToObject(root, "pwd", cfg_pwd)
		|| !cJSON_AddNumberToObject(root, "player_tp", player_tp_pos)
		|| !cJSON_AddNumberToObject(root, "granny_tp", granny_tp_pos)) {
		cJSON_Delete(root);
		return NULL;
	}

	char* buf = cJSON_Print(root);
	cJSON_Delete(root);
	return buf;
}

bool save_cfg(const char* cfg_name) {
	if (!cfg_name || !*cfg_name)
		return false;

	const char* dir = init_path();
	if (!dir)
		return false;

	char file[MAX_PATH];
	int n = snprintf(file, sizeof(file), "%s\\%s.json", dir, cfg_name);
	if (n < 0 || n >= (int)sizeof(file))
		return false;

	char* json = build_cfg_json();
	if (!json)
		return false;

	HANDLE h = CreateFileA(file, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE) {
		cJSON_free(json);
		return false;
	}

	DWORD len = (DWORD)strlen(json);
	DWORD written = 0;
	BOOL ok = WriteFile(h, json, len, &written, NULL);

	CloseHandle(h);
	cJSON_free(json);

	return ok && written == len;
}
