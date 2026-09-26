/*
 * granny_teleport.h
 *
 *  Created on: Sep 25, 2026
 *      Author: ir0n1c
 */

#ifndef INC_GRANNY_GRANNY_TELEPORT_H_
#define INC_GRANNY_GRANNY_TELEPORT_H_

#include "unityengine/structs.h"
#include <stdbool.h>

bool get_granny_position(UnityEngine_Vector3_o *out_pos);
void teleport_granny_to_position(UnityEngine_Vector3_o *pos);


#endif /* INC_GRANNY_GRANNY_TELEPORT_H_ */
