# Computer Graphics Project

Simple labirinth game

## Description

This is a straightforward labyrinth game where the objective is to navigate through the maze and find the exit.

The maze walls are statically defined in the file `src/walls.c`, within the constant `the_walls`.
However, the structure of the code allows for rewriting with dynamic wall loading by modifying only the `load_walls`
function, without affecting other parts of the program.

## Assets

Assets can be downloaded from here:

https://drive.google.com/drive/folders/1PYi7EQtkOhjznKYWmnMSt3MmTQeTzTY0?usp=sharing

## Compiling

```sh
make
```

On Windows:

```sh
make windows
```

## Controls

- mouse: look arround
- `w`, `a`, `s`, `d`: move
- `<shift>`: run
- `<space>`: look back
- `+`, `-`: change light value
