#include <stdlib.h>

#define DYNAMIC_ARRAY(TYPE)                                                    \
  struct {                                                                     \
    TYPE *data;                                                                \
    size_t size;                                                               \
    size_t capacity;                                                           \
  }

#define DYNAMIC_ARRAY_INIT                                                     \
  { NULL, 0, 0 }

#define DYNAMIC_ARRAY_PUSH(ARRAY, ITEM)                                        \
  do {                                                                         \
    if ((ARRAY).size == (ARRAY).capacity) {                                    \
      (ARRAY).capacity = (ARRAY).capacity * 2 + 8;                             \
      (ARRAY).data =                                                           \
          reallocate((ARRAY).data, (ARRAY).size * sizeof(*(ARRAY).data),       \
                     (ARRAY).capacity * sizeof(*(ARRAY).data));                \
    }                                                                          \
    (ARRAY).data[(ARRAY).size++] = (ITEM);                                     \
  } while (0)

#define DYNAMIC_ARRAY_FREE(ARRAY)                                              \
  do {                                                                         \
    free((ARRAY).data);                                                        \
    (ARRAY).data = NULL;                                                       \
    (ARRAY).size = 0;                                                          \
    (ARRAY).capacity = 0;                                                      \
  } while (0)

void *reallocate(void *pointer, size_t oldSize, size_t newSize);
