#include <game.h>
#include <utils.h>
#include <walls.h>
#include <light.h>
#include <help_menu.h>

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

	game->camera.x = -20;
	game->camera.y = 1;
	game->camera.z = -20;
	game->camera.yaw = 0;

	game->last_update_time = (double)SDL_GetTicks() / 1000;
	game->camera.sprint_limit = MAX_SPRINT_LIMIT;

	game->light_level = 8;

	SDL_SetRelativeMouseMode(SDL_TRUE);

	if (!game->window) {
		fprintf(stderr, "[ERROR] SDL_Window creation failed: %s\n", SDL_GetError());
		return;
	}
	if (!game->gl_context) {
		fprintf(stderr, "[ERROR] SDL_GL context creation failed: %s\n", SDL_GetError());
		return;
	}

	if (init_textures(&game->textures) != 0) {
		fprintf(stderr, "[ERROR] texture loading failed\n");
		return;
	}

	if (init_help_menu() != 0) {
		fprintf(stderr, "[ERROR] help menu init failed\n");
		return;
	}

	game->walls_len = load_walls(&game->walls, 0);

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

	game->camera.yaw = game->camera.yaw < 0 ? 360 + game->camera.yaw : game->camera.yaw;

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

		if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F1) {
			game->show_help_menu = !game->show_help_menu;
		}
	}
}

void update_game(Game* game)
{
	// debug only
	printf("Game running: %d | Camera: x=%.2f y=%.2f z=%.2f yaw=%.2f pitch=%.2f sprint_limit: %f light_lvl: %d\n",
		game->is_running, game->camera.x, game->camera.y, game->camera.z,
		game->camera.yaw, game->camera.pitch,
		game->camera.sprint_limit, (int) game->light_level
	);

	const double current_time = (double) SDL_GetTicks64() / 1000;
	const double elapsed_time = fabs(current_time - game->last_update_time);

	update_camera(&game->camera, game->walls, game->walls_len);
	update_light_level(&game->light_level, current_time, &game->last_keypress_time);

	game->last_update_time = current_time;
}


void render_game(Game* game)
{
	Camera* cam = &game->camera;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	apply_camera_transform(cam);


	update_lights(&game->camera, game->light_level);


	render_floor(game->textures.ids[TexFloor]);
	render_roof(game->textures.ids[TexRoof]);
	render_walls(game->walls, game->walls_len, game->textures.ids[TexWallNormal]);

	if (game->show_help_menu) {
		render_help_menu(game->textures.ids[TexHelpMenu], game->width, game->height);
	}

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


static int init_opengl(Game* game)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	init_lightning();

	glClearColor(0.1, 0.1, 0.1, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	const float fov = 60.0f;
	const float aspect_ratio = (float)game->width / (float)game->height;
	const float near_plane = 0.1f;
	const float far_plane = 1000.0f;
	const float top = near_plane * tanf((fov * M_PI / 180.0f) / 2);
	const float bottom = -top;
	const float right = top * aspect_ratio;
	const float left = -right;

	glFrustum(left, right, bottom, top, near_plane, far_plane);

	return 0;
}
