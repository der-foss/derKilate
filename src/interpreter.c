#include "kilate/interpreter.h"

#include <stdio.h>
#include <string.h>

#include "kilate/environment.h"
#include "kilate/error.h"
#include "kilate/file.h"
#include "kilate/hashmap.h"
#include "kilate/node.h"
#include "kilate/parser.h"

interpreter* interpreter_make(
    node_vector* nodes_vector,
    node_vector* native_functions_nodes_vector) {
  if (nodes_vector == NULL)
    error_fatal("Node's Vector is invalid.");
  if (native_functions_nodes_vector == NULL)
    error_fatal("Native Functions Node's Vector is invalid.");

  interpreter* interpreter = malloc(sizeof(interpreter));
  interpreter->functions = hash_map_make(sizeof(node*));
  interpreter->native_functions = hash_map_make(sizeof(native_fn));

  // register all funcs
  for (size_t i = 0; i < nodes_vector->size; i++) {
    node** nodePtr = (node**)vector_get(nodes_vector, i);
    if (nodePtr != NULL) {
      node* node = *nodePtr;
      if (node->type == NODE_FUNCTION) {
        hash_map_put(interpreter->functions, node->function_n.fn_name,
                         nodePtr);
      }
    }
  }

  for (size_t i = 0; i < native_functions_nodes_vector->size; i++) {
    native_fnentry** entryPtr =
        (native_fnentry**)vector_get(native_functions_nodes_vector, i);
    if (entryPtr != NULL) {
      native_fnentry* entry = *entryPtr;
      hash_map_put(interpreter->native_functions, entry->name, entryPtr);
    }
  }

  interpreter->env = environment_make(NULL);

  return interpreter;
}

void interpreter_delete(interpreter* self) {
  if (self == NULL)
    return;
  hash_map_delete(self->functions);
  hash_map_delete(self->native_functions);
  environment_destroy(self->env);
  free(self);
}

interpreter_result interpreter_run(interpreter* self) {
  if (self == NULL)
    error_fatal("Interpreter is invalid.");

  node** mainPtr = (node**)hash_map_get(self->functions, "main");
  if (mainPtr == NULL) {
    error_fatal("Your program needs a main function!");
  }
  node* main = *mainPtr;

  if (main->function_n.fn_return_type == NULL ||
      !str_equals(main->function_n.fn_return_type, "bool")) {
    error_fatal("Main function should return bool.");
  }

  return interpreter_run_fn(self, main, NULL);
}

interpreter_result interpreter_run_fn(interpreter* self,
                                              node* func,
                                              node_fnparam_vector* params) {
  if (self == NULL)
    error_fatal("Interpreter is invalid.");

  if (func == NULL || func->type != NODE_FUNCTION) {
    error_fatal("Functin Node Not is a Valid Function");
  }

  environment* old = self->env;
  self->env = environment_make(NULL);

  if (params != NULL && func->function_n.fn_params != NULL) {
    for (size_t i = 0; i < params->size; i++) {
      node_fnparam* param = *(node_fnparam**)vector_get(params, i);
      node_fnparam* fnParam =
          *(node_fnparam**)vector_get(func->function_n.fn_params, i);

      node_valuetype actualType = param->type;
      void* actualValue = param->value;

      if (param->type == NODE_VALUE_TYPE_VAR) {
        node* real_var = environment_getvar(old, (str)param->value);
        if (real_var == NULL) {
          error_fatal("Variable not defined: %s", (str)param->value);
        }
        actualType =
            parser_str_to_nodevaluetype(real_var->vardec_n.var_type);
        actualValue = real_var->vardec_n.var_value;
      }

      if (fnParam->type != NODE_VALUE_TYPE_ANY && fnParam->type != actualType) {
        error_fatal(
            "Argument %zu to function '%s' expected type '%s', but got '%s'",
            i + 1, func->function_n.fn_name,
            parser_nodevaluetype_to_str(fnParam->type),
            parser_nodevaluetype_to_str(actualType));
      }

      node* var = var_dec_node_make(
          fnParam->value, parser_nodevaluetype_to_str(fnParam->type),
          actualType, actualValue);
      node* var_copy = node_copy(var);
      environment_definevar(self->env, var_copy->vardec_n.var_name,
                                var_copy);
    }
  }

  for (size_t i = 0; i < func->function_n.fn_body->size; i++) {
    node** stmtPtr =
        (node**)vector_get(func->function_n.fn_body, i);
    if (stmtPtr != NULL) {
      node* stmt = *stmtPtr;
      interpreter_result result = interpreter_run_node(self, stmt);
      if (result.type == IRT_RETURN) {
        environment* to_destroy = self->env;
        self->env = old;
        environment_destroy(to_destroy);
        return result;
      }
    }
  }

  environment* to_destroy = self->env;
  self->env = old;
  environment_destroy(to_destroy);

  // default value
  return (interpreter_result){.type = IRT_FUNC, .data = NULL};
}

interpreter_result interpreter_run_node(interpreter* self,
                                                node* n) {
  if (self == NULL)
    error_fatal("Interpreter is invalid.");
  if (n == NULL)
    error_fatal("Node is invalid.");

  switch (n->type) {
    case NODE_CALL: {
      node** calledPtr = (node**)hash_map_get(
          self->functions, n->call_n.fn_call_name);
      native_fnentry** nativeFnEntryPtr =
          (native_fnentry**)hash_map_get(self->native_functions,
                                                 n->call_n.fn_call_name);

      // if ptr not null, so its a fn
      if (calledPtr != NULL) {
        node* called = *calledPtr;
        interpreter_result result =
            interpreter_run_fn(self, called, n->call_n.fn_call_params);
        return result;
      } else if (nativeFnEntryPtr != NULL) {
        // else if native ptr is not null, so its native fn
        native_fndata* nativeFnData = malloc(sizeof(native_fndata));
        nativeFnData->params = n->call_n.fn_call_params;
        nativeFnData->env = self->env;

        native_fnentry* nativeFnEntry = *nativeFnEntryPtr;
        native_fn nativeFn = *nativeFnEntry->fn;
        node* nativeFnResult = nativeFn(nativeFnData);
        interpreter_result result =
            (interpreter_result){.data = nativeFnResult, .type = IRT_FUNC};
        return result;
      } else {
        // else not found
        error_fatal("Function not found: %s", n->call_n.fn_call_name);
      }
    }

    case NODE_RETURN: {
      void* value = NULL;
      if (n->return_n.return_value != NULL) {
        value = n->return_n.return_value;  // or evaluate this node if needed
      }
      return (interpreter_result){.type = IRT_RETURN, .data = value};
    }

    case NODE_VARDEC: {
      environment_definevar(self->env, n->vardec_n.var_name,
                                node_copy(n));
      return (interpreter_result){.type = IRT_FUNC, .data = NULL};
    }

    default:
      error_fatal("Unknown node type %d", n->type);
  }
  return (interpreter_result){.type = IRT_FUNC, .data = NULL};
}
