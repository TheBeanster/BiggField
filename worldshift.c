#include "tilemap.h"

#include "entity.h"



int world_posx = (WORLD_TMBLOCKLOAD_WIDTH >> 1) + 1;
int world_posy = (WORLD_TMBLOCKLOAD_HEIGHT >> 1) + 1;



void ShiftTilemapAndEntities(int shiftx, int shifty)
{
	if (shiftx == 0 && shifty == 0) return;

	world_posx += shiftx;
	world_posy += shifty;

	TilemapBlock* newblocks[WORLD_TMBLOCKLOAD_COUNT] = { 0 };
	Bool decorateblocks[WORLD_TMBLOCKLOAD_COUNT] = { 0 };

	for (int y = 0; y < WORLD_TMBLOCKLOAD_HEIGHT; y++)
	{
		for (int x = 0; x < WORLD_TMBLOCKLOAD_WIDTH; x++)
		{
			TilemapBlock** oldblock = GetWorldBlockPtr(x + shiftx, y + shifty);
			if (oldblock)
			{
				newblocks[x + (y * WORLD_TMBLOCKLOAD_WIDTH)] = *oldblock;
			}
			else
			{
				TilemapBlock** loadblock = GetWorldBlockPtr(WORLD_TMBLOCKLOAD_WIDTH - x - 1, WORLD_TMBLOCKLOAD_HEIGHT - y - 1);
				FREE(*loadblock); // Delete the old block
				newblocks[x + (y * WORLD_TMBLOCKLOAD_WIDTH)] = LoadBlock(
					world_posx - (WORLD_TMBLOCKLOAD_WIDTH >> 1) - 1 + x,
					world_posy - (WORLD_TMBLOCKLOAD_HEIGHT >> 1) - 1 + y);
			}
		}
	}

	for (int y = 0; y < WORLD_TMBLOCKLOAD_HEIGHT; y++)
		for (int x = 0; x < WORLD_TMBLOCKLOAD_WIDTH; x++)
			*GetWorldBlockPtr(x, y) = newblocks[x + (y * WORLD_TMBLOCKLOAD_WIDTH)];

	for (int y = 0; y < WORLD_TMBLOCKLOAD_HEIGHT; y++)
		for (int x = 0; x < WORLD_TMBLOCKLOAD_WIDTH; x++)
			if (!(x == 0 || y == 0 || x == (WORLD_TMBLOCKLOAD_WIDTH - 1) || y == (WORLD_TMBLOCKLOAD_HEIGHT - 1)))
				DecorateBlockTiles(x, y);



	Entity* iterator = world_entities.begin;
	while (iterator)
	{
		iterator->x -= (shiftx * (TMBLOCK_WIDTH * TILE_SIZE));
		iterator->y -= (shifty * (TMBLOCK_HEIGHT * TILE_SIZE));

		iterator = iterator->next;
	}

}
