#pragma once
#include <GL/gl.h>
#include <SDL2/SDL.h>

typedef struct {} HelpMenu;

int init_help_menu(void);
void render_help_menu(GLuint texture_id, uint32_t window_w, uint32_t window_h);
