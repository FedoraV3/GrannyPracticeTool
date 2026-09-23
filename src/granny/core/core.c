#include "granny/core/core.h"
#include <inttypes.h>

void* game_assembly_base = NULL;
// i wonder if this is compatible for other windows since this is a font variable
// or nuklear can handle this part for me
struct nk_user_font *nuklear_font;
struct nk_context *nuklear_ctx;


__declspec(align(8)) struct System_String_Fields // sizeof=0x8
{
	int32_t str_length;
	uint16_t first_char;
	uint8_t padding[2];
};

// need this to make it 24
typedef struct System_String_o // sizeof=0x18
{
	 void* unused;
	 void* unused2;
     struct System_String_Fields fields;
} System_String_o;

// returns a null terminated string
void get_obj_name_to_c_str(void* obj_addr) {
	
}

// basically the most important part of the program, its what starts the heart of the program
void core_init() {
	game_assembly_base = (void*)GetModuleHandleW(L"GameAssembly.dll");
}