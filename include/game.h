#pragma once
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <stdbool.h>
#include <stdint.h>
#include "camera.h"

typedef struct Game
{
	SDL_Window* window;
	SDL_GLContext gl_context;
	double last_update_time;
	Uint32 width;
	Uint32 height;
	Camera camera;
	bool is_running;
} Game;

void init_game(Game* game, Uint32 width, Uint32 height);

void destroy_game(Game* game);

void handle_game_events(Game* game);

void update_game(Game* game);

void render_game(Game* game);

static inline
bool likely(bool x)
{
	return __builtin_expect(x, true);
}

static inline
bool unlikely(bool x)
{
	return __builtin_expect(x, false);
}

static inline
Game create_game(Uint32 width, Uint32 height)
{
	Game game;
	init_game(&game, width, height);
	return game;
}
