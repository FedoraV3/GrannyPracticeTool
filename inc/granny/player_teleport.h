//
// Created by ir0n1c on 9/29/2026.
//

#ifndef GRANNYPRACTICETOOL_PLAYER_TELEPORT_H
#define GRANNYPRACTICETOOL_PLAYER_TELEPORT_H
#include "unityengine/structs.h"

// this looks oddly similar to granny_teleport does it not?
// these are safe to call since it dereferences directly without calling any unity funcs
// but for safety and architecture well we will make them use unity funcs still
void get_player_position(UnityEngine_Vector3_o *out_pos);
void teleport_player_to_position(UnityEngine_Vector3_o *pos);

#endif // GRANNYPRACTICETOOL_PLAYER_TELEPORT_H
