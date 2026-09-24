// handles anything for the ue_object

#include "granny/unityengine/ue_object.h"
#include "core/core.h"
#include "granny/unityengine/structs.h"
#include "granny/unityengine/typedefs.h"
#include <string.h>
#include <Windows.h>

// returns a null terminated string
char *ue_obj_get_obj_name(System_String_o* str) {	
	char* buffer = malloc(str->fields.str_length);
	if (buffer == NULL) {
		free(str); 
		return buffer; 
	}
	
	// copy the string to the new char
	/*
	for (int i = 0; i < str->fields.str_length; i++) {
		printf("%c", ((uint16_t*)&str->fields.first_char)[i]);
	}
	*/
	
	// we don't need to use memcpy_s since they both will be always the same size
	// if im wrong haha then boom my program crashes
	memcpy(buffer, &str->fields.first_char, str->fields.str_length);
	
	// i wrote this and commented it i didnt use it hahahahahaha
	// free(buffer);
	return buffer;
}

uint16_t *create_ue_str_from_str(char* buffer, size_t buffer_s) {
	uint16_t *str = malloc(buffer_s);
	if (str == NULL)
		return str;
	
	if (MultiByteToWideChar(
		CP_ACP, 
		MB_PRECOMPOSED, 
		buffer, 
		buffer_s, 
		str, 
		buffer_s
	) == 0) {
		free(str);
		return NULL;
	}
	
	return str;
}