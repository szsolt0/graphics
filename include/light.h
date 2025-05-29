#pragma once
//#include <GL/glew.h>
#include <GL/gl.h>

#include <utils.h>
#include <camera.h>

typedef struct Spotlight
{
	GLfloat position[4];
	GLfloat direction[3];

	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];

	GLfloat cutoff;
	GLfloat exponent;

	GLfloat constant_att;
	GLfloat linear_att;
	GLfloat quadraticAtt;

	GLenum gl_light_id;
} Spotlight;

int init_lightning(void);
void update_lights(Camera* cam, uint8_t light_lvl);
void update_light_level(int8_t* lvl, double current_time, double* last_light_update_time);
