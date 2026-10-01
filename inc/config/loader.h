//
// Created by ir0n1c on 9/29/2026.
//

#ifndef GRANNYPRACTICETOOL_LOADER_H
#define GRANNYPRACTICETOOL_LOADER_H

#include <stdbool.h>

// loads %localappdata%\GrannyPracticeTool\<cfg_name>.sh0k0cfg into the values in data.h
// data.h is only touched if the whole file is valid
#ifdef __cplusplus
extern "C" {
#endif

bool load_cfg(const char* cfg_name);
void unload_cfg(void);

#ifdef __cplusplus
}
#endif

#endif // GRANNYPRACTICETOOL_LOADER_H
