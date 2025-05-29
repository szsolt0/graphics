#include <textures.h>
#include <utils.h>

#include <SDL2/SDL_image.h>

#include <errno.h>
#include <stdio.h>


#define BASE_DIR "assets/textures/"

typedef struct TextureFileMapEntry
{
	size_t offset;
	const char* file;
} TextureFileMapEntry;

static const TextureFileMapEntry texture_file_map[] = {
	{TexHelpMenu,   BASE_DIR "help_menu.png"},
	{TexWallNormal, BASE_DIR "wall.png"},
	{TexFloor,      BASE_DIR "floor.png"},
	{TexRoof,       BASE_DIR "roof.png"},
};

static GLuint load_texture(const char* file)
{
	SDL_Surface* surface = IMG_Load(file);
	if (!surface) {
		fprintf(stderr, "Failed to load texture: %s\n", IMG_GetError());
		return 0;
	}

	SDL_Surface* formatted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
	SDL_FreeSurface(surface);

	if (!formatted) {
		fprintf(stderr, "Failed to convert surface format: %s\n", IMG_GetError());
		return 0;
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SDL_FreeSurface(formatted);
	return texture;
}

int init_textures(Textures* t)
{
	memzero(t, sizeof *t);

	for (size_t i = 0; i < ARRAY_LEN(texture_file_map); ++i) {
		const GLuint tex_id = load_texture(texture_file_map[i].file);

		if (unlikely(tex_id == 0)) {
			fprintf(stderr, "failed to load texture file '%s'\n", texture_file_map[i].file);

			// a texture file not found, so "unload" all
			// textures and reset t to zeros
			destroy_textures(t);
			memzero(t, sizeof *t);

			// "no entry" is the closest error code to what happened
			return ENOENT;
		}

		t->ids[texture_file_map[i].offset] = tex_id;
	}

	return 0;
}

int destroy_textures(Textures* t)
{
	glDeleteTextures(TEXTURES_LEN, t->ids);

	return 0;
}
