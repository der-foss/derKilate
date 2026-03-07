#include "kilate/util/native.h"

#include <stdlib.h>
#include <string.h>

#include "kilate/native.h"

str native_fndata_getstr(native_fndata* data, size_t index) {
  if (index >= data->params->size)
    return "";

  node_fnparam* param =
      *(node_fnparam**)vector_get(data->params, index);
  if (param->type == NODE_VALUE_TYPE_STRING) {
    return param->value;
  }

  node* var = environment_getvar(data->env, param->value);
  if (!var || var->vardec_n.var_value_type != NODE_VALUE_TYPE_STRING)
    return NULL;

  if (var->vardec_n.var_value == NULL)
    return "";

  return (str)var->vardec_n.var_value;
}

int native_fndata_getint(native_fndata* data, size_t index, bool* ok) {
  if (index >= data->params->size) {
    if (ok) *ok = false;
    return 0;
  }

  node_fnparam* param =
      *(node_fnparam**)vector_get(data->params, index);
  if (param->type == NODE_VALUE_TYPE_INT) {
    if (ok) *ok = true;
    return (int)(intptr_t)param->value;
  }

  node* var = environment_getvar(data->env, param->value);
  if (!var || var->vardec_n.var_value_type != NODE_VALUE_TYPE_INT) {
    if (ok) *ok = false;
    return 0;
  }

  if (ok) *ok = true;
  return (int)(intptr_t)var->vardec_n.var_value;
}

float native_fndata_getfloat(native_fndata* data, size_t index, bool* ok) {
  if (index >= data->params->size) {
    if (ok) *ok = false;
    return 0;
  }

  node_fnparam* param =
      *(node_fnparam**)vector_get(data->params, index);
  if (param->type == NODE_VALUE_TYPE_FLOAT) {
    if (ok) *ok = true;
    return (float)(intptr_t)param->value;
  }

  node* var = environment_getvar(data->env, param->value);
  if (!var || var->vardec_n.var_value_type != NODE_VALUE_TYPE_FLOAT) {
    if (ok) *ok = false;
    return 0;
  }

  if (ok) *ok = true;
  return *(float*)(intptr_t)var->vardec_n.var_value;
}

long native_fndata_getlong(native_fndata* data, size_t index, bool* ok) {
  if (index >= data->params->size) {
    if (ok) *ok = false;
    return 0;
  }

  node_fnparam* param =
      *(node_fnparam**)vector_get(data->params, index);
  if (param->type == NODE_VALUE_TYPE_LONG) {
    if (ok) *ok = true;
    return (long)(intptr_t)param->value;
  }

  node* var = environment_getvar(data->env, param->value);
  if (!var || var->vardec_n.var_value_type != NODE_VALUE_TYPE_LONG) {
    if (ok) *ok = false;
    return 0;
  }

  if (ok) *ok = true;
  return (long)(intptr_t)var->vardec_n.var_value;
}

bool native_fndata_getbool(native_fndata* data, size_t index, bool* ok) {
  if (index >= data->params->size) {
    if (ok) *ok = false;
    return false;
  }

  node_fnparam* param =
      *(node_fnparam**)vector_get(data->params, index);
  if (param->type == NODE_VALUE_TYPE_BOOL) {
    if (ok) *ok = true;
    return (bool)(intptr_t)param->value;
  }

  node* var = environment_getvar(data->env, param->value);
  if (!var || var->vardec_n.var_value_type != NODE_VALUE_TYPE_BOOL) {
    if (ok) *ok = false;
    return false;
  }

  if (ok) *ok = true;
  return (bool)(intptr_t)var->vardec_n.var_value;
}

void params_add(str_vector* params, const str param) {
  str s = (str)param;
  vector_push_back(params, &s);
}

native_fnentry* native_fnentry_make(const str name,
                                  str_vector* reqParams,
                                  native_fn fn) {
  native_fnentry* entry =
      (native_fnentry*)malloc(sizeof(native_fnentry));
  entry->name = strdup(name);
  entry->requiredParams = reqParams;
  entry->fn = fn;
  return entry;
}