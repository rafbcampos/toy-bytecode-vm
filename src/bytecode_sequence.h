#ifndef bytecode_sequence_h
#define bytecode_sequence_h

#include "dynamic_array.h"

typedef enum {
  OP_CONSTANT,
  OP_NIL,
  OP_TRUE,
  OP_FALSE,
  OP_EQUAL,
  OP_NOT_EQUAL,
  OP_GREATER,
  OP_GREATER_EQUAL,
  OP_LESS,
  OP_LESS_EQUAL,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_NOT,
  OP_NEGATE,
  OP_RETURN,
} OpCode;

typedef struct {
  Uint8Array code;
  IntArray lines;
  ValueArray constants;
} BytecodeSequence;

void init_bytecode_sequence(BytecodeSequence *sequence);
void free_bytecode_sequence(BytecodeSequence *sequence);
void write_bytecode_sequence(BytecodeSequence *sequence, uint8_t byte,
                             int line);
int add_constant(BytecodeSequence *sequence, Value value);

#endif // !bytecode_sequence_h
