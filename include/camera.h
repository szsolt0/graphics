#pragma once
#include <SDL2/SDL.h>

static const Uint32 MAX_RUN_LIMIT = 200;

typedef struct Camera {
	float x, y, z;
	float yaw, pitch; // in degrees
	Uint32 run_limit;
} Camera;

void move_camera(Camera* game);
