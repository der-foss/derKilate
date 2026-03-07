#ifndef __FILE_H__
#define __FILE_H__

#include <stdio.h>

#include "kilate/string.h"

#define FILE_MODE_READ "r"
#define FILE_MODE_WRITE "w"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  FILE* std_file;
  str path;
} file;

file* file_open(str, str);

void file_close(file*);

size_t file_get_length(file*);

str file_read_text(file*);

#ifdef __cplusplus
}
#endif

#endif