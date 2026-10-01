#include "granny/item_spawn.h"
#include "config/data.h"
#include "core/core.h"
#include "granny/unityengine/typedefs.h"
#include "granny/unityengine/ue_object.h"
#include "runtime_constants.h"

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEM_DEFS 128
#define MAX_SPAWNED_COPIES 256

typedef struct item_def_name {
	char item_name[ITEM_LOCATION_NAME_SIZE];
	char display_name[ITEM_LOCATION_NAME_SIZE];
	int item;
} item_def_name;

typedef struct scanned_item {
	char key[ITEM_LOCATION_NAME_SIZE];
	UnityEngine_Vector3_o pos;
} scanned_item;

typedef struct spawned_copy {
	uint32_t spawner;
	uint32_t item_object;
} spawned_copy;

static SRWLOCK location_lock = SRWLOCK_INIT;
static item_location locations[MAX_ITEM_LOCATIONS];
static int location_count = 0;
static volatile LONG locations_version = 0;
static volatile LONG locations_dirty = 0;

static scanned_item scanned[MAX_ITEM_LOCATIONS];

static uint32_t inventory_handle = 0;
static volatile LONG inventory_ready = 0;
static uint32_t item_seed_data_type_handle = 0;

static item_def_name item_defs[MAX_ITEM_DEFS];
static int item_def_count = 0;
static char display_names[ITEM_LAST + 1][ITEM_LOCATION_NAME_SIZE];
static volatile LONG names_resolved = 0;

static spawned_copy spawned_copies[MAX_SPAWNED_COPIES];
static int spawned_copy_count = 0;

static void copy_string(System_String_o* str, char* out, size_t out_size) {
	out[0] = '\0';
	if (!str)
		return;

	char* s = ue_obj_get_obj_name(str);
	if (s)
		strncpy_s(out, out_size, s, _TRUNCATE);
	free(s);
}

static void clean_display_name(char* s) {
	char* out = s;
	bool in_tag = false;

	for (char* c = s; *c; c++) {
		if (*c == '<') {
			in_tag = true;
			continue;
		}
		if (in_tag) {
			if (*c == '>')
				in_tag = false;
			continue;
		}
		*out++ = (*c == '\n' || *c == '\r' || *c == '\t') ? ' ' : *c;
	}
	*out = '\0';

	char* start = s;
	while (*start == ' ')
		start++;

	size_t len = strlen(start);
	while (len > 0 && start[len - 1] == ' ')
		len--;

	memmove(s, start, len);
	s[len] = '\0';
}

static void format_numbered(char* out, const char* base, int n) {
	if (n <= 1) {
		strncpy_s(out, ITEM_LOCATION_NAME_SIZE, base, _TRUNCATE);
		return;
	}

	char suffix[16];
	int suffix_len = snprintf(suffix, sizeof(suffix), " (%d)", n);
	snprintf(out, ITEM_LOCATION_NAME_SIZE, "%.*s%s", ITEM_LOCATION_NAME_SIZE - 1 - suffix_len, base, suffix);
}

static void make_display_names_unique(void) {
	bool duplicate[ITEM_LAST + 1] = { false };

	for (int i = ITEM_FIRST; i <= ITEM_LAST; i++) {
		for (int j = i + 1; j <= ITEM_LAST; j++) {
			if (_stricmp(display_names[i], display_names[j]) == 0)
				duplicate[i] = duplicate[j] = true;
		}
	}

	for (int i = ITEM_FIRST; i <= ITEM_LAST; i++) {
		if (!duplicate[i] || _stricmp(display_names[i], item_names[i]) == 0)
			continue;

		char suffix[ITEM_LOCATION_NAME_SIZE];
		int suffix_len = snprintf(suffix, sizeof(suffix), " (%s)", item_names[i]);
		char unique[ITEM_LOCATION_NAME_SIZE];
		snprintf(unique, sizeof(unique), "%.*s%s", ITEM_LOCATION_NAME_SIZE - 1 - suffix_len, display_names[i], suffix);
		strcpy_s(display_names[i], ITEM_LOCATION_NAME_SIZE, unique);
	}
}

