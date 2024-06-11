#include "vm.h"
#include "bytecode_sequence.h"
#include "compiler.h"
#include "debug.h"
#include <stdarg.h>
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

static Value peek(VM *vm, int distance) { return vm->stack_top[-1 - distance]; }

static void runtime_error(VM *vm, const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputs("\n", stderr);

  size_t instruction = vm->ip - vm->bytecode->code.data - 1;
  LineNumber line = vm->bytecode->lines.data[instruction];
  fprintf(stderr, "[line %d] in script\n", line.line);
  reset_stack(vm);
}

InterpretResult run(VM *vm) {
#define READ_BYTE() (*vm->ip++)
#define READ_CONSTANT() (vm->bytecode->constants.data[READ_BYTE()])
#define BINARY_OP(vm, value_type, op)                                          \
  do {                                                                         \
    if (!IS_NUMBER(peek(vm, 0)) || !IS_NUMBER(peek(vm, 1))) {                  \
      runtime_error(vm, "Operands must be numbers.");                          \
      return INTERPRET_RUNTIME_ERROR;                                          \
    }                                                                          \
    double b = AS_NUMBER(pop(vm));                                             \
    double a = AS_NUMBER(pop(vm));                                             \
    push(vm, value_type(a op b));                                              \
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
      BINARY_OP(vm, NUMBER_VAL, +);
      break;
    case OP_SUBTRACT:
      BINARY_OP(vm, NUMBER_VAL, -);
      break;
    case OP_MULTIPLY:
      BINARY_OP(vm, NUMBER_VAL, *);
      break;
    case OP_DIVIDE:
      BINARY_OP(vm, NUMBER_VAL, /);
      break;
    case OP_NEGATE: {
      if (!IS_NUMBER(peek(vm, 0))) {
        runtime_error(vm, "Operand must be a number.");
        return INTERPRET_RUNTIME_ERROR;
      }
      push(vm, NUMBER_VAL(-AS_NUMBER(pop(vm))));
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
