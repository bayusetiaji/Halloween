#pragma once
#include "vec.h"
#include "image.h"
#include "particle.h"

class Sorcerer
{
public:
	Sorcerer();
	Sorcerer(Image);
	Sorcerer(Image, Vec, Vec);
	void locate(Vec);
	void reset(Vec);
	void jump(double);
	void update(double);
	void draw();
	void dehealth();
	void explode();
	int healthLevel() const;
	bool die() const;
	Vec position() const;
	double radius() const;
private:
	Particle particles[100];
	Image texture;
	Vec pos;
	Vec vel;
	Vec gravity;
	int health;
	double time;
	double rad;
};