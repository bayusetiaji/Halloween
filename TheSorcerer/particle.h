#pragma once

#include <sl.h>
#include "vec.h"

class Particle
{
public:
	Particle();
	Particle(Vec);
	void emit(Vec);
	void update(double);
	void draw();
	void reset(Vec);
	bool live() const;
private:
	static int randbt(int, int);
	Vec pos;
	Vec vel;
	double alpha;
	double time;
};