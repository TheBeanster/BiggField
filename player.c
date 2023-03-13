#include "entity.h"

#include "system.h"



Entity* player = NULL;

void Player_Update(Entity* e)
{
	if (keyboard_inputs[SDL_SCANCODE_LEFT]) e->velx -= 0.08f;
	if (keyboard_inputs[SDL_SCANCODE_RIGHT]) e->velx += 0.08f;
	if (keyboard_inputs[SDL_SCANCODE_UP]) e->vely -= 0.08f;
	if (keyboard_inputs[SDL_SCANCODE_DOWN]) e->vely += 0.08f;

	if (keyboard_inputs[SDL_SCANCODE_LCTRL])
	{
		Entity* projectile = SpawnEntity(ET_TESTENTITIY, e->x, e->y);
		projectile->velx = RandomRange(3.5f, 4.5f);
		projectile->vely = RandomRange(-1.2f, -1.4f);
	}
}
