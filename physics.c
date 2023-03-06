#include "entity.h"



void UpdateEntityPhysics(Entity* e)
{
	e->x += e->velx;
	e->y += e->vely;
}
