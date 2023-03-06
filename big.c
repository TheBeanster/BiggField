#include "big.h"

#include "utility.h"
#include "graphics.h"

#include "entity.h"
#include "camera.h"
#include "tilemap.h"



Bool BigInit()
{
	InitDebugTilemap();

	CreateEntity(ET_TESTENTITIY, 10, 10);

	CreateEntity(ET_PLAYER, 300, 160);

	return TRUE;
}



void BigUpdate()
{
	UpdateWorldEntities();
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
