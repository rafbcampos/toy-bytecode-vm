#ifndef dynamic_array_h
#define dynamic_array_h

#include "value.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct {
  int *data;
  size_t size;
  size_t capacity;
} IntArray;

typedef struct {
  double *data;
  size_t size;
  size_t capacity;
} DoubleArray;

typedef struct {
  uint8_t *data;
  size_t size;
  size_t capacity;
} Uint8Array;

typedef struct {
  Value *data;
  size_t size;
  size_t capacity;
} ValueArray;

// operations

// init
void IntArray_init(IntArray *array);
void DoubleArray_init(DoubleArray *array);
void Uint8Array_init(Uint8Array *array);
void ValueArray_init(ValueArray *array);

// push
void IntArray_push(IntArray *array, int item);
void DoubleArray_push(DoubleArray *array, double item);
void Uint8Array_push(Uint8Array *array, uint8_t item);
void ValueArray_push(ValueArray *array, Value item);

// free
void IntArray_free(IntArray *array);
void DoubleArray_free(DoubleArray *array);
void Uint8Array_free(Uint8Array *array);
void ValueArray_free(ValueArray *array);

#endif // !dynamic_array_h
