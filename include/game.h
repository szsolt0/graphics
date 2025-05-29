#pragma once
#include <utils.h>
#include <walls.h>
#include <camera.h>
#include <textures.h>

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdbool.h>
#include <stdint.h>

typedef struct Game
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* sprint_bar;
	Wall* walls;
	size_t walls_len;
	SDL_GLContext gl_context;
	double last_update_time;
	double last_keypress_time;
	Uint32 width;
	Uint32 height;
	Textures textures;
	Camera camera;
	int8_t light_level;
	bool is_running;
	bool show_help_menu;
} Game;

void init_game(Game* game, Uint32 width, Uint32 height);

void destroy_game(Game* game);

void handle_game_events(Game* game);

void update_game(Game* game);

void render_game(Game* game);
