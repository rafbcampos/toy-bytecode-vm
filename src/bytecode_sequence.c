#include "bytecode_sequence.h"
#include "dynamic_array.h"
#include <stdio.h>

void init_bytecode_sequence(BytecodeSequence *sequence) {
  if (sequence == NULL) {
    fprintf(stderr, "Error: sequence is NULL\n");
    exit(EXIT_FAILURE);
  }

  sequence->code = (OpCodeArray)DYNAMIC_ARRAY_INIT;
  sequence->lines = (LineArray)DYNAMIC_ARRAY_INIT;
  sequence->constants = (ValueArray)DYNAMIC_ARRAY_INIT;
}

void free_bytecode_sequence(BytecodeSequence *sequence) {
  DYNAMIC_ARRAY_FREE(sequence->code);
  DYNAMIC_ARRAY_FREE(sequence->lines);
  DYNAMIC_ARRAY_FREE(sequence->constants);
}

void write_bytecode_sequence(BytecodeSequence *sequence, uint8_t byte,
                             int line) {
  DYNAMIC_ARRAY_PUSH(sequence->code, byte);
  if (sequence->lines.size == 0 ||
      sequence->lines.data[sequence->lines.size - 1].line != line) {
    LineNumber lineNumber = {line, 1};
    DYNAMIC_ARRAY_PUSH(sequence->lines, lineNumber);
  } else {
    sequence->lines.data[sequence->lines.size - 1].count++;
  }
}

int add_constant(BytecodeSequence *sequence, Value value) {
  DYNAMIC_ARRAY_PUSH(sequence->constants, value);
  return sequence->constants.size - 1;
}

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

int get_line(BytecodeSequence *sequence, int instructionIndex) {
  int instructionCount = 0;
  for (int i = 0; i < sequence->lines.size; i++) {
    instructionCount += sequence->lines.data[i].count;
    if (instructionCount > instructionIndex) {
      return sequence->lines.data[i].line;
    }
  }
  return -1;
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
