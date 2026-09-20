#include "granny/core/core.h"

void* game_assembly_base = NULL;
// i wonder if this is compatible for other windows since this is a font variable
// or nuklear can handle this part for me
struct nk_user_font *nuklear_font;
struct nk_context *nuklear_ctx;


// basically the most important part of the program, its what starts the heart of the program
void core_init() {
	game_assembly_base = (void*)GetModuleHandleW(L"GameAssembly.dll");
}