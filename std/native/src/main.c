#include <kilate/native.h>
#include <kilate/string.h>
#include <kilate/vector.h>

#include "io.h"

KILATE_NATIVE_REGISTER() {
  {
    // Register native print method
    str_vector* requiredParams = vector_make(sizeof(str*));
    str any = "any";
    vector_push_back(requiredParams, &any);
    native_register_fn("print", requiredParams, std_print);
  }
  {
    // Register native system method
    str_vector* requiredParams = vector_make(sizeof(str*));
    str str = "string";
    vector_push_back(requiredParams, &str);
    native_register_fn("system", requiredParams, std_system);
  }
  {
    // Register native system method
    str_vector* requiredParams = vector_make(sizeof(str*));
    str str = "long";
    vector_push_back(requiredParams, &str);
    native_register_fn("sleep", requiredParams, std_sleep);
  }
}