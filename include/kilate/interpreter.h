#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "kilate/environment.h"
#include "kilate/hashmap.h"
#include "kilate/node.h"
#include "kilate/string.h"
#include "kilate/vector.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  hashmap* functions;
  hashmap* native_functions;
  environment* env;
} interpreter;

typedef enum { IRT_FUNC, IRT_RETURN } interpreter_result_type;

typedef struct {
  void* data;
  interpreter_result_type type;
} interpreter_result;

interpreter* interpreter_make(node_vector*, node_vector*);

void interpreter_delete(interpreter*);

interpreter_result interpreter_run(interpreter*);

interpreter_result interpreter_run_fn(interpreter*,
                                              node*,
                                              str_vector*);

interpreter_result interpreter_run_node(interpreter*, node*);

#ifdef __cplusplus
}
#endif

#endif