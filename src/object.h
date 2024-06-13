#ifndef object_h
#define object_h

#include "value.h"
#include "vm.h"

typedef enum {
  OBJ_STRING,
} ObjType;

struct Obj {
  ObjType type;
  Obj *next;
};

struct ObjString {
  Obj obj;
  int length;
  char *chars;
  uint32_t hash;
};

#define OBJ_TYPE(value) (AS_OBJ(value)->type)
#define IS_STRING(value) is_obj_type(value, OBJ_STRING)
#define AS_STRING(value) ((ObjString *)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString *)AS_OBJ(value))->chars)

static inline bool is_obj_type(Value value, ObjType type) {
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

ObjString *copy_string(VM *vm, const char *chars, int length);
ObjString *take_string(VM *vm, char *chars, int length);
void free_objects(VM *vm);

#endif // !object_h
