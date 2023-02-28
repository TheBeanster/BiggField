#include <stdio.h>
#include <SDL.h>

#include "system.h"
#include "graphics.h"



int main(int argc, char** argv)
{
	printf("eyo?\n");

	InitSystem();

	while (1)
	{
		UpdateInputs();
		if (system_inputs & SYSINP_EXIT) break;

		ClearScreen();

		SetColor(255, 0, 0, 255);
		FillRect(mousex, mousey, 10, 10);

		UpdateWindow();
	}

	return 0;
}
