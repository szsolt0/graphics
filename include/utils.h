#pragma once
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct vec3
{
	float x;
	float y;
	float z;
} vec3;

typedef struct Color
{
	float red;
	float green;
	float blue;
} Color;

static inline
double degree_to_radian(double degree)
{
	return degree * M_PI / 180.0;
}

static inline
void* memzero(void* ptr, size_t len)
{
	return memset(ptr, 0, len);
}

static inline
Uint32 min_u32(Uint32 a, Uint32 b)
{
	return (a < b) ? a : b;
}

static inline
Uint32 max_u32(Uint32 a, Uint32 b)
{
	return (a > b) ? a : b;
}