static void resolve_item_names(uint8_t* inventory) {
	uint8_t* list = *(uint8_t**)(inventory + INVENTORY_ITEM_DEFS);
	uint8_t* array = list ? *(uint8_t**)(list + IL2CPP_LIST_ITEMS) : NULL;
	if (!array)
		return;

	int32_t size = *(int32_t*)(list + IL2CPP_LIST_SIZE);
	uint32_t capacity = *(uint32_t*)(array + IL2CPP_ARRAY_MAX_LENGTH);
	uint8_t** defs = (uint8_t**)(array + IL2CPP_ARRAY_ITEMS);

	bool assigned[ITEM_LAST + 1] = { false };
	for (int i = ITEM_FIRST; i <= ITEM_LAST; i++)
		strcpy_s(display_names[i], ITEM_LOCATION_NAME_SIZE, item_names[i]);

	item_def_count = 0;
	for (int32_t i = 0; i < size && (uint32_t)i < capacity && item_def_count < MAX_ITEM_DEFS; i++) {
		uint8_t* def = defs[i];
		if (!def)
			continue;

		item_def_name* entry = &item_defs[item_def_count];
		copy_string(*(System_String_o**)(def + ITEM_DEFS_ITEM_NAME), entry->item_name, sizeof(entry->item_name));
		copy_string(*(System_String_o**)(def + ITEM_DEFS_TEXT_HIGHLIGHTED), entry->display_name, sizeof(entry->display_name));
		clean_display_name(entry->display_name);
		entry->item = *(int32_t*)(def + ITEM_DEFS_ITEM_INT);

		if (!entry->display_name[0])
			strcpy_s(entry->display_name, sizeof(entry->display_name), entry->item_name);
		if (!entry->display_name[0])
			continue;

		if (entry->item >= ITEM_FIRST && entry->item <= ITEM_LAST && !assigned[entry->item]) {
			strcpy_s(display_names[entry->item], ITEM_LOCATION_NAME_SIZE, entry->display_name);
			assigned[entry->item] = true;
		}

		item_def_count++;
	}

	make_display_names_unique();
	InterlockedExchange(&names_resolved, 1);
}

const char* get_item_display_name(int item) {
	if (item < ITEM_FIRST || item > ITEM_LAST)
		return "None";
	return names_resolved ? display_names[item] : item_names[item];
}

static const char* location_label(const char* key) {
	for (int i = 0; i < item_def_count; i++) {
		if (strcmp(item_defs[i].item_name, key) != 0)
			continue;

		int item = item_defs[i].item;
		if (item >= ITEM_FIRST && item <= ITEM_LAST)
			return display_names[item];
		return item_defs[i].display_name;
	}
	return key;
}

static void* get_item_seed_data_type(void) {
	void* cached = ue_handle_target(item_seed_data_type_handle);
	if (cached)
		return cached;

	void* domain = ((il2cpp_domain_get_t)(game_assembly_base + IL2CPP_DOMAIN_GET))();
	void* assembly = domain ? ((il2cpp_domain_assembly_open_t)(game_assembly_base + IL2CPP_DOMAIN_ASSEMBLY_OPEN))(domain, "Assembly-CSharp") : NULL;
	void* image = assembly ? ((il2cpp_assembly_get_image_t)(game_assembly_base + IL2CPP_ASSEMBLY_GET_IMAGE))(assembly) : NULL;
	void* klass = image ? ((il2cpp_class_from_name_t)(game_assembly_base + IL2CPP_CLASS_FROM_NAME))(image, "", "ItemSeedData") : NULL;
	void* type = klass ? ((il2cpp_class_get_type_t)(game_assembly_base + IL2CPP_CLASS_GET_TYPE))(klass) : NULL;
	void* type_object = type ? ((il2cpp_type_get_object_t)(game_assembly_base + IL2CPP_TYPE_GET_OBJECT))(type) : NULL;

	item_seed_data_type_handle = ue_handle_new(type_object);
	return type_object;
}

static bool is_spawned_copy(void* game_object) {
	for (int i = 0; i < spawned_copy_count; i++) {
		if (ue_handle_target(spawned_copies[i].item_object) == game_object)
			return true;
	}
	return false;
}

static int compare_float(float a, float b) {
	return a < b ? -1 : (a > b ? 1 : 0);
}

