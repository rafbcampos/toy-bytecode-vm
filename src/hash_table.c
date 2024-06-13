#include "hash_table.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_MAX_LOAD 0.75

Value NULL_VALUE = {0};

static Entry *find_entry(HashTable *table, ObjString *key) {
  uint32_t index = key->hash % table->capacity;
  Entry *entry = table->entries[index];

  while (entry != NULL) {
    // If the key is found, return the entry:
    if (strcmp(entry->key->chars, key->chars) == 0) {
      return entry;
    }
    // else, move trough the linked list:
    entry = entry->next;
  }

  return NULL;
}

// Allocate memory for new entries or resize the existing entries:
static void adjust_capacity(HashTable *table, size_t capacity) {
  Entry **entries = malloc(sizeof(Entry *) * capacity);

  if (entries == NULL) {
    printf("Failed to allocate memory for entries.\n");
    exit(EXIT_FAILURE);
  }

  // Initialize the entries with NULL:
  for (size_t i = 0; i < capacity; i++) {
    entries[i] = NULL;
  }

  // Move the existing entries to the new entries
  // rehashing the keys:
  for (size_t i = 0; i < table->capacity; i++) {
    Entry *entry = table->entries[i];
    while (entry != NULL) {
      Entry *next = entry->next;
      uint32_t index = entry->key->hash % capacity;
      entry->next = entries[index];
      entries[index] = entry;
      entry = next;
    }
  }

  free(table->entries);
  table->entries = entries;
  table->capacity = capacity;
}

void hash_table_free(HashTable *table) {
  for (size_t i = 0; i < table->capacity; i++) {
    Entry *entry = table->entries[i];
    while (entry != NULL) {
      Entry *next = entry->next;
      free(entry);
      entry = next;
    }
  }
  if (table->entries != NULL) {
    free(table->entries);
  }
  if (table != NULL) {
    free(table);
  }
  hash_table_init(table);
}

bool hash_table_set(HashTable *table, ObjString *key, Value value) {
  if (key == NULL || key->chars == NULL) {
    printf("Key is NULL.\n");
    exit(EXIT_FAILURE);
  }

  if (table->size + 1 > table->capacity * TABLE_MAX_LOAD) {
    size_t capacity = table->capacity < 8 ? 8 : table->capacity * 2;
    adjust_capacity(table, capacity);
  }

  uint32_t index = key->hash % table->capacity;
  Entry *entry = find_entry(table, key);
  bool isNewKey = entry == NULL;

  if (isNewKey) {
    entry = malloc(sizeof(Entry));
    if (entry == NULL) {
      printf("Failed to allocate memory for entry.\n");
      exit(EXIT_FAILURE);
    }
    entry->key = key;
    entry->next = table->entries[index];
    table->entries[index] = entry;
    table->size++;
  } else {
    // free the value if the key already exists:
    free_value(entry->value);
    // set the new value:
    entry->value = value;
  }

  return isNewKey;
}

Value hash_table_get(HashTable *table, ObjString *key) {
  if (table == NULL) {
    printf("Table is NULL.\n");
    exit(EXIT_FAILURE);
  }
  if (key == NULL || key->chars == NULL) {
    printf("Key is NULL.\n");
    exit(EXIT_FAILURE);
  }
  if (table->size == 0)
    return NULL_VALUE;

  Entry *entry = find_entry(table, key);
  return entry != NULL ? entry->value : NULL_VALUE;
}

bool hash_table_delete(HashTable *table, ObjString *key) {
  if (table == NULL) {
    printf("Table is NULL.\n");
    exit(EXIT_FAILURE);
  }
  if (key == NULL || key->chars == NULL) {
    printf("Key is NULL.\n");
    exit(EXIT_FAILURE);
  }
  if (table->size == 0)
    return false;

  Entry *entry = find_entry(table, key);
  if (entry == NULL)
    return false;

  Value value = entry->value;
  table->size--;

  if (entry->next != NULL) {
    Entry *next = entry->next;
    entry->key = next->key;
    entry->value = next->value;
    entry->next = next->next;
    free(next);
  } else {
    entry->key = NULL;
    entry->value = NULL_VALUE;
  }

  return true;
}
