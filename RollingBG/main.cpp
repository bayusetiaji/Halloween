#include <sl.h>
#include <cstdio>

int main()
{
	slWindow(800, 600, "Rolling", false);

	int bg = slLoadTexture("skynite.png");

	double xOffs1 = 400;
	double xOffs2 = 1200;

	while (!slShouldClose())
	{
		printf("offset1: %.2f, offset2: %.2f\n", xOffs1, xOffs2);
		
		slSetForeColor(1, 1, 1, 1);
		slSprite(bg, xOffs1, 300, 804, 601);
		slSprite(bg, xOffs2, 300, 804, 601);
		
		xOffs1 -= 10;
		xOffs2 -= 10;

		if (xOffs1 <= -400)
			xOffs1 = 1200;
		if (xOffs2 <= -400)
			xOffs2 = 1200;

		slRender();
	}
	slClose();

	return 0;
}