#include "line.h"

void Line::drawSprite(RenderWindow &app, int width)
{
	Sprite s = sprite;
	int w = s.getTextureRect().width;
	int h = s.getTextureRect().height;

	float destX = X + scale * spriteX * width / 2;
	float destY = Y + 4;
	float destW = w * W / 266;
	float destH = h * W / 266;

	destX += destW * spriteX; //offsetX
	destY += destH * (-1);    //offsetY

	float clipH = destY + destH - clip;
	if (clipH<0) clipH = 0;

	if (clipH >= destH) return;
	s.setTextureRect(IntRect(0, 0, w, h - h*clipH / destH));
	s.setScale(destW / w, destH / h);
	s.setPosition(destX, destY);
	app.draw(s);
}
