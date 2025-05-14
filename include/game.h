#pragma once
#include <utils.h>
#include <walls.h>
#include <stdint.h>
#include <camera.h>
#include <textures.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdbool.h>

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
	Textures textures;
	Camera camera;
	bool is_running;
} Game;

void init_game(Game* game, Uint32 width, Uint32 height);

void destroy_game(Game* game);

void handle_game_events(Game* game);

void update_game(Game* game);

void render_game(Game* game);
