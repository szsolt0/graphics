#pragma once
#include <GL/gl.h>

typedef struct Textures
{
	GLuint sprint_bar;
	GLuint wall_normal;
	GLuint roof;
	GLuint floor;
} Textures;

int init_textures(Textures* t);
int destroy_textures(Textures* t);