static int compare_scanned(const void* a, const void* b) {
	const scanned_item* x = a;
	const scanned_item* y = b;

	int c = strcmp(x->key, y->key);
	if (c == 0)
		c = compare_float(x->pos.fields.x, y->pos.fields.x);
	if (c == 0)
		c = compare_float(x->pos.fields.z, y->pos.fields.z);
	if (c == 0)
		c = compare_float(x->pos.fields.y, y->pos.fields.y);
	return c;
}

static int scan_items(void) {
	void* type = get_item_seed_data_type();
	if (!type)
		return 0;

	uint8_t* objects = ((UnityEngine_Object_Find_Objects_Of_Type)(game_assembly_base + UNITYENGINE_OBJECT_FIND_OBJECTS_OF_TYPE))(type, false, NULL);
	if (!objects)
		return 0;

	uint32_t object_count = *(uint32_t*)(objects + IL2CPP_ARRAY_MAX_LENGTH);
	uint8_t** items = (uint8_t**)(objects + IL2CPP_ARRAY_ITEMS);

	int count = 0;
	for (uint32_t i = 0; i < object_count && count < MAX_ITEM_LOCATIONS; i++) {
		uint8_t* seed_data = items[i];
		if (!seed_data)
			continue;

		void* game_object = ((UnityEngine_Component_Get_GameObject)(game_assembly_base + UNITYENGINE_COMPONENT_GET_GAMEOBJECT))(seed_data, NULL);
		if (is_spawned_copy(game_object))
			continue;

		void* transform = ((UnityEngine_Component_Get_Transform)(game_assembly_base + UNITYENGINE_COMPONENT_GET_TRANSFORM))(seed_data, NULL);
		if (!transform)
			continue;

		scanned_item* entry = &scanned[count];
		copy_string(*(System_String_o**)(seed_data + ITEM_SEED_DATA_ITEM_NAME), entry->key, sizeof(entry->key));
		if (!entry->key[0])
			strcpy_s(entry->key, sizeof(entry->key), "item");

		((UnityEngine_Transform_Get_Position)(game_assembly_base + UNITYENGINE_TRANSFORM_GET_POSITION))(&entry->pos, transform, NULL);
		count++;
	}

	return count;
}

void item_locations_refresh(void) {
	int count = scan_items();
	qsort(scanned, count, sizeof(scanned_item), compare_scanned);

	AcquireSRWLockExclusive(&location_lock);

	int n = 0;
	for (int i = 0; i < count; i++) {
		n = (i > 0 && strcmp(scanned[i].key, scanned[i - 1].key) == 0) ? n + 1 : 1;

		item_location* loc = &locations[i];
		format_numbered(loc->key, scanned[i].key, n);
		format_numbered(loc->label, location_label(scanned[i].key), n);
		loc->pos = scanned[i].pos;
	}

	location_count = count;
	InterlockedIncrement(&locations_version);

	ReleaseSRWLockExclusive(&location_lock);
}

void item_locations_mark_dirty(void) {
	InterlockedExchange(&locations_dirty, 1);
}

long item_locations_version(void) {
	return InterlockedCompareExchange(&locations_version, 0, 0);
}

int copy_item_locations(item_location* out, int max) {
	AcquireSRWLockShared(&location_lock);
	int count = location_count < max ? location_count : max;
	memcpy(out, locations, sizeof(item_location) * count);
	ReleaseSRWLockShared(&location_lock);

	return count;
}

static void free_spawned_copies(void) {
	for (int i = 0; i < spawned_copy_count; i++) {
		ue_handle_free(&spawned_copies[i].spawner);
		ue_handle_free(&spawned_copies[i].item_object);
	}
	spawned_copy_count = 0;
}

void item_spawn_reset(void) {
	AcquireSRWLockExclusive(&location_lock);
	location_count = 0;
	InterlockedIncrement(&locations_version);
	ReleaseSRWLockExclusive(&location_lock);

	ue_handle_free(&inventory_handle);
	InterlockedExchange(&inventory_ready, 0);
	free_spawned_copies();
	InterlockedExchange(&locations_dirty, 0);
}

bool has_item_inventory(void) {
	return InterlockedCompareExchange(&inventory_ready, 0, 0) != 0;
}

