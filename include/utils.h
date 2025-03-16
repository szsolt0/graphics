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
