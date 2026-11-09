#include <sl.h>
#include <random>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#define NOMINMAX
#include <Windows.h>

#include "keylistener.h"
#include "sorcerer.h"
#include "ghost.h"
#include "cloud.h"

int score = 0;
int highscore = 0;
bool newBest = false;
std::random_device rd;
const Vec gravity{ 0, -0.6 };

enum class GameState
{
	Ready,
	Playing,
	Paused,
	GameOver
};

GameState state = GameState::Ready;

const double GHOST_SPAWN_INTERVAL = 0.7;
const double INVULNERABLE_TIME = 1.2;

bool musicOn = true;
bool showDebug = false;

double ghostCrTimer = 0;
double invulnerableTimer = 0;
double damageFlash = 0;
double scorePop = 0;
double bgOffset1 = 400;
double bgOffset2 = 1200;

int font;
int font2;
int sndJump;
int sndColl;
int imgSky;
int imgCloud;
int imgSorc;
int imgGhost;
int imgThunder;

Sorcerer sorc;
std::vector<Ghost> ghosts;
std::vector<Cloud> clouds;

bool collision(const Sorcerer&, const Ghost&);
void addGhost();
void initGame();
void loadHighscore();
void saveHighscore();
void startGame();
void updateGame(double);
void updateClouds();
void updateBackground();
void drawGame();
void drawHUD();
void drawBG();
void drawReadyScreen();
void drawPauseScreen();
void drawGameOverScreen();
void drawPanel(double, double, double, double);
void drawText(const char*, int, int, double, double, int);
void damage();
void toggleMusic();
const char* stateName(GameState);

