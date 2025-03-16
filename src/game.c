#include "game.h"
#include "utils.h"
#include <stdio.h>
#include <GL/gl.h>

static
int init_sdl(Game* game);

static
void init_opengl(Game* game);

void init_game(Game* game, Uint32 width, Uint32 height)
{
	game->is_running = false;
	game->width = width;
	game->height = height;
	if (init_sdl(game) != 0) {
		return;
	}
	init_opengl(game);
	game->last_update_time = (double)SDL_GetTicks() / 1000;
	game->is_running = true;

	SDL_SetRelativeMouseMode(SDL_TRUE);

	if (!game->window) {
		fprintf(stderr, "[ERROR] SDL_Window creation failed: %s\n", SDL_GetError());
	}
	if (!game->gl_context) {
		fprintf(stderr, "[ERROR] SDL_GL context creation failed: %s\n", SDL_GetError());
	}

	memset(&game->camera, 0, sizeof game->camera);
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

void move_camera(Game* game) {
	float speed = 0.1f;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	Camera* cam = &game->camera;

	if (keystate[SDL_SCANCODE_LSHIFT]) {
		speed *= 2.0f;
	}

	const float dx = cosf(degree_to_radian(cam->yaw)) * speed;
	const float dz = sinf(degree_to_radian(cam->yaw)) * speed;

	if (keystate[SDL_SCANCODE_D]) { cam->x += dx; cam->z -= dz; }
	if (keystate[SDL_SCANCODE_S]) { cam->x += dz; cam->z += dx; }
	if (keystate[SDL_SCANCODE_A]) { cam->x -= dx; cam->z += dz; }
	if (keystate[SDL_SCANCODE_W]) { cam->x -= dz; cam->z -= dx; }
}


void handle_game_events(Game* game)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			game->is_running = false;
		}

		if (event.type == SDL_MOUSEMOTION) {
			const float sensitivity = 0.1f;
			game->camera.yaw   -= event.motion.xrel * sensitivity;
			game->camera.pitch -= event.motion.yrel * sensitivity;

			// restrict pitch to [-89°, 89°]
			if (game->camera.pitch > +89.0f) game->camera.pitch = +89.0f;
			if (game->camera.pitch < -89.0f) game->camera.pitch = -89.0f;
		}
	}
}

void update_game(Game* game)
{
	printf("Game running: %d | Camera: x=%.2f y=%.2f z=%.2f yaw=%.2f pitch=%.2f\n",
		game->is_running, game->camera.x, game->camera.y, game->camera.z,
		game->camera.yaw, game->camera.pitch
	);

	double current_time;
	double elapsed_time;

	current_time = (double)SDL_GetTicks() / 1000;
	elapsed_time = current_time - game->last_update_time;
	game->last_update_time = current_time;

	move_camera(game);
}

void draw_cube(float x, float y, float z, float size) {
	float half = size / 2.0f;

	glBegin(GL_QUADS);

	// Front face (red)
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(x - half, y - half, z + half);
	glVertex3f(x + half, y - half, z + half);
	glVertex3f(x + half, y + half, z + half);
	glVertex3f(x - half, y + half, z + half);

	// Back face (green)
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(x - half, y - half, z - half);
	glVertex3f(x + half, y - half, z - half);
	glVertex3f(x + half, y + half, z - half);
	glVertex3f(x - half, y + half, z - half);

	// Left face (blue)
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(x - half, y - half, z - half);
	glVertex3f(x - half, y - half, z + half);
	glVertex3f(x - half, y + half, z + half);
	glVertex3f(x - half, y + half, z - half);

	// Right face (yellow)
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(x + half, y - half, z - half);
	glVertex3f(x + half, y - half, z + half);
	glVertex3f(x + half, y + half, z + half);
	glVertex3f(x + half, y + half, z - half);

	// Top face (cyan)
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(x - half, y + half, z - half);
	glVertex3f(x + half, y + half, z - half);
	glVertex3f(x + half, y + half, z + half);
	glVertex3f(x - half, y + half, z + half);

	// Bottom face (magenta)
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(x - half, y - half, z - half);
	glVertex3f(x + half, y - half, z - half);
	glVertex3f(x + half, y - half, z + half);
	glVertex3f(x - half, y - half, z + half);

	glEnd();
}

void draw_cubes() {
	for (int x = -2; x <= 2; x++) {
		for (int y = -2; y <= 2; y++) {
			draw_cube(x * 2.5f, y * 2.5f, -10.0f, 2.0f);
		}
	}
}


void render_game(Game* game)
{
	//printf("Rendering frame...\n");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	const bool space = keystate[SDL_SCANCODE_SPACE];
	const float extra_yaw = space ? 180.0f : 0.0f;

	// camera view
	glRotatef(-game->camera.pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(-game->camera.yaw + extra_yaw, 0.0f, 1.0f, 0.0f);
	glTranslatef(-game->camera.x, -game->camera.y, -game->camera.z);

	draw_cubes();

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

	SDL_GL_SetSwapInterval(1); // VSync
	return 0;
}

void setup_lighting() {
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


static
void init_opengl(Game* game)
{
	glEnable(GL_DEPTH_TEST);

	setup_lighting();


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
}
