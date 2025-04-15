#pragma once
#include <SDL2/SDL.h>

static const float MAX_SPRINT_LIMIT = 300;

typedef struct Camera {
	float x, y, z;
	float yaw, pitch; // in degrees
	float sprint_limit;
} Camera;

void update_camera(Camera* cam);
