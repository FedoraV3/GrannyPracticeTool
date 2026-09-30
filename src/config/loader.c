//
// Created by ir0n1c on 9/29/2026.
//

#include "config/loader.h"
#include "config/data.h"
#include "config/writer/writer.h"
#include "cJSON.h"

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// configs are tiny, anything bigger than this is not ours
#define MAX_CFG_SIZE (64 * 1024)

// returns a null terminated heap buffer with the file contents, caller frees
static char* read_file(const char* file) {
	HANDLE h = CreateFileA(file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE)
		return NULL;

	LARGE_INTEGER size;
	if (!GetFileSizeEx(h, &size) || size.QuadPart <= 0 || size.QuadPart > MAX_CFG_SIZE) {
		CloseHandle(h);
		return NULL;
	}

	DWORD len = (DWORD)size.QuadPart;
	char* buf = malloc(len + 1);
	if (!buf) {
		CloseHandle(h);
		return NULL;
	}

	DWORD read = 0;
	BOOL ok = ReadFile(h, buf, len, &read, NULL);
	CloseHandle(h);

	if (!ok || read != len) {
		free(buf);
		return NULL;
	}

	buf[len] = '\0';
	return buf;
}

// accepts both numbers and numeric strings since the old parser wrote strings
static bool get_float(const cJSON* root, const char* key, float* out) {
	const cJSON* item = cJSON_GetObjectItemCaseSensitive(root, key);

	if (cJSON_IsNumber(item)) {
		*out = (float)item->valuedouble;
		return true;
	}

	if (cJSON_IsString(item) && item->valuestring) {
		char* end = NULL;
		float v = strtof(item->valuestring, &end);
		if (end == item->valuestring || *end != '\0')
			return false;
		*out = v;
		return true;
	}

	return false;
}

// vectors are stored as { "x": .., "y": .., "z": .. }
static bool get_vec3(const cJSON* root, const char* key, UnityEngine_Vector3_o* out) {
	const cJSON* obj = cJSON_GetObjectItemCaseSensitive(root, key);
	if (!cJSON_IsObject(obj))
		return false;

	UnityEngine_Vector3_o v;
	if (!get_float(obj, "x", &v.fields.x) || !get_float(obj, "y", &v.fields.y) || !get_float(obj, "z", &v.fields.z))
		return false;

	*out = v;
	return true;
}

bool load_cfg(const char* cfg_name) {
	if (!cfg_name || !*cfg_name)
		return false;

	const char* dir = init_path();
	if (!dir)
		return false;

	char file[MAX_PATH];
	int n = snprintf(file, sizeof(file), "%s\\%s.json", dir, cfg_name);
	if (n < 0 || n >= (int)sizeof(file))
		return false;

	char* json = read_file(file);
	if (!json)
		return false;

	cJSON* root = cJSON_Parse(json);
	free(json);
	if (!cJSON_IsObject(root)) {
		cJSON_Delete(root);
		return false;
	}

	// pwd has to be the first object, otherwise this isnt our config
	const cJSON* first = root->child;
	if (!first || !first->string || strcmp(first->string, "pwd") != 0
		|| !cJSON_IsString(first) || strcmp(first->valuestring, cfg_pwd) != 0) {
		cJSON_Delete(root);
		return false;
	}

	UnityEngine_Vector3_o p_pos, g_pos;
	bool ok = get_vec3(root, "player_tp", &p_pos) && get_vec3(root, "granny_tp", &g_pos);
	cJSON_Delete(root);

	if (!ok)
		return false;

	player_tp_pos = p_pos;
	granny_tp_pos = g_pos;
	return true;
}

void unload_cfg(void) {
	// 0,0,0 is impossible
	player_tp_pos = (UnityEngine_Vector3_o){0};
	granny_tp_pos = (UnityEngine_Vector3_o){0};
}
