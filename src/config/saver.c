//
// Created by ir0n1c on 9/29/2026.
//

#include "config/saver.h"
#include "config/data.h"
#include "config/writer/writer.h"
#include "cJSON.h"

#include <Windows.h>
#include <stdlib.h>
#include <string.h>

// adds { "x": .., "y": .., "z": .. } under key
static bool add_vec3(cJSON* root, const char* key, const UnityEngine_Vector3_o* v) {
	cJSON* obj = cJSON_AddObjectToObject(root, key);
	return obj
		&& cJSON_AddNumberToObject(obj, "x", v->fields.x)
		&& cJSON_AddNumberToObject(obj, "y", v->fields.y)
		&& cJSON_AddNumberToObject(obj, "z", v->fields.z);
}

static bool add_item_selections(cJSON* root) {
	cJSON* obj = cJSON_AddObjectToObject(root, "items");
	if (!obj)
		return false;

	item_selection* selections = malloc(sizeof(item_selection) * MAX_ITEM_SELECTIONS);
	if (!selections)
		return false;

	int count = copy_item_selections(selections, MAX_ITEM_SELECTIONS);
	bool ok = true;
	for (int i = 0; i < count && ok; i++)
		ok = cJSON_AddNumberToObject(obj, selections[i].location, selections[i].item) != NULL;

	free(selections);
	return ok;
}

// builds the json from the values in data.h, pwd is always the first object
static char* build_cfg_json(void) {
	cJSON* root = cJSON_CreateObject();
	if (!root)
		return NULL;

	if (!cJSON_AddStringToObject(root, "pwd", cfg_pwd)
		|| !add_vec3(root, "player_tp", &player_tp_pos)
		|| !add_vec3(root, "granny_tp", &granny_tp_pos)
		|| !add_item_selections(root)) {
		cJSON_Delete(root);
		return NULL;
	}

	char* buf = cJSON_Print(root);
	cJSON_Delete(root);
	return buf;
}

bool save_cfg(const char* cfg_name) {
	char file[MAX_PATH];
	if (!build_cfg_file_path(cfg_name, file, sizeof(file)))
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
