#include "vm.h"
#include "bytecode_sequence.h"
#include "debug.h"
#include <stdio.h>

void resetStack(VM *vm) { vm->stack_top = vm->stack; }

void init_vm(VM *vm) {
  if (vm == NULL) {
    printf("vm is NULL\n");
    return;
  }
  vm->ip = NULL;
  vm->bytecode = NULL;
  resetStack(vm);
}

void free_vm(VM *vm) {
  if (vm == NULL) {
    printf("vm is NULL\n");
    return;
  }
  init_vm(vm);
}

void push(VM *vm, Value value) {
  *vm->stack_top = value;
  vm->stack_top++;
}

Value pop(VM *vm) {
  vm->stack_top--;
  return *vm->stack_top;
}

InterpretResult run(VM *vm) {
#define READ_BYTE() (*vm->ip++)
#define READ_CONSTANT() (vm->bytecode->constants.data[READ_BYTE()])

  if (vm == NULL || vm->ip == NULL || vm->bytecode == NULL) {
    printf("vm, vm->ip, or vm->bytecode is NULL\n");
    return INTERPRET_COMPILE_ERROR;
  }

  if (vm->bytecode->constants.data == NULL) {
    printf("vm->bytecode->constants.data is NULL\n");
    return INTERPRET_COMPILE_ERROR;
  }

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("          ");
    for (Value *slot = vm->stack; slot < vm->stack_top; slot++) {
      printf("[ ");
      printValue(*slot);
      printf(" ]");
    }
    printf("\n");
    disassembleInstruction(vm->bytecode,
                           (int)(vm->ip - vm->bytecode->code.data));
#endif
    uint8_t instruction = READ_BYTE();
    switch (instruction) {
    case OP_CONSTANT: {
      Value constant = READ_CONSTANT();
      push(vm, constant);
      break;
    }
    case OP_RETURN: {
      printValue(pop(vm));
      printf("\n");
      return INTERPRET_OK;
    }
    default: {
      return INTERPRET_COMPILE_ERROR;
    }
    }
  }
  return INTERPRET_OK;

#undef READ_BYTE
#undef READ_CONSTANT
}

InterpretResult interpret(VM *vm, BytecodeSequence *bytecode_sequence) {
  if (vm == NULL) {
    printf("vm is NULL\n");
    return INTERPRET_COMPILE_ERROR;
  }

  if (bytecode_sequence == NULL || bytecode_sequence->code.data == NULL) {
    printf("bytecode_sequence or bytecode_sequence->code.data is NULL\n");
    return INTERPRET_COMPILE_ERROR;
  }

  vm->bytecode = bytecode_sequence;
  vm->ip = bytecode_sequence->code.data;

  return run(vm);
}
