#include "vec.h"

Vec::Vec(): x{ 0 }, y{ 0 } { }

Vec::Vec(double _x, double _y) : x{ _x }, y{ _y } { }

Vec Vec::operator+(Vec v)
{
	return { v.x + x, v.y + y };
}

Vec Vec::operator*(double s)
{
	return { x * s, y * s };
}

Vec Vec::operator+=(Vec v)
{
	x += v.x;
	y += v.y;
	return { x, y };
}

Vec Vec::operator*=(Vec v)
{
	x *= v.x;
	y *= v.y;
	return { x, y };
}