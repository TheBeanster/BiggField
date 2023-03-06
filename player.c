#include "entity.h"

#include "system.h"



Entity* player = NULL;

void Player_Update(Entity* e)
{
	printf("{ %f, %f }\n", e->x, e->y);
	if (keyboard_inputs[SDL_SCANCODE_LEFT]) e->velx -= 0.02f;
	if (keyboard_inputs[SDL_SCANCODE_RIGHT]) e->velx += 0.02f;
	if (keyboard_inputs[SDL_SCANCODE_UP]) e->vely -= 0.02f;
	if (keyboard_inputs[SDL_SCANCODE_DOWN]) e->vely += 0.02f;
}