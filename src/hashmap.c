#include "kilate/hashmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kilate/error.h"
#include "kilate/string.h"

hashmap* hash_map_make(size_t itemSize) {
  hashmap* hashMap = malloc(sizeof(hashmap));
  hashMap->itemSize = itemSize;
  hashMap->capacity = 64;
  hashMap->itens = vector_make(sizeof(hashitem*));
  for (size_t i = 0; i < hashMap->capacity; i++) {
    hashitem* null_ptr = NULL;
    vector_push_back(hashMap->itens, &null_ptr);
  }
  return hashMap;
}

void hash_map_delete(hashmap* self) {
  for (size_t i = 0; i < self->itens->size; ++i) {
    hashitem* item = *(hashitem**)vector_get(self->itens, i);
    if (item != NULL) {
      free(item->key);
      free(item->value);
      free(item);
    }
  }
  vector_delete(self->itens);
  free(self);
}

unsigned int hash_map_hash(hashmap* self, str key) {
  if (self == NULL)
    error_fatal("Hashmap is null.");
  if (key == NULL)
    error_fatal("Key is null.");

  unsigned int hash = 5381;
  int c;
  while ((c = *key++)) {
    hash = ((hash << 5) + hash) + c;  // hash * 33 + c
  }
  return hash % self->capacity;
}
void* hash_map_get(hashmap* self, str key) {
  if (self == NULL)
    error_fatal("Hashmap is null.");
  if (key == NULL)
    error_fatal("Key is null.");

  unsigned int index = hash_map_hash(self, key);

  hashitem** itemPtr = (hashitem**)vector_get(self->itens, index);
  hashitem* item = *itemPtr;

  while (item) {
    if (str_equals(item->key, key)) {
      return item->value;
    }
    item = item->next;
  }
  return NULL;
}

void hash_map_put(hashmap* self, str key, void* value) {
  if (self == NULL)
    error_fatal("Hashmap is null.");
  if (key == NULL)
    error_fatal("Key is null.");

  unsigned int index = hash_map_hash(self, key);
  hashitem** headPtr = (hashitem**)vector_get(self->itens, index);
  hashitem* head = *headPtr;

  hashitem* item = head;
  while (item) {
    if (str_equals(item->key, key)) {
      memcpy(item->value, value, self->itemSize);
      return;
    }
    item = item->next;
  }

  hashitem* newItem = malloc(sizeof(hashitem));
  newItem->key = strdup(key);
  newItem->value = malloc(self->itemSize);
  memcpy(newItem->value, value, self->itemSize);
  newItem->next = head;

  *headPtr = newItem;
}
