all:
	gcc -Iinclude/ -o game src/*.c -Wall -Wextra -Wpedantic -Wformat=2 -Werror=vla -g -O2 -lSDL2 -lGL -lGLU -lGLEW -lSDL2_image -lm
