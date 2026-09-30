//
// Created by ir0n1c on 9/29/2026.
//

#include "../../../inc/config/parser/parser.h"
#include "../../../inc/config/data.h"
#include "cJSON.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// creates { "x": "..", "y": "..", "z": ".." } and adds it under key
static cJSON* add_vec3(cJSON* root, const char* key, const UnityEngine_Vector3_o* v) {
	cJSON* obj = cJSON_AddObjectToObject(root, key);
	if (obj == NULL)
		return NULL;

	char buf[128];
	snprintf(buf, sizeof(buf), "%f", v->fields.x);
	if (cJSON_AddStringToObject(obj, "x", buf) == NULL)
		return NULL;
	snprintf(buf, sizeof(buf), "%f", v->fields.y);
	if (cJSON_AddStringToObject(obj, "y", buf) == NULL)
		return NULL;
	snprintf(buf, sizeof(buf), "%f", v->fields.z);
	if (cJSON_AddStringToObject(obj, "z", buf) == NULL)
		return NULL;

	return obj;
}

// returns a null terminated string with the new json obj
char* create_cfg_json() {
	cJSON* root = cJSON_CreateObject();

	if (root == NULL) {
		cJSON_Delete(root);
		return NULL;
	}

	cJSON* name = cJSON_CreateString("hash");

	cJSON* player_tp = add_vec3(root, "player_tp", &player_tp_pos);
	cJSON* granny_tp = add_vec3(root, "granny_tp", &granny_tp_pos);

	if (name == NULL || player_tp == NULL || granny_tp == NULL) {
		cJSON_Delete(root);
		return NULL;
	}

	char* buf = cJSON_Print(root);
	return buf;
}