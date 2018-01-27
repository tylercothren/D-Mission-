#include "car.h"

void Car::SyncCarData() 
{
	// API Call with carId and token to receive data.
}

void Car::UpdateStats()
{
	// Formula for horsePower, torque, acceleration, suspention based on data.
}

void Car::updateRPMFloor()
{
	rpmFloor = 1000; //idle for now?
}

void Car::gasPedalDown()
{
	rpm += rpm_dx;
	if (clutch == false) // Only Engine Revs when clutch is disengaged
		speed += acceleration;
}

void Car::brakePedalDown()
{

}

void Car::clutchPedalDown()
{

}

/// <summary>
/// Results of shifting the car.
/// </summary>
/// <param name="to">To.</param>
void Car::Shift(int to)
{
	if (to == 0)
	{
		currentGear = 0;
		acceleration = 0;
		rpm_dx = engineRpm_dx;
	}
	else if (to == 1)
	{
		currentGear = 1;
		acceleration = firstGearAcceleration;
		rpm_dx = firstGearRpm_dx;
	}
	else if (to == 2)
	{
		currentGear = 2;
		acceleration = secondGearAcceleration;
		rpm_dx = secondGearRpm_dx;
	}
	else if (to == 3)
	{
		currentGear = 3;
		acceleration = thirdGearAcceleration;
		rpm_dx = thirdGearRpm_dx;
	}
	else if (to == 4)
	{
		currentGear = 4;
		acceleration = fourthGearAcceleration;
		rpm_dx = fourthGearRpm_dx;
	}
	else if (to == 5)
	{
		currentGear = 5;
		acceleration = fifthGearAcceleration;
		rpm_dx = fifthGearRpm_dx;
	}
}