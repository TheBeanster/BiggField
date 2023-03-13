#include "tilemap.h"

#include "graphics.h"
#include "system.h"
#include "texture.h"

#include "camera.h"

#include "entity.h"



TilemapBlock* world_tilemapblocks[WORLD_TMBLOCKLOAD_COUNT] = { 0 };



TilemapBlock* GetWorldBlock(int x, int y)
{
	if (x < 0 || y < 0 || x >= WORLD_TMBLOCKLOAD_WIDTH || y >= WORLD_TMBLOCKLOAD_HEIGHT) return NULL;
	return world_tilemapblocks[x + (y * WORLD_TMBLOCKLOAD_WIDTH)];
}



TilemapBlock** GetWorldBlockPtr(int x, int y)
{
	if (x < 0 || y < 0 || x >= WORLD_TMBLOCKLOAD_WIDTH || y >= WORLD_TMBLOCKLOAD_HEIGHT) return NULL;
	return &world_tilemapblocks[x + (y * WORLD_TMBLOCKLOAD_WIDTH)];
}



void InitDebugTilemap()
{
	for (int i = 0; i < WORLD_TMBLOCKLOAD_COUNT; i++)
	{
		world_tilemapblocks[i] = ALLOC_TYPE(TilemapBlock);

		for (int y = 0; y < TMBLOCK_HEIGHT; y++)
		{
			for (int x = 0; x < TMBLOCK_WIDTH; x++)
			{
				world_tilemapblocks[i]->tiles[x | (y << TMBLOCK_WIDTH_SHIFT)] = (x == 0 || y == 0 || x == TMBLOCK_WIDTH_MASK || y == TMBLOCK_HEIGHT_MASK);
			}
		}

		world_tilemapblocks[i]->tiles[6 | (6 << TMBLOCK_WIDTH_SHIFT)] = 1;
		world_tilemapblocks[i]->tiles[11 | (0 << TMBLOCK_WIDTH_SHIFT)] = 0;
		world_tilemapblocks[i]->tiles[12 | (0 << TMBLOCK_WIDTH_SHIFT)] = 0;
		world_tilemapblocks[i]->tiles[11 | (7 << TMBLOCK_WIDTH_SHIFT)] = 0;
		world_tilemapblocks[i]->tiles[12 | (7 << TMBLOCK_WIDTH_SHIFT)] = 0;
		world_tilemapblocks[i]->tiles[0 | (3 << TMBLOCK_WIDTH_SHIFT)] = 0;
		world_tilemapblocks[i]->tiles[0 | (4 << TMBLOCK_WIDTH_SHIFT)] = 0;
		world_tilemapblocks[i]->tiles[15 | (3 << TMBLOCK_WIDTH_SHIFT)] = 0;
		world_tilemapblocks[i]->tiles[15 | (4 << TMBLOCK_WIDTH_SHIFT)] = 0;

		for (int j = 0; j < i; j++)
		{
			world_tilemapblocks[i]->tiles[((j & 7) + 1) | (1 << TMBLOCK_WIDTH_SHIFT)] = 1;
		}
	}
}



Tile GetTile(int x, int y)
{
	if (x < 0 || x >= (WORLD_TMBLOCKLOAD_WIDTH * TMBLOCK_WIDTH) ||
		y < 0 || y >= (WORLD_TMBLOCKLOAD_HEIGHT * TMBLOCK_HEIGHT))
		return 0;
	TilemapBlock* b = world_tilemapblocks[
		(x >> TMBLOCK_WIDTH_SHIFT) + 
		((y >> TMBLOCK_HEIGHT_SHIFT) * WORLD_TMBLOCKLOAD_WIDTH)];
	if (!b)
		return 0;
	return b->tiles[
		x & TMBLOCK_WIDTH_MASK | 
		((y & TMBLOCK_HEIGHT_MASK) << TMBLOCK_WIDTH_SHIFT)];
}

Bool GetTileSolid(int x, int y)
{
	return GetTile(x, y) != 0;
}





int world_block_width = 0;
int world_block_height = 0;
int world_block_count = 0;



void DeleteBlockEntities(TilemapBlock* block)
{
	if (!block) return;
	Entity* iterator = block->entities.begin;
	while (iterator)
	{
		Entity* del = iterator;
		iterator = iterator->blocknext;
		UnlinkFromList(&world_entities, del);
		FREE(del);
	}
}



void RenderTilemap()
{
	SetColor(0, 0, 0, 255);

	SDL_Rect dstrect = { 0, 0, 8, 8 };
	SDL_Rect srcrect = { 0, 0, 8, 8 };

	for (int by = 0; by < WORLD_TMBLOCKLOAD_HEIGHT; by++)
	{
		for (int bx = 0; bx < WORLD_TMBLOCKLOAD_WIDTH; bx++)
		{
			TilemapBlock* block = world_tilemapblocks[bx + (by * WORLD_TMBLOCKLOAD_WIDTH)];
			int b_posx = bx * (TMBLOCK_WIDTH * TILE_SIZE) - camera_renderpos_x;
			int b_posy = by * (TMBLOCK_HEIGHT * TILE_SIZE) - camera_renderpos_y;

			if (!block)
			{
				FillRect(b_posx, b_posy, TMBLOCK_WIDTH << TILE_SHIFT, TMBLOCK_HEIGHT << TILE_SHIFT);
			} else
			{
				for (int ty = 0; ty < TMBLOCK_HEIGHT; ty++)
				{
					for (int tx = 0; tx < TMBLOCK_WIDTH; tx++)
					{
						Tile t = block->tiles[tx | (ty << TMBLOCK_WIDTH_SHIFT)];
						if (t)
						{
							if (t == 0xFFFF)
							{
								FillRect(b_posx + (tx << TILE_SHIFT), b_posy + (ty << TILE_SHIFT), 8, 8);
							} else
							{
								srcrect.x = GET_TILE_TYPE(t) << TILE_SHIFT;
								srcrect.y = (GET_TILE_STYLE(t) - 1) << TILE_SHIFT;
								dstrect.x = b_posx + (tx << TILE_SHIFT);
								dstrect.y = b_posy + (ty << TILE_SHIFT);
								SDL_RenderCopy(main_renderer, tilemaptiles_texture, &srcrect, &dstrect);
							}
							//FillRect(b_posx + (tx << TILE_SHIFT), b_posy + (ty << TILE_SHIFT), 8, 8);
						}
					}
				}
			}
		}
	}
}
