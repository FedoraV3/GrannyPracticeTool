#ifndef AI_GRANNY_H
#define AI_GRANNY_H

#include <stdbool.h>

extern void* granny_ai_ptr;

#define GRANNY_AI_FIXED_UPDATE_RVA 0x1bde60
bool ai_granny_hook_install();

#endif