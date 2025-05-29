#include <cube.h>

void draw_cube(GLuint texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
		// Front face
		glNormal3f(0, 0, 1);
		glTexCoord2f(0, 0); glVertex3f(-0.5f, -0.5f,  0.5f);
		glTexCoord2f(1, 0); glVertex3f( 0.5f, -0.5f,  0.5f);
		glTexCoord2f(1, 1); glVertex3f( 0.5f,  0.5f,  0.5f);
		glTexCoord2f(0, 1); glVertex3f(-0.5f,  0.5f,  0.5f);

		// Back face
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3f( 0.5f, -0.5f, -0.5f);
		glTexCoord2f(1, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
		glTexCoord2f(1, 1); glVertex3f(-0.5f,  0.5f, -0.5f);
		glTexCoord2f(0, 1); glVertex3f( 0.5f,  0.5f, -0.5f);

		// Left face
		glNormal3f(-1, 0, 0);
		glTexCoord2f(0, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
		glTexCoord2f(1, 0); glVertex3f(-0.5f, -0.5f,  0.5f);
		glTexCoord2f(1, 1); glVertex3f(-0.5f,  0.5f,  0.5f);
		glTexCoord2f(0, 1); glVertex3f(-0.5f,  0.5f, -0.5f);

		// Right face
		glNormal3f(1, 0, 0);
		glTexCoord2f(0, 0); glVertex3f( 0.5f, -0.5f,  0.5f);
		glTexCoord2f(1, 0); glVertex3f( 0.5f, -0.5f, -0.5f);
		glTexCoord2f(1, 1); glVertex3f( 0.5f,  0.5f, -0.5f);
		glTexCoord2f(0, 1); glVertex3f( 0.5f,  0.5f,  0.5f);

		// Top face
		glNormal3f(0, 1, 0);
		glTexCoord2f(0, 0); glVertex3f(-0.5f,  0.5f,  0.5f);
		glTexCoord2f(1, 0); glVertex3f( 0.5f,  0.5f,  0.5f);
		glTexCoord2f(1, 1); glVertex3f( 0.5f,  0.5f, -0.5f);
		glTexCoord2f(0, 1); glVertex3f(-0.5f,  0.5f, -0.5f);

		// Bottom face
		glNormal3f(0, -1, 0);
		glTexCoord2f(0, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
		glTexCoord2f(1, 0); glVertex3f( 0.5f, -0.5f, -0.5f);
		glTexCoord2f(1, 1); glVertex3f( 0.5f, -0.5f,  0.5f);
		glTexCoord2f(0, 1); glVertex3f(-0.5f, -0.5f,  0.5f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}
