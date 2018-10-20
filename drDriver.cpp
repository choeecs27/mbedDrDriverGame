/*
 * drDriver.cpp
 *
 *  Created on: Oct 7, 2018
 *      Author: choeecs27
 */

#include "drDriver.hpp"

drDriver::drDriver(): roadCar(2, 3, 0)
{
	// TODO Auto-generated constructor stub
	hPosition = DR_DRIVER_HPOSITION;
	width = 2;
}


bool drDriver::UpdatePosition(float extSpeed, unsigned int currentTime)
{
	return false;
}

bool drDriver::AdjustVPosition(int step)
{
	int tmpVPos = vPosition + step;
	if (tmpVPos >=0 && tmpVPos <=3)
		vPosition = tmpVPos;
}
