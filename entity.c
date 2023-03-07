#include "entity.h"

#include "graphics.h"
#include <math.h>
#include <stdio.h>



const EntityType entity_types[NUM_ENTITYTYPES] =
{
	{ 0 },

	{
		ET_PLAYER,
		"Player",
		0,
		{
			-3,
			-7,
			6,
			14
		},
		&Player_Update
	},

	{
		ET_TESTENTITIY,
		"TestEntity",
		
		0,
		{
			0,
			0,
			8,
			8
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
	e->clip_hitbox.offsetx = e->type->clip_hitbox.offsetx;
	e->clip_hitbox.offsety = e->type->clip_hitbox.offsety;
	e->clip_hitbox.w = e->type->clip_hitbox.w;
	e->clip_hitbox.h = e->type->clip_hitbox.h;

	if (type == ET_PLAYER)
		player = e;

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

	default:
		break;
	}

	SetColor(0, 255, 0, 255);
	FillRectInWorld(
		(int)roundf(e->x) + e->clip_hitbox.offsetx,
		(int)roundf(e->y) + e->clip_hitbox.offsety,
		e->clip_hitbox.w,
		e->clip_hitbox.h);
}



#include "system.h"

void TestEntity_Update(Entity* e)
{
	if (keyboard_inputs[SDL_SCANCODE_LEFT]) e->velx -= 0.02f;
	if (keyboard_inputs[SDL_SCANCODE_RIGHT]) e->velx += 0.02f;
	if (keyboard_inputs[SDL_SCANCODE_UP]) e->vely -= 0.02f;
	if (keyboard_inputs[SDL_SCANCODE_DOWN]) e->vely += 0.02f;
}
