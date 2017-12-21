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

	void SyncCarData();
	void UpdateStats();

	int carId;
	std::string modelName;
	int modelId;

	// Dynamic
	float angle;
	float wheelAngle;
	float speed;
	CarState state;

	// Semi Dynamic - Stats
	float fuelAmount;
	float horsePower;
	float torque;
	float acceleration;
	float suspension;
	
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
		state = Neutral;
		x = 0;
		y = 0;
		SyncCarData();
		UpdateStats();
	}
};
#endif
