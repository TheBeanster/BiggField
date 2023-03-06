#include "entity.h"

#include "graphics.h"
#include <math.h>
#include <stdio.h>



const EntityType entity_types[NUM_ENTITYTYPES] =
{
	{ 0 },
	{
		ET_TESTENTITIY,
		"TestEntity",
		
		0,
		{
			0,
			0,
			6,
			6
		},
		&TestEntity_Update
		
	},
};



List world_entities = { 0 };



void UpdateWorldEntities()
{
	Entity* iterator = world_entities.begin;
	while (iterator)
	{
		UpdateEntity(iterator);
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



void ClearWorldEntities()
{
	Entity* iterator = world_entities.begin;
	while (iterator)
	{
		Entity* del = iterator;
		iterator = iterator->next;
		DestroyEntity(del);
	}
	world_entities.begin = NULL;
	world_entities.end = NULL;
	world_entities.count = 0;
}





Entity* CreateEntity(EntityTypeID type, float x, float y)
{
	Entity* e = ALLOC_TYPE(Entity);
	e->type = &entity_types[type];
	e->x = x;
	e->y = y;
	e->flags = e->type->spawnflags;
	e->htibox.offsetx = e->type->hitbox.offsetx;
	e->htibox.offsety = e->type->hitbox.offsety;
	e->htibox.w = e->type->hitbox.w;
	e->htibox.h = e->type->hitbox.h;

	PushBackList(&world_entities, e);
	return e;
}



void DestroyEntity(Entity* e)
{
	FREE(e);
}



void UpdateEntity(Entity* e)
{
	if (e->flags & EF_NOUPDATE) return;

	if (e->type == ET_NULL)
	{
		printf("ERROR! : Entity type is null\n");
		e->flags |= EF_DELETE;
		return;
	}

	if (e->type->update_function)
		e->type->update_function(e);

	if (!(e->flags & EF_NOPHYSICS))
		UpdateEntityPhysics(e);

}



void RenderEntity(Entity* e)
{
	if (e->flags & EF_NORENDER) return;

	switch (e->type->id)
	{
	case ET_NULL:
		printf("ERROR! Tried to render entity of type NULL\n");
		break;

	case ET_TESTENTITIY:
		SetColor(0, 255, 0, 255);
		FillRectInWorld(roundf(e->x), roundf(e->y), 8, 8);
		break;

	default:
		break;
	}
}





void TestEntity_Update(Entity* e)
{
	printf("TestEntity_Update. e.x = %f\n", e->x);
}
