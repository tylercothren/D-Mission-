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
		acceleration = firstGearAcceleration;
		rpm_dx = redline / (torque * .75);
	}
	else if (from == 0 && to == 2)
	{
		acceleration = secondGearAcceleration;
		rpm_dx = redline / (torque * .85);
	}
	else if (from == 0 && to == 3)
	{
		acceleration = thirdGearAcceleration;
		rpm_dx = redline / (torque * .95);
	}
	else if (from == 1 && to == 0)
	{
		acceleration = 0;
	}
	else if (from == 1 && to == 2)
	{
		acceleration = secondGearAcceleration;
		rpm_dx = redline / (torque * .85);
	}
	else if (from == 1 && to == 3)
	{
		acceleration = thirdGearAcceleration;
		rpm_dx = redline / (torque * .95);
	}
	else if (from == 2 && to == 0)
	{
		acceleration = 0;
	}
	else if (from == 2 && to == 1)
	{
		acceleration = firstGearAcceleration;
		rpm_dx = redline / (torque * .75);
	}
	else if (from == 2 && to == 3)
	{
		acceleration = thirdGearAcceleration;
		rpm_dx = redline / (torque * .95);
	}
	else if (from == 3 && to == 0)
	{
		acceleration = 0;
	}
	else if (from == 3 && to == 1)
	{
		acceleration = firstGearAcceleration;
		rpm_dx = redline / (torque * .75);
	}
	else if (from == 3 && to == 2)
	{
		acceleration = secondGearAcceleration;
		rpm_dx = redline / (torque * .85);
	}
	else if (from == 3 && to == 4)
	{
		acceleration = fourthGearAcceleration;
		rpm_dx = redline / (torque * 1.5);
	}
	else if (from == 4 && to == 0)
	{
		acceleration = 0;
	}
	else if (from == 4 && to == 3)
	{
		acceleration = thirdGearAcceleration;
		rpm_dx = redline / (torque * 1);
	}
	else if (from == 4 && to == 5)
	{
		acceleration = fifthGearAcceleration;
		rpm_dx = redline / (torque * 1.75);
	}
	else if (from == 5 && to == 4)
	{
		acceleration = fourthGearAcceleration;
		rpm_dx = redline / (torque * 1.5);
	}
	else if (from == 5 && to == 0)
	{
		acceleration = 0;
	}
}