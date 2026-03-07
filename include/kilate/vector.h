#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  void* data;
  size_t capacity;
  size_t size;
  size_t itemSize;
} vector;

vector* vector_make(size_t);

void vector_delete(vector*);

const void* vector_get(vector*, size_t);

void vector_set(vector*, const size_t, const void*);

void vector_reserve(vector*, const size_t);

void vector_insert(vector*, const size_t, const void*);

void vector_push_back(vector*, const void*);

void vector_remove(vector*, size_t);

#ifdef __cplusplus
}
#endif

#endif