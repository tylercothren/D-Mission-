#include "car.h"

void Car::SyncCarData() 
{
	// API Call with carId and token to receive data.
}

void Car::UpdateStats()
{
	// Formula for horsePower, torque, acceleration, suspention based on data.
}

/// <summary>
/// Results of shifting the car.
/// </summary>
/// <param name="from">From.</param>
/// <param name="to">To.</param>
void Car::Shift(int from, int to)
{
	if (from == 0 && to == 1)
	{
		currentGear = 1;
		acceleration = firstGearAcceleration;
		rpm_dx = torque;
	}
	else if (from == 0 && to == 2)
	{
		currentGear = 2;
		acceleration = secondGearAcceleration;
		rpm_dx = torque * .85;
	}
	else if (from == 0 && to == 3)
	{
		currentGear = 3;
		acceleration = thirdGearAcceleration;
		rpm_dx = torque * .5;
	}
	else if (from == 1 && to == 0)
	{
		currentGear = 0;
		acceleration = 0;
	}
	else if (from == 1 && to == 2)
	{
		currentGear = 2;
		acceleration = secondGearAcceleration;
		rpm_dx = torque * .85;
	}
	else if (from == 1 && to == 3)
	{
		currentGear = 3;
		acceleration = thirdGearAcceleration;
		rpm_dx = torque * .5;
	}
	else if (from == 2 && to == 0)
	{
		currentGear = 0;
		acceleration = 0;
	}
	else if (from == 2 && to == 1)
	{
		currentGear = 1;
		acceleration = firstGearAcceleration;
		rpm_dx = torque;
	}
	else if (from == 2 && to == 3)
	{
		currentGear = 3;
		acceleration = thirdGearAcceleration;
		rpm_dx = torque * .5;
	}
	else if (from == 3 && to == 0)
	{
		currentGear = 0;
		acceleration = 0;
	}
	else if (from == 3 && to == 1)
	{
		currentGear = 1;
		acceleration = firstGearAcceleration;
		rpm_dx = torque;
	}
	else if (from == 3 && to == 2)
	{
		currentGear = 2;
		acceleration = secondGearAcceleration;
		rpm_dx = torque * .85;
	}
	else if (from == 3 && to == 4)
	{
		currentGear = 4;
		acceleration = fourthGearAcceleration;
		rpm_dx = torque * .25;
	}
	else if (from == 4 && to == 0)
	{
		currentGear = 0;
		acceleration = 0;
	}
	else if (from == 4 && to == 3)
	{
		currentGear = 3;
		acceleration = thirdGearAcceleration;
		rpm_dx = torque * .5;
	}
	else if (from == 4 && to == 5)
	{
		acceleration = fifthGearAcceleration;
		rpm_dx = torque * .125;
	}
	else if (from == 5 && to == 4)
	{
		currentGear = 4;
		acceleration = fourthGearAcceleration;
		rpm_dx = torque * .25;
	}
	else if (from == 5 && to == 0)
	{
		currentGear = 0;
		acceleration = 0;
	}
}