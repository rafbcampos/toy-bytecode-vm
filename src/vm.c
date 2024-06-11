#include "vm.h"
#include "bytecode_sequence.h"
#include "compiler.h"
#include "debug.h"
#include <stdbool.h>
#include <stdio.h>

void reset_stack(VM *vm) { vm->stack_top = vm->stack; }

void init_vm(VM *vm) {
  if (vm == NULL) {
    printf("vm is NULL\n");
    return;
  }
  vm->ip = NULL;
  vm->bytecode = NULL;
  reset_stack(vm);
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
#define BINARY_OP(op)                                                          \
  do {                                                                         \
    double b = pop(vm);                                                        \
    double a = pop(vm);                                                        \
    push(vm, a op b);                                                          \
  } while (false)

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
      print_value(*slot);
      printf(" ]");
    }
    printf("\n");
    disassemble_instruction(vm->bytecode,
                            (int)(vm->ip - vm->bytecode->code.data));
#endif
    uint8_t instruction = READ_BYTE();
    switch (instruction) {
    case OP_CONSTANT: {
      Value constant = READ_CONSTANT();
      push(vm, constant);
      break;
    }
    case OP_ADD:
      BINARY_OP(+);
      break;
    case OP_SUBTRACT:
      BINARY_OP(-);
      break;
    case OP_MULTIPLY:
      BINARY_OP(*);
      break;
    case OP_DIVIDE:
      BINARY_OP(/);
      break;
    case OP_NEGATE: {
      push(vm, -(pop(vm)));
      break;
    }
    case OP_RETURN: {
      print_value(pop(vm));
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
#undef BINARY_OP
}

InterpretResult interpret(VM *vm, BytecodeSequence *bytecode_sequence,
                          const char *source) {
  if (vm == NULL) {
    printf("vm is NULL\n");
    return INTERPRET_COMPILE_ERROR;
  }

  if (bytecode_sequence == NULL) {
    printf("bytecode_sequence is NULL\n");
    return INTERPRET_COMPILE_ERROR;
  }

  compile(bytecode_sequence, source);

  vm->bytecode = bytecode_sequence;
  vm->ip = bytecode_sequence->code.data;

  return run(vm);
}
