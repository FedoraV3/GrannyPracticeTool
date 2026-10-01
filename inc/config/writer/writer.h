//
// Created by ir0n1c on 9/29/2026.
//

#ifndef GRANNYPRACTICETOOL_WRITER_H
#define GRANNYPRACTICETOOL_WRITER_H

#include <stdbool.h>
#include <stddef.h>

#define CFG_EXTENSION ".sh0k0cfg"
#define CFG_NAME_SIZE 64

#ifdef __cplusplus
extern "C" {
#endif

const char* init_path(void);
bool is_valid_cfg_name(const char* cfg_name);
bool build_cfg_file_path(const char* cfg_name, char* out, size_t out_size);
int list_cfgs(char (*names)[CFG_NAME_SIZE], int max);

#ifdef __cplusplus
}
#endif

#endif // GRANNYPRACTICETOOL_WRITER_H
