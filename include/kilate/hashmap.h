#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include <stdio.h>

#include "kilate/string.h"
#include "kilate/vector.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef vector hashmap_vector;
typedef vector hashitem_vector;

typedef struct {
  hashitem_vector* itens;
  size_t itemSize;
  size_t capacity;
} hashmap;

typedef struct {
  str key;
  void* value;
  void* next;
} hashitem;

hashmap* hash_map_make(size_t);

void hash_map_delete(hashmap*);

unsigned int hash_map_hash(hashmap*, str);

void* hash_map_get(hashmap*, str);

void hash_map_put(hashmap*, str, void*);

#ifdef __cplusplus
}
#endif

#endif