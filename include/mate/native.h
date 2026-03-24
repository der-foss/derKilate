#ifndef __NATIVE_H__
#define __NATIVE_H__

#include "mate/environment.h"
#include "mate/interpreter.h"
#include "mate/string.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct native_fndata_t {
        interpreter_t *inter;
        node_arg_vector_t *args;
} native_fndata_t;

typedef return_node_t *(*native_fn_t)(native_fndata_t *);

#define MATE_NATIVE_REGISTER() void MATE_NATIVE_REGISTER()

extern node_vector_t *native_functions;

void native_init();

void native_load_extern();

void native_end();

void native_register_function_node(native_function_node_t *);

void native_register_fn(const char *, const char *, node_vector_t *, native_fn_t);

native_function_node_t *native_find_function(const char *);

void native_register_all_functions();

#ifdef __cplusplus
}
#endif

#endif
