#pragma once

class KeyListener
{
public:
	KeyListener(int);
	bool keyDown(double);
	bool keyPressed();
	void test();

private:
	int key;
	bool pressed;
	double time;
};