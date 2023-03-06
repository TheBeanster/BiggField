#include "entity.h"

#include <math.h>
#include "tilemap.h"

#include "graphics.h"



float gravity = 0.01f;



static void clip_entity(Entity* e)
{
	int x = (int)roundf(e->x) + e->clip_hitbox.offsetx; // Integer world position
	int y = (int)roundf(e->y) + e->clip_hitbox.offsety; // Integer world position
	char w = e->clip_hitbox.w;
	char h = e->clip_hitbox.h;
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
				x = (rightx & ~TILE_MASK) - e->clip_hitbox.offsetx - w;
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
				x = ((leftx + TILE_SIZE) & ~TILE_MASK) - e->clip_hitbox.offsetx;
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
			if (GetTileSolid((leftx >> TILE_SHIFT) + i - (e->velx >= 0.0f), bottomy >> TILE_SHIFT))
			{
				y = (bottomy & ~TILE_MASK) - e->clip_hitbox.offsety - h;
				clippedy = TRUE;
				break;
			}
		}
	} else
	{
		// Check the top edge
		for (int i = 1; i < horizontal_reach; i++)
		{
			if (GetTileSolid((leftx >> TILE_SHIFT) + i - (e->velx >= 0.0f), topy >> TILE_SHIFT))
			{
				y = ((topy + TILE_SIZE) & ~TILE_MASK) - e->clip_hitbox.offsety;
				clippedy = TRUE;
				break;
			}
		}
	}


	
	if (e->velx >= 0.0f)
	{
		float right_grid_offset = fmodf(e->x + (float)e->clip_hitbox.offsetx + (float)w - 0.5f, TILE_SIZE);

		if (e->vely >= 0.0f)
		{
			float bottom_grid_offset = fmodf(e->y + (float)e->clip_hitbox.offsety + (float)h - 0.5f, TILE_SIZE);

			if (GetTileSolid(rightx >> TILE_SHIFT, bottomy >> TILE_SHIFT))
			{
				float velRatio;
				if (e->velx == 0) velRatio = 0x1000000; else
					velRatio = e->vely / e->velx;

				if (bottom_grid_offset > right_grid_offset * velRatio)
				{
					// You're in a wall
					x = (rightx & ~TILE_MASK) - e->clip_hitbox.offsetx - (int)w;
					clippedx = TRUE;
				} else
				{
					// You're in a floor
					y = (bottomy & ~TILE_MASK) - e->clip_hitbox.offsety - (int)h;
					clippedy = TRUE;
				}
			}
		} else
		{
			float top_grid_offset = fmodf(e->y + (float)e->clip_hitbox.offsety + 0.5f, TILE_SIZE);

			if (GetTileSolid(rightx >> TILE_SHIFT, topy >> TILE_SHIFT))
			{
				float velRatio;
				if (e->velx == 0) velRatio = 0x1000000; else
					velRatio = -e->vely / e->velx;

				if (((float)TILE_SIZE - top_grid_offset) < right_grid_offset * velRatio)
				{
					// You're in a ceiling
					y = ((topy + TILE_SIZE) & ~TILE_MASK) - e->clip_hitbox.offsety;
					clippedy = TRUE;
				} else
				{
					// You're in a wall
					x = (rightx & ~TILE_MASK) - e->clip_hitbox.offsetx - (int)w;
					clippedx = TRUE;
				}
			}
		}
	} else
	{
		float left_grid_offset = fmodf(e->x + (float)e->clip_hitbox.offsetx + 0.5f, TILE_SIZE);

		if (e->vely >= 0.0f)
		{
			float bottom_grid_offset = fmodf(e->y + (float)e->clip_hitbox.offsety + (float)h - 0.5f, TILE_SIZE);

			if (GetTileSolid(leftx >> TILE_SHIFT, bottomy >> TILE_SHIFT))
			{
				float velRatio;
				if (e->velx == 0) velRatio = 0x1000000; else
					velRatio = -e->vely / e->velx;

				if (bottom_grid_offset > ((float)TILE_SIZE - left_grid_offset) * velRatio)
				{
					// You're in a wall
					x = ((leftx + TILE_SIZE) & ~TILE_MASK) - e->clip_hitbox.offsetx;
					clippedx = TRUE;
				} else
				{
					// You're in a floor
					y = (bottomy & ~TILE_MASK) - e->clip_hitbox.offsety - (int)h;
					clippedy = TRUE;
				}
			}
		} else
		{
			float top_grid_offset = fmodf(e->y + (float)e->clip_hitbox.offsety + 0.5f, TILE_SIZE);

			if (GetTileSolid(leftx >> TILE_SHIFT, topy >> TILE_SHIFT))
			{
				float vel_ratio;
				if (e->velx == 0) vel_ratio = 0x1000000; else
					vel_ratio = e->vely / e->velx;

				if (top_grid_offset < (left_grid_offset - (float)TILE_SIZE) * vel_ratio + (float)TILE_SIZE)
				{
					// You're in a wall
					x = ((leftx + TILE_SIZE) & ~TILE_MASK) - e->clip_hitbox.offsetx;
					clippedx = TRUE;
				} else
				{
					// You're in a ceiling
					y = ((topy + TILE_SIZE) & ~TILE_MASK) - e->clip_hitbox.offsety;
					clippedy = TRUE;
				}
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
