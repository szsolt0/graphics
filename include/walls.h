#pragma once
#include <GL/glew.h>
#include <GL/gl.h>

#include <stdint.h>
#include <unistd.h>

typedef struct Wall
{
	// coordinates for the bottom of the wall
	int32_t x0, z0, x1, z1;
} Wall;

/// @brief load wall definitions from a file
/// @param walls the walls struct to initialize
/// @param file a readable file to read file data from.
/// @return the number of walls loaded, or a negative number indicating an error
///
/// currently, this function hardcodes the wall definitions, a prober version will
/// be implemented later.
ssize_t load_walls(Wall** walls, GLuint* texture, const char* file);

/// @brief render the walls in the game
/// @param walls the walls to render
/// @param walls_len the number of walls stored in `walls`
void render_walls(const Wall* walls, size_t walls_len, GLuint texture);
