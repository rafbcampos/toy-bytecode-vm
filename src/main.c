#include "bytecode_sequence.h"
#include "vm.h"

int main() {
  BytecodeSequence bytecode_sequence;
  initBytecodeSequence(&bytecode_sequence);

  int a = addConstant(&bytecode_sequence, 5.0);
  writeBytecodeSequence(&bytecode_sequence, OP_CONSTANT, 1);
  writeBytecodeSequence(&bytecode_sequence, a, 1);

  int b = addConstant(&bytecode_sequence, 3.0);
  writeBytecodeSequence(&bytecode_sequence, OP_CONSTANT, 2);
  writeBytecodeSequence(&bytecode_sequence, b, 2);

  writeBytecodeSequence(&bytecode_sequence, OP_NEGATE, 3);
  writeBytecodeSequence(&bytecode_sequence, OP_ADD, 3);

  int c = addConstant(&bytecode_sequence, 2.0);
  writeBytecodeSequence(&bytecode_sequence, OP_CONSTANT, 4);
  writeBytecodeSequence(&bytecode_sequence, c, 4);
  writeBytecodeSequence(&bytecode_sequence, OP_MULTIPLY, 4);

  writeBytecodeSequence(&bytecode_sequence, OP_RETURN, 4);

  VM vm;
  init_vm(&vm);
  free_vm(&vm);
  interpret(&vm, &bytecode_sequence);
  freeBytecodeSequence(&bytecode_sequence);

  return 0;
}
