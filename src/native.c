#include "kilate/native.h"

#include <dirent.h>
#include <dlfcn.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kilate/config.h"
#include "kilate/lexer.h"
#include "kilate/node.h"
#include "kilate/string.h"
#include "kilate/vector.h"

node_vector* native_functions = NULL;

void native_init() {
  native_functions = vector_make(sizeof(native_fnentry*));
  native_load_extern();
}

void native_load_extern() {
  // Load ALL Native Libs found
  for (size_t i = 0; i < libs_native_directories->size; i++) {
    str dir = *(str*)vector_get(libs_native_directories, i);
    DIR* d = opendir(dir);
    if (!d)
      return;

    struct dirent* entry;
    while ((entry = readdir(d))) {
      if (strstr(entry->d_name, ".so")) {
        char path[512];
        snprintf(path, sizeof(path), "%s%s", dir, entry->d_name);

        void* handle = dlopen(path, RTLD_NOW);
        if (!handle) {
          fprintf(stderr, "Error loading %s: %s\n", path, dlerror());
          continue;
        }

        void (*extern_native_init)() = dlsym(handle, "KILATE_NATIVE_REGISTER");
        if (!extern_native_init) {
          fprintf(stderr, "Function KILATE_NATIVE_REGISTER not found in %s\n",
                  path);
          continue;
        }
        extern_native_init();
      }
    }
    closedir(d);
  }
}

void native_end() {
  for (size_t i = 0; i < native_functions->size; ++i) {
    native_fnentry* entry =
        *(native_fnentry**)vector_get(native_functions, i);
    free(entry->name);
    if (entry->requiredParams != NULL)
      vector_delete(entry->requiredParams);
    free(entry);
  }
  vector_delete(native_functions);
}

void native_register_fnentry(native_fnentry* entry) {
  vector_push_back(native_functions, &entry);
}

void native_register_fn(str name,
                            str_vector* requiredParams,
                            native_fn fn) {
  native_fnentry* entry = malloc(sizeof(native_fnentry));
  entry->name = strdup(name);
  entry->fn = fn;
  entry->requiredParams = requiredParams;
  native_register_fnentry(entry);
}

native_fnentry* native_find_function(str name) {
  for (size_t i = 0; i < native_functions->size; ++i) {
    native_fnentry* entry =
        *(native_fnentry**)vector_get(native_functions, i);
    if (str_equals(entry->name, name)) {
      return entry;
    }
  }
  return NULL;
}