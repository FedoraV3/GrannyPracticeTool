#ifndef AI_GRANNY_H
#define AI_GRANNY_H

#include <stdbool.h>

#define GRANNY_AI_FIXED_UPDATE_RVA 0x1bde60
bool ai_granny_hook_install();
void* get_ai_granny_ptr();

#endif