void item_spawn_tick(void* inventory) {
	if (inventory != ue_handle_target(inventory_handle)) {
		ue_handle_set(&inventory_handle, inventory);
		InterlockedExchange(&inventory_ready, inventory != NULL);
		if (inventory)
			item_locations_mark_dirty();
	}

	if (!ue_object_alive(inventory))
		return;

	if (!names_resolved)
		resolve_item_names(inventory);

	if (InterlockedExchange(&locations_dirty, 0))
		item_locations_refresh();
}

static void* resolve_metadata(uintptr_t rva) {
	void** metadata = (void**)(game_assembly_base + rva);
	((il2cpp_init_runtime_metadata_t)(game_assembly_base + IL2CPP_INIT_RUNTIME_METADATA))(metadata);
	return *metadata;
}

static void destroy_object(void* obj) {
	if (ue_object_alive(obj))
		((UnityEngine_Object_Destroy)(game_assembly_base + UNITYENGINE_OBJECT_DESTROY))(obj, NULL);
}

static void destroy_spawned_copies(void) {
	for (int i = 0; i < spawned_copy_count; i++) {
		destroy_object(ue_handle_alive_target(spawned_copies[i].spawner));
		destroy_object(ue_handle_alive_target(spawned_copies[i].item_object));
	}
	free_spawned_copies();
}

void spawn_item_at(int item, const UnityEngine_Vector3_o* pos) {
	if (item < ITEM_FIRST || item > ITEM_LAST || !pos)
		return;

	uint8_t* inventory = ue_handle_alive_target(inventory_handle);
	if (!inventory)
		return;

	void* prefab = *(void**)(inventory + INVENTORY_ITEMS_PREFAB);
	if (!ue_object_alive(prefab))
		return;

	void* instantiate_method = resolve_metadata(METHOD_OBJECT_INSTANTIATE_GAMEOBJECT);
	void* get_component_method = resolve_metadata(METHOD_GAMEOBJECT_GET_COMPONENT_ITEMSPAWN);

	UnityEngine_Vector3_o spawn_pos = *pos;
	UnityEngine_Quaternion_o spawn_rot = { 0.0f, 0.0f, 0.0f, 1.0f };

	void* spawned = ((UnityEngine_Object_Instantiate)(game_assembly_base + UNITYENGINE_OBJECT_INSTANTIATE))(prefab, &spawn_pos, &spawn_rot, instantiate_method);
	if (!spawned)
		return;

	ItemSpawn_o* item_spawn = NULL;
	((UnityEngine_GameObject_Get_Component)(game_assembly_base + UNITYENGINE_GAMEOBJECT_GET_COMPONENT))(spawned, (void**)&item_spawn, get_component_method);

	void* item_object = item_spawn ? *(void**)((uint8_t*)item_spawn + itemspawn_item_offsets[item]) : NULL;
	if (!ue_object_alive(item_object)) {
		destroy_object(spawned);
		return;
	}

	item_spawn->fields.CountItem = (float)item;
	item_spawn->fields.DropForceItem = 0.0f;

	void* item_transform = ((UnityEngine_GameObject_Get_Transform)(game_assembly_base + UNITYENGINE_GAMEOBJECT_GET_TRANSFORM))(item_object, NULL);
	if (item_transform)
		((UnityEngine_Transform_Set_Position)(game_assembly_base + UNITYENGINE_TRANSFOM_SET_POSITION))(item_transform, &spawn_pos, NULL);

	if (spawned_copy_count < MAX_SPAWNED_COPIES) {
		spawned_copies[spawned_copy_count].spawner = ue_handle_new(spawned);
		spawned_copies[spawned_copy_count].item_object = ue_handle_new(item_object);
		spawned_copy_count++;
	}
}

void spawn_selected_items(void) {
	if (!ue_handle_alive_target(inventory_handle))
		return;

	item_location* copy = malloc(sizeof(item_location) * MAX_ITEM_LOCATIONS);
	if (!copy)
		return;

	destroy_spawned_copies();

	int count = copy_item_locations(copy, MAX_ITEM_LOCATIONS);
	for (int i = 0; i < count; i++) {
		int item = get_item_selection(copy[i].key);
		if (item != 0)
			spawn_item_at(item, &copy[i].pos);
	}

	free(copy);
}
