CC = gcc
CFLAGS = -g -Wall
OBJS = build/main.o build/bytecode_sequence.o build/dynamic_array.o build/debug.o
TARGET = build/main

$(TARGET): | build $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

build:
	mkdir -p build

build/main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o build/main.o

build/bytecode_sequence.o: bytecode_sequence.c
	$(CC) $(CFLAGS) -c bytecode_sequence.c -o build/bytecode_sequence.o

build/dynamic_array.o: dynamic_array.c
	$(CC) $(CFLAGS) -c dynamic_array.c -o build/dynamic_array.o

build/debug.o: debug.c
	$(CC) $(CFLAGS) -c debug.c -o build/debug.o

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f build/*.o $(TARGET)

