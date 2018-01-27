#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "car.h"
#include "line.h"

using namespace sf;

void drawQuad(RenderWindow &w, Color c, int x1, int y1, int w1, int x2, int y2, int w2);
void project(Line &l, int camX, int camY, int camZ);

int width = 1024;
int height = 768;
int roadW = 2000;
int segmentLength = 200;
int viewDistance = 300;
int maxFPS = 60;

float leftRoadBound = -1;
float rightRoadBound = 1;
float decel = -2;
float step = 1/maxFPS;
float cameraDepth = 0.84;
float horizonLine = height / 2;
float centerLine = width / 2;
float maxZ = viewDistance * segmentLength;

std::vector<sf::Sprite> objects;


/// <summary>
/// Draws the quad.
/// </summary>
/// <param name="w">The w.</param>
/// <param name="c">The c.</param>
/// <param name="x1">The x1.</param>
/// <param name="y1">The y1.</param>
/// <param name="w1">The w1.</param>
/// <param name="x2">The x2.</param>
/// <param name="y2">The y2.</param>
/// <param name="w2">The w2.</param>
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

/// <summary>
/// Projects the specified l.
/// </summary>
/// <param name="l">The l.</param>
/// <param name="camX">The cam x.</param>
/// <param name="camY">The cam y.</param>
/// <param name="camZ">The cam z.</param>
void project(Line &l, int camX, int camY, int camZ)
{
	l.scale = cameraDepth / (l.z - camZ);
	l.X = (1 + l.scale*(l.x - camX)) * width / 2;
	l.Y = (1 - l.scale*(l.y - camY)) * height / 2;
	l.W = l.scale * roadW  * width / 2;
}

float accelerate(float vi, float a, float t)
{
	float vf = vi + (a * t);
	return vf;
}

void limit(float &x, float upperBound, float lowerBound) 
{
	if (x > upperBound)
		x = upperBound;
	else if (x < lowerBound)
		x = lowerBound;
}