// anotations _In_, _In_opt_ to avoid warning C28251 
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpStr, _In_ int nShowCmd)
{
	slWindow(800, 600, "The Sorcerer", false);

	initGame();

	KeyListener keyJump{ ' ' };
	KeyListener keyReset{ 'R' };
	KeyListener keyPause{ 'P' };
	KeyListener keyEscape{ SL_KEY_ESCAPE };
	KeyListener keyMusic{ 'M' };
	KeyListener keyDebug{ SL_KEY_F3 };

	PlaySound(L"halloween.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	while (!slShouldClose())
	{
		double dt = slGetDeltaTime();

		if (keyDebug.keyPressed())
			showDebug = !showDebug;

		if (keyMusic.keyPressed())
			toggleMusic();

		if (state == GameState::Ready)
		{
			updateClouds();
			updateBackground();

			if (keyJump.keyPressed())
				startGame();
		}
		else if (state == GameState::Playing)
		{
			if (keyPause.keyPressed() || keyEscape.keyPressed())
			{
				state = GameState::Paused;
			}
			else
			{
				if (keyJump.keyPressed())
				{
					sorc.jump(9);
					slSoundPlay(sndJump);
				}

				ghostCrTimer += dt;
				if (ghostCrTimer >= GHOST_SPAWN_INTERVAL)
				{
					addGhost();
					ghostCrTimer = 0;
				}

				updateGame(120);
				updateBackground();
			}
		}
		else if (state == GameState::Paused)
		{
			if (keyPause.keyPressed() || keyEscape.keyPressed())
				state = GameState::Playing;
			else if (keyReset.keyPressed())
				startGame();
		}
		else if (state == GameState::GameOver)
		{
			sorc.update(120);

			if (keyReset.keyPressed())
				startGame();
			else if (keyEscape.keyPressed())
				break;
		}

		if (damageFlash > 0)
			damageFlash = std::max(0.0, damageFlash - dt * 3.0);
		if (scorePop > 0)
			scorePop = std::max(0.0, scorePop - dt * 4.0);

		drawGame();
		drawHUD();

		slRender();
	}

	saveHighscore();
	slClose();

	return 0;
}

bool collision(const Sorcerer& s, const Ghost& g)
{
	double dx = s.position().x - g.position().x;
	double dy = s.position().y - g.position().y;
	double radiusSum = s.radius() + g.radius();
	if (dx * dx + dy * dy <= radiusSum * radiusSum)
		return true;

	return false;
}

void addGhost()
{
	double x = 930;
	double y = (rd() % 5 + 1) * 100;
	ghosts.push_back({ { imgGhost, 64, 64 }, { x, y } });
}

void loadHighscore()
{
	std::ifstream in("highscore.txt");
	if (in)
	{
		in >> highscore;
		if (!in || highscore < 0)
			highscore = 0;
	}
}

void saveHighscore()
{
	std::ofstream out("highscore.txt");
	if (out)
		out << highscore;
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

	loadHighscore();

	for (int i = 0; i < 3; ++i)
	{
		double x = 400 + i * 400;
		double y = 250 + rd() % 3 * 50;
		clouds.push_back({ { imgCloud, 256, 166 }, { x, y } });
	}

	sorc = { { imgSorc, 128, 128 }, { 200, 300 }, gravity };
	sorc.reset({ 200, 300 });
}

void startGame()
{
	ghosts.clear();
	sorc.reset({ 200, 300 });
	score = 0;
	newBest = false;
	ghostCrTimer = 0;
	invulnerableTimer = 0;
	damageFlash = 0;
	scorePop = 0;
	state = GameState::Playing;
}

void damage()
{
	sorc.explode();
	sorc.dehealth();
	slSoundPlay(sndColl);

	damageFlash = 1.0;
	invulnerableTimer = INVULNERABLE_TIME;
}

void updateGame(double _fps)
{
	sorc.update(_fps);

	if (invulnerableTimer > 0)
		invulnerableTimer = std::max(0.0, invulnerableTimer - slGetDeltaTime());

	if (sorc.position().y >= 600 || sorc.position().y <= 0)
	{
		if (invulnerableTimer <= 0)
			damage();

		sorc.locate({ 200, 300 });
	}

	updateClouds();

	for (auto& g : ghosts)
	{
		g.update(_fps);

		if (invulnerableTimer <= 0 && collision(sorc, g))
		{
			g.killed();
			damage();
		}

		if (g.position().x <= 0)
		{
			++score;
			scorePop = 1.0;

			if (score > highscore)
			{
				highscore = score;
				newBest = true;
				saveHighscore();
			}

			g.killed();
		}
	}

	if (sorc.die())
	{
		saveHighscore();
		state = GameState::GameOver;
	}

	ghosts.erase(
		std::remove_if(std::begin(ghosts), std::end(ghosts), [](Ghost g) { return !g.live(); }),
		std::end(ghosts));
}

void updateClouds()
{
	for (auto& c : clouds)
	{
		c.update(120);
		if (!c.live())
			c.reset({ 930, 250 + rd() % 3 * 50.0 });
	}
}

void updateBackground()
{
	double speed = 12.0 * slGetDeltaTime();

	bgOffset1 -= speed;
	bgOffset2 -= speed;

	if (bgOffset1 < -400)
		bgOffset1 = 1200;
	if (bgOffset2 < -400)
		bgOffset2 = 1200;
}

void drawGame()
{
	drawBG();

	double shake = damageFlash * damageFlash * 6.0;
	if (shake > 0.1)
	{
		slPush();
		slTranslate(sin(slGetTime() * 90.0) * shake, cos(slGetTime() * 70.0) * shake);
	}

	for (auto& c : clouds)
		c.draw();

	bool blinking = state == GameState::Playing
		&& invulnerableTimer > 0
		&& fmod(slGetTime(), 0.16) < 0.08;

	if (state == GameState::Ready)
	{
		slPush();
		slTranslate(0, sin(slGetTime() * 2.0) * 8.0);
		sorc.draw();
		slPop();
	}
	else if (!blinking)
	{
		sorc.draw();
	}

	for (auto& g : ghosts)
		g.draw();

	if (shake > 0.1)
		slPop();

	if (damageFlash > 0)
	{
		slSetForeColor(1.0, 0.1, 0.12, 0.35 * damageFlash);
		slRectangleFill(400, 300, 800, 600);
	}
}

void drawBG()
{
	slSetForeColor(0.8, 0.8, 0.8, 1);
	slSprite(imgSky, bgOffset1, 300, 800, 601);
	slSprite(imgSky, bgOffset2, 300, 800, 601);
}

void drawText(const char* _text, int _font, int _size, double _x, double _y, int _align)
{
	slSetFont(_font, _size);
	slSetTextAlign(_align);

	double height = slGetTextHeight(_text);
	slText(_x, _y - height * 0.5, _text);
}

void drawPanel(double _x, double _y, double _width, double _height)
{
	slSetForeColor(0.02, 0.01, 0.05, 0.92);
	slRectangleFill(_x, _y, _width, _height);

	// soft outer glow
	slSetForeColor(0.55, 0.25, 0.9, 0.14);
	slRectangleOutline(_x, _y, _width + 18, _height + 18);

	slSetForeColor(0.75, 0.45, 1.0, 0.9);
	slRectangleOutline(_x, _y, _width, _height);

	slSetForeColor(0.75, 0.45, 1.0, 0.22);
	slRectangleOutline(_x, _y, _width - 10, _height - 10);
}

void drawReadyScreen()
{
	slSetForeColor(0.01, 0.0, 0.03, 0.72);
	slRectangleFill(400, 300, 800, 600);

	drawPanel(400, 310, 540, 350);

	slSetForeColor(1, 1, 1, 0.18);
	slSprite(imgGhost, 190, 435, 40, 40);
	slSprite(imgGhost, 610, 435, 40, 40);

	slSetForeColor(1, 0.55, 0.2, 1);
	drawText("The Sorcerer", font2, 54, 400, 438, SL_ALIGN_CENTER);

	slSetForeColor(0.85, 0.8, 1.0, 0.85);
	drawText("A Halloween Flappy Adventure", font, 13, 400, 392, SL_ALIGN_CENTER);

	slSetForeColor(0.75, 0.45, 1.0, 0.35);
	slLine(280, 370, 520, 370);

	slSetForeColor(1, 1, 1, 0.85);
	drawText("SPACE   fly", font, 16, 400, 334, SL_ALIGN_CENTER);

	slSetForeColor(1, 1, 1, 0.65);
	std::string musicHint = std::string("P / ESC   pause        M   music  ") + (musicOn ? "ON" : "OFF");
	drawText(musicHint.c_str(), font, 12, 400, 304, SL_ALIGN_CENTER);

	slSetForeColor(1, 1, 1, 0.45);
	drawText("BEST SCORE", font, 9, 400, 270, SL_ALIGN_CENTER);

	slSetForeColor(1, 0.85, 0.3, 0.95);
	drawText(std::to_string(highscore).c_str(), font, 22, 400, 248, SL_ALIGN_CENTER);

	double pulse = 0.5 + 0.5 * sin(slGetTime() * 4.0);
	slSetForeColor(1, 0.6, 0.2, 0.55 + 0.45 * pulse);
	drawText("PRESS SPACE TO START", font2, 22, 400, 205, SL_ALIGN_CENTER);

	slSetForeColor(1, 1, 1, 0.35);
	drawText("a game by bayusetiaji@amikom.ac.id", font, 9, 400, 148, SL_ALIGN_CENTER);
}

void drawPauseScreen()
{
	slSetForeColor(0.01, 0.0, 0.03, 0.72);
	slRectangleFill(400, 300, 800, 600);

	drawPanel(400, 300, 380, 250);

	slSetForeColor(1, 0.55, 0.2, 1);
	drawText("PAUSED", font2, 52, 400, 360, SL_ALIGN_CENTER);

	slSetForeColor(1, 1, 1, 0.8);
	drawText("P / ESC   resume", font, 14, 400, 305, SL_ALIGN_CENTER);

	slSetForeColor(1, 1, 1, 0.6);
	std::string music = std::string("M   music  ") + (musicOn ? "ON" : "OFF");
	drawText(music.c_str(), font, 11, 400, 275, SL_ALIGN_CENTER);
	drawText("R   restart", font, 11, 400, 250, SL_ALIGN_CENTER);
}

void drawGameOverScreen()
{
	slSetForeColor(0.01, 0.0, 0.03, 0.75);
	slRectangleFill(400, 300, 800, 600);

	drawPanel(400, 300, 540, 380);

	slSetForeColor(1, 0.35, 0.3, 1);
	drawText("GAME OVER", font2, 58, 400, 438, SL_ALIGN_CENTER);

	slSetForeColor(1, 1, 1, 0.45);
	drawText("SCORE", font, 9, 310, 380, SL_ALIGN_CENTER);

	slSetForeColor(1, 1, 1, 0.95);
	drawText(std::to_string(score).c_str(), font, 34, 310, 350, SL_ALIGN_CENTER);

	slSetForeColor(1, 1, 1, 0.45);
	drawText("BEST", font, 9, 490, 380, SL_ALIGN_CENTER);

	slSetForeColor(1, 0.85, 0.3, 0.95);
	drawText(std::to_string(highscore).c_str(), font, 34, 490, 350, SL_ALIGN_CENTER);

	if (newBest)
	{
		double pulse = 0.5 + 0.5 * sin(slGetTime() * 6.0);
		slSetForeColor(1, 0.9, 0.35, 0.55 + 0.45 * pulse);
		drawText("NEW BEST!", font2, 22, 400, 310, SL_ALIGN_CENTER);
	}

	slSetForeColor(1, 0.6, 0.2, 0.9);
	drawText("Press R to play again", font, 15, 400, 268, SL_ALIGN_CENTER);

	slSetForeColor(1, 1, 1, 0.45);
	std::string musicHint = std::string("SPACE  fly     P  pause     M  music  ") + (musicOn ? "ON" : "OFF");
	drawText(musicHint.c_str(), font, 9, 400, 240, SL_ALIGN_CENTER);

	slSetForeColor(0.75, 0.45, 1.0, 0.35);
	slLine(260, 220, 540, 220);

	slSetForeColor(1, 1, 1, 0.4);
	drawText("a game by bayusetiaji@amikom.ac.id", font, 9, 400, 198, SL_ALIGN_CENTER);
	drawText("fonts: Halloween Too / F25 Bank Printer", font, 8, 400, 182, SL_ALIGN_CENTER);
	drawText("music: Halloween - CapCut audio library", font, 8, 400, 168, SL_ALIGN_CENTER);

	slSetForeColor(1, 1, 1, 0.3);
	drawText("Press ESC to quit", font, 9, 400, 142, SL_ALIGN_CENTER);
}

const char* stateName(GameState _state)
{
	switch (_state)
	{
	case GameState::Ready: return "READY";
	case GameState::Playing: return "PLAYING";
	case GameState::Paused: return "PAUSED";
	case GameState::GameOver: return "GAME OVER";
	}

	return "UNKNOWN";
}

void drawHUD()
{
	// HUD panels keep the top-center of the playfield clear
	slSetForeColor(0.02, 0.01, 0.05, 0.5);
	slRectangleFill(105, 572, 190, 48);
	slRectangleFill(705, 572, 170, 48);

	slSetForeColor(0.75, 0.45, 1.0, 0.16);
	slRectangleOutline(105, 572, 190, 48);
	slRectangleOutline(705, 572, 170, 48);

	// score
	slSetForeColor(1, 1, 1, 0.9);
	slSprite(imgGhost, 38, 572, 34, 34);

	slSetForeColor(1, 1, 1, 0.45);
	drawText("SCORE", font, 9, 62, 588, SL_ALIGN_LEFT);

	double pop = scorePop > 0 ? scorePop : 0;
	slSetForeColor(1, 1.0 - 0.4 * pop, 1.0 - 0.5 * pop, 1);
	drawText(std::to_string(score).c_str(), font, 20 + (int)(pop * 8), 62, 568, SL_ALIGN_LEFT);

	slSetForeColor(1, 1, 1, 0.45);
	drawText("BEST", font, 9, 160, 588, SL_ALIGN_LEFT);

	slSetForeColor(1, 1, 1, 0.8);
	drawText(std::to_string(highscore).c_str(), font, 14, 160, 568, SL_ALIGN_LEFT);

	// lives
	slSetForeColor(1, 1, 1, 0.8);
	slSprite(imgThunder, 632, 572, 22, 30);

	slSetForeColor(1, 1, 1, 0.45);
	drawText("LIVES", font, 9, 652, 588, SL_ALIGN_LEFT);

	int health = sorc.healthLevel();
	double r = 0.3;
	double g = 1.0;
	double b = 0.45;

	if (health <= 2)
	{
		r = 1.0;
		g = 0.35;
		b = 0.3;
	}
	else if (health == 3)
	{
		r = 1.0;
		g = 0.8;
		b = 0.3;
	}

	for (int i = 0; i < 5; ++i)
	{
		double x = 662 + i * 22;

		if (i < health)
		{
			slSetForeColor(r, g, b, 0.9);
			slRectangleFill(x, 568, 16, 10);

			slSetForeColor(r, g, b, 0.35);
			slRectangleOutline(x, 568, 20, 14);
		}
		else
		{
			slSetForeColor(1, 1, 1, 0.10);
			slRectangleFill(x, 568, 16, 10);

			slSetForeColor(1, 1, 1, 0.18);
			slRectangleOutline(x, 568, 20, 14);
		}
	}

	if (state == GameState::Playing)
	{
		slSetForeColor(1, 1, 1, 0.35);
		std::string hint = std::string("SPACE  JUMP     P  PAUSE     M  MUSIC  ") + (musicOn ? "ON" : "OFF");
		drawText(hint.c_str(), font, 9, 400, 14, SL_ALIGN_CENTER);
	}

	if (showDebug)
	{
		std::string debug = "state ";
		debug += stateName(state);
		debug += "   ghosts " + std::to_string(ghosts.size());
		debug += "   fps " + std::to_string((int)ceil(1.0 / slGetDeltaTime()));
		debug += "   pos " + std::to_string((int)sorc.position().x) + "," + std::to_string((int)sorc.position().y);

		slSetForeColor(1, 1, 1, 0.55);
		drawText(debug.c_str(), font, 9, 10, 32, SL_ALIGN_LEFT);

		slSetForeColor(1, 1, 1, 0.3);
		drawText("F3  debug", font, 9, 10, 18, SL_ALIGN_LEFT);
	}

	if (state == GameState::Ready)
		drawReadyScreen();
	else if (state == GameState::Paused)
		drawPauseScreen();
	else if (state == GameState::GameOver)
		drawGameOverScreen();
}

void toggleMusic()
{
	musicOn = !musicOn;

	if (musicOn)
		PlaySound(L"halloween.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	else
		PlaySound(NULL, NULL, 0);
}
