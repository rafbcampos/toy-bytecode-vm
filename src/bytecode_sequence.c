#include "bytecode_sequence.h"
#include "dynamic_array.h"
#include <stdio.h>

void init_bytecode_sequence(BytecodeSequence *sequence) {
  if (sequence == NULL) {
    fprintf(stderr, "Trying to init a BytecodeSequence, but got NULL\n");
    exit(EXIT_FAILURE);
  }

  Uint8Array_init(&sequence->code);
  IntArray_init(&sequence->lines);
  ValueArray_init(&sequence->constants);
}

void free_bytecode_sequence(BytecodeSequence *sequence) {
  if (sequence == NULL) {
    fprintf(stderr, "Trying to free an BytecodeSequence, but got NULL\n");
    exit(EXIT_FAILURE);
  }

  Uint8Array_free(&sequence->code);
  IntArray_free(&sequence->lines);
  ValueArray_free(&sequence->constants);
}

void write_bytecode_sequence(BytecodeSequence *sequence, uint8_t byte,
                             int line) {
  Uint8Array_push(&sequence->code, byte);
  IntArray_push(&sequence->lines, line);
}

int add_constant(BytecodeSequence *sequence, Value value) {
  ValueArray_push(&sequence->constants, value);
  return sequence->constants.size - 1;
}
