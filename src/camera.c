#include <camera.h>
#include <utils.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>

#include <stdbool.h>
#include <math.h>

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
	const float MOVEMENT_THRESHOLD = 0.001f;

	if ((wall->x0 - wall->x1) > 0.001f) {
		return false;
	}

	const float wall_x = wall->x0;
	const float min_z = fmin(wall->z0, wall->z1);
	const float max_z = fmax(wall->z0, wall->z1);

	if (min_z <= prev_z && prev_z <= max_z) {
		float min_x = wall_x - WALL_THICKNESS / 2.0f;
		float max_x = wall_x + WALL_THICKNESS / 2.0f;

		if ((prev_x < min_x && next_x >= min_x) || (prev_x > max_x && next_x <= max_x)) {
			return true;
		}

		if (min_x <= next_x && next_x <= max_x) {
			if (fabs(prev_x - next_x) > MOVEMENT_THRESHOLD) {
				return true;
			}
		}
	}

	return false;
}


static bool collides_z(float prev_x, const float next_x, const float prev_z, const float next_z, const Wall* wall) {
	const float WALL_THICKNESS = 0.2f;
	const float MOVEMENT_THRESHOLD = 0.001f;

	if ((wall->z0 - wall->z1) > 0.001f) {
		return false;
	}

	const float wall_z = wall->z0;
	const float min_x = fmin(wall->x0, wall->x1);
	const float max_x = fmax(wall->x0, wall->x1);

	if (min_x <= prev_x && prev_x <= max_x) {
		float min_z = wall_z - WALL_THICKNESS / 2.0f;
		float max_z = wall_z + WALL_THICKNESS / 2.0f;

		if ((prev_z < min_z && next_z >= min_z) || (prev_z > max_z && next_z <= max_z)) {
			return true;
		}

		if (min_z <= next_z && next_z <= max_z) {
			if (fabs(prev_z - next_z) > MOVEMENT_THRESHOLD) {
				return true;
			}
		}
	}

	return false;
}


static void move_camera(Camera* cam, Uint8 const* keystate, const Wall* walls, size_t wallcnt) {
	float speed = 0.1f;
	bool is_moving = false;

	if (cam->sprint_limit > 0 && keystate[SDL_SCANCODE_LSHIFT]) {
		speed *= 10.0f;
		cam->sprint_limit -= 1;
	}

	float dx = cosf(degree_to_radian(cam->yaw)) * speed;
	float dz = sinf(degree_to_radian(cam->yaw)) * speed;

	float old_x = cam->x;
	float old_z = cam->z;

	if (keystate[SDL_SCANCODE_D]) { cam->x += dx; cam->z -= dz; is_moving = true; }
	if (keystate[SDL_SCANCODE_S]) { cam->x += dz; cam->z += dx; is_moving = true; }
	if (keystate[SDL_SCANCODE_A]) { cam->x -= dx; cam->z += dz; is_moving = true; }
	if (keystate[SDL_SCANCODE_W]) { cam->x -= dz; cam->z -= dx; is_moving = true; }

	bool coll_x = false, coll_z = false;
	for (size_t i = 0; i < wallcnt; ++i) {
		const Wall* w = &walls[i];
		if (collides_x(old_x, cam->x, old_z, cam->z, w)) {
			coll_x = true;
		}
		if (collides_z(old_x, cam->x, old_z, cam->z, w)) {
			coll_z = true;
		}
	}

	if (coll_x) cam->x = old_x;
	if (coll_z) cam->z = old_z;


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
	//const BoolPair colls = collides_with_x_or_z(walls, wallcnt, cam->x, cam->y);

	rotate_camera(cam, keystate);
	move_camera(cam, keystate, walls, wallcnt);

	glTranslatef(-cam->x, -cam->y, -cam->z);
}
