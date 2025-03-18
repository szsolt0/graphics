#include <camera.h>
#include <utils.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

void move_camera(Camera* cam) {
	float speed = 0.1f;
	bool is_moving = false;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	if (cam->run_limit > 0 && keystate[SDL_SCANCODE_LSHIFT]) {
		speed *= 2.5f;
		cam->run_limit -= 1;
	}

	const float dx = cosf(degree_to_radian(cam->yaw)) * speed;
	const float dz = sinf(degree_to_radian(cam->yaw)) * speed;

	if (keystate[SDL_SCANCODE_D]) { cam->x += dx; cam->z -= dz; is_moving = true; }
	if (keystate[SDL_SCANCODE_S]) { cam->x += dz; cam->z += dx; is_moving = true; }
	if (keystate[SDL_SCANCODE_A]) { cam->x -= dx; cam->z += dz; is_moving = true; }
	if (keystate[SDL_SCANCODE_W]) { cam->x -= dz; cam->z -= dx; is_moving = true; }

	// recover run limit on idle
	if (!is_moving) {
		cam->run_limit = min_u32(cam->run_limit + 1, MAX_RUN_LIMIT);
	}
}
