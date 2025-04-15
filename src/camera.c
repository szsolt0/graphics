#include <camera.h>
#include <utils.h>

#include <SDL2/SDL.h>
#include <GL/gl.h>

#include <stdbool.h>

static void move_camera(Camera* cam, Uint8 const* keystate) {
	float speed = 0.1f;
	bool is_moving = false;

	if (cam->sprint_limit > 0 && keystate[SDL_SCANCODE_LSHIFT]) {
		speed *= 10.0f;
		cam->sprint_limit -= 1;
	}

	const float dx = cosf(degree_to_radian(cam->yaw)) * speed;
	const float dz = sinf(degree_to_radian(cam->yaw)) * speed;

	if (keystate[SDL_SCANCODE_D]) { cam->x += dx; cam->z -= dz; is_moving = true; }
	if (keystate[SDL_SCANCODE_S]) { cam->x += dz; cam->z += dx; is_moving = true; }
	if (keystate[SDL_SCANCODE_A]) { cam->x -= dx; cam->z += dz; is_moving = true; }
	if (keystate[SDL_SCANCODE_W]) { cam->x -= dz; cam->z -= dx; is_moving = true; }

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
	glTranslatef(-cam->x, -cam->y, -cam->z);
}

void update_camera(Camera* cam)
{
	Uint8 const* keystate = SDL_GetKeyboardState(NULL);
	rotate_camera(cam, keystate);
	move_camera(cam, keystate);
}
