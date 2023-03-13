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
		1.0f,
		0.0f,
		100.0f,

		&Player_Update
	},

	{
		ET_TESTENTITIY,
		"TestEntity",
		
		0,
		{
			-3,
			-3,
			6,
			6
		},
		0.995f,
		-0.5f,
		10.0f,

		&TestEntity_Update
		
	},
};





static TilemapBlock* get_tilemapblock(float x, float y)
{
	if (x < 0 || y < 0) return NULL;

	int blockx = (int)x >> (TILE_SHIFT + TMBLOCK_WIDTH_SHIFT);
	int blocky = (int)y >> (TILE_SHIFT + TMBLOCK_HEIGHT_SHIFT);

	if (blockx < 0 || blockx >= WORLD_TMBLOCKLOAD_WIDTH || blocky < 0 || blocky >= WORLD_TMBLOCKLOAD_HEIGHT) return NULL;

	return world_tilemapblocks[blockx + (blocky * WORLD_TMBLOCKLOAD_WIDTH)];
}



static Bool update_entity_block(Entity* e)
{
	TilemapBlock* block = get_tilemapblock(e->x, e->y);

	if (!block)
		return FALSE; // Out of bounds

	if (e->block != block)
	{
		// Entity has changed block
		if (e->block)
		{
			// If this block isn't null then remove self from it
			UnlinkFromList(&e->block->entities, &e->blockprev);
			e->block = NULL;
		}
	} else
	{
		// In same block as before
		return TRUE;
	}

	// Push self into new block
	PushBackList(&block->entities, &e->blockprev);
	e->block = block;

	return TRUE;
}





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





Entity* SpawnEntity(EntityTypeID type, float x, float y)
{
	Entity* e = ALLOC_TYPE(Entity);
	if (!e)
	{
		printf("ERROR! : Couldn't allocate memory for entity\n");
		return NULL;
	}

	e->x = x;
	e->y = y;

	if (!update_entity_block(e))
	{
		FREE(e);
		return NULL;
	}

	e->type = &entity_types[type];
	e->flags = e->type->spawnflags;
	e->clip_hitbox.offsetx = e->type->clip_hitbox.offsetx;
	e->clip_hitbox.offsety = e->type->clip_hitbox.offsety;
	e->clip_hitbox.w = e->type->clip_hitbox.w;
	e->clip_hitbox.h = e->type->clip_hitbox.h;
	e->friction = e->type->friction;
	e->bounce = e->type->bounce;
	e->mass = e->type->mass;

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

	if (!update_entity_block(e))
		e->flags |= EF_DELETE;
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

}
