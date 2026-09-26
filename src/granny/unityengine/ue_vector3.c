/*
 * ue_vector3.c
 *
 *  Created on: Sep 26, 2026
 *      Author: ir0n1c
 */

#include "granny/unityengine/ue_vector3.h"
#include "granny/unityengine/structs.h"
#include <stdlib.h>
 
UnityEngine_Vector3_o *create_vec3(float x, float y, float z) {
	UnityEngine_Vector3_Fields fields = { .x = x, .y = y, .z = z };
	// if you looked at structs.h you will see why i do this
	UnityEngine_Vector3_o *vec = malloc(sizeof(fields));
	if (vec != NULL) {
		vec->fields = fields;
	}

	return vec;
}
