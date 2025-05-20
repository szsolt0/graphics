#include <light.h>

#include <light.h>
#include <math.h>

int init_lightning(void)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat light_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
	GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	GLfloat light_position[] = {0.0f, 1.0f, 0.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 20.0f);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10.0f);

	return 0;
}

void update_lights(Camera* cam) {
	float yaw_rad = degree_to_radian(cam->yaw);
	float pitch_rad = degree_to_radian(cam->pitch);

	// Calculate direction vector
	float dir_z = cosf(pitch_rad) * cosf(yaw_rad);
	float dir_y = -sinf(pitch_rad);
	float dir_x = cosf(pitch_rad) * sinf(yaw_rad);

	GLfloat light_pos[] = { dir_x, dir_y, dir_z, 0.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	GLfloat ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuse[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}
