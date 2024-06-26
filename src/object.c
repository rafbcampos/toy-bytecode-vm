#include "object.h"
#include <stdio.h>
#include <string.h>

static uint32_t hash_string(const char *key, int length) {
  uint32_t hash = 2166136261u;
  for (int i = 0; i < length; i++) {
    hash ^= (uint8_t)key[i];
    hash *= 16777619;
  }
  return hash;
}

static Obj *allocate_object(VM *vm, size_t size, ObjType type) {
  Obj *object = (Obj *)realloc(NULL, size);
  if (object == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }
  object->type = type;
  object->next = vm->objects;
  vm->objects = object;
  return object;
}

static ObjString *allocate_string(VM *vm, char *chars, int length,
                                  uint32_t hash) {
  ObjString *string =
      (ObjString *)allocate_object(vm, sizeof(ObjString), OBJ_STRING);
  string->length = length;
  string->chars = chars;
  return string;
}

ObjString *copy_string(VM *vm, const char *chars, int length) {
  char *heapChars = realloc(NULL, length + 1);
  if (heapChars == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }
  memcpy(heapChars, chars, length);
  heapChars[length] = '\0';
  ObjString *result =
      allocate_string(vm, heapChars, length, hash_string(heapChars, length));
  return result;
}

ObjString *take_string(VM *vm, char *chars, int length) {
  return allocate_string(vm, chars, length, hash_string(chars, length));
}

static void free_object(Obj *object) {
  switch (object->type) {
  case OBJ_STRING: {
    ObjString *string = (ObjString *)object;
    free(string->chars);
    break;
  }
  }
  free(object);
}

void free_objects(VM *vm) {
  Obj *object = vm->objects;
  while (object != NULL) {
    Obj *next = object->next;
    free_object(object);
    object = next;
  }
}
