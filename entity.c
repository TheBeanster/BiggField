#include "entity.h"

#include "graphics.h"
#include <math.h>
#include <stdio.h>



Entity* SpawnEntity(EntityType type, float x, float y)
{
	return nullptr;
}



void UpdateEntity(Entity* e, float dt)
{
	switch (e->type)
	{
	case ET_NULL:
		printf("ERROR! Tried to update entity of type NULL\n");
		break;

	case ET_TESTENTITIY:
		e->x += 10.0f * dt;
		break;
		
	default:
		break;
	}
}



void RenderEntity(Entity* e)
{
	switch (e->type)
	{
	case ET_NULL:
		printf("ERROR! Tried to render entity of type NULL\n");
		break;

	case ET_TESTENTITIY:
		SetColor(0, 255, 0, 255);
		FillRect(roundf(e->x), roundf(e->y), 8, 8);
		break;

	default:
		break;
	}
}
