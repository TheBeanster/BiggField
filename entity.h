#pragma once

#include "utility.h"



enum
{
	EF_NOUPDATE = (1 << 0),
	EF_NORENDER = (1 << 1),

	EF_DELETE = (1 << 15),
};
typedef unsigned short EntityFlags;

enum
{
	ET_NULL,
	ET_TESTENTITIY,
	NUM_ENTITYTYPES
};
typedef unsigned short EntityTypeID;

typedef struct
{
	const char* name;
	struct
	{
		EntityFlags spawnflags;
	} info;
} EntityType;
extern const EntityType entity_types[NUM_ENTITYTYPES];

#define E_TYPEINFO(entity) (entity_types[entity->type].info)



typedef struct Entity
{
	ListLinksHeader(Entity);

	EntityFlags flags;
	EntityTypeID type;

	float x;
	float y;
} Entity;



extern List world_entities;

void UpdateWorldEntities();

void RenderWorldEntities();

void ClearWorldEntities();



Entity* CreateEntity(EntityTypeID type, float x, float y);

void DestroyEntity(Entity* e);

void UpdateEntity(Entity* e);

void RenderEntity(Entity* e);
