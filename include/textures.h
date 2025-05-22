#pragma once
#include <GL/gl.h>
#include <stddef.h>

enum {
	TexShadow     = 0,
	TexWallNormal = 1,
	TexRoof       = 2,
	TexFloor      = 3,
};

#define TEXTURES_LEN 4

typedef struct Textures
{
	GLuint ids[TEXTURES_LEN];
} Textures;

int init_textures(Textures* t);
int destroy_textures(Textures* t);
