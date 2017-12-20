#include <SFML\Graphics.hpp>
#include <Thor\Resources.hpp>
#include <iostream>

using namespace sf;
using namespace thor;

float width = 1024.0;
float height = 768.0;
int roadW = 2000;
int segL = 200; // Segment Length
float camD = 0.84; // Camera Depth

namespace Textures
{
	enum Type
	{
		Tree1,
		Tree2,
		Billboard1,
		BackGround,
		_180SX,
		Empty
	};
}

thor::ResourceHolder<sf::Texture, Textures::Type> textures; //All Textures Holder

class Line
{
public:
	float x, y, z; //3D Center of Line
	float X, Y, W; //screen coord
	float scale, curve, clip;
	Textures::Type object;
	float objectX;

	Line()
	{ 
		scale = curve = clip = x = y = z = X = Y = W = 0;
		object = Textures::Empty;
	}

	//From world to screen coordinates
	void project(float camX, float camY, float camZ)
	{
		scale = camD / (z - camZ);
		X = (1 + scale * (x - camX)) * width / 2;
		Y = (1 - scale * (y - camY)) * height / 2;
		W = scale * roadW * width / 2;
	}

	void drawLineObjects(RenderWindow &app)
	{
		if (object == Textures::Empty) return;

		Sprite sprite;
		sprite.setTexture(textures[object]);

		float textureWidth = sprite.getTextureRect().width;
		float textureHeight = sprite.getTextureRect().height;

		float resultSpriteX = 0.0f;
		float resultSpriteY = 0.0f;
		float resultSpriteWidth = 0.0f;
		float resultSpriteHeight = 0.0f;

		//Sprite Size and Position Logic
		resultSpriteX = X + (scale * objectX * width / 2); // Line's X + ( Line's Scale * Object's X Offset * Screen Width / 2)
		resultSpriteY = Y + 4; // Line's Y + 4 (So it appears in middle of line?)
		resultSpriteWidth = textureWidth * W / 266; // Texture's Width * Line's Width / 266 
		resultSpriteHeight = textureHeight * W / 266; // Texture's Height * Line's Width / 266 

		resultSpriteX += objectX; // Offset X
		resultSpriteY += resultSpriteHeight * (-1); // Offset Y

		float clipH = resultSpriteY + resultSpriteHeight - clip;
		if (clipH < 0) clipH = 0;
		if (clipH >= resultSpriteHeight) return;

		//Set Sprite Size in relation to Line distance
		sprite.setTextureRect(IntRect(0, 0, textureWidth, (textureHeight - (textureHeight * clipH / resultSpriteHeight))));
		sprite.setScale(resultSpriteWidth / textureWidth, resultSpriteHeight / textureHeight);
		sprite.setPosition(resultSpriteX, resultSpriteY);

		app.draw(sprite);
	}
};

void drawQuad(RenderWindow &w, Color c, float x1, float y1, float w1, float x2, float y2, float w2)
{
	ConvexShape shape(4);
	shape.setFillColor(c);
	shape.setPoint(0, Vector2f(x1 - w1, y1));
	shape.setPoint(1, Vector2f(x2 - w2, y2));
	shape.setPoint(2, Vector2f(x2 + w2, y2));
	shape.setPoint(3, Vector2f(x1 + w1, y1));
	w.draw(shape);
}

int main()
{
	//// Create Application Window ////
	RenderWindow app(VideoMode(width, height), "D-Mission!");
	app.setFramerateLimit(60);

	//// Intro and Splash View ////

	//// Main Menu ////

	//// Load Race ////

	try
	{
		textures.acquire(Textures::Tree1, thor::Resources::fromFile<sf::Texture>("tree1.png"));
		textures.acquire(Textures::BackGround, thor::Resources::fromFile<sf::Texture>("bg.jpg"));
	}
	catch (thor::ResourceLoadingException& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}

	RectangleShape background(Vector2f(width*1.25, height));
	background.setTexture(&textures[Textures::BackGround]);

	//Create Track in Memeory
	std::vector<Line> lines(1600);

	//// Track Plan ////
	for (int i = 0; i < 1600; i++)
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
			line.object = Textures::Tree1;
			line.objectX = 0.0;
		}

		lines[i] = line;
	}

	int N = lines.size();
	int pos = 200;
	float playerX = 0;

	//// Start Race ////

	while (app.isOpen())
	{
		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				app.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left)) playerX -= 200;
		if (Keyboard::isKeyPressed(Keyboard::Right)) playerX += 200;
		if (Keyboard::isKeyPressed(Keyboard::Up)) pos += 200;
		if (Keyboard::isKeyPressed(Keyboard::Down)) pos -= 200;

		//Circular track
		while (pos >= N * segL) pos -= N * segL;
		while (pos < 0) pos += N * segL;

		app.clear();
		int startPos = pos / segL;
		float camH = 1500 + lines[startPos].y;
		float x = 0, dx = 0;
		float maxY = height;

		//// Draw Background ////
		background.setPosition(-128.0f + (playerX * -.006), -300.0f + (camH * .008)); //Background moves with camera
		app.draw(background);

		//// Draw Road to Horizon Line ////
		for (int n = startPos; n <= startPos + 300; n++)
		{
			Line &l = lines[n%N];
			l.project(playerX - x, camH, pos - (n >= N ? N * segL : 0));
			x += dx;
			dx += l.curve;

			l.clip = maxY;
			if (l.Y >= maxY) continue;
			maxY = l.Y;

			Color grass = (n / 3) % 2 ? Color(0, 77, 0) : Color(0, 102, 0);
			Color rumble = (n / 3) % 2 ? Color(255, 255, 255) : Color(153, 0, 0);
			Color road = (n / 3) % 2 ? Color(107, 107, 107) : Color(105, 105, 105);

			Line p = lines[(n - 1) % N]; //previous line

			drawQuad(app, grass,  0,   p.Y, width,      0,   l.Y, width);
			drawQuad(app, rumble, p.X, p.Y, p.W * 1.15, l.X, l.Y, l.W*1.15);
			drawQuad(app, road,   p.X, p.Y, p.W,        l.X, l.Y, l.W);
		}

		//// Draw Objects from Horizon line to Player ////
		for (int n = startPos + 300; n >= startPos; n--)
		{
			Line &l = lines[n%N];
			l.drawLineObjects(app);
		}
		app.display();
	}

	return 0;
}
