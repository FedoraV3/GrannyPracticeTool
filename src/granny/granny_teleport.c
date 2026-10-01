/*
 * granny_teleport.c
 *
 *  Created on: Sep 25, 2026
 *      Author: ir0n1c
 */

#include "granny/granny_teleport.h"
#include "core/core.h"
#include "granny/unityengine/structs.h"
#include "granny/unityengine/typedefs.h"
#include "granny/unityengine/ue_object.h"
#include "hooks/AI_Granny/ai_granny.h"
#include "runtime_constants.h"

#define NAVMESH_ALL_AREAS (-1)
#define GRANNY_NAVMESH_SAMPLE_DISTANCE 5.0f

void teleport_granny_to_position(UnityEngine_Vector3_o *pos) {
	void* granny = get_ai_granny();
	if (granny == NULL)
		return;

	void* transform = ((UnityEngine_Component_Get_Transform)(game_assembly_base + UNITYENGINE_COMPONENT_GET_TRANSFORM))(granny, NULL);
	if (transform == NULL)
		return;

	void* agent = *(void**)((uint8_t*)granny + AI_GRANNY_AGENT);
	if (!ue_object_alive(agent) || !((UnityEngine_Behaviour_Get_Enabled)(game_assembly_base + UNITYENGINE_BEHAVIOUR_GET_ENABLED))(agent, NULL)) {
		((UnityEngine_Transform_Set_Position)(game_assembly_base + UNITYENGINE_TRANSFOM_SET_POSITION))(transform, pos, NULL);
		return;
	}

	UnityEngine_Vector3_o target = *pos;
	UnityEngine_AI_NavMeshHit_o hit;
	if (((UnityEngine_NavMesh_Sample_Position)(game_assembly_base + UNITYENGINE_NAVMESH_SAMPLE_POSITION))(&target, &hit, GRANNY_NAVMESH_SAMPLE_DISTANCE, NAVMESH_ALL_AREAS, NULL))
		target = hit.m_Position;

	((UnityEngine_NavMeshAgent_Warp)(game_assembly_base + UNITYENGINE_NAVMESHAGENT_WARP))(agent, &target, NULL);
}
