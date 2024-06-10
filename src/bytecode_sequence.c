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
  if (sequence->lines.size == 0 ||
      sequence->lines.data[sequence->lines.size - 1].line != line) {
    LineNumber lineNumber = {line, 1};
    DYNAMIC_ARRAY_PUSH(sequence->lines, lineNumber);
  } else {
    sequence->lines.data[sequence->lines.size - 1].count++;
  }
}

void writeBytecodeSequenceLong(BytecodeSequence *sequence, uint32_t constant,
                               int line) {
  writeBytecodeSequence(sequence, OP_CONSTANT_LONG, line);
  writeBytecodeSequence(sequence, (constant >> 16) & 0xFF, line);
  writeBytecodeSequence(sequence, (constant >> 8) & 0xFF, line);
  writeBytecodeSequence(sequence, constant & 0xFF, line);
}

int addConstant(BytecodeSequence *sequence, Value value) {
  DYNAMIC_ARRAY_PUSH(sequence->constants, value);
  return sequence->constants.size - 1;
}

void printValue(Value value) { printf("%g", value); }

int getLine(BytecodeSequence *sequence, int instructionIndex) {
  int instructionCount = 0;
  for (int i = 0; i < sequence->lines.size; i++) {
    instructionCount += sequence->lines.data[i].count;
    if (instructionCount > instructionIndex) {
      return sequence->lines.data[i].line;
    }
  }
  return -1;
}
