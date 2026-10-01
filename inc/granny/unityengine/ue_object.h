/*
 * ue_object.h
 *
 *  Created on: Sep 24, 2026
 *      Author: ir0n1c
 */

#ifndef INC_GRANNY_UNITYENGINE_UE_OBJECT_H_
#define INC_GRANNY_UNITYENGINE_UE_OBJECT_H_

#include "structs.h"
#include <stdbool.h>
#include <stdint.h>

char *ue_obj_get_obj_name(System_String_o* str);
void* create_ue_string(char* str);
bool ue_object_alive(void* obj);

uint32_t ue_handle_new(void* obj);
void* ue_handle_target(uint32_t handle);
void* ue_handle_alive_target(uint32_t handle);
void ue_handle_set(uint32_t* handle, void* obj);
void ue_handle_free(uint32_t* handle);

#endif /* INC_GRANNY_UNITYENGINE_UE_OBJECT_H_ */
