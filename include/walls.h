#pragma once
//#include <GL/glew.h>
#include <GL/gl.h>

#include <stdint.h>
#include <unistd.h>

typedef struct Wall
{
	// coordinates for the bottom of the wall
	float x0, z0, x1, z1;
} Wall;

ssize_t load_walls(Wall** walls, GLuint texture);

/// @brief render the walls in the game
/// @param walls the walls to render
/// @param walls_len the number of walls stored in `walls`
void render_walls(const Wall* walls, size_t walls_len, GLuint texture);

void render_floor(GLuint floor_texture);
void render_roof(GLuint ceil_texture);
