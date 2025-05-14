#include <light.h>

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

	return 0;
}


void update_lights(Camera* cam) {
	float light_distance = 2.0f;

	float light_x = cam->x + light_distance * sinf(degree_to_radian(cam->yaw)) * cosf(degree_to_radian(cam->pitch));
	float light_y = cam->y + 0.8f;
	float light_z = cam->z + light_distance * cosf(degree_to_radian(cam->yaw)) * cosf(degree_to_radian(cam->pitch));

	GLfloat light_position[] = { light_x, light_y, light_z, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}
