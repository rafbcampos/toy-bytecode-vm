#include "bytecode_sequence.h"
#include "dynamic_array.h"
#include <stdio.h>

void initBytecodeSequence(BytecodeSequence *sequence) {
  sequence->code = (OpCodeArray)DYNAMIC_ARRAY_INIT;
  sequence->lines = (LineArray)DYNAMIC_ARRAY_INIT;
  sequence->constants = (ValueArray)DYNAMIC_ARRAY_INIT;
}

void freeBytecodeSequence(BytecodeSequence *sequence) {
  DYNAMIC_ARRAY_FREE(sequence->code);
  DYNAMIC_ARRAY_FREE(sequence->lines);
  DYNAMIC_ARRAY_FREE(sequence->constants);
}

void writeBytecodeSequence(BytecodeSequence *sequence, uint8_t byte, int line) {
  DYNAMIC_ARRAY_PUSH(sequence->code, byte);
  DYNAMIC_ARRAY_PUSH(sequence->lines, line);
}

int addConstant(BytecodeSequence *sequence, Value value) {
  DYNAMIC_ARRAY_PUSH(sequence->constants, value);
  return sequence->constants.size - 1;
}

void printValue(Value value) { printf("%g", value); }
