#include <sl.h>
#include "sorcerer.h"
#include "image.h"

Sorcerer::Sorcerer()
{
	Sorcerer({0, 0, 0}, {0, 0}, {0, 0});
}

Sorcerer::Sorcerer(Image _img)
{
	Sorcerer(_img, { 0, 0 }, { 0, 0 });
}

Sorcerer::Sorcerer(Image _img, Vec _pos, Vec _grav) : texture{ _img }, pos{ _pos }, gravity { _grav }
{
	rad = _img.width / 4.0;
	reset(_pos);
}

void Sorcerer::locate(Vec _pos)
{
	pos = _pos;
}

void Sorcerer::reset(Vec _pos)
{
	pos = _pos;
	vel = { 0, 0 };
	health = 5;
	time = 0;
}

void Sorcerer::jump(double _power)
{
	vel.y = _power;
}

void Sorcerer::update(double _fps)
{
	if (die())
		return;

	time += slGetDeltaTime();
	if (time >= 1 / _fps)
	{
		vel += gravity;
		pos += vel;

		time = 0;
	}

	for (Particle& p : particles)
		p.update(_fps);
}

void Sorcerer::draw()
{
	slSetForeColor(1, 1, 1, 1);
	slSprite(texture.res_id, pos.x, pos.y, texture.width, texture.height);
	
	for (Particle p : particles)
		p.draw();
}

void Sorcerer::dehealth()
{
	health--;
}

void Sorcerer::explode()
{
	for (Particle& p : particles)
		p.emit(pos);
}

int Sorcerer::healthLevel() const
{
	return health;
}

bool Sorcerer::die() const
{
	return health <= 0;
}

Vec Sorcerer::position() const
{
	return pos;
}

double Sorcerer::radius() const
{
	return rad;
}
