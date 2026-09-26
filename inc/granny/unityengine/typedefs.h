/*
 * typedefs.h
 *
 *  Created on: Sep 24, 2026
 *      Author: ir0n1c
 */

// same thing as structs.h
#ifndef INC_GRANNY_UNITYENGINE_TYPEDEFS_H_
#define INC_GRANNY_UNITYENGINE_TYPEDEFS_H_

#include <inttypes.h>
#include "structs.h"

// for scenes
typedef int64_t (__fastcall *UnityEngine_SceneManagement_SceneManager_LoadScene)(System_String_o *scene, void* method);

// unityengine general functions
typedef System_String_o* (__fastcall *UnityEngine_Object_Get_Name)(void* obj, void* method);
typedef UnityEngine_Vector3_o* (__fastcall *UnityEngine_Transform_Get_Position)(UnityEngine_Vector3_o *vec_buf, void* transform, void* method);
typedef void (__fastcall *UnityEngine_Transform_Set_Position)(void *transform, UnityEngine_Vector3_o *vec_buf, void* method);
typedef void* (__fastcall *UnityEngine_GameObject_Get_Transform)(void* game_object, void* method);
typedef void* (__fastcall *UnityEngine_Component_Get_GameObject)(void* component, void* method);

// il2cpp
// so that we can call functions in the mod thread
typedef void* (*il2cpp_domain_get_t)(void);
typedef void* (*il2cpp_thread_attach_t)(void* domain);
typedef void  (*il2cpp_thread_detach_t)(void* thread);
typedef void* (*il2cpp_string_new_t)(const char*);

#endif /* INC_GRANNY_UNITYENGINE_TYPEDEFS_H_ */
