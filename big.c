#include "big.h"

#include "utility.h"
#include "graphics.h"

#include "entity.h"



Bool BigInit()
{
	CreateEntity(ET_TESTENTITIY, 10, 10);

	return TRUE;
}



void BigUpdate(float dt)
{
	UpdateWorldEntities(dt);
}



void BigRender()
{
	ClearScreen();

	RenderWorldEntities();
}



void BigQuit()
{
}
