#ifndef hash_table
#define hash_table

#include "object.h"
#include "value.h"
#include <stdlib.h>

typedef struct Entry {
  ObjString *key;
  Value value;
  struct Entry *next;
} Entry;

typedef struct {
  size_t size;
  size_t capacity;
  Entry **entries;
} HashTable;

void hash_table_init(HashTable *table);
void hash_table_free(HashTable *table);
bool hash_table_set(HashTable *table, ObjString *key, Value value);
Value hash_table_get(HashTable *table, ObjString *key);
bool hash_table_delete(HashTable *table, ObjString *key);

#endif // !hash_table
