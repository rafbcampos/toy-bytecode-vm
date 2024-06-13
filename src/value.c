#include "value.h"
#include <stdio.h>

void print_value(Value value) {
  if (IS_NUMBER(value)) {
    printf("%g", AS_NUMBER(value));
  } else if (IS_BOOL(value)) {
    printf("%s", AS_BOOL(value) ? "true" : "false");
  } else if (IS_NIL(value)) {
    printf("nil");
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
  default:
    return false; // Unreachable.
  }
}
