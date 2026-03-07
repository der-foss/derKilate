#ifndef __CONFIG__
#define __CONFIG__

#include "kilate/vector.h"

#ifdef __cplusplus
extern "C" {
#endif

extern vector* files;
extern vector* libs_directories;
extern vector* libs_native_directories;

void config_init();
void config_end();

#ifdef __cplusplus
}
#endif

#endif