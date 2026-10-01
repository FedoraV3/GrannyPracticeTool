// definition of unity structs

#ifndef STRUCTS_H
#define STRUCTS_H

#include <inttypes.h>
#include <stdbool.h>

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

typedef struct UnityEngine_Quaternion_o
{
	float x;
	float y;
	float z;
	float w;
} UnityEngine_Quaternion_o;

typedef struct UnityEngine_AI_NavMeshHit_o
{
	UnityEngine_Vector3_o m_Position;
	UnityEngine_Vector3_o m_Normal;
	float m_Distance;
	int32_t m_Mask;
	int32_t m_Hit;
} UnityEngine_AI_NavMeshHit_o;

typedef struct ItemSpawn_Fields {
    uint8_t header[0x10]; // 0x00 il2cpp object header
    bool    Spawned;      // 0x10
    float   CountItem;    // 0x14 (compiler pads 0x11-0x13)
    struct UnityEngine_GameObject_o *crossbow;       // 0x18
    struct UnityEngine_GameObject_o *plier;          // 0x20
    struct UnityEngine_GameObject_o *battery;        // 0x28
    struct UnityEngine_GameObject_o *gas;            // 0x30
    struct UnityEngine_GameObject_o *seed;           // 0x38
    struct UnityEngine_GameObject_o *book;           // 0x40
    struct UnityEngine_GameObject_o *winch;          // 0x48
    struct UnityEngine_GameObject_o *carbattery;     // 0x50
    struct UnityEngine_GameObject_o *carkey;         // 0x58
    struct UnityEngine_GameObject_o *cutter;         // 0x60
    struct UnityEngine_GameObject_o *code;           // 0x68
    struct UnityEngine_GameObject_o *baton;          // 0x70
    struct UnityEngine_GameObject_o *eckey;          // 0x78
    struct UnityEngine_GameObject_o *hammer;         // 0x80
    struct UnityEngine_GameObject_o *padlock;        // 0x88
    struct UnityEngine_GameObject_o *mas;            // 0x90
    struct UnityEngine_GameObject_o *kugg1;          // 0x98
    struct UnityEngine_GameObject_o *kugg2;          // 0xA0
    struct UnityEngine_GameObject_o *meat;           // 0xA8
    struct UnityEngine_GameObject_o *melon;          // 0xB0
    struct UnityEngine_GameObject_o *spray;          // 0xB8
    struct UnityEngine_GameObject_o *plank;          // 0xC0
    struct UnityEngine_GameObject_o *playh;          // 0xC8
    struct UnityEngine_GameObject_o *remote;         // 0xD0
    struct UnityEngine_GameObject_o *data;           // 0xD8
    struct UnityEngine_GameObject_o *rusty;          // 0xE0
    struct UnityEngine_GameObject_o *safe;           // 0xE8
    struct UnityEngine_GameObject_o *screw;          // 0xF0
    struct UnityEngine_GameObject_o *shotgun;        // 0xF8
    struct UnityEngine_GameObject_o *sp1;            // 0x100
    struct UnityEngine_GameObject_o *sp2;            // 0x108
    struct UnityEngine_GameObject_o *sp3;            // 0x110
    struct UnityEngine_GameObject_o *shotgun2;       // 0x118
    struct UnityEngine_GameObject_o *spark;          // 0x120
    struct UnityEngine_GameObject_o *special;        // 0x128
    struct UnityEngine_GameObject_o *spider;         // 0x130
    struct UnityEngine_GameObject_o *syringe;        // 0x138
    struct UnityEngine_GameObject_o *t1;             // 0x140
    struct UnityEngine_GameObject_o *t2;             // 0x148
    struct UnityEngine_GameObject_o *t3;             // 0x150
    struct UnityEngine_GameObject_o *t4;             // 0x158
    struct UnityEngine_GameObject_o *teddy;          // 0x160
    struct UnityEngine_GameObject_o *text;           // 0x168
    struct UnityEngine_GameObject_o *topp;           // 0x170
    struct UnityEngine_GameObject_o *wp;             // 0x178
    struct UnityEngine_GameObject_o *vas;            // 0x180
    struct UnityEngine_GameObject_o *vas2;           // 0x188
    struct UnityEngine_GameObject_o *vas3;           // 0x190
    struct UnityEngine_GameObject_o *wheel;          // 0x198
    struct UnityEngine_GameObject_o *stick;          // 0x1A0
    struct UnityEngine_GameObject_o *wrench;         // 0x1A8
    struct UnityEngine_GameObject_o *rat;            // 0x1B0
    struct UnityEngine_GameObject_o *ornamentbomb;   // 0x1B8
    struct UnityEngine_GameObject_o *ornamentfreeze; // 0x1C0
    struct UnityEngine_GameObject_o *fuse;           // 0x1C8
    float   DropForceItem;                           // 0x1D0 (tail pad to 0x1D8)
} ItemSpawn_Fields;

typedef struct ItemSpawn_o // sizeof=0x1E8
{
	void *klass;
	void *monitor;
	ItemSpawn_Fields fields;
} ItemSpawn_o;

#endif