#include "bytecode_sequence.h"
#include "debug.h"

int main() {
  BytecodeSequence bytecode_sequence;
  initBytecodeSequence(&bytecode_sequence);
  int constant = addConstant(&bytecode_sequence, 1.2);
  writeBytecodeSequence(&bytecode_sequence, OP_CONSTANT, 123);
  writeBytecodeSequence(&bytecode_sequence, constant, 123);
  writeBytecodeSequence(&bytecode_sequence, OP_RETURN, 123);
  disassembleBytecodeSequence(&bytecode_sequence, "test bytecode_sequence");
  freeBytecodeSequence(&bytecode_sequence);
  return 0;
}
