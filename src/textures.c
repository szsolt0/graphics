#include <textures.h>
#include <utils.h>

#include <SDL2/SDL_image.h>

static GLuint load_texture(const char* file)
{
	SDL_Surface* surface = IMG_Load(file);
	if (!surface) {
		panic("Failed to load texture: %s", IMG_GetError());
	}

	SDL_Surface* formatted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
	SDL_FreeSurface(surface);

	if (!formatted) {
		panic("Failed to convert surface format: %s", SDL_GetError());
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, formatted->w, formatted->h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, formatted->pixels
	);

	SDL_FreeSurface(formatted);
	return texture;
}
