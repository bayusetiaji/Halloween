#pragma once

struct Vec
{
	Vec();
	Vec(double, double);
	Vec operator+(Vec);
	Vec operator*(double);
	Vec operator+=(Vec);
	Vec operator*=(Vec);

	double x;
	double y;
};