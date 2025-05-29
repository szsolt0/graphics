#include <camera.h>
#include <utils.h>

#include <SDL2/SDL.h>
//#include <GL/glew.h>
#include <GL/gl.h>

#include <stdbool.h>
#include <math.h>
#include <stdio.h>

// old variant, not the best :(
/*static bool collides_x(float prev_x, const float next_x, const float prev_z, const float next_z, const Wall* wall) {
	const float WALL_THICKNESS = 0.2;

	if ((wall->x0 - wall->x1) > 0.001f) {
		// z collider, not interested
		return false;
	}

	const float wall_x = wall->x0;

	const float min_z = fmin(wall->z0, wall->z1);
	const float max_z = fmax(wall->z0, wall->z1);

	if (min_z <= prev_z && prev_z <= max_z) {
		// wall is in "front" of us

		float min_x = wall_x - WALL_THICKNESS / 2.0f;
		float max_x = wall_x + WALL_THICKNESS / 2.0f;

		if (min_x <= next_x && next_x <= max_x) {
			return true;
		}
	}

	return false;
}*/

static bool collides_x(float prev_x, const float next_x, const float prev_z, const float next_z, const Wall* wall) {
	const float WALL_THICKNESS = 0.2f;
	const float CAMERA_RADIUS = 0.1f;

	// only x colliders
	if (fabsf(wall->x0 - wall->x1) > 0.001f) {
		return false;
	}

	const float wall_x = wall->x0;
	const float min_z = fminf(wall->z0, wall->z1) - CAMERA_RADIUS;
	const float max_z = fmaxf(wall->z0, wall->z1) + CAMERA_RADIUS;

	if ((prev_z >= min_z && prev_z <= max_z) || (next_z >= min_z && next_z <= max_z)) {
		const float min_x = wall_x - WALL_THICKNESS / 2.0f - CAMERA_RADIUS;
		const float max_x = wall_x + WALL_THICKNESS / 2.0f + CAMERA_RADIUS;

		if (next_x >= min_x && next_x <= max_x) {
			return true;
		}
	}

	return false;
}


static bool collides_z(float prev_x, const float next_x, const float prev_z, const float next_z, const Wall* wall) {
	const float WALL_THICKNESS = 0.2f;
	const float CAMERA_RADIUS = 0.1f;

	// only z colliders
	if (fabsf(wall->z0 - wall->z1) > 0.001f) {
		return false;
	}

	const float wall_z = wall->z0;
	const float min_x = fminf(wall->x0, wall->x1) - CAMERA_RADIUS;
	const float max_x = fmaxf(wall->x0, wall->x1) + CAMERA_RADIUS;

	if ((prev_x >= min_x && prev_x <= max_x) || (next_x >= min_x && next_x <= max_x)) {
		const float min_z = wall_z - WALL_THICKNESS / 2.0f - CAMERA_RADIUS;
		const float max_z = wall_z + WALL_THICKNESS / 2.0f + CAMERA_RADIUS;

		if (next_z >= min_z && next_z <= max_z) {
			return true;
		}
	}

	return false;
}



static void move_camera(Camera* cam, Uint8 const* keystate, const Wall* walls, size_t wallcnt)
{
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

			float new_x = cam->x + move_x;
			float new_z = cam->z + move_z;

			bool blocked_x = false, blocked_z = false;

			for (size_t j = 0; j < wallcnt; ++j) {
				const Wall* w = &walls[j];
				if (collides_x(cam->x, new_x, cam->z, cam->z, w)) blocked_x = true;
				if (collides_z(cam->x, cam->x, cam->z, new_z, w)) blocked_z = true;
			}

			if (!blocked_x) cam->x = new_x;
			if (!blocked_z) cam->z = new_z;
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
