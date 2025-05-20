#pragma once
#include <walls.h>

#include <SDL2/SDL.h>

static const float MAX_SPRINT_LIMIT = 300;

typedef struct Camera {
	float x, y, z;
	float yaw, pitch; // in degrees
	float sprint_limit;
} Camera;

void update_camera(Camera* restrict cam, const Wall* restrict walls, size_t wallcnt);
void apply_camera_transform(Camera const* cam);
