#include <camera.h>
#include <utils.h>

#include <SDL2/SDL.h>
//#include <GL/glew.h>
#include <GL/gl.h>

#include <stdbool.h>
#include <math.h>
#include <stdio.h>


static bool aabb_intersect(float ax, float az, float aradius, float bx0, float bz0, float bx1, float bz1) {
    float amin_x = ax - aradius;
    float amax_x = ax + aradius;
    float amin_z = az - aradius;
    float amax_z = az + aradius;

    float bmin_x = fminf(bx0, bx1);
    float bmax_x = fmaxf(bx0, bx1);
    float bmin_z = fminf(bz0, bz1);
    float bmax_z = fmaxf(bz0, bz1);

    return (
		amax_x > bmin_x && amin_x < bmax_x &&
		amax_z > bmin_z && amin_z < bmax_z
	);
}


static void move_camera(Camera* cam, Uint8 const* keystate, const Wall* walls, size_t wallcnt)
{
	const float CAMERA_RADIUS = 0.1;

	float speed = 0.1f;
	bool is_moving = false;

	if (cam->sprint_limit > 0 && keystate[SDL_SCANCODE_LSHIFT]) {
		speed *= 10.0f;
		cam->sprint_limit -= 1;
	}

	int steps = (speed > 0.2f) ? (int)(speed / 0.2f) : 1;
	float step_size = speed / steps;

	for (int i = 0; i < steps; ++i) {
		float yaw_rad = degree_to_radian(cam->yaw);
		float forward_x = sinf(yaw_rad);
		float forward_z = cosf(yaw_rad);

		float left_x = -forward_z;
		float left_z = forward_x;

		float move_x = 0.0f;
		float move_z = 0.0f;

		if (keystate[SDL_SCANCODE_S]) {
			move_x += forward_x;
			move_z += forward_z;
			is_moving = true;
		}
		if (keystate[SDL_SCANCODE_W]) {
			move_x -= forward_x;
			move_z -= forward_z;
			is_moving = true;
		}
		if (keystate[SDL_SCANCODE_A]) {
			move_x += left_x;
			move_z += left_z;
			is_moving = true;
		}
		if (keystate[SDL_SCANCODE_D]) {
			move_x -= left_x;
			move_z -= left_z;
			is_moving = true;
		}

		// Normalize move vector if not zero
		float length = sqrtf(move_x * move_x + move_z * move_z);
		if (length > 0.0f) {
			move_x = (move_x / length) * step_size;
			move_z = (move_z / length) * step_size;

			bool block_full = false;
			bool block_x = false;
			bool block_z = false;

			for (size_t j = 0; j < wallcnt; ++j) {
				const Wall* w = &walls[j];

				float wall_thickness = 0.2f;
				float pad = wall_thickness / 2.0f;

				float wx0 = fminf(w->x0, w->x1) - pad;
				float wx1 = fmaxf(w->x0, w->x1) + pad;
				float wz0 = fminf(w->z0, w->z1) - pad;
				float wz1 = fmaxf(w->z0, w->z1) + pad;

				// block full movement
				if (aabb_intersect(cam->x + move_x, cam->z + move_z, CAMERA_RADIUS, wx0, wz0, wx1, wz1)) {
					block_full = true;
				}

				// block x movement
				if (aabb_intersect(cam->x + move_x, cam->z, CAMERA_RADIUS, wx0, wz0, wx1, wz1)) {
					block_x = true;
				}

				// block z movement
				if (aabb_intersect(cam->x, cam->z + move_z, CAMERA_RADIUS, wx0, wz0, wx1, wz1)) {
					block_z = true;
				}
			}

			if (!block_full) {
				cam->x += move_x;
				cam->z += move_z;
			} else {
				if (!block_x) cam->x += move_x;
				if (!block_z) cam->z += move_z;
			}
		}
	}

	// recover sprint on idle
	if (!is_moving) {
		cam->sprint_limit = min_u32(cam->sprint_limit + 1, MAX_SPRINT_LIMIT);
	}
}



static void rotate_camera(Camera* cam, Uint8 const* keystate)
{
	const bool space = keystate[SDL_SCANCODE_SPACE];
	const float true_yaw = space ? 180.0f : 0.0f;

	// camera view
	glRotatef(-cam->pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(-cam->yaw + true_yaw, 0.0f, 1.0f, 0.0f);
}

void update_camera(Camera* restrict cam, const Wall* restrict walls, size_t wallcnt)
{
	Uint8 const* keystate = SDL_GetKeyboardState(NULL);
	move_camera(cam, keystate, walls, wallcnt);
}

void apply_camera_transform(Camera const* cam)
{
	glRotatef(-cam->pitch, 1.0f, 0.0f, 0.0f);

	float true_yaw = 0.0f;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	if (keystate[SDL_SCANCODE_SPACE]) {
		true_yaw = 180.0f;
	}
	glRotatef(-cam->yaw + true_yaw, 0.0f, 1.0f, 0.0f);

	glTranslatef(-cam->x, -cam->y, -cam->z);
}
