#ifndef CORE_H
#define CORE_H

#include <Windows.h>
#include "lib/nuklear/nuklear.h"

extern void* game_assembly_base;
extern struct nk_user_font font;
extern struct nk_context ctx;

void core_init();
	
#endif