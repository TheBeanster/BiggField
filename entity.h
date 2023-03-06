#pragma once

#include "utility.h"



enum
{
	EF_NOUPDATE = (1 << 0),
	EF_NORENDER = (1 << 1),

	EF_NOPHYSICS = (1 << 2),
	EF_NOCLIPPING = (1 << 3),
	EF_NOGRAVITY = (1 << 4),

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



typedef void (*EntityUpdateFunction)(struct Entity*);

typedef struct
{
	EntityTypeID id;
	const char* name;

	EntityFlags spawnflags;
	struct
	{
		char offsetx;
		char offsety;
		char w;
		char h;
	} hitbox;

	EntityUpdateFunction update_function;

} EntityType;

extern const EntityType entity_types[NUM_ENTITYTYPES];



typedef struct Entity
{
	ListLinksHeader(Entity);

	EntityFlags flags;
	EntityType* type;

	float x;
	float y;
	float velx;
	float vely;

	struct
	{
		char offsetx;
		char offsety;
		char w;
		char h;
	} htibox;
	

} Entity;



extern List world_entities;

void UpdateWorldEntities();

void RenderWorldEntities();

void ClearWorldEntities();



Entity* CreateEntity(EntityTypeID type, float x, float y);

void DestroyEntity(Entity* e);

void UpdateEntity(Entity* e);

void UpdateEntityPhysics(Entity* e);

void RenderEntity(Entity* e);



void TestEntity_Update(Entity* e);
