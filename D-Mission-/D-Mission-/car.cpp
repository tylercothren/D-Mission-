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
		if (rpm <= redline - 300) // Cant Accelerate if engine is not turning faster
			speed += acceleration;
		else
			speed += 1;
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
		if(currentGear < 1) //upshifting
			rpm -= (1 - currentGear) * 2000;
		else if (currentGear > 1) //downshifting
			rpm += (currentGear - 1) * 2000;
		currentGear = 1;
		acceleration = firstGearAcceleration;
		rpm_dx = firstGearRpm_dx;
	}
	else if (to == 2)
	{
		if (currentGear < 2) //upshifting
			rpm -= (2 - currentGear) * 2000;
		else if (currentGear > 2) //downshifting
			rpm += (currentGear - 2) * 2000;
		currentGear = 2;
		acceleration = secondGearAcceleration;
		rpm_dx = secondGearRpm_dx;
	}
	else if (to == 3)
	{
		if (currentGear < 3) //upshifting
			rpm -= (3 - currentGear) * 2000;
		else if (currentGear > 3) //downshifting
			rpm += (currentGear - 3) * 2000;
		currentGear = 3;
		acceleration = thirdGearAcceleration;
		rpm_dx = thirdGearRpm_dx;
	}
	else if (to == 4)
	{
		if (currentGear < 4) //upshifting
			rpm -= (4 - currentGear) * 2000;
		else if (currentGear > 4) //downshifting
			rpm += (currentGear - 4) * 2000;
		currentGear = 4;
		acceleration = fourthGearAcceleration;
		rpm_dx = fourthGearRpm_dx;
	}
	else if (to == 5)
	{
		if (currentGear < 5) //upshifting
			rpm -= (5 - currentGear) * 2000;
		else if (currentGear > 5) //downshifting
			rpm += (currentGear - 5) * 2000;
		currentGear = 5;
		acceleration = fifthGearAcceleration;
		rpm_dx = fifthGearRpm_dx;
	}
}