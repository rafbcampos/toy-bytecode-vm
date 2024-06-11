#include "debug.h"
#include "bytecode_sequence.h"
#include <stdio.h>

int simple_instruction(const char *name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

int constant_instruction(const char *name, BytecodeSequence *sequence,
                         int offset) {
  uint8_t constant = sequence->code.data[offset++];
  printf("%-16s %4d '", name, constant);
  print_value(sequence->constants.data[constant]);
  printf("'\n");
  return offset;
}

int disassemble_instruction(BytecodeSequence *bytecode_sequence, int offset) {
  printf("%04d ", offset);

  int line = get_line(bytecode_sequence, offset);

  if (offset > 0 && line == get_line(bytecode_sequence, offset - 1)) {
    printf("   | ");
  } else {
    printf("%4d ", line);
  }

  uint8_t instruction = bytecode_sequence->code.data[offset];
  switch (instruction) {
  case OP_RETURN:
    return simple_instruction("OP_RETURN", offset);
  case OP_ADD:
    return simple_instruction("OP_ADD", offset);
  case OP_SUBTRACT:
    return simple_instruction("OP_SUBTRACT", offset);
  case OP_MULTIPLY:
    return simple_instruction("OP_MULTIPLY", offset);
  case OP_DIVIDE:
    return simple_instruction("OP_DIVIDE", offset);
  case OP_NEGATE:
    return simple_instruction("OP_NEGATE", offset);
  case OP_CONSTANT:
    return constant_instruction("OP_CONSTANT", bytecode_sequence, offset + 1);
  case OP_NIL:
    return simple_instruction("OP_NIL", offset);
  case OP_TRUE:
    return simple_instruction("OP_TRUE", offset);
  case OP_FALSE:
    return simple_instruction("OP_FALSE", offset);
  default:
    printf("Unknown opcode %d\n", instruction);
    return offset + 1;
  }
  return offset;
}

void disassemble_bytecode_sequence(BytecodeSequence *bytecode_sequence,
                                   const char *name) {
  printf("== %s ==\n", name);

  int length = (bytecode_sequence->code).size;
  for (int offset = 0; offset < length;) {
    offset = disassemble_instruction(bytecode_sequence, offset);
  }
}
