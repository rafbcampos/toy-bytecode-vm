#ifndef degug_h
#define degug_h

#include "bytecode_sequence.h"

void disassemble_bytecode_sequence(BytecodeSequence *bytecode_sequence,
                                   const char *name);
int disassemble_instruction(BytecodeSequence *bytecode_sequence, int offset);

#endif // !degug_h
