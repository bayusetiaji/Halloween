#include <sl.h>
#include <random>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <Windows.h>

#include "keylistener.h"
#include "sorcerer.h"
#include "ghost.h"
#include "cloud.h"

int score = 0;
int highscore = 0;
bool gameover = false;
std::random_device rd;
const Vec gravity{ 0, -0.6 };

int font;
int font2;
int sndJump;
int sndColl;
int imgSky;
int imgMoon;
int imgCloud;
int imgSorc;
int imgGhost;
int imgThunder;

Sorcerer sorc;
std::vector<Ghost> ghosts;
std::vector<Cloud> clouds;

bool collision(Sorcerer, Ghost);
void addGhost();
void initGame();
void updateGame(double);
void drawGame();
void resetGame();
void drawHUD();
void drawBG();

// anotations _In_, _In_opt_ to avoid warning C28251 
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpStr, _In_ int nShowCmd)
{
	slWindow(800, 600, "The Sorcerer", false);

	initGame();

	KeyListener keyJump{ ' ' };
	KeyListener keyReset{ 'R' };

	PlaySound(L"halloween.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	double ghostCrTimer = 0;
	while (!slShouldClose())
	{		
		if (!gameover)
		{
			if (keyJump.keyPressed())
			{
				sorc.jump(9);
				slSoundPlay(sndJump);
			}

			ghostCrTimer += slGetDeltaTime();
			if (ghostCrTimer >= 0.6)
			{
				addGhost();
				ghostCrTimer = 0;
			}
			updateGame(120);
		}
		else
		{
			if (keyReset.keyPressed())
				resetGame();
		}

		drawGame();
		drawHUD();

		slRender();
	}

	slClose();

	return 0;
}

bool collision(Sorcerer s, Ghost g)
{
	double dx = s.position().x - g.position().x;
	double dy = s.position().y - g.position().y;
	if (hypot(dx, dy) <= s.radius() + g.radius())
		return true;

	return false;
}

void addGhost()
{
	double x = 930;
	double y = (rd() % 5 + 1) * 100;
	ghosts.push_back({ { imgGhost, 64, 64 }, { x, y } });
}

void initGame()
{
	font = slLoadFont("F25_Bank_Printer.ttf");
	font2 = slLoadFont("Halloween Too.ttf");
	sndJump = slLoadWAV("jump.wav");
	sndColl = slLoadWAV("collision.wav");
	imgSky = slLoadTexture("skynite.png");
	imgCloud = slLoadTexture("cloud.png");
	imgSorc = slLoadTexture("sorcerer.png");
	imgGhost = slLoadTexture("ghost.png");
	imgThunder = slLoadTexture("thunder.png");

	for (int i = 0; i < 3; ++i)
	{
		double x = 400 + i * 400;
		double y = 250 + rd() % 3 * 50;
		clouds.push_back({ { imgCloud, 256, 166 }, { x, y } });
	}

	sorc = { { imgSorc, 128, 128 }, { 200, 300 }, gravity };

	addGhost();
}

void updateGame(double _fps)
{
	sorc.update(_fps);
	if (sorc.position().y >= 600 || sorc.position().y <= 0)
	{
		sorc.explode();
		sorc.dehealth();
		sorc.locate({ 200, 200 });
		slSoundPlay(sndColl);
	}

	for (auto& c : clouds)
	{
		c.update(120);
		if (!c.live())
			c.reset({ 930, 250 + rd() % 3 * 50.0 });
	}

	for (auto& g : ghosts)
	{
		g.update(_fps);

		if (collision(sorc, g))
		{
			g.killed();
			sorc.explode();
			sorc.dehealth();

			slSoundPlay(sndColl);
		}

		if (g.position().x <= 0)
		{
			++score;
			highscore = score > highscore ? score : highscore;
			g.killed(); 
		}
	}

	if (sorc.die())
		gameover = true;
	
	ghosts.erase(
		std::remove_if(std::begin(ghosts), std::end(ghosts), [](Ghost g) { return !g.live(); }),
		std::end(ghosts));
}

void drawGame()
{
	drawBG();

	for (auto& c : clouds)
		c.draw();

	sorc.draw();

	for (auto& g : ghosts)
		g.draw();
}

void resetGame()
{
	ghosts.clear();
	sorc.reset({200, 200});
	score = 0;
	gameover = false;
}

void drawHUD()
{
	slSetForeColor(1, 1, 1, 0.5);
	slSetFont(font2, 36);
	slSetTextAlign(SL_ALIGN_CENTER);
	slText(400, 560, "The Sorcerer");

	slSetForeColor(1, 1, 1, 0.7);
	slSprite(imgGhost, 25, 575, 32, 32);
	slSetForeColor(1, 1, 1, 0.7);
	slSetFont(font, 14);
	slSetTextAlign(SL_ALIGN_RIGHT);
	slText(100, 575, std::to_string(score).c_str());
	slText(100, 560, std::to_string(highscore).c_str());

	slSetForeColor(1, 1, 1, 1);
	slSprite(imgThunder, 750, 580, 22, 32);
	slSetForeColor(0.1, 1, 0.1, 0.5);
	double width = 16 * sorc.healthLevel();
	double offsX = 750 - (80 - width) / 2;
	slRectangleFill(offsX, 580, width, 12);
	slRectangleOutline(750, 580, 80, 12);

	slSetForeColor(1, 1, 1, 0.7);
	slSetFontSize(12);
	slSetTextAlign(SL_ALIGN_LEFT);
	slText(10, 40, "debug");
	slText(15, 25, ("ghostsSize: " + std::to_string(ghosts.size())).c_str());
	slText(15, 10, ("frameRate : " + std::to_string((int)ceil(1 / slGetDeltaTime()))).c_str());
	slSetTextAlign(SL_ALIGN_RIGHT);
	slSetFontSize(14);
	slText(780, 40, "Space to Jump");

	if (gameover)
	{
		slSetForeColor(0, 0, 0, 0.8);
		slRectangleFill(400, 300, 800, 600);
		slSetForeColor(1, 0.5, 0.5, 1);
		slSetFont(font2, 72);
		slSetTextAlign(SL_ALIGN_CENTER);
		slText(400, 300, "Game Over");
		slSetFont(font, 18);
		slText(400, 275, "Pres 'R' to replay..");

		slSetFont(font, 12);
		slSetForeColor(1, 1, 1, 0.7);
		slText(400, 220, "a game by bayusetiaji@amikom.ac.id");
		slText(400, 180, "credit:");
		slText(400, 160, "font - Halloween Too by Phelan Riessen");
		slText(400, 140, "font - F25 Bank Printer by Volker Busse");
		slText(400, 120, "bgm - Halloween from CapCut audio lib");
	}
}

void drawBG()
{
	static double xOffs1 = 400;
	static double xOffs2 = 1200;
	slSetForeColor(0.8, 0.8, 0.8, 1);
	slSprite(imgSky, xOffs1, 300, 800, 601);
	slSprite(imgSky, xOffs2, 300, 800, 601);
	xOffs1 -= 0.2;
	xOffs2 -= 0.2;
	if (xOffs1 < -400)
		xOffs1 = 1200;
	if (xOffs2 < -400)
		xOffs2 = 1200;
}