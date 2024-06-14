#pragma once

#include "vec.h"
#include "image.h"

class Cloud
{
public:
	Cloud(Image);
	Cloud(Image, Vec);
	void update(double);
	void reset(Vec);
	void draw();
	bool live() const;

private:
	Image texture;
	Vec pos;
	bool life;
	double time;
};