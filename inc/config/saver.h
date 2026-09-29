//
// Created by ir0n1c on 9/29/2026.
//

#ifndef GRANNYPRACTICETOOL_SAVER_H
#define GRANNYPRACTICETOOL_SAVER_H

#include <stdbool.h>

// saves the values in data.h to %localappdata%\GrannyPracticeTool\<cfg_name>.json
bool save_cfg(const char* cfg_name);

#endif // GRANNYPRACTICETOOL_SAVER_H
