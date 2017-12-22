#ifndef CAR_H
#define CAR_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

struct Car
{
	enum CarState
	{
		Neutral,
		TurnLeft,
		TurnRight
	};

	enum EngineState
	{
		Off,
		Idle,
		Stalled,
		Throttle
	};

	void SyncCarData();
	void UpdateStats();
	void Shift(int from, int to);

	int carId;
	std::string modelName;
	int modelId;

	// Dynamic
	bool clutch;
	float angle;
	float wheelAngle;
	float rpm;
	float speed;
	CarState carState;
	EngineState engineState;

	// Semi Dynamic - Stats
	float maxSpeed;
	float redline;
	float fuelAmount;
	float horsePower;
	float torque;
	float acceleration;
	float rpm_dx;
	float brake;
	float suspension;
	int currentGear;

	float firstGearAcceleration;
	float secondGearAcceleration;
	float thirdGearAcceleration;
	float fourthGearAcceleration;
	float fifthGearAcceleration;

	// Semi Dynamic - Physical Parts
	int liveryId;
	int colorId;
	int camShaftId;
	int crankShaftId;
	int pistonsId;
	int turboId;
	int intercoolerId;
	int oilCoolerId;
	int wheelSetId;
	int fuelPumpId;
	int fuelInjectorsId;
	int suspensionId;
	int ecuId;
	int fuelCellId;
	int RadiatorId;
	int transmissionId;
	int driveShaftId;
	int differentialId;
	int bodyKitId;

	float x;
	float y;

	Car()
	{
		carState = Neutral;
		x = 0;
		y = 0;
		speed = 0.0;
		fuelAmount = 0.0;
		horsePower = 0.0;
		torque = 0.0;
		acceleration = 0.0;
		brake = 0.0;
		suspension = 0.0;
		clutch = 0.0;
		angle = 0.0;
		wheelAngle = 0.0;
		rpm = 0.0;
		speed = 0.0;
		maxSpeed = 650;
		redline = 7000;
		fuelAmount = 0.0;
		horsePower = 0.0;
		torque = 480;
		acceleration = 0.0;
		rpm_dx = 0;
		brake = 0.0;
		suspension = 0.0;
		currentGear = 0;
		firstGearAcceleration = 25;
		secondGearAcceleration = 22;
		thirdGearAcceleration = 15;
		fourthGearAcceleration = 10;
		fifthGearAcceleration = 5;
		SyncCarData();
		UpdateStats();
	}
};
#endif
