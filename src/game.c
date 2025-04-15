#include "game.h"
#include "utils.h"
#include <walls.h>

#include <GL/gl.h>

#include <stdio.h>

static int init_sdl(Game* game);
static int init_opengl(Game* game);

void init_game(Game* game, Uint32 width, Uint32 height)
{
	memzero(game, sizeof *game);
	game->width = width;
	game->height = height;

	if (0 != init_sdl(game)) {
		return;
	}

	if (0 != init_opengl(game)) {
		return;
	};

	game->last_update_time = (double)SDL_GetTicks() / 1000;
	game->camera.sprint_limit = MAX_SPRINT_LIMIT;

	SDL_SetRelativeMouseMode(SDL_TRUE);

	game->sprint_bar = IMG_LoadTexture(game->renderer, "assets/run_limit.png");
	if (!game->sprint_bar) {
		fprintf(stderr, "[ERROR] sprint_bar texture load failed: %s\n", SDL_GetError());
		return;
	}

	if (!game->window) {
		fprintf(stderr, "[ERROR] SDL_Window creation failed: %s\n", SDL_GetError());
		return;
	}
	if (!game->gl_context) {
		fprintf(stderr, "[ERROR] SDL_GL context creation failed: %s\n", SDL_GetError());
		return;
	}

	const ssize_t walls_len = load_walls(&game->walls, &game->wall_texture, "asd.txt");
	if (walls_len < 0) {
		fprintf(stderr, "[ERROR] loading walls from '%s' failed\n", "asd.txt");
		return;
	}
	game->walls_len = walls_len;

	game->is_running = true;
}

void destroy_game(Game* game)
{
	if (game->gl_context != NULL) {
		SDL_GL_DeleteContext(game->gl_context);
	}

	if (game->window != NULL) {
		SDL_DestroyWindow(game->window);
	}

	SDL_Quit();
}

static void handle_mouse_motion(Game* game, SDL_Event const* event)
{
	const float sensitivity = 0.1f;
	game->camera.yaw   -= event->motion.xrel * sensitivity;
	game->camera.yaw    = fmodf(game->camera.yaw, 360.0f); // restrict to circle
	game->camera.pitch -= event->motion.yrel * sensitivity;

	// restrict pitch to [-89°, 89°]
	if (game->camera.pitch > +89.0f) game->camera.pitch = +89.0f;
	if (game->camera.pitch < -89.0f) game->camera.pitch = -89.0f;
}

void handle_game_events(Game* game)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			game->is_running = false;
		}

		if (event.type == SDL_MOUSEMOTION) {
			handle_mouse_motion(game, &event);
		}
	}
}

void update_game(Game* game)
{
	// debug only
	printf("Game running: %d | Camera: x=%.2f y=%.2f z=%.2f yaw=%.2f pitch=%.2f run_limit: %f\n",
		game->is_running, game->camera.x, game->camera.y, game->camera.z,
		game->camera.yaw, game->camera.pitch,
		game->camera.sprint_limit
	);

	const double current_time = (double) SDL_GetTicks64() / 1000;
	const double elapsed_time = current_time - game->last_update_time;;

	game->last_update_time = current_time;
}


void render_game(Game* game)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	update_camera(&game->camera);
	render_walls(game->walls, game->walls_len, game->wall_texture);

	SDL_GL_SwapWindow(game->window);
}


static int init_sdl(Game* game) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "[ERROR] SDL initialization error: %s\n", SDL_GetError());
		return 1;
	}

	game->window = SDL_CreateWindow(
		"game",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		game->width, game->height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);
	if (!game->window) {
		fprintf(stderr, "[ERROR] SDL_Window creation failed: %s\n", SDL_GetError());
		return 1;
	}

	game->gl_context = SDL_GL_CreateContext(game->window);
	if (!game->gl_context) {
		fprintf(stderr, "[ERROR] SDL_GL context creation failed: %s\n", SDL_GetError());
		return 1;
	}

	game->renderer = SDL_CreateRenderer(game->window, -1, 0);
	if (!game->renderer) {
		fprintf(stderr, "[ERROR] SDL_Renderer creation failed: %s\n", SDL_GetError());
		return 1;
	}

	SDL_GL_SetSwapInterval(1); // VSync
	return 0;
}

static void init_lighting() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat lightPos[] = { 0.0f, 0.0f, 5.0f, 1.0f };
	GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}


static int init_opengl(Game* game)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	//setup_lighting();


	glClearColor(0.1, 0.1, 0.1, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// manual perspective projection (??)
	float fov = 60.0f;
	float aspect_ratio = (float)game->width / (float)game->height;
	float near_plane = 0.1f;
	float far_plane = 1000.0f;
	float top = near_plane * tanf((fov * M_PI / 180.0f) / 2);
	float bottom = -top;
	float right = top * aspect_ratio;
	float left = -right;

	glFrustum(left, right, bottom, top, near_plane, far_plane);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return 0;
}
