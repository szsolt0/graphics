CC := gcc
CFLAGS := -Iinclude -Wall -Wextra -Wpedantic -Wformat=2 -Werror=vla -g -O2
LDLIBS := -lSDL2 -lGL -lSDL2_image -lm

SRC := $(wildcard src/*.c)

TARGET := game

all:
	$(CC) $(CFLAGS) $(LDLIBS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm $(TARGET)

.PHONY: all clean run
