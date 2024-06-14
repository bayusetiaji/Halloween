#include <sl.h>
#include <cstdio>
#include "keylistener.h"

KeyListener::KeyListener(int c) : key{ c }, pressed{ false }, time{ 0 }
{
}

bool KeyListener::keyDown(double _d)
{
	if (slGetKey(key))
	{
		time += slGetDeltaTime();
		if (time >= _d)
		{
			time = 0;
			return true;
		}
	}
	
	return false;
}

bool KeyListener::keyPressed()
{
	if (slGetKey(key))
	{
		if (!pressed)
		{
			pressed = true;
			return true;
		}
	}
	else
	{
		pressed = false;
	}

	return false;
}

void KeyListener::test()
{
	if (slGetKey(key))
	{
		printf("key: %c\n", key);
	}
}
