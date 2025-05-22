CC := gcc
CFLAGS := -Iinclude -Wall -Wextra -Wpedantic -Wformat=2 -Werror=vla -g -O2
LDLIBS := -lSDL2 -lGL -lSDL2_image -lm

WINDOWS_LDLIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lm

SRC := $(wildcard src/*.c)

TARGET := game

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDLIBS)

windows:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(WINDOWS_LDLIBS)

run: all
	./$(TARGET)

clean:
	rm $(TARGET)

.PHONY: all clean run windows
