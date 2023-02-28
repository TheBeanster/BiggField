#include "entity.h"

#include "graphics.h"
#include <math.h>
#include <stdio.h>



List world_entities = { 0 };



void UpdateWorldEntities(float dt)
{
	Entity* iterator = world_entities.begin;
	while (iterator)
	{
		UpdateEntity(iterator, dt);
		iterator = iterator->next;
	}

	iterator = world_entities.begin;
	while (iterator)
	{
		if (iterator->flags & EF_DELETE)
		{
			Entity* del = iterator;
			iterator = iterator->next;
			UnlinkFromList(&world_entities, del);
			DestroyEntity(del);
		} else
			iterator = iterator->next;
	}
}



void RenderWorldEntities()
{
	Entity* iterator = world_entities.begin;
	while (iterator)
	{
		RenderEntity(iterator);
		iterator = iterator->next;
	}
}





Entity* CreateEntity(EntityType type, float x, float y)
{
	Entity* e = ALLOC_TYPE(Entity);
	e->type = type;
	e->x = x;
	e->y = y;
	e->flags = 0;
	PushBackList(&world_entities, e);
	return e;
}



void DestroyEntity(Entity* e)
{
}



void UpdateEntity(Entity* e, float dt)
{
	switch (e->type)
	{
	case ET_NULL:
		printf("ERROR! Tried to update entity of type NULL\n");
		break;

	case ET_TESTENTITIY:
		e->x += 1.0f * dt;
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
