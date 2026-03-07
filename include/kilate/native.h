#ifndef __NATIVE_H__
#define __NATIVE_H__

#include "kilate/environment.h"
#include "kilate/lexer.h"
#include "kilate/node.h"
#include "kilate/parser.h"
#include "kilate/vector.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  environment* env;
  node_fnparam_vector* params;
} native_fndata;

typedef node* (*native_fn)(native_fndata*);

typedef struct {
  str name;
  str_vector* requiredParams;
  native_fn fn;
} native_fnentry;

#define KILATE_NATIVE_REGISTER() void KILATE_NATIVE_REGISTER()

extern node_vector* native_functions;

void native_init();

void native_load_extern();

void native_end();

void native_register_fnentry(native_fnentry*);

void native_register_fn(str, str_vector*, native_fn);

native_fnentry* native_find_function(str);

void native_register_all_functions();

#ifdef __cplusplus
}
#endif

#endif