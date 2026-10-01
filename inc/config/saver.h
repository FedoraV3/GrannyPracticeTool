//
// Created by ir0n1c on 9/29/2026.
//

#ifndef GRANNYPRACTICETOOL_SAVER_H
#define GRANNYPRACTICETOOL_SAVER_H

#include <stdbool.h>

// saves the values in data.h to %localappdata%\GrannyPracticeTool\<cfg_name>.sh0k0cfg
#ifdef __cplusplus
extern "C" {
#endif

bool save_cfg(const char* cfg_name);

#ifdef __cplusplus
}
#endif

#endif // GRANNYPRACTICETOOL_SAVER_H
