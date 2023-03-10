#include "big.h"

#include "utility.h"
#include "graphics.h"

#include "entity.h"
#include "camera.h"
#include "tilemap.h"

#include "system.h"
#include "texture.h"



int framecount = 0;



Bool BigInit()
{
	OpenTilemapFile("testTilemapFile.bigtilemap");

	TestLoadTilemap();

	LoadTextures();

	SpawnEntity(ET_PLAYER,
		((WORLD_TMBLOCKLOAD_WIDTH >> 1) << (TILE_SHIFT + TMBLOCK_WIDTH_SHIFT)) - ((TMBLOCK_WIDTH >> 1) << TILE_SHIFT),
		((WORLD_TMBLOCKLOAD_HEIGHT >> 1) << (TILE_SHIFT + TMBLOCK_HEIGHT_SHIFT)) - ((TMBLOCK_HEIGHT >> 1) << TILE_SHIFT));

	return TRUE;
}



void BigUpdate()
{
	if (!(framecount & 0x1f))
	{
		if (keyboard_inputs[SDL_SCANCODE_D]) ShiftTilemapAndEntities(1, 0);
		if (keyboard_inputs[SDL_SCANCODE_A]) ShiftTilemapAndEntities(-1, 0);
	}

	ClearScreen();

	UpdateWorldEntities();

	//printf("%i, %i\n", ((int)player->x >> (TMBLOCK_WIDTH_SHIFT + TILE_SHIFT)) - 2, ((int)player->y >> (TMBLOCK_HEIGHT_SHIFT + TILE_SHIFT)) - 4);
	ShiftTilemapAndEntities(((int)player->x >> (TMBLOCK_WIDTH_SHIFT + TILE_SHIFT)) - (WORLD_TMBLOCKLOAD_WIDTH >> 1), ((int)player->y >> (TMBLOCK_HEIGHT_SHIFT + TILE_SHIFT)) - (WORLD_TMBLOCKLOAD_HEIGHT >> 1));

	printf("Num entities: %i\n", world_entities.count);

	framecount++;
}



void BigRender()
{
	
	UpdateCamera();

	RenderTilemap();

	RenderWorldEntities();
}



void BigQuit()
{
}
