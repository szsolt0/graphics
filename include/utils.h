#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>

#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))

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
uint32_t min_u32(uint32_t a, uint32_t b)
{
	return (a < b) ? a : b;
}

static inline
uint32_t max_u32(uint32_t a, uint32_t b)
{
	return (a > b) ? a : b;
}


static inline
bool likely(bool x)
{
	return __builtin_expect(x, true);
}

static inline
bool unlikely(bool x)
{
	return __builtin_expect(x, false);
}
