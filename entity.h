#pragma once

#include "utility.h"

#include "tilemap.h"



enum
{
	EF_NOUPDATE = (1 << 0),
	EF_NORENDER = (1 << 1),

	EF_NOPHYSICS = (1 << 2),
	EF_NOCLIPPING = (1 << 3),
	EF_NOGRAVITY = (1 << 4),

	EF_ONFLOOR = (1 << 5),
	EF_ONLWALL = (1 << 6),
	EF_ONRWALL = (1 << 7),
	EF_ONCEILING = (1 << 8),

	EF_DELETE = (1 << 15),
};
typedef unsigned short EntityFlags;



enum
{
	ET_NULL,
	ET_PLAYER,
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
	} clip_hitbox;
	float friction;
	float bounce;
	float mass;

	EntityUpdateFunction update_function;

} EntityType;

extern const EntityType entity_types[NUM_ENTITYTYPES];



typedef struct Entity
{
	ListLinksHeader(Entity);

	struct Entity* blockprev;
	struct Entity* blocknext;
	TilemapBlock* block;

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
	} clip_hitbox;
	float friction;
	float bounce;
	float mass;

} Entity;



extern List world_entities;

void UpdateWorldEntities();

void RenderWorldEntities();

void ClearWorldEntities();



Entity* SpawnEntity(EntityTypeID type, float x, float y);

void DestroyEntity(Entity* e);

void UpdateEntity(Entity* e);

extern float gravity;
void UpdateEntityPhysics(Entity* e);

void RenderEntity(Entity* e);





extern Entity* player;
void Player_Update(Entity* e);

void TestEntity_Update(Entity* e);
