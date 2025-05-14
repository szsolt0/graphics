#pragma once
#include <GL/gl.h>
#include <stddef.h>

static const size_t TexShadow     = 0;
static const size_t TexWallNormal = 1;
static const size_t TexRoof       = 2;
static const size_t TexFloor      = 3;

#define TEXTURES_LEN 4

typedef struct Textures
{
	GLuint ids[TEXTURES_LEN];
} Textures;

int init_textures(Textures* t);
int destroy_textures(Textures* t);
