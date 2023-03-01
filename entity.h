#pragma once

#include "utility.h"

enum
{
	ET_NULL,
	ET_TESTENTITIY,
};
typedef unsigned char EntityType;

enum
{
	EF_NOUPDATE = (1 << 0),
	EF_NORENDER = (1 << 1),
	
	EF_DELETE = (1 << 15),
};
typedef unsigned short EntityFlags;



typedef struct Entity
{
	ListLinksHeader(Entity);

	EntityFlags flags;
	EntityType type;

	float x;
	float y;
} Entity;



extern List world_entities;

void UpdateWorldEntities();

void RenderWorldEntities();



Entity* CreateEntity(EntityType type, float x, float y);

void DestroyEntity(Entity* e);

void UpdateEntity(Entity* e);

void RenderEntity(Entity* e);
