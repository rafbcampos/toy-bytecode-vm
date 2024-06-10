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

int disassembleInstruction(BytecodeSequence *bytecode_sequence, int offset) {
  printf("%04d ", offset);

  if (offset > 0 && bytecode_sequence->lines.data[offset] ==
                        bytecode_sequence->lines.data[offset - 1]) {
    printf("   | ");
  } else {
    printf("%4d ", bytecode_sequence->lines.data[offset]);
  }

  uint8_t instruction = bytecode_sequence->code.data[offset];
  switch (instruction) {
  case OP_RETURN:
    return simpleInstruction("OP_RETURN", offset);
  case OP_CONSTANT:
    return constantInstruction("OP_CONSTANT", bytecode_sequence, offset + 1);
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
