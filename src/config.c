#include <stdio.h>
#include <stdlib.h>

#include "kilate/config.h"
#include "kilate/string.h"
#include "kilate/vector.h"

vector* files = NULL;
vector* libs_directories = NULL;
vector* libs_native_directories = NULL;

void config_init() {
  files = vector_make(sizeof(str));
  libs_directories = vector_make(sizeof(str));
  libs_native_directories = vector_make(sizeof(str));
}

void config_end() {
  for (size_t i = 0; i < files->size; ++i) {
    str filename = *(str*)vector_get(files, i);
    free(filename);
  }

  for (size_t i = 0; i < libs_directories->size; ++i) {
    str lib = *(str*)vector_get(libs_directories, i);
    free(lib);
  }

  for (size_t i = 0; i < libs_native_directories->size; ++i) {
    str lib = *(str*)vector_get(libs_native_directories, i);
    free(lib);
  }

  vector_delete(libs_directories);
  vector_delete(libs_native_directories);
  vector_delete(files);
}