#ifndef bytecode_sequence_h
#define bytecode_sequence_h

#include "dynamic_array.h"
#include <stdint.h>

typedef double Value;
typedef DYNAMIC_ARRAY(uint8_t) OpCodeArray;
typedef DYNAMIC_ARRAY(int) LineArray;
typedef DYNAMIC_ARRAY(Value) ValueArray;

typedef enum {
  OP_CONSTANT,
  OP_RETURN,
} OpCode;

typedef struct {
  OpCodeArray code;
  LineArray lines;
  ValueArray constants;
} BytecodeSequence;

void initBytecodeSequence(BytecodeSequence *sequence);
void freeBytecodeSequence(BytecodeSequence *sequence);
void writeBytecodeSequence(BytecodeSequence *sequence, uint8_t byte, int line);
int addConstant(BytecodeSequence *sequence, Value value);
void printValue(Value value);

#endif
