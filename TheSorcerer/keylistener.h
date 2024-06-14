#pragma once

class KeyListener
{
public:
	KeyListener(int);
	bool keyDown(double);
	bool keyPressed();

private:
	int key;
	bool pressed;
	double time;
};