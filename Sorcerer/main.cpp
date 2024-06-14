// sorcerer
#include <sl.h>
#include <random>


int main()
{
	slWindow(800, 400, "Sorcerer", false);

	std::random_device rd;

	// sprites
	int imgSorc = slLoadTexture("sorcerer.png");
	int imgMoon = slLoadTexture("bigmoon.png");
	int imgCloud = slLoadTexture("cloud.png");
	// soundFX
	int sndJump = slLoadWAV("jump.wav");

	double pxCloud = 930;
	double pyCloud = 150;
	double pxSorc = 300;
	double pySorc = 400;
	double vySorc = 0;

	double gy = -0.1;

	while (!slShouldClose())
	{
		slSetBackColor(0, 0, 0);

		slSetForeColor(0.2, 0.2, 0.2, 1);
		slSprite(imgMoon, 400, 200, 512, 512);

		slSetForeColor(1, 1, 1, 0.5);
		slSprite(imgCloud, pxCloud, pyCloud, 256, 166);

		slSetForeColor(1, 1, 1, 1);
		slSetForeColor(1, 1, 1, 1);
		slSprite(imgSorc, pxSorc, pySorc, 128, 128);
		
		slRender();

		vySorc += gy;
		pySorc += vySorc;

		if (slGetKey(' '))
		{
			vySorc = 3;
			slSoundPlay(sndJump);
		}

		if (pySorc <= 0)
		{
			pySorc = 400;
			vySorc = 0;
		}

		pxCloud-=3;
		if (pxCloud <= -130)
		{
			pxCloud = 930;
			pyCloud = (rd() % 4 + 1) * 100;
		}
	}

	slClose();

	return 0;
}

