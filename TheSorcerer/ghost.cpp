#include <sl.h>
#include <random>
#include <cmath>
#include "ghost.h"

Ghost::Ghost(Image _img)
{
	Ghost(_img, { 0, 0 });
}

Ghost::Ghost(Image _img, Vec _pos) : texture{ _img }, pos{ _pos }, life{ true }, time{ 0 }, angle{ 0 }
{
	rad = _img.width / 4.0;
	std::random_device rd;
	int rand = rd() % 10;
	if (rand > 8)
		type = 2;
	else if (rand > 5)
		type = 1;
	else
		type = 0;
}

void Ghost::update(double _fps)
{
	time += slGetDeltaTime();
	if (time >= 1 / _fps)
	{
		angle = type == 2 ? angle + 3 : 0;
		pos.y = type == 2 ? pos.y + 3 * sin(angle * 3.14 / 180) : pos.y;
		pos.x = type == 1 ? pos.x - 6 : pos.x - 3;
		time = 0;
	}
}

void Ghost::killed()
{
	life = false;
}

void Ghost::draw()
{
	if (type == 0)
		slSetForeColor(1, 1, 1, 0.5);
	else if (type == 1)
		slSetForeColor (1, 0.2, 0.2, 0.8);
	else
		slSetForeColor(1, 0.9, 0.29, 0.8);

	slSprite(texture.res_id, pos.x, pos.y, texture.width, texture.height);
}

bool Ghost::live() const
{
	return life;
}

double Ghost::radius() const
{
	return rad;
}

Vec Ghost::position() const
{
	return pos;
}
