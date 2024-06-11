#include "bytecode_sequence.h"
#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void repl(VM *vm, BytecodeSequence *bytecode_sequence) {
  char line[1024];
  for (;;) {
    printf("> ");
    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }
    interpret(vm, bytecode_sequence, line);
  }
}

static char *read_file(const char *path) {
  FILE *file = fopen(path, "rb");

  if (file == NULL) {
    printf("Error opening file: %s", path);
    exit(74);
  }

  fseek(file, 0L, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  char *buffer = (char *)malloc(file_size + 1);

  if (buffer == NULL) {
    printf("Error allocating memory");
    exit(74);
  }

  size_t bytes_read = fread(buffer, sizeof(char), file_size, file);

  if (bytes_read < file_size) {
    printf("Error reading file: %s", path);
    exit(74);
  }

  buffer[bytes_read] = '\0';

  fclose(file);
  return buffer;
}

static void run_file(VM *vm, BytecodeSequence *bytecode_sequence,
                     const char *path) {
  char *source = read_file(path);
  InterpretResult result = interpret(vm, bytecode_sequence, source);
  free(source);

  if (result == INTERPRET_COMPILE_ERROR)
    exit(65);
  if (result == INTERPRET_RUNTIME_ERROR)
    exit(70);
}

int main(int argc, const char *argv[]) {
  VM vm;
  BytecodeSequence bytecode_sequence;
  init_vm(&vm);
  init_bytecode_sequence(&bytecode_sequence);

  if (argc == 1) {
    repl(&vm, &bytecode_sequence);
  } else if (argc == 2) {
    run_file(&vm, &bytecode_sequence, argv[1]);
  } else {
    exit(64);
  }

  free_vm(&vm);
  free_bytecode_sequence(&bytecode_sequence);

  return 0;
}
