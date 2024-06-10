#include "bytecode_sequence.h"
#include "debug.h"

int main() {
  BytecodeSequence bytecode_sequence;
  initBytecodeSequence(&bytecode_sequence);
  int constant = addConstant(&bytecode_sequence, 1.2);
  int longConstant = addConstant(&bytecode_sequence, 1000);
  writeBytecodeSequence(&bytecode_sequence, OP_CONSTANT, 1);
  writeBytecodeSequence(&bytecode_sequence, constant, 1);
  writeBytecodeSequenceLong(&bytecode_sequence, longConstant, 2);
  writeBytecodeSequence(&bytecode_sequence, OP_RETURN, 3);
  disassembleBytecodeSequence(&bytecode_sequence, "test bytecode_sequence");
  freeBytecodeSequence(&bytecode_sequence);
  return 0;
}
