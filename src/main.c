#include "bytecode_sequence.h"
#include "vm.h"

int main() {
  BytecodeSequence bytecode_sequence;
  initBytecodeSequence(&bytecode_sequence);

  int constant = addConstant(&bytecode_sequence, 1.2);
  int constant2 = addConstant(&bytecode_sequence, 1000);

  writeBytecodeSequence(&bytecode_sequence, OP_CONSTANT, 1);
  writeBytecodeSequence(&bytecode_sequence, constant, 1);

  writeBytecodeSequence(&bytecode_sequence, OP_CONSTANT, 2);
  writeBytecodeSequence(&bytecode_sequence, constant2, 2);

  writeBytecodeSequence(&bytecode_sequence, OP_RETURN, 3);

  VM vm;
  init_vm(&vm);
  free_vm(&vm);
  interpret(&vm, &bytecode_sequence);
  freeBytecodeSequence(&bytecode_sequence);

  return 0;
}
