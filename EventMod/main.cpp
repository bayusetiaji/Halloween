#include <cstdio>
#include <sl.h>
#include "keylistener.h"

int main()
{
	slWindow(400, 400, "Clicked", false);

	KeyListener keyC{ 'C' };
	KeyListener keyP{ 'P' };

	while (!slShouldClose())
	{
		if(keyC.keyDown(2))
			printf("keyC: down\n");

		if(keyP.keyPressed())
			printf("keyP: pressed\n");

		

		slRender();
	}
	slClose();

	return 0;
}
