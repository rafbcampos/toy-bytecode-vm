#include "debug.h"
#include "bytecode_sequence.h"
#include <stdio.h>

int simpleInstruction(const char *name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

int constantInstruction(const char *name, BytecodeSequence *sequence,
                        int offset) {
  uint8_t constant = sequence->code.data[offset++];
  printf("%-16s %4d '", name, constant);
  printValue(sequence->constants.data[constant]);
  printf("'\n");
  return offset;
}

int constantLongInstruction(const char *name, BytecodeSequence *sequence,
                            int offset) {
  uint32_t constant = (sequence->code.data[offset] << 16) |
                      (sequence->code.data[offset + 1] << 8) |
                      sequence->code.data[offset + 2];
  printf("%-16s %4d '", name, constant);
  printValue(sequence->constants.data[constant]);
  printf("'\n");
  return offset + 3;
}

int disassembleInstruction(BytecodeSequence *bytecode_sequence, int offset) {
  printf("%04d ", offset);

  int line = getLine(bytecode_sequence, offset);

  if (offset > 0 && line == getLine(bytecode_sequence, offset - 1)) {
    printf("   | ");
  } else {
    printf("%4d ", line);
  }

  uint8_t instruction = bytecode_sequence->code.data[offset];
  switch (instruction) {
  case OP_RETURN:
    return simpleInstruction("OP_RETURN", offset);
  case OP_CONSTANT:
    return constantInstruction("OP_CONSTANT", bytecode_sequence, offset + 1);
  case OP_CONSTANT_LONG:
    return constantLongInstruction("OP_CONSTANT_LONG", bytecode_sequence,
                                   offset + 1);
  default:
    printf("Unknown opcode %d\n", instruction);
    return offset + 1;
  }
  return offset;
}

void disassembleBytecodeSequence(BytecodeSequence *bytecode_sequence,
                                 const char *name) {
  printf("== %s ==\n", name);

  int length = (bytecode_sequence->code).size;
  for (int offset = 0; offset < length;) {
    offset = disassembleInstruction(bytecode_sequence, offset);
  }
}
