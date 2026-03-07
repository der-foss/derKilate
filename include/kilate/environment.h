#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "kilate/bool.h"
#include "kilate/node.h"
#include "kilate/string.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct env_entry {
  str name;
  node* value;
  struct env_entry* next;
} env_entry;

typedef struct environment {
  env_entry* variables;
  struct environment* parent;
} environment;

environment* environment_make(environment* parent);

void environment_destroy(environment* env);

bool environment_definevar(environment* env,
                                   const str name,
                                   void* value);

node* environment_getvar(environment* env, const str name);

bool environment_setvar(environment* env,
                                const str name,
                                void* value);

#ifdef __cplusplus
}
#endif

#endif