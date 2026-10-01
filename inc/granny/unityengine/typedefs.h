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
#include <stdbool.h>
#include "structs.h"

// for scenes
typedef int64_t (__fastcall *UnityEngine_SceneManagement_SceneManager_LoadScene)(System_String_o *scene, void* method);

// unityengine general functions
typedef System_String_o* (__fastcall *UnityEngine_Object_Get_Name)(void* obj, void* method);
typedef UnityEngine_Vector3_o* (__fastcall *UnityEngine_Transform_Get_Position)(UnityEngine_Vector3_o *vec_buf, void* transform, void* method);
typedef void (__fastcall *UnityEngine_Transform_Set_Position)(void *transform, UnityEngine_Vector3_o *vec_buf, void* method);
typedef void* (__fastcall *UnityEngine_GameObject_Get_Transform)(void* game_object, void* method);
typedef void* (__fastcall *UnityEngine_Component_Get_GameObject)(void* component, void* method);
typedef void*  (__fastcall *UnityEngine_Component_Get_Transform)(void* transform, void* method);
typedef void* (__fastcall *UnityEngine_Object_Instantiate)(void* original, UnityEngine_Vector3_o* position, UnityEngine_Quaternion_o* rotation, void* method);
typedef void (__fastcall *UnityEngine_GameObject_Get_Component)(void* game_object, void** out_component, void* method);
typedef bool (__fastcall *UnityEngine_Behaviour_Get_Enabled)(void* behaviour, void* method);
typedef void (__fastcall *UnityEngine_Behaviour_Set_Enabled)(void* behaviour, bool value, void* method);
typedef void (__fastcall *UnityEngine_Collider_Set_Enabled)(void* collider, bool value, void* method);
typedef bool (__fastcall *UnityEngine_NavMeshAgent_Warp)(void* agent, UnityEngine_Vector3_o* position, void* method);
typedef bool (__fastcall *UnityEngine_Object_Op_Implicit)(void* obj, void* method);
typedef void (__fastcall *UnityEngine_Object_Destroy)(void* obj, void* method);
typedef bool (__fastcall *UnityEngine_NavMesh_Sample_Position)(UnityEngine_Vector3_o* source_position, UnityEngine_AI_NavMeshHit_o* hit, float max_distance, int32_t area_mask, void* method);
typedef void (__fastcall *MobileFPS_Update)(void* mobile_fps, void* method);
typedef void (__fastcall *SeedManager_GeneratePlacement)(void* seed_manager, void* method);
typedef void (__fastcall *ObjectsManager_Start)(void* objects_manager, void* method);
typedef void* (__fastcall *UnityEngine_Object_Find_Objects_Of_Type)(void* type, bool include_inactive, void* method);
typedef void (__fastcall *PickRay_Update)(void* pick_ray, void* method);
typedef void* (__fastcall *il2cpp_init_runtime_metadata_t)(void** metadata);

// il2cpp
// so that we can call functions in the mod thread
typedef void* (*il2cpp_domain_get_t)(void);
typedef void* (*il2cpp_thread_attach_t)(void* domain);
typedef void  (*il2cpp_thread_detach_t)(void* thread);
typedef void* (*il2cpp_string_new_t)(const char*);
typedef void* (*il2cpp_domain_assembly_open_t)(void* domain, const char* name);
typedef void* (*il2cpp_assembly_get_image_t)(void* assembly);
typedef void* (*il2cpp_class_from_name_t)(void* image, const char* name_space, const char* name);
typedef void* (*il2cpp_class_get_type_t)(void* klass);
typedef void* (*il2cpp_type_get_object_t)(void* type);
typedef uint32_t (*il2cpp_gchandle_new_t)(void* obj, bool pinned);
typedef void* (*il2cpp_gchandle_get_target_t)(uint32_t handle);
typedef void (*il2cpp_gchandle_free_t)(uint32_t handle);

#endif /* INC_GRANNY_UNITYENGINE_TYPEDEFS_H_ */
