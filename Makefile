CC := gcc
CFLAGS := -Iinclude/ -Wall -Wextra -Wpedantic -Wformat=2 -Werror=vla -g -O2
LDLIBS := -lSDL2 -lGL -lGLU -lGLEW -lSDL2_image -lm

SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c, obj/%.o, $(SRC))

TARGET := game

all: $(TARGET)

run: all
	./$(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDLIBS)

obj/%.o: src/%.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj:
	mkdir -p obj

clean:
	rm -rf obj $(TARGET)

.PHONY: all clean run
