CC = gcc
CPPFLAGS = -Iinclude
TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

run: clean default
	-./$(TARGET)
	./$(TARGET) -f ./mynewdb.db -n
	./$(TARGET) -f ./mynewdb.db -a "Super Cow,root ln,13"

test:
	-./$(TARGET) -n

default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.db

$(TARGET): $(OBJ)
	$(CC) -o $@ $?

obj/%.o : src/%.c
	$(CC) -c $< -o $@ $(CPPFLAGS)


