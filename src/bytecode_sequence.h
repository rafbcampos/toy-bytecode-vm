#ifndef bytecode_sequence_h
#define bytecode_sequence_h

#include "dynamic_array.h"
#include <stdint.h>

typedef double Value;
typedef struct {
  int line;
  int count;
} LineNumber;
typedef DYNAMIC_ARRAY(uint8_t) OpCodeArray;
typedef DYNAMIC_ARRAY(LineNumber) LineArray;
typedef DYNAMIC_ARRAY(Value) ValueArray;

typedef enum {
  OP_CONSTANT,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_NEGATE,
  OP_RETURN,
} OpCode;

typedef struct {
  OpCodeArray code;
  LineArray lines;
  ValueArray constants;
} BytecodeSequence;

void init_bytecode_sequence(BytecodeSequence *sequence);
void free_bytecode_sequence(BytecodeSequence *sequence);
void write_bytecode_sequence(BytecodeSequence *sequence, uint8_t byte,
                             int line);
int add_constant(BytecodeSequence *sequence, Value value);
void print_value(Value value);
int get_line(BytecodeSequence *sequence, int instructionIndex);

#endif
