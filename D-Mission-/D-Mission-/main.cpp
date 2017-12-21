#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

int width = 1024;
int height = 768;
int roadW = 2000;
int segL = 200; //segment length
float camD = 0.84; //camera depth
int viewDistance = 300;
float horizonLine = height / 2;
float centerLine = width / 2;
float maxZ = viewDistance * segL;
std::vector<sf::Sprite> objects;

void drawQuad(RenderWindow &w, Color c, int x1, int y1, int w1, int x2, int y2, int w2)
{
	ConvexShape shape(4);
	shape.setFillColor(c);
	shape.setPoint(0, Vector2f(x1 - w1, y1));
	shape.setPoint(1, Vector2f(x2 - w2, y2));
	shape.setPoint(2, Vector2f(x2 + w2, y2));
	shape.setPoint(3, Vector2f(x1 + w1, y1));
	w.draw(shape);
}

struct Car
{
	enum CarState
	{
		Neutral,
		TurnLeft,
		TurnRight
	};
	CarState state;
	float x, y;
	Car() 
	{
		state = Neutral;
		x = 0;
		y = 0;
	}
};

struct Line
{
	float x, y, z; //3d center of line
	float X, Y, W; //screen coord
	float curve, spriteX, clip, scale;
	Sprite sprite;

	Line()
	{
		spriteX = curve = x = y = z = 0;
	}

	void project(int camX, int camY, int camZ)
	{
		scale = camD / (z - camZ);
		X = (1 + scale*(x - camX)) * width / 2;
		Y = (1 - scale*(y - camY)) * height / 2;
		W = scale * roadW  * width / 2;
	}

	void drawSprite(RenderWindow &app)
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
};


int main()
{
	RenderWindow app(VideoMode(width, height), "D-Mission!");
	app.setFramerateLimit(60);

	Texture textures[5];

	textures[0].loadFromFile("Assets/tree1.png");
	textures[0].setSmooth(true);
	objects.push_back(sf::Sprite(textures[0]));

	textures[1].loadFromFile("Assets/house1.png");
	textures[1].setSmooth(true);
	objects.push_back(sf::Sprite(textures[1]));

	textures[2].loadFromFile("Assets/180sx_neutral.png");
	textures[2].setSmooth(true);
	objects.push_back(sf::Sprite(textures[2]));

	textures[3].loadFromFile("Assets/180sx_turnLeft.png");
	textures[3].setSmooth(true);
	objects.push_back(sf::Sprite(textures[3]));

	textures[4].loadFromFile("Assets/180sx_turnRight.png");
	textures[4].setSmooth(true);
	objects.push_back(sf::Sprite(textures[4]));

	Texture bg;
	bg.loadFromFile("Assets/bg.jpg");
	bg.setRepeated(true);
	Sprite sBackground(bg);
	sBackground.setTextureRect(IntRect(0, 0, 5000, 411));
	sBackground.setPosition(-2000, 0);

	std::vector<Line> lines;

	Car car = Car();

	for (int i = 0; i<1600; i++)
	{
		Line line;
		line.z = i*segL;

		//// Curve Example at Track Position
		if (i > 300 && i < 700) line.curve = 2;

		//// Elevation Change Example
		if (i > 750) line.y = sin(i / 30.0) * 1500;

		//// Add Trees
		if (i % 50 == 0)
		{
			line.spriteX = -1.2; line.sprite = objects[0];
		}

		//// Add houses
		if (i % 200 == 0)
		{
			line.spriteX = 3.2; line.sprite = objects[1];
		}

		lines.push_back(line);
	}

	int N = lines.size();
	float playerX = 0;
	int pos = 0;
	int H = 1500;

	while (app.isOpen())
	{
		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				app.close();
		}

		int speed = 0;
		car.state = Car::Neutral;

		if (Keyboard::isKeyPressed(Keyboard::Right)) 
		{
			playerX += 0.1;
			car.state = Car::TurnRight;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			playerX -= 0.1;
			car.state = Car::TurnLeft;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) speed = 200;
		if (Keyboard::isKeyPressed(Keyboard::Down)) speed = -200;
		if (Keyboard::isKeyPressed(Keyboard::Tab)) speed *= 3;
		if (Keyboard::isKeyPressed(Keyboard::W)) H += 100;
		if (Keyboard::isKeyPressed(Keyboard::S)) H -= 100;

		pos += speed;
		while (pos >= N*segL) pos -= N*segL;
		while (pos < 0) pos += N*segL;

		app.clear();
		app.draw(sBackground);

		int startPos = pos / segL;
		int camH = lines[startPos].y + H;
		if (speed>0) sBackground.move(-lines[startPos].curve * 2, 0);
		if (speed<0) sBackground.move(lines[startPos].curve * 2, 0);

		int maxy = height;
		float x = 0, dx = 0;

		//// Draw Road from Screen to Horizon Line ////
		for (int n = startPos; n<startPos + 300; n++)
		{
			Line &l = lines[n%N];
			l.project(playerX*roadW - x, camH, startPos*segL - (n >= N ? N*segL : 0));
			x += dx;
			dx += l.curve;

			l.clip = maxy;
			if (l.Y >= maxy) continue;
			maxy = l.Y;

			Color grass = (n / 3) % 2 ? Color(0, 77, 0) : Color(0, 102, 0);
			Color rumble = (n / 3) % 2 ? Color(255, 255, 255) : Color(153, 0, 0);
			Color road = (n / 3) % 2 ? Color(107, 107, 107) : Color(105, 105, 105);

			Line p = lines[(n - 1) % N]; //previous line

			drawQuad(app, grass, 0, p.Y, width, 0, l.Y, width);
			drawQuad(app, rumble, p.X, p.Y, p.W*1.2, l.X, l.Y, l.W*1.2);
			drawQuad(app, road, p.X, p.Y, p.W, l.X, l.Y, l.W);
		}

		//// Draw Objects from Horizon line to Player ////
		for (int n = startPos + 300; n > startPos; n--) 
		{
			lines[n%N].drawSprite(app);
		}

		//// Draw Car ////
		if (car.state == Car::TurnRight)
		{
			objects[4].setScale(.3, .3);
			objects[4].setPosition(centerLine - (objects[3].getTextureRect().width / 2 * .3), horizonLine + 100);
			app.draw(objects[4]);
		}
		else if (car.state == Car::TurnLeft)
		{
			objects[3].setScale(.3, .3);
			objects[3].setPosition(centerLine - (objects[3].getTextureRect().width / 2 * .3), horizonLine + 100);
			app.draw(objects[3]);
		}
		else if(car.state == Car::Neutral)
		{
			objects[2].setScale(.3, .3);
			objects[2].setPosition(centerLine - (objects[2].getTextureRect().width / 2 * .3), horizonLine + 100);
			app.draw(objects[2]);
		}

		app.display();
	}
	return 0;
}
