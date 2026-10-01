#ifndef GRANNYPRACTICETOOL_ITEM_SPAWN_H
#define GRANNYPRACTICETOOL_ITEM_SPAWN_H

#include "config/data.h"
#include "granny/unityengine/structs.h"

#include <stdbool.h>

#define MAX_ITEM_LOCATIONS 512

typedef struct item_location {
	char key[ITEM_LOCATION_NAME_SIZE];
	char label[ITEM_LOCATION_NAME_SIZE];
	UnityEngine_Vector3_o pos;
} item_location;

typedef struct item_spawn_args {
	UnityEngine_Vector3_o pos;
	int item;
} item_spawn_args;

#ifdef __cplusplus
extern "C" {
#endif

void item_spawn_reset(void);
void item_spawn_tick(void* inventory);
bool has_item_inventory(void);

void item_locations_refresh(void);
void item_locations_mark_dirty(void);
long item_locations_version(void);
int copy_item_locations(item_location* out, int max);

const char* get_item_display_name(int item);

void spawn_item_at(int item, const UnityEngine_Vector3_o* pos);
void spawn_selected_items(void);

#ifdef __cplusplus
}
#endif

#endif
