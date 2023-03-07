#include "big.h"

#include "utility.h"
#include "graphics.h"

#include "entity.h"
#include "camera.h"
#include "tilemap.h"

#include "system.h"



int framecount = 0;



Bool BigInit()
{
	OpenTilemapFile("testTilemapFile.bigtilemap");

	//InitDebugTilemap();

	TestLoadTilemap();

	CreateEntity(ET_PLAYER, 300, 300);

	return TRUE;
}



void BigUpdate()
{
	if (!(framecount & 0x1f))
	{
		if (keyboard_inputs[SDL_SCANCODE_D]) ShiftTilemapAndEntities(1, 0);
		if (keyboard_inputs[SDL_SCANCODE_A]) ShiftTilemapAndEntities(-1, 0);
	}

	UpdateWorldEntities();

	framecount++;
}



void BigRender()
{
	ClearScreen();

	UpdateCamera();

	RenderTilemap();

	RenderWorldEntities();
}



void BigQuit()
{
}
