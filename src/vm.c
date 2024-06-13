#include "vm.h"
#include "bytecode_sequence.h"
#include "compiler.h"
#include "debug.h"
#include "object.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void reset_stack(VM *vm) { vm->stack_top = vm->stack; }

void init_vm(VM *vm) {
  if (vm == NULL) {
    fprintf(stderr, "Trying to init the VM, but got NULL\n");
    exit(EXIT_FAILURE);
  }
  vm->ip = NULL;
  vm->bytecode = NULL;
  vm->objects = NULL;
  reset_stack(vm);
}

void free_vm(VM *vm) {
  if (vm == NULL) {
    fprintf(stderr, "Trying to free the VM, but got NULL\n");
    exit(EXIT_FAILURE);
  }
  free_objects(vm);
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
  int line = vm->bytecode->lines.data[instruction];
  fprintf(stderr, "[line %d] in script\n", line);
  reset_stack(vm);
}

static bool is_falsey(Value value) {
  return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static void concatenate(VM *vm) {
  ObjString *b = AS_STRING(pop(vm));
  ObjString *a = AS_STRING(pop(vm));

  int length = a->length + b->length;
  char *chars = realloc(NULL, length + 1);
  if (chars == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }
  memcpy(chars, a->chars, a->length);
  memcpy(chars + a->length, b->chars, b->length);
  chars[length] = '\0';
  ObjString *result = take_string(vm, chars, length);
  push(vm, OBJ_VAL(result));
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

  if (vm == NULL) {
    printf("vm is NULL\n");
    return INTERPRET_COMPILE_ERROR;
  }

  if (vm->ip == NULL || vm->bytecode == NULL) {
    printf("vm is not initialized\n");
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
    case OP_NIL:
      push(vm, NIL_VAL);
      break;
    case OP_TRUE:
      push(vm, BOOL_VAL(true));
      break;
    case OP_FALSE:
      push(vm, BOOL_VAL(false));
      break;
    case OP_ADD: {
      if (IS_STRING(peek(vm, 0)) && IS_STRING(peek(vm, 1))) {
        concatenate(vm);
      } else if (IS_NUMBER(peek(vm, 0)) && IS_NUMBER(peek(vm, 1))) {
        double b = AS_NUMBER(pop(vm));
        double a = AS_NUMBER(pop(vm));
        push(vm, NUMBER_VAL(a + b));
      } else {
        runtime_error(vm, "Operands must be two numbers or two strings.");
        return INTERPRET_RUNTIME_ERROR;
      }
      break;
    }
    case OP_SUBTRACT:
      BINARY_OP(vm, NUMBER_VAL, -);
      break;
    case OP_MULTIPLY:
      BINARY_OP(vm, NUMBER_VAL, *);
      break;
    case OP_DIVIDE:
      BINARY_OP(vm, NUMBER_VAL, /);
      break;
    case OP_EQUAL: {
      Value b = pop(vm);
      Value a = pop(vm);
      push(vm, BOOL_VAL(values_equal(a, b)));
      break;
    }
    case OP_NOT_EQUAL: {
      Value b = pop(vm);
      Value a = pop(vm);
      push(vm, BOOL_VAL(!values_equal(a, b)));
      break;
    }
    case OP_GREATER:
      BINARY_OP(vm, BOOL_VAL, >);
      break;
    case OP_LESS:
      BINARY_OP(vm, BOOL_VAL, <);
      break;
    case OP_GREATER_EQUAL:
      BINARY_OP(vm, BOOL_VAL, >=);
      break;
    case OP_LESS_EQUAL:
      BINARY_OP(vm, BOOL_VAL, <=);
      break;
    case OP_NOT:
      push(vm, BOOL_VAL(is_falsey(pop(vm))));
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

  compile(vm, bytecode_sequence, source);

  vm->bytecode = bytecode_sequence;
  vm->ip = bytecode_sequence->code.data;

  return run(vm);
}
