#include "kilate/environment.h"

#include <stdlib.h>
#include <string.h>

#include "kilate/bool.h"
#include "kilate/error.h"
#include "kilate/string.h"

environment* environment_make(environment* parent) {
  environment* env = malloc(sizeof(environment));
  if (env == NULL)
    error_fatal("Failed to allocate environment");

  env->variables = NULL;
  env->parent = parent;
  return env;
}

void environment_destroy(environment* env) {
  if (env == NULL)
    return;

  env_entry* current = env->variables;
  while (current) {
    env_entry* next = current->next;
    free(current->name);
    if (current->value) {
      node_delete((node*)current->value);
    }
    free(current);
    current = next;
  }

  free(env);
}

bool environment_definevar(environment* env,
                                   const str name,
                                   void* value) {
  if (env == NULL || name == NULL)
    error_fatal("Environment or name is null.");

  env_entry* current = env->variables;
  while (current) {
    if (str_equals(current->name, name)) {
      return false;
    }
    current = current->next;
  }

  env_entry* new_entry = malloc(sizeof(env_entry));
  if (!new_entry)
    return false;

  new_entry->name = strdup(name);
  new_entry->value = value;
  new_entry->next = env->variables;

  env->variables = new_entry;
  return true;
}

node* environment_getvar(environment* env, const str name) {
  if (env == NULL || name == NULL)
    error_fatal("Environment or name is null.");

  environment* current_env = env;
  while (current_env) {
    env_entry* current = current_env->variables;
    while (current) {
      if (strcmp(current->name, name) == 0) {
        return current->value;
      }
      current = current->next;
    }
    current_env = current_env->parent;
  }

  return NULL;
}

bool environment_setvar(environment* env,
                                const str name,
                                void* value) {
  if (env == NULL || name == NULL)
    error_fatal("Environment or name is null.");

  environment* current_env = env;
  while (current_env) {
    env_entry* current = current_env->variables;
    while (current) {
      if (strcmp(current->name, name) == 0) {
        current->value = value;
        return true;
      }
      current = current->next;
    }
    current_env = current_env->parent;
  }

  return false;
}