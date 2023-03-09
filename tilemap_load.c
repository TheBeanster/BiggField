#include "tilemap.h"



FILE* tilemap_file = NULL;



Bool OpenTilemapFile(const char* const name)
{
	tilemap_file = fopen(name, "rb");

	if (!tilemap_file)
	{
		printf("ERROR! : Couldn't open tilemap file\n");
		return FALSE;
	}

	fread(&world_block_width, sizeof(int), 1, tilemap_file);
	fread(&world_block_height, sizeof(int), 1, tilemap_file);
	world_block_count = world_block_width * world_block_height;

	printf("Opened tilemap \"%s\". w = %i, h = %i\n", name, world_block_width, world_block_height);

	return TRUE;
}



static int get_block_ptr(int i)
{
	fseek(tilemap_file, 8 + (i << 2), SEEK_SET);
	int ptr;
	fread(&ptr, sizeof(int), 1, tilemap_file);
	return ptr;
}



TilemapBlock* LoadBlock(int bx, int by)
{
	if (bx < 0 || bx >= world_block_width || by < 0 || by >= world_block_height) return NULL;

	int ptr = get_block_ptr(bx + (by * world_block_width));
	if (!ptr) return NULL;

	TilemapBlock* block = MALLOC(sizeof(TilemapBlock));
	
	fseek(tilemap_file, ptr, SEEK_SET);

	for (int i = 0; i < TMBLOCK_COUNT >> 1; i++)
	{
		unsigned char byte = fgetc(tilemap_file);
		block->tiles[(i << 1)] = (Tile)(byte & 0b00001111);
		block->tiles[(i << 1) + 1] = (Tile)(byte >> 4);
	}

	return block;
}



void DecorateBlockTiles(int bx, int by)
{
	TilemapBlock* block = GET_BLOCK(bx, by);

	printf("Decorating block, %i, %i\n", bx, by);

	if (!block) return;

	/*TilemapBlock* blockleft = bx > 0 ? GET_BLOCK(bx - 1, by) : NULL;
	TilemapBlock* blockright = bx < WORLD_TMBLOCKLOAD_WIDTH - 1 ? GET_BLOCK(bx + 1, by) : NULL;
	TilemapBlock* blockup = by > 0 ? GET_BLOCK(bx, by - 1) : NULL;
	TilemapBlock* blockdown = by < WORLD_TMBLOCKLOAD_HEIGHT - 1 ? GET_BLOCK(bx, by + 1) : NULL;

	printf("(%i, %i) : l%i, r%i, u%i, d%i\n", bx, by, blockleft, blockright, blockup, blockdown);*/
	
	for (int y = 0; y < TMBLOCK_HEIGHT; y++)
	{
		for (int x = 0; x < TMBLOCK_WIDTH; x++)
		{
			block->tiles[x | (y << TMBLOCK_WIDTH_SHIFT)] = (block->tiles[x | (y << TMBLOCK_WIDTH_SHIFT)]) << 8;
		}
	}
}



void TestLoadTilemap()
{
	for (int y = 0; y < WORLD_TMBLOCKLOAD_HEIGHT; y++)
		for (int x = 0; x < WORLD_TMBLOCKLOAD_WIDTH; x++)
			world_tilemapblocks[x + (y * WORLD_TMBLOCKLOAD_WIDTH)] = LoadBlock(x, y);

	for (int y = 0; y < WORLD_TMBLOCKLOAD_HEIGHT; y++)
		for (int x = 0; x < WORLD_TMBLOCKLOAD_WIDTH; x++)
			DecorateBlockTiles(x, y);
}