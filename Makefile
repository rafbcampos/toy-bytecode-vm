CC = gcc
CFLAGS = -g -Wall
OBJS = build/main.o build/bytecode_sequence.o build/dynamic_array.o build/debug.o build/vm.o
TARGET = build/main

$(TARGET): | build $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

build:
	mkdir -p build

build/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o 

build/bytecode_sequence.o: src/bytecode_sequence.c
	$(CC) $(CFLAGS) -c src/bytecode_sequence.c -o build/bytecode_sequence.o

build/vm.o: src/vm.c
	$(CC) $(CFLAGS) -c src/vm.c -o build/vm.o

build/dynamic_array.o: src/dynamic_array.c
	$(CC) $(CFLAGS) -c src/dynamic_array.c -o build/dynamic_array.o

build/debug.o: src/debug.c
	$(CC) $(CFLAGS) -c src/debug.c -o build/debug.o

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f build/*.o $(TARGET)