/// <summary>
/// Entry point.
/// </summary>
/// <returns></returns>
int main()
{
	RenderWindow app(VideoMode(width, height), "D-Mission!");
	app.setFramerateLimit(maxFPS);

	Texture textures[10];

	textures[0].loadFromFile("Assets/tree1.png");
	textures[0].setSmooth(true);
	objects.push_back(sf::Sprite(textures[0]));

	textures[1].loadFromFile("Assets/house2.png");
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

	textures[5].loadFromFile("Assets/building1.png");
	textures[5].setSmooth(true);
	objects.push_back(sf::Sprite(textures[5]));

	textures[6].loadFromFile("Assets/building2.png");
	textures[6].setSmooth(true);
	objects.push_back(sf::Sprite(textures[6]));

	textures[7].loadFromFile("Assets/speedometer.png");
	textures[7].setSmooth(true);
	objects.push_back(sf::Sprite(textures[7]));

	textures[8].loadFromFile("Assets/gaugeNeedle.png");
	textures[8].setSmooth(true);
	objects.push_back(sf::Sprite(textures[8]));

	textures[9].loadFromFile("Assets/tachometer.png");
	textures[9].setSmooth(true);
	objects.push_back(sf::Sprite(textures[9]));

	Texture bg;
	bg.loadFromFile("Assets/bg.jpg");
	bg.setRepeated(true);
	Sprite sBackground(bg);
	sBackground.setTextureRect(IntRect(0, 0, 5000, 411));
	sBackground.setPosition(-2000, 0);

	std::vector<Line> lines;

	Car playerCar = Car();

	for (int i = 0; i<1600; i++)
	{
		Line line;
		line.z = i*segmentLength;

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
	int pos = 200;
	int H = 1500;
	int dt = 0;
	playerCar.speed = 0.0f;

	//// Game Loop ////
	while (app.isOpen())
	{
		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				app.close();
		}

		//// Reset Car ////
		playerCar.clutch = false;		
		playerCar.rpm = accelerate(playerCar.rpm, -50, 1); // RPM's drop when foot is not on pedal.
		playerCar.updateRPMFloor();
		limit(playerCar.rpm, playerCar.redline, playerCar.rpmFloor); // RPM's don't drop past a certain point depending on how fast the car is moving/gear transmission is in.
		limit(playerCar.speed, playerCar.maxSpeed, 0);
		//if (playerCar.rpm >= playerCar.redline)
			//playerCar.acceleration = decel * -1;
		playerCar.speed = accelerate(playerCar.speed, decel, 1); // Natural friction with road and air.
		playerCar.carState = Car::Neutral;

		if (Keyboard::isKeyPressed(Keyboard::Space)) playerCar.clutch = true;
		if (Keyboard::isKeyPressed(Keyboard::Right)) 
		{
			playerX += 0.05 + (0.5 * (playerCar.speed / playerCar.maxSpeed));
			playerCar.carState = Car::TurnRight;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			playerX -= 0.05 + (0.5 * (playerCar.speed / playerCar.maxSpeed));
			playerCar.carState = Car::TurnLeft;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			playerCar.gasPedalDown();
		}
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			playerCar.speed = accelerate(playerCar.speed, playerCar.brake, 1);
		}
		if (Keyboard::isKeyPressed(Keyboard::Tab)) playerCar.speed *= 3;
		if (Keyboard::isKeyPressed(Keyboard::W)) H += 100; // Hax
		if (Keyboard::isKeyPressed(Keyboard::S)) H -= 100; // Hax
		if (Keyboard::isKeyPressed(Keyboard::E))
		{
			if (playerCar.currentGear == 0)
				playerCar.Shift(0);
			else if (playerCar.currentGear == 1)
				playerCar.Shift(0);
			else if (playerCar.currentGear == 2)
				playerCar.Shift(0);
			else if (playerCar.currentGear == 3)
				playerCar.Shift(0);
			else if (playerCar.currentGear == 4)
				playerCar.Shift(0);
			else if (playerCar.currentGear == 5)
				playerCar.Shift(0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Num1) == true && playerCar.clutch == true)
		{
			if (playerCar.rpm >= 1000 && playerCar.currentGear == 0)
				playerCar.Shift(1);
			else if(playerCar.currentGear == 2)
				playerCar.Shift(1);
			else if (playerCar.currentGear == 3)
				playerCar.Shift(1);
			else
				playerCar.engineState = Car::Stalled;
		}
		if (Keyboard::isKeyPressed(Keyboard::Num2) == true && playerCar.clutch == true)
		{
			if (playerCar.rpm >= 1000 && playerCar.currentGear == 1)
				playerCar.Shift(2);
			else if (playerCar.currentGear == 3)
				playerCar.Shift(2);
			else if (playerCar.rpm >= 2000 && playerCar.currentGear == 0)
				playerCar.Shift(2);
			else
				playerCar.engineState = Car::Stalled;
		}
		if (Keyboard::isKeyPressed(Keyboard::Num3) == true && playerCar.clutch == true)
		{
			if (playerCar.rpm >= 2000 && playerCar.currentGear == 2)
				playerCar.Shift(3);
			else if (playerCar.rpm >= 3000 && playerCar.currentGear == 0)
				playerCar.Shift(3);
			else if (playerCar.rpm >= 2000 && playerCar.currentGear == 1)
				playerCar.Shift(3);
			else if (playerCar.currentGear == 4)
				playerCar.Shift(3);
			else
				playerCar.engineState = Car::Stalled;
		}
		if (Keyboard::isKeyPressed(Keyboard::Num4) == true && playerCar.clutch == true)
		{
			if (playerCar.rpm >= 5000 && playerCar.currentGear == 3)
				playerCar.Shift(4);
			else if (playerCar.currentGear == 5)
				playerCar.Shift(4);
			else
				playerCar.engineState = Car::Stalled;
		}
		if (Keyboard::isKeyPressed(Keyboard::Num5) == true && playerCar.clutch == true)
		{
			if (playerCar.rpm >= 5000 && playerCar.currentGear == 4)
				playerCar.Shift(5);
			else
				playerCar.engineState = Car::Stalled;
		}

		//// Effect of Player's Car Velocity ////
		limit(playerCar.speed, 1000000, 0);

		pos += playerCar.speed * 7;

		while (pos >= N*segmentLength) pos -= N*segmentLength;
		while (pos < 0) pos += N*segmentLength;

		app.clear();
		app.draw(sBackground);

		int startPos = pos / segmentLength;
		int camH = lines[startPos].y + H;
		if (playerCar.speed>0) sBackground.move(-lines[startPos].curve * 2, 0);
		if (playerCar.speed<0) sBackground.move(lines[startPos].curve * 2, 0);

		int maxy = height;
		float x = 0, dx = 0;


		//// Draw Road from Screen to Horizon Line ////
		for (int n = startPos; n<startPos + 300; n++)
		{
			Line &l = lines[n%N];
			project(l, playerX*roadW - x, camH, startPos*segmentLength - (n >= N ? N*segmentLength : 0));
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
			lines[n%N].drawSprite(app, width);
		}

		//// Draw Car ////
		if (playerCar.carState == Car::TurnRight)
		{
			objects[4].setScale(.3, .3);
			objects[4].setPosition(centerLine - (objects[3].getTextureRect().width / 2 * .3), horizonLine + 100);
			app.draw(objects[4]);
		}
		else if (playerCar.carState == Car::TurnLeft)
		{
			objects[3].setScale(.3, .3);
			objects[3].setPosition(centerLine - (objects[3].getTextureRect().width / 2 * .3), horizonLine + 100);
			app.draw(objects[3]);
		}
		else if(playerCar.carState == Car::Neutral)
		{
			objects[2].setScale(.3, .3);
			objects[2].setPosition(centerLine - (objects[2].getTextureRect().width / 2 * .3), horizonLine + 100);
			app.draw(objects[2]);
		}

		//// Draw UI ////
		
		//Speedometer
		objects[7].setScale(.5, .5);
		objects[7].setPosition(20, (horizonLine*2) - 135);
		app.draw(objects[7]);

		objects[8].setScale(.5, .5); 
		objects[8].setOrigin(180, 141);
		objects[8].setPosition(111, (horizonLine * 2) - 55);
		objects[8].setRotation(-110 + (210 * (playerCar.speed/ playerCar.maxSpeed)));
		app.draw(objects[8]);

		sf::Text speedometer;
		sf::Font font;
		if (!font.loadFromFile("Assets/Fonts/helvetica.ttf"))
		{
			// error...
		}
		speedometer.setFont(font);
		std::string str = std::to_string(playerCar.speed);
		str.erase(str.find_last_not_of('0') + 1, std::string::npos);
		str.erase(str.find_last_not_of('.') + 1, std::string::npos);
		speedometer.setString(str);
		speedometer.setCharacterSize(20);
		speedometer.setPosition(105, (horizonLine * 2) - 30);
		speedometer.setFillColor(Color::Red);
		app.draw(speedometer);

		sf::Text accelerationGauge;
		accelerationGauge.setFont(font);
		accelerationGauge.setString("Acceleration: " + std::to_string(playerCar.acceleration) + "/frame");
		accelerationGauge.setCharacterSize(24);
		accelerationGauge.setFillColor(Color::Red);
		accelerationGauge.setPosition(0, 30);
		app.draw(accelerationGauge);

		//Tachometer
		objects[9].setScale(.15, .15);
		objects[9].setPosition(190, (horizonLine * 2) - 150);
		app.draw(objects[9]);
		sf::Text rpm;
		rpm.setFont(font);
		rpm.setString("RPM: " + std::to_string(playerCar.rpm));
		rpm.setCharacterSize(24);
		rpm.setFillColor(Color::Red);
		rpm.setPosition(0, 60);
		app.draw(rpm);

		sf::Text gear;
		gear.setFont(font);
		gear.setString("Gear: " + std::to_string(playerCar.currentGear));
		gear.setCharacterSize(24);
		gear.setFillColor(Color::Red);
		gear.setPosition(0, 90);
		app.draw(gear);

		sf::Text clutch;
		clutch.setFont(font);
		clutch.setString("Clutch: " + std::to_string(playerCar.clutch));
		clutch.setCharacterSize(24);
		clutch.setFillColor(Color::Red);
		clutch.setPosition(0, 120);
		app.draw(clutch);

		sf::Text engineState;
		engineState.setFont(font);
		engineState.setString("Engine: " + std::to_string(playerCar.engineState));
		engineState.setCharacterSize(24);
		engineState.setFillColor(Color::Red);
		engineState.setPosition(0, 150);
		app.draw(engineState);

		app.display();
	}
	return 0;
}

