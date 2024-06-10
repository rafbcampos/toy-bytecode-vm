#ifndef debug_h
#define debug_h

#include "bytecode_sequence.h"

void disassembleBytecodeSequence(BytecodeSequence *bytecode_sequence,
                                 const char *name);
int disassembleInstruction(BytecodeSequence *bytecode_sequence, int offset);

#endif
