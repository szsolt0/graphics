#include <walls.h>
#include <utils.h>

#include <GL/gl.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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
		fprintf(stderr, "Failed to convert surface format: %s\n", SDL_GetError());
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(formatted);
	return texture;
}

ssize_t load_walls(Wall** walls, GLuint* texture, const char* file)
{
	*texture = load_texture("assets/wall2.png");

	const Wall the_walls[] = {
		(Wall) {0, 0, 10, 0},
		(Wall) {10, 0, 10, 50},
		(Wall) {10, 50, 800, 50},
	};

	Wall* wall_ptr = malloc(sizeof(Wall) * ARRAY_LEN(the_walls));

	if (!wall_ptr) {
		*walls = NULL;
		return -ENOMEM;
	}

	memcpy(wall_ptr, the_walls, sizeof the_walls);
	*walls = wall_ptr;

	return ARRAY_LEN(the_walls);
}

static void print_walls(const Wall* w, size_t walls_len)
{
	for (size_t i = 0; i < walls_len; ++i) {
		printf("wall: [%d %d %d %d]\n", w->x0, w->x1, w->z0, w->z1);
	}
}

/// @brief render the walls in the game
/// @param walls the walls to render
/// @param walls_len the number of walls stored in `walls`
void render_walls(const Wall* walls, size_t walls_len, GLuint texture) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glColor3f(1.0f, 1.0f, 1.0f);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	for (size_t i = 0; i < walls_len; ++i) {
		const Wall* wall = &walls[i];

		const float x_wall_width = fabs(wall->x1 - wall->x0);
		const float z_wall_width = fabs(wall->z1 - wall->z0);
		const float wall_width = fabs(sqrt(x_wall_width*x_wall_width + z_wall_width*z_wall_width));

		glBegin(GL_QUADS);

		//glColor3f(1.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(wall->x0, 0.0f, wall->z0);
		glTexCoord2f(wall_width, 0.0f); glVertex3f(wall->x1, 0.0f, wall->z1);
		glTexCoord2f(wall_width, 1.0f); glVertex3f(wall->x1, 1.0f, wall->z1);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(wall->x0, 1.0f, wall->z0);

		glEnd();
	}
}
