#include "value.h"
#include "object.h"
#include <stdio.h>
#include <string.h>

void print_object(Value value) {
  switch (OBJ_TYPE(value)) {
  case OBJ_STRING:
    printf("%s", AS_CSTRING(value));
    break;
  }
}

void print_value(Value value) {
  if (IS_NUMBER(value)) {
    printf("%g", AS_NUMBER(value));
  } else if (IS_BOOL(value)) {
    printf("%s", AS_BOOL(value) ? "true" : "false");
  } else if (IS_NIL(value)) {
    printf("nil");
  } else if (IS_OBJ(value)) {
    print_object(value);
  } else {
    printf("Unknown value type");
  }
}

bool values_equal(Value a, Value b) {
  if (a.type != b.type)
    return false;
  switch (a.type) {
  case VAL_BOOL:
    return AS_BOOL(a) == AS_BOOL(b);
  case VAL_NIL:
    return true;
  case VAL_NUMBER:
    return AS_NUMBER(a) == AS_NUMBER(b);
  case VAL_OBJ: {
    ObjString *aString = AS_STRING(a);
    ObjString *bString = AS_STRING(b);
    return aString->length == bString->length &&
           memcmp(aString->chars, bString->chars, aString->length) == 0;
  }
  default:
    return false; // Unreachable.
  }
}
