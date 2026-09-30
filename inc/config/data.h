//
// Created by ir0n1c on 9/29/2026.
//

#ifndef GRANNYPRACTICETOOL_DATA_H
#define GRANNYPRACTICETOOL_DATA_H

#include "../granny/unityengine/ue_vector3.h"

#ifdef __cplusplus
extern "C" {
#endif

// this is the pwd that will be seen as the first object of the json file. if it
// isnt found then the loader will refuse to load
extern const char cfg_pwd[];

extern UnityEngine_Vector3_o player_tp_pos;
extern UnityEngine_Vector3_o granny_tp_pos;

#ifdef __cplusplus
}
#endif

#endif // GRANNYPRACTICETOOL_DATA_H
