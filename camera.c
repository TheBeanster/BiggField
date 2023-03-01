#include "camera.h"

#include "system.h"



float camera_x = 0.0;
float camera_y = 0.0;

int camera_renderpos_x = 0;
int camera_renderpos_y = 0;



void UpdateCamera()
{
	if (keyboard_inputs[SDL_SCANCODE_LEFT])		camera_x -= 2.5f;
	if (keyboard_inputs[SDL_SCANCODE_RIGHT])	camera_x += 2.5f;
	if (keyboard_inputs[SDL_SCANCODE_UP])		camera_y -= 2.5f;
	if (keyboard_inputs[SDL_SCANCODE_DOWN])		camera_y += 2.5f;

	camera_renderpos_x = (int)roundf(camera_x);
	camera_renderpos_y = (int)roundf(camera_y);
}