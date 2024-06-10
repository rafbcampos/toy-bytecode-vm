#ifndef vm_h
#define vm_h

#include "bytecode_sequence.h"

#define DEBUG_TRACE_EXECUTION

#define STACK_MAX 256

typedef struct {
  BytecodeSequence *bytecode;
  uint8_t *ip; // instruction pointer
  Value stack[STACK_MAX];
  Value *stack_top;
} VM;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

void init_vm(VM *vm);
void free_vm(VM *vm);
void push(VM *vm, Value value);
Value pop(VM *vm);
InterpretResult interpret(VM *vm, BytecodeSequence *bytecode);

#endif
