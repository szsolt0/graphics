#include <walls.h>
#include <utils.h>

#include <GL/gl.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*Wall the_walls[] = {
	// outer frame
	{0.0f, 0.0f, 5.0f, 0.0f}, // bottom
	{5.0f, 0.0f, 5.0f, 4.0f}, // right
	{5.0f, 5.0f, 0.0f, 5.0f}, // top
	{0.0f, 5.0f, 0.0f, 0.0f}, // left

	// inner
	{1.0f, 0.0f, 1.0f, 4.0f},
	{1.0f, 4.0f, 2.0f, 4.0f},
	{2.0f, 4.0f, 2.0f, 1.0f},
	{2.0f, 1.0f, 3.0f, 1.0f},
	{3.0f, 1.0f, 3.0f, 3.0f},
	{3.0f, 3.0f, 4.0f, 3.0f},
	{4.0f, 3.0f, 4.0f, 0.0f},
};*/

Wall the_walls[] = {
	// outer frame
	{1.0f, 0.0f, 10.0f, 0.0f},
	{10.0f, 0.0f, 10.0f, 10.0f},
	{10.0f, 10.0f, 0.0f, 10.0f},
	{0.0f, 10.0f, 0.0f, 0.0f},

	// inner maze
	{1.0f, 0.0f, 1.0f, 8.0f},
	{1.0f, 8.0f, 3.0f, 8.0f},
	{3.0f, 8.0f, 3.0f, 5.0f},
	{3.0f, 5.0f, 5.0f, 5.0f},
	{5.0f, 5.0f, 5.0f, 2.0f},
	{5.0f, 2.0f, 2.0f, 2.0f},
	{2.0f, 2.0f, 2.0f, 1.0f},

	{6.0f, 0.0f, 6.0f, 7.0f},
	{6.0f, 7.0f, 8.0f, 7.0f},
	{8.0f, 7.0f, 8.0f, 3.0f},
	{8.0f, 3.0f, 9.5f, 3.0f},

	// cross connectors
	{3.0f, 6.0f, 6.0f, 6.0f},
	{4.0f, 3.0f, 7.0f, 3.0f},

	// dead-ends
	{9.0f, 3.0f, 9.0f, 1.0f},
	{9.0f, 1.0f, 7.0f, 1.0f},
	{7.0f, 1.0f, 7.0f, 0.0f},

	// center box
	{4.0f, 7.0f, 4.0f, 9.0f},
	{4.0f, 9.0f, 6.0f, 9.0f},
	{6.0f, 9.0f, 6.0f, 7.0f},
	{6.0f, 7.0f, 4.0f, 7.0f},
};


ssize_t load_walls(Wall** walls, GLuint texture)
{
	*walls = the_walls;
	return ARRAY_LEN(the_walls);
}

static void print_walls(const Wall* w, size_t walls_len)
{
	for (size_t i = 0; i < walls_len; ++i) {
		printf("wall: [%f %f %f %f]\n", w->x0, w->x1, w->z0, w->z1);
	}
}

void render_walls(const Wall* walls, size_t walls_len, GLuint texture) {
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glColor3f(1.0f, 1.0f, 1.0f);

	for (size_t i = 0; i < walls_len; ++i) {
		const Wall* wall = &walls[i];

		const float dx = wall->x1 - wall->x0;
		const float dz = wall->z1 - wall->z0;
		const float length = sqrtf(dx*dx + dz*dz);

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);   glVertex3f(wall->x0, 0.0f, wall->z0);
			glTexCoord2f(length, 0.0f); glVertex3f(wall->x1, 0.0f, wall->z1);
			glTexCoord2f(length, 5.0f); glVertex3f(wall->x1, 5.0f, wall->z1);
			glTexCoord2f(0.0f, 5.0f);   glVertex3f(wall->x0, 5.0f, wall->z0);
		glEnd();
	}
}

void render_floor(GLuint floor_texture) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, floor_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);
		glTexCoord2f(  0.0f,   0.0f);  glVertex3f(-100.0f,  0.0f, -100.0f);
		glTexCoord2f(100.0f,   0.0f);  glVertex3f( 100.0f,  0.0f, -100.0f);
		glTexCoord2f(100.0f, 100.0f);  glVertex3f( 100.0f,  0.0f,  100.0f);
		glTexCoord2f(  0.0f, 100.0f);  glVertex3f(-100.0f,  0.0f,  100.0f);
	glEnd();
}

void render_roof(GLuint ceil_texture) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ceil_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);
		glTexCoord2f(  0.0f,   0.0f);  glVertex3f(-100.0f,  5.0f, -100.0f);
		glTexCoord2f(100.0f,   0.0f);  glVertex3f( 100.0f,  5.0f, -100.0f);
		glTexCoord2f(100.0f, 100.0f);  glVertex3f( 100.0f,  5.0f,  100.0f);
		glTexCoord2f(  0.0f, 100.0f);  glVertex3f(-100.0f,  5.0f,  100.0f);
	glEnd();
}
