#include <light.h>
#include <utils.h>

#include <math.h>
#include <stdio.h>

enum {
	MAX_LIGHT_LVL = 16,
};

int init_lightning(void)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	GLfloat light_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
	GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	GLfloat light_position[] = {0.0f, 1.0f, 0.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);


	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 1.0f);
	//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10.0f);

	return 0;
}

void update_lights(Camera* cam, uint8_t light_lvl) {

	GLfloat light_pos[] = { +0.2, -0.3, 0, 1.0f };
	GLfloat spot_dir[]  = { 0, 0, -1 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_dir);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f); // flash light is not fragmented enugh for nice look :(
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0.1f);

	GLfloat ambient_val  = 0.16f;
	GLfloat diffuse_val  = 0.16f * light_lvl;
	GLfloat specular_val = 0.16f * light_lvl;

	GLfloat ambient[]  = { ambient_val,  ambient_val,  ambient_val,  1.0f };
	GLfloat diffuse[]  = { diffuse_val,  diffuse_val,  diffuse_val,  1.0f };
	GLfloat specular[] = { specular_val, specular_val, specular_val, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// no light if far away
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.16f);
}



void update_light_level(int8_t* lvl, double current_time, double* last_light_update_time)
{
	if (fabs(current_time - *last_light_update_time) < 0.2) {
		// don't update too quickly
		return;
	}


	const Uint8* const keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_KP_PLUS] || keystate[SDL_SCANCODE_EQUALS]) {
		*lvl = (*lvl == MAX_LIGHT_LVL) ? MAX_LIGHT_LVL : (*lvl + 1);
		*last_light_update_time = current_time;
		return;
	}

	if (keystate[SDL_SCANCODE_KP_MINUS] || keystate[SDL_SCANCODE_MINUS]) {
		*lvl = (*lvl == 0) ? 0 : (*lvl - 1);
		*last_light_update_time = current_time;
		return;
	}

	if (keystate[SDL_SCANCODE_KP_EQUALS]) {
		*lvl = MAX_LIGHT_LVL;
		*last_light_update_time = current_time;
		return;
	}
}
