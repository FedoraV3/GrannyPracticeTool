//
// Created by ir0n1c on 9/29/2026.
//

#ifndef GRANNYPRACTICETOOL_LOADER_H
#define GRANNYPRACTICETOOL_LOADER_H

#include <stdbool.h>

// loads %localappdata%\GrannyPracticeTool\<cfg_name>.json into the values in data.h
// data.h is only touched if the whole file is valid
bool load_cfg(const char* cfg_name);
void unload_cfg(void);

#endif // GRANNYPRACTICETOOL_LOADER_H
