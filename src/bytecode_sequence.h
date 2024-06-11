#ifndef bytecode_sequence_h
#define bytecode_sequence_h

#include "dynamic_array.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
  VAL_BOOL,
  VAL_NIL,
  VAL_NUMBER,
} ValueType;

typedef struct {
  ValueType type;
  union {
    bool boolean;
    double number;
  } as;
} Value;

#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)
#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NIL(value) ((value).type == VAL_NIL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define BOOL_VAL(value) ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})

typedef struct {
  int line;
  int count;
} LineNumber;

typedef DYNAMIC_ARRAY(uint8_t) OpCodeArray;

typedef DYNAMIC_ARRAY(LineNumber) LineArray;

typedef DYNAMIC_ARRAY(Value) ValueArray;

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
bool values_equal(Value a, Value b);

#endif
