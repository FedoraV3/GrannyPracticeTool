// handles anything for the ue_object

#include "granny/unityengine/ue_object.h"
#include "core/core.h"
#include "granny/unityengine/structs.h"
#include "granny/unityengine/typedefs.h"
#include <string.h>
#include <Windows.h>
#include <stringapiset.h>
#include <winnls.h>

// returns a null terminated string
char *ue_obj_get_obj_name(System_String_o *str) {
	int wlen = str->fields.str_length;
	LPCWCH wsrc = (LPCWCH)&str->fields.first_char;

	if (wlen == 0) {
		char *empty = malloc(1);
		if (empty) empty[0] = '\0';
		return empty;
	}

	int size = WideCharToMultiByte(
		CP_UTF8, 
		0, 
		wsrc, 
		wlen, 
		NULL, 
		0, 
		NULL, 
		NULL
	);
	if (size == 0) return NULL;

	char *buffer = malloc(size + 1);
	if (!buffer) return NULL;

	if (WideCharToMultiByte(
		CP_UTF8, 
		0, 
		wsrc, 
		wlen, 
		buffer, 
		size, 
		NULL, 
		NULL
	) == 0) {
		free(buffer);
		return NULL;
	}

	buffer[size] = '\0';
	return buffer;
}

// requires a null terminated string
void* create_ue_string(char* str) {
	// typedef void* (*il2cpp_string_new_t)(const char*);
	return ((il2cpp_string_new_t)(game_assembly_base + IL2CPP_STRING_NEW))(str);
}

bool ue_object_alive(void* obj) {
	return obj != NULL && ((UnityEngine_Object_Op_Implicit)(game_assembly_base + UNITYENGINE_OBJECT_OP_IMPLICIT))(obj, NULL);
}

uint32_t ue_handle_new(void* obj) {
	return obj ? ((il2cpp_gchandle_new_t)(game_assembly_base + IL2CPP_GCHANDLE_NEW))(obj, false) : 0;
}

void* ue_handle_target(uint32_t handle) {
	return handle ? ((il2cpp_gchandle_get_target_t)(game_assembly_base + IL2CPP_GCHANDLE_GET_TARGET))(handle) : NULL;
}

void* ue_handle_alive_target(uint32_t handle) {
	void* obj = ue_handle_target(handle);
	return ue_object_alive(obj) ? obj : NULL;
}

void ue_handle_free(uint32_t* handle) {
	if (*handle)
		((il2cpp_gchandle_free_t)(game_assembly_base + IL2CPP_GCHANDLE_FREE))(*handle);
	*handle = 0;
}

void ue_handle_set(uint32_t* handle, void* obj) {
	if (ue_handle_target(*handle) == obj)
		return;

	ue_handle_free(handle);
	*handle = ue_handle_new(obj);
}
