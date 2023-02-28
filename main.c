#include <stdio.h>
#include <SDL.h>

#include "system.h"

#include "big.h"



int main(int argc, char** argv)
{
	printf("eyo?\n");

	InitSystem();

	while (1)
	{
		UpdateInputs();
		if (system_inputs & SYSINP_EXIT) break;

		BigUpdate(1.0f / 60.0f);

		BigRender();

		UpdateWindow();
	}

	return 0;
}
