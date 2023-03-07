#include "entity.h"

#include "system.h"



Entity* player = NULL;

void Player_Update(Entity* e)
{
	if (keyboard_inputs[SDL_SCANCODE_LEFT]) e->velx -= 0.08f;
	if (keyboard_inputs[SDL_SCANCODE_RIGHT]) e->velx += 0.08f;
	if (keyboard_inputs[SDL_SCANCODE_UP]) e->vely -= 0.08f;
	if (keyboard_inputs[SDL_SCANCODE_DOWN]) e->vely += 0.08f;
}