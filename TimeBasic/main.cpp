#include <sl.h>

class Foo
{
public:
	Foo();
	void update(double);
	void draw();
private:
	double x;
	double y;
	double time;
};

int main()
{
	slWindow(400, 400, "Delta Time", false);
	Foo f;

	while (!slShouldClose())
	{
		f.update(1/120.0);
		f.draw();
		
		slRender();
	}
	slClose();

	return 0;
}

Foo::Foo()
{
	x = 200;
	y = 200;
	time = 0;
}

void Foo::update(double _delay)
{
	time += slGetDeltaTime();
	if (time >= _delay)
	{
		x++;
		y++;
		time = 0;
	}
}

void Foo::draw()
{
	slSetForeColor(0, 1, 0, 1);
	slCircleFill(x, y, 20, 25);
}
