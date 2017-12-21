#ifndef LINE_H
#define LINE_H
#include <SFML/Graphics.hpp>

using namespace sf;

class Line
{
public:
	void drawSprite(RenderWindow &app, int width);
	float x, y, z; //3d center of line
	float X, Y, W; //screen coord
	float curve, spriteX, clip, scale;
	Sprite sprite;

	Line()
	{
		spriteX = curve = x = y = z = 0;
	}
};
#endif
