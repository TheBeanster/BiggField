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



/// @brief Loads the data of a block
/// @param x World block coord 
/// @param y World block coord
/// @return Pointer to the new block or NULL
static TilemapBlock* load_block(int x, int y)
{
	if (x < 0 || x >= world_block_width || y < 0 || y >= world_block_height) return NULL;

	int ptr = get_block_ptr(x + (y * world_block_width));
	printf("Loading block (%i, %i) from address %X\n", x, y, ptr);
	if (!ptr) return NULL;

	TilemapBlock* block = MALLOC(sizeof(TilemapBlock));
	for (int y = 0; y < TMBLOCK_HEIGHT; y++)
	{
		for (int x = 0; x < TMBLOCK_WIDTH; x++)
		{
			block->tiles[x | (y << TMBLOCK_WIDTH_SHIFT)] = (x == 0 || y == 0 || x == TMBLOCK_WIDTH_MASK || y == TMBLOCK_HEIGHT_MASK);
		}
	}

	block->tiles[6 | (6 << TMBLOCK_WIDTH_SHIFT)] = 1;
	block->tiles[11 | (0 << TMBLOCK_WIDTH_SHIFT)] = 0;
	block->tiles[12 | (0 << TMBLOCK_WIDTH_SHIFT)] = 0;
	block->tiles[11 | (7 << TMBLOCK_WIDTH_SHIFT)] = 0;
	block->tiles[12 | (7 << TMBLOCK_WIDTH_SHIFT)] = 0;
	block->tiles[0 | (3 << TMBLOCK_WIDTH_SHIFT)] = 0;
	block->tiles[0 | (4 << TMBLOCK_WIDTH_SHIFT)] = 0;
	block->tiles[15 | (3 << TMBLOCK_WIDTH_SHIFT)] = 0;
	block->tiles[15 | (4 << TMBLOCK_WIDTH_SHIFT)] = 0;

	return block;

	/*for (int i = 0; i < TMBLOCK_COUNT >> 1; i++)
	{

	}*/
}



void TestLoadTilemap()
{
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			world_tilemapblocks[x + (y * WORLD_TMBLOCKLOAD_WIDTH)] = load_block(x, y);
		}
	}
}