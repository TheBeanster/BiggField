#include "big.h"

#include "utility.h"
#include "graphics.h"

#include "entity.h"
#include "camera.h"
#include "tilemap.h"



Bool BigInit()
{
	OpenTilemapFile("testTilemapFile.bigtilemap");

	//InitDebugTilemap();

	TestLoadTilemap();

	CreateEntity(ET_TESTENTITIY, 30, 30);

	CreateEntity(ET_PLAYER, 300, 300);

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
