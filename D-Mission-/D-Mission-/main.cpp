#include <SFML\Graphics.hpp>
using namespace sf;

float width = 1024.0;
float height = 768.0;
int roadW = 2000;
int segL = 200; // Segment Length
float camD = 0.84; // Camera Depth

struct Line
{
	float x, y, z; //3D Center of Line
	float X, Y, W; //screen coord
	float scale, curve, spriteX, clip;
	Sprite sprite;

	Line() { curve = x = y = z = 0; }

	//From world to screen coordinates
	void project(int camX, int camY, int camZ) 
	{
		scale = camD / (z - camZ);
		X = (1 + scale*(x - camX)) * width / 2;
		Y = (1 - scale*(y - camY)) * height / 2;
		W = scale * roadW * width / 2;
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

		destX += destW * spriteX;
		destY += destH * (-1);

		float clipH = destY + destH - clip;
		if (clipH < 0) clipH = 0;

		if (clipH >= destH) return;
		s.setTextureRect(IntRect(0, 0, w, (h - h) * (clipH / destH)));
		s.setScale(destW / w, destH / h);
		s.setPosition(destX, destY);
		app.draw(s);
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
	RenderWindow app(VideoMode(width, height), "D-Mission!");
	app.setFramerateLimit(60);

	RectangleShape background(Vector2f(width*1.25, height));
	Texture bg;
	bg.loadFromFile("bg.jpg");
	bg.setRepeated(true);
	background.setTexture(&bg);

	Texture t;
	t.loadFromFile("tree1.png");
	t.setSmooth(true);
	Sprite sTree;
	sTree.setTexture(t);

	std::vector<Line> lines;

	//// Track Plan ////
	for (int i = 0; i < 1600; i++)
	{
		Line line;
		line.z = i*segL;

		//// Curve Example at Track Position
		if (i < 300 && i < 700) line.curve = 2;

		//// Elevation Change Example at Track Position
		if (i > 300) line.y = sin(i / 30.0) * 1500;

		if (i % 2 == 0) 
		{
			line.spriteX = -2.5;
			line.sprite = &sTree;
		}

		lines.push_back(line);
	}

	int N = lines.size();
	int pos = 200;
	int playerX = 0;

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

		//Infinite track
		while (pos >= N*segL) pos -= N*segL;
		while (pos < 0) pos += N*segL;

		app.clear();
		int startPos = pos / segL;
		int camH = 1500 + lines[startPos].y;
		float x = 0, dx = 0;
		int maxY = height;

		//// Draw Background ////
		background.setPosition(-128.0f + (playerX * -.006), -300.0f + (camH * .01)); //Background moves with camera
		app.draw(background);

		//// Draw Road to Horizon Line////
		for (int n = startPos; n <= startPos+300; n++) 
		{
			Line &l = lines[n%N];
			l.project(playerX - x, camH, pos - (n >= N?N*segL:0));
			x += dx;
			dx += l.curve;

			if (l.Y >= maxY) continue;
			maxY = l.Y;

			Color grass = (n / 3) % 2 ? Color(16, 200, 16) : Color(0, 154, 0);
			Color rumble = (n / 3) % 2 ? Color(255, 255, 255) : Color(0, 0, 0);
			Color road = (n / 3) % 2 ? Color(107, 107, 107) : Color(105, 105, 105);

			Line p = lines[(n - 1)% N]; //previous line

			drawQuad(app, grass, 0, p.Y, width, 0, l.Y, width);
			drawQuad(app, rumble, p.X, p.Y, p.W*1.2, l.X, l.Y, l.W*1.2);
			drawQuad(app, road, p.X, p.Y, p.W, l.X, l.Y, l.W);
		}

		//// Draw Objects ////
		for (int n = startPos + 300; n > startPos; n--) 
		{
			lines[n%N].drawSprite(app);
		}

		app.display();
	}

	return 0;
}