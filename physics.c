#include "entity.h"

#include <math.h>
#include "tilemap.h"

#include "graphics.h"



float gravity = 0.005f;



static void clip_entity(Entity* e)
{
	int x = (int)roundf(e->x) + e->htibox.offsetx; // Integer world position
	int y = (int)roundf(e->y) + e->htibox.offsety; // Integer world position
	char w = e->htibox.w;
	char h = e->htibox.h;
	int leftx = x; // x position of left edge
	int topy = y; // y position of top edge
	int rightx = x + w - 1; // x position of right edge
	int bottomy = y + h - 1; // y position of bottom edge
	
	// Reset collision flags
	e->flags &= ~(EF_ONFLOOR | EF_ONLWALL | EF_ONRWALL | EF_ONCEILING);

	// The number of tiles the hitbox reaches accross vertically
	int vertical_reach = (h >> TILE_SHIFT) + 1;
	if (( (topy & TILE_MASK) + ((h - 1) & TILE_MASK) ) > (TILE_SIZE - 1)) vertical_reach++;
	if (!(h & TILE_MASK)) vertical_reach--; // Less tile reach if multiple of 8

	// The number of tiles the hitbox reaches accross horizontally
	int horizontal_reach = (w >> TILE_SHIFT) + 1;
	if (( (leftx & TILE_MASK) + ((w - 1) & TILE_MASK) ) > (TILE_SIZE - 1)) horizontal_reach++;
	if (!(w & TILE_MASK)) horizontal_reach--; // Less tile reach if multiple of 8

	Bool clippedx = FALSE;
	Bool clippedy = FALSE;



	// === EDGE COLLISION ===

	if (e->velx >= 0.0f) // If moving to the right
	{
		// Check the right edge
		for (int i = 1; i < vertical_reach; i++)
		{
			if (GetTileSolid(rightx >> TILE_SHIFT, (topy >> TILE_SHIFT) + i - (e->vely >= 0.0f)))
			{
				x = (rightx & ~TILE_MASK) - e->htibox.offsetx - w;
				clippedx = TRUE;
				break;
			}
		}
	} else
	{
		// Check the left edge
		for (int i = 1; i < vertical_reach; i++)
		{
			if (GetTileSolid(leftx >> TILE_SHIFT, (topy >> TILE_SHIFT) + i - (e->vely >= 0.0f)))
			{
				x = ((leftx + TILE_SIZE) & ~TILE_MASK) - e->htibox.offsetx;
				clippedx = TRUE;
				break;
			}
		}
	}

	if (e->vely >= 0.0f) // If moving down
	{
		// Check the bottom edge
		for (int i = 1; i < horizontal_reach; i++)
		{
			if (GetTileSolid((leftx >> TILE_SHIFT) + 1 - (e->velx >= 0.0f), bottomy >> TILE_SHIFT))
			{
				y = (bottomy & ~TILE_MASK) - e->htibox.offsety - h;
				clippedy = TRUE;
				break;
			}
		}
	} else
	{
		// Check the top edge
		for (int i = 1; i < horizontal_reach; i++)
		{
			if (GetTileSolid((leftx >> TILE_SHIFT) + 1 - (e->velx >= 0.0f), topy >> TILE_SHIFT))
			{
				x = ((topy + TILE_SIZE) & ~TILE_MASK) - e->htibox.offsety;
				clippedy = TRUE;
				break;
			}
		}
	}



	if (clippedx)
	{
		e->x = (float)x;
		if (e->velx >= 0.0f)
			e->flags |= EF_ONLWALL;
		else
			e->flags |= EF_ONRWALL;

		e->velx = 0.0f; 
	}
	if (clippedy)
	{
		e->y = (float)y;
		if (e->vely >= 0.0f)
			e->flags |= EF_ONFLOOR;
		else
			e->flags |= EF_ONCEILING;

		e->vely = 0.0f; 
	}

	SetColor(255, 0, 0, 255);
	DrawRectInWorld(x, y, w, h);

}



void UpdateEntityPhysics(Entity* e)
{
	if (!(e->flags & EF_NOGRAVITY))
		e->vely += gravity;
	
	e->x += e->velx;
	e->y += e->vely;

	if (!(e->flags & EF_NOCLIPPING))
		clip_entity(e);

	
}
