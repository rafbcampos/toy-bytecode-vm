#include "dynamic_array.h"
#include <stdio.h>
#include <stdlib.h>

// init
void IntArray_init(IntArray *array) {
  array->data = NULL;
  array->size = 0;
  array->capacity = 0;
}

void DoubleArray_init(DoubleArray *array) {
  array->data = NULL;
  array->size = 0;
  array->capacity = 0;
}

void Uint8Array_init(Uint8Array *array) {
  array->data = NULL;
  array->size = 0;
  array->capacity = 0;
}

void ValueArray_init(ValueArray *array) {
  array->data = NULL;
  array->size = 0;
  array->capacity = 0;
}

// push
void IntArray_push(IntArray *array, int item) {
  if (array->size == array->capacity) {
    int new_capacity = array->capacity == 0 ? 1 : array->capacity * 2;
    int *new_data = realloc(array->data, new_capacity * sizeof(int));
    if (new_data == NULL) {
      fprintf(stderr, "Failed to allocate memory for IntArray.\n");
      exit(EXIT_FAILURE);
    }
    array->data = new_data;
    array->capacity = new_capacity;
  }
  array->data[array->size++] = item;
}

void DoubleArray_push(DoubleArray *array, double item) {
  if (array->size == array->capacity) {
    int new_capacity = array->capacity == 0 ? 1 : array->capacity * 2;
    double *new_data = realloc(array->data, new_capacity * sizeof(double));
    if (new_data == NULL) {
      fprintf(stderr, "Failed to allocate memory for DoubleArray.\n");
      exit(EXIT_FAILURE);
    }
    array->data = new_data;
    array->capacity = new_capacity;
  }
  array->data[array->size++] = item;
}

void Uint8Array_push(Uint8Array *array, uint8_t item) {
  if (array->size == array->capacity) {
    int new_capacity = array->capacity == 0 ? 1 : array->capacity * 2;
    uint8_t *new_data = realloc(array->data, new_capacity * sizeof(uint8_t));
    if (new_data == NULL) {
      fprintf(stderr, "Failed to allocate memory for Uint8Array.\n");
      exit(EXIT_FAILURE);
    }
    array->data = new_data;
    array->capacity = new_capacity;
  }
  array->data[array->size++] = item;
}

void ValueArray_push(ValueArray *array, Value item) {
  if (array->size == array->capacity) {
    int new_capacity = array->capacity == 0 ? 1 : array->capacity * 2;
    Value *new_data = realloc(array->data, new_capacity * sizeof(Value));
    if (new_data == NULL) {
      fprintf(stderr, "Failed to allocate memory for ValueArray.\n");
      exit(EXIT_FAILURE);
    }
    array->data = new_data;
    array->capacity = new_capacity;
  }
  array->data[array->size++] = item;
}

// free
void IntArray_free(IntArray *array) {
  free(array->data);
  free(array);
}

void DoubleArray_free(DoubleArray *array) {
  free(array->data);
  free(array);
}

void Uint8Array_free(Uint8Array *array) {
  free(array->data);
  free(array);
}

void ValueArray_free(ValueArray *array) {
  free(array->data);
  free(array);
}
