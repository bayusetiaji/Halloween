#include "particle.h"
#include <random>

Particle::Particle()
{
	Particle({ 0, 0 });
}

Particle::Particle(Vec _pos) : time{ 0 }, alpha{ 0 }
{
	reset(_pos);
}

void Particle::emit(Vec _pos)
{
	pos = _pos;
	alpha = randbt(0, 200) / 255.0;
}

void Particle::update(double _fps)
{
	time += slGetDeltaTime();
	if (time >= 1 / _fps)
	{
		pos += vel;
		alpha -= 0.02;

		if (alpha <= 0)
			reset(pos);

		time = 0;
	}
}

void Particle::draw()
{
	slSetForeColor(1, 1, 1, alpha);
	slCircleFill(pos.x, pos.y, 10, 25);
}

void Particle::reset(Vec _pos)
{
	pos = _pos;
	vel = { randbt(-15, 15) / 10.0, randbt(-15, 15) / 10.0 };
}

bool Particle::live() const
{
	return alpha > 0;
}

int Particle::randbt(int _start, int _end)
{
	static std::random_device rd;
	return _start + rd() % (_end - _start + 1);
}
