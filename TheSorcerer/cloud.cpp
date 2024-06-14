#include <sl.h>
#include "cloud.h"

Cloud::Cloud(Image _img)
{
	Cloud(_img, {0, 0});
}

Cloud::Cloud(Image _img, Vec _pos): texture{ _img }, pos{ _pos }, time{ 0 }, life{ true }
{
}

void Cloud::update(double _fps)
{
	time += slGetDeltaTime();
	if (time >= 1 / _fps)
	{
		pos.x -= 1.5;
		time = 0;

		if (pos.x <= -400)
			life = false;
	}
}

void Cloud::reset(Vec _pos)
{
	pos = _pos;
	life = true;
}

void Cloud::draw()
{
	slSetForeColor(1, 1, 1, 0.5);
	slSprite(texture.res_id, pos.x, pos.y, texture.width, texture.height);
}

bool Cloud::live() const
{
	return life;
}
