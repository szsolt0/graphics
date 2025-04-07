#pragma once
#include <walls.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <stdbool.h>
#include <stdint.h>
#include <camera.h>
#include <utils.h>

typedef struct Game
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* sprint_bar;
	Wall* walls;
	size_t walls_len;
	SDL_GLContext gl_context;
	double last_update_time;
	Uint32 width;
	Uint32 height;
	GLuint wall_texture;
	Camera camera;
	bool is_running;
} Game;

void init_game(Game* game, Uint32 width, Uint32 height);

void destroy_game(Game* game);

void handle_game_events(Game* game);

void update_game(Game* game);

void render_game(Game* game);

static inline
Game create_game(Uint32 width, Uint32 height)
{
	Game game;
	init_game(&game, width, height);
	return game;
}
