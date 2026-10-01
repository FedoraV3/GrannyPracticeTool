//
// Created by ir0n1c on 9/29/2026.
//

#ifndef GRANNYPRACTICETOOL_DATA_H
#define GRANNYPRACTICETOOL_DATA_H

#include "../granny/unityengine/ue_vector3.h"

#include <stdbool.h>

#define ITEM_LOCATION_NAME_SIZE 64
#define MAX_ITEM_SELECTIONS 512

typedef struct item_selection {
	char location[ITEM_LOCATION_NAME_SIZE];
	int item;
} item_selection;

#ifdef __cplusplus
extern "C" {
#endif

// this is the pwd that will be seen as the first object of the json file. if it
// isnt found then the loader will refuse to load
extern const char cfg_pwd[];

extern UnityEngine_Vector3_o player_tp_pos;
extern UnityEngine_Vector3_o granny_tp_pos;

int get_item_selection(const char* location);
bool set_item_selection(const char* location, int item);
void clear_item_selections(void);
int copy_item_selections(item_selection* out, int max);
void replace_item_selections(const item_selection* selections, int count);
long item_selections_version(void);

#ifdef __cplusplus
}
#endif

#endif // GRANNYPRACTICETOOL_DATA_H
