#ifndef __NODE_H__
#define __NODE_H__

#include "kilate/lexer.h"
#include "kilate/string.h"
#include "kilate/vector.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  NODE_FUNCTION,
  NODE_CALL,
  NODE_RETURN,
  NODE_VARDEC,
  NODE_IMPORT
} nodetype;

typedef enum {
  NODE_VALUE_TYPE_INT,
  NODE_VALUE_TYPE_FLOAT,
  NODE_VALUE_TYPE_LONG,
  NODE_VALUE_TYPE_STRING,
  NODE_VALUE_TYPE_BOOL,
  NODE_VALUE_TYPE_VAR,
  NODE_VALUE_TYPE_FUNC,
  NODE_VALUE_TYPE_CALL,
  NODE_VALUE_TYPE_ANY
} node_valuetype;

typedef struct node node;
typedef vector node_vector;

typedef struct {
  str value;
  node_valuetype type;
} node_fnparam;

typedef vector node_fnparam_vector;

struct node {
  nodetype type;

  struct {
    str fn_name;
    str fn_return_type;
    node_vector* fn_body;
    node_fnparam_vector* fn_params;
  } function_n;

  struct {
    str fn_call_name;
    node_fnparam_vector* fn_call_params;
  } call_n;

  struct {
    node_valuetype return_type;
    void* return_value;
  } return_n;

  struct {
    str var_name;
    str var_type;
    node_valuetype var_value_type;
    void* var_value;
  } vardec_n;

  struct {
    str import_path;
  } import_n;
};

void node_delete(node*);

node* node_copy(node*);

node_fnparam* node_fnparam_copy(node_fnparam*);

void node_delete_params(node_fnparam_vector*);

node* function_node_make(str,
                                 str,
                                 node_vector*,
                                 node_fnparam_vector*);

node* call_node_make(str, node_fnparam_vector*);

node* return_node_make(node_valuetype, void*);

node* var_dec_node_make(str, str, node_valuetype, void*);

node* import_node_make(str);

#ifdef __cplusplus
}
#endif

#endif