// definition of unity structs

#ifndef STRUCTS_H
#define STRUCTS_H

#include <inttypes.h>

struct __declspec(align(8)) System_String_Fields // sizeof=0x8
{
	int32_t str_length;
	uint16_t first_char;
	uint8_t padding[2];
};

// need this to make it 24
typedef struct System_String_o // sizeof=0x18
{
	 void* klass;
	 void* monitor;
     struct System_String_Fields fields;
} System_String_o;

typedef struct UnityEngine_SceneManagement_LoadSceneParameters_Fields // sizeof=0x8
{
	int32_t m_LoadSceneMode;
	int32_t m_LocalPhysicsMode;
} UnityEngine_SceneManagement_LoadSceneParameters_Fields;

typedef struct UnityEngine_SceneManagement_LoadSceneParameters_o // sizeof=0x8
{
	UnityEngine_SceneManagement_LoadSceneParameters_Fields fields;
} UnityEngine_SceneManagement_LoadSceneParameters_o;

typedef struct UnityEngine_Vector3_Fields // sizeof=0xC
{
	float x;
	float y;
	float z;
} UnityEngine_Vector3_Fields;

typedef struct UnityEngine_Vector3_o // sizeof=0xC
{
	UnityEngine_Vector3_Fields fields;
} UnityEngine_Vector3_o;

#endif