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
	void updateRPMFloor();
	void gasPedalDown();
	void brakePedalDown();
	void clutchPedalDown();
	void Shift(int to);

	int carId;
	std::string modelName;
	int modelId;

	// Dynamic
	bool clutch;
	float angle;
	float wheelAngle;
	float rpm;
	float rpm_dx;
	float rpmFloor;
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
	float brake;
	float suspension;
	int currentGear;

	// Transmission Stats
	float engineRpm_dx;
	float firstGearAcceleration;
	float firstGearRpm_dx;
	float secondGearAcceleration;
	float secondGearRpm_dx;
	float thirdGearAcceleration;
	float thirdGearRpm_dx;
	float fourthGearAcceleration;
	float fourthGearRpm_dx;
	float fifthGearAcceleration;
	float fifthGearRpm_dx;

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
		torque = 480.0;
		acceleration = 0.0;
		brake = 0.0;
		suspension = 0.0;
		clutch = 0.0;
		angle = 0.0;
		wheelAngle = 0.0;
		rpm = 0.0;
		rpmFloor = 3000;
		speed = 0.0;
		maxSpeed = 260;
		redline = 7000;
		fuelAmount = 0.0;
		horsePower = 0.0;
		torque = 480;
		acceleration = 0.0;
		firstGearAcceleration = 1 + 1;
		secondGearAcceleration = 1 + .8;
		thirdGearAcceleration = 1 + .6;
		fourthGearAcceleration = 1 + .5;
		fifthGearAcceleration = 1 + .3;
		engineRpm_dx = torque * 1;
		firstGearRpm_dx = torque * .25;
		secondGearRpm_dx = torque * .23;
		thirdGearRpm_dx = torque * .21;
		fourthGearRpm_dx = torque * .19;
		fifthGearRpm_dx = torque * .17;
		rpm_dx = engineRpm_dx;
		brake = 0.0;
		suspension = 0.0;
		currentGear = 0;		
		SyncCarData();
		UpdateStats();
	}
};
#endif
