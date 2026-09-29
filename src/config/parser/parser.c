//
// Created by ir0n1c on 9/29/2026.
//

#include "../../../inc/config/parser/parser.h"
#include "../../../inc/config/data.h"
#include "cJSON.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// returns a null terminated string with the new json obj
char* create_cfg_json() {
	cJSON* root = cJSON_CreateObject();

	if (root == NULL) {
		cJSON_Delete(root);
		return NULL;
	}

	cJSON* name = cJSON_CreateString("hash");

	char p_buf[128];
	snprintf(p_buf, sizeof(p_buf), "%f", player_tp_pos);
	cJSON* player_tp_pos = cJSON_CreateString(p_buf);
	cJSON_AddItemToObject(root, "player_tp", player_tp_pos);

	char g_buf[128];
	snprintf(g_buf, sizeof(g_buf), "%f", granny_tp_pos);
	cJSON* granny_tp_pos = cJSON_CreateString(g_buf);
	cJSON_AddItemToObject(root, "granny_tp", granny_tp_pos);

	if (name == NULL || player_tp_pos == NULL || granny_tp_pos == NULL) {
		cJSON_Delete(root);
		return NULL;
	}

	char* buf = cJSON_Print(root);
	return buf;
}