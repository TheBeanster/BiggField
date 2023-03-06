#include "tilemap.h"

#include "graphics.h"

#include "camera.h"



TilemapBlock* world_tilemapblocks[WORLD_TMBLOCK_COUNT] = { 0 };



void InitDebugTilemap()
{
	for (int i = 0; i < WORLD_TMBLOCK_COUNT; i++)
	{
		world_tilemapblocks[i] = MALLOC(sizeof(TilemapBlock));
		for (int y = 0; y < TMBLOCK_HEIGHT; y++)
		{
			for (int x = 0; x < TMBLOCK_WIDTH; x++)
			{
				world_tilemapblocks[i]->tiles[x | (y << TMBLOCK_WIDTH_SHIFT)] = (x == 0 || y == 0 || x == TMBLOCK_WIDTH_MASK || y == TMBLOCK_HEIGHT_MASK);
			}
		}

		world_tilemapblocks[i]->tiles[6 | (6 << TMBLOCK_WIDTH_SHIFT)] = 1;
	}
}



Tile GetTile(int x, int y)
{
	if (x < 0 || x >= (WORLD_TMBLOCK_SIZE * TMBLOCK_WIDTH) ||
		y < 0 || y >= (WORLD_TMBLOCK_SIZE * TMBLOCK_HEIGHT))
		return 0;
	TilemapBlock* b = world_tilemapblocks[
		(x >> TMBLOCK_WIDTH_SHIFT) + 
		((y >> TMBLOCK_HEIGHT_SHIFT) * WORLD_TMBLOCK_SIZE)];
	if (!b)
		return 0;
	return b->tiles[
		x & TMBLOCK_WIDTH_MASK | 
		((y & TMBLOCK_HEIGHT_MASK) << TMBLOCK_WIDTH_SHIFT)];
}

Bool GetTileSolid(int x, int y)
{
	SetColor(255, 255, 0, 255);
	DrawRectInWorld(x << TILE_SHIFT, y << TILE_SHIFT, 8, 8);
	return GetTile(x, y) != 0;
}



void RenderTilemap()
{
	SetColor(255, 255, 255, 255);

	for (int by = 0; by < WORLD_TMBLOCK_SIZE; by++)
	{
		for (int bx = 0; bx < WORLD_TMBLOCK_SIZE; bx++)
		{
			TilemapBlock* block = world_tilemapblocks[bx + (by * WORLD_TMBLOCK_SIZE)];
			if (!block) continue;

			int b_posx = bx * (TMBLOCK_WIDTH * TILE_SIZE) - camera_renderpos_x;
			int b_posy = by * (TMBLOCK_HEIGHT * TILE_SIZE) - camera_renderpos_y;

			for (int ty = 0; ty < TMBLOCK_HEIGHT; ty++)
			{
				for (int tx = 0; tx < TMBLOCK_WIDTH; tx++)
				{
					if (block->tiles[tx | (ty << TMBLOCK_WIDTH_SHIFT)])
					{
						FillRect(b_posx + (tx << TILE_SHIFT), b_posy + (ty << TILE_SHIFT), 8, 8);
					}
				}
			}
		}
	}
}
