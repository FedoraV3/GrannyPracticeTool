/*
 * ue_object.h
 *
 *  Created on: Sep 24, 2026
 *      Author: ir0n1c
 */

#ifndef INC_GRANNY_UNITYENGINE_UE_OBJECT_H_
#define INC_GRANNY_UNITYENGINE_UE_OBJECT_H_

#include "structs.h"
#include <stdint.h>

char *ue_obj_get_obj_name(System_String_o* str);
uint16_t *create_ue_str_from_str(char* buffer, size_t buffer_s);

#endif /* INC_GRANNY_UNITYENGINE_UE_OBJECT_H_ */
