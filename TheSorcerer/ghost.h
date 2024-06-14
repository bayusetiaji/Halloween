#pragma once

#include "vec.h"
#include "image.h"

class Ghost
{
public:
	Ghost(Image);
	Ghost(Image, Vec);
	void update(double);
	void killed();
	void draw();
	bool live() const;
	double radius() const;
	Vec position() const;

private:
	Image texture;
	Vec pos;
	bool life;
	double time;
	double rad;
	double angle;
	int type;
};