/*
 * roadObject.cpp
 *
 *  Created on: Oct 7, 2018
 *      Author: choeecs27
 */

#include "roadObject.hpp"

#include "mbed.h"
extern Serial pc;


obstacle::obstacle(int vPosition, unsigned int timeAppear)
{
	this->timeAppear = timeAppear;
	hPosition = 15;
	width = 4;
	speed = 0;

	//
	// vPosition can only be 0 or 2
	//
	this->vPosition = (vPosition == 0) ? 0:2;
}

bool obstacle::UpdatePosition(float extSpeed, unsigned int currentTime)
{
	bool isObjectOut = false;
	float currentPosition = 15 - ((currentTime - timeAppear) * (extSpeed - speed) / 1000);
	if (currentPosition < 0)
	{
		hPosition = 0;
		isObjectOut = true;
	}
	else
	{
		hPosition = (unsigned int) currentPosition;
	}
	return isObjectOut;
}

bool obstacle::DoIBlockDrDriver(unsigned int vPos, unsigned int hPos)
{
	bool bBlockingDrDriver = false;
	if (hPosition == hPos + 1)
	{
		if ((vPos >= vPosition) && (vPos <= (vPosition + 1)))
		{
			bBlockingDrDriver = true;
		}
	}
	return bBlockingDrDriver;
}

void obstacle::PrintOnLcd(char line[2][17])
{
	//pc.printf("v: %d h: %d\r\n", vPosition, hPosition);
	unsigned int vPos = vPosition == 0? 0:1;
	line[vPos][hPosition] = '@';
}





roadCar::roadCar(int vPosition, float speed, unsigned int timeAppear)
{
	this->timeAppear = timeAppear;
	this->speed = speed;
	this->vPosition = vPosition;
	hPosition = 15;
	width = 2;
}

bool roadCar::UpdatePosition(float extSpeed, unsigned int currentTime)
{
	bool isObjectOut = false;
	float currentPosition = 15 - ((currentTime - timeAppear) * (extSpeed - speed) / 1000);
	if (currentPosition < 0)
	{
		hPosition = 0;
		isObjectOut = true;
	}
	else
	{
		hPosition = (unsigned int) currentPosition;
	}
	return isObjectOut;
}

bool roadCar::DoIBlockDrDriver(unsigned int vPos, unsigned int hPos)
{
	bool bBlockingDrDriver = false;
	if (hPosition == hPos + 1)
	{
		if (vPos == vPosition)
		{
			bBlockingDrDriver = true;
		}
	}
	return bBlockingDrDriver;
}

void roadCar::PrintOnLcd(char line[2][17])
{
	//pc.printf("v: %d h: %d\r\n", vPosition, hPosition);
	char curPrint = (vPosition % 2 == 0)? 0x02 : 0x01;
	if (line[vPosition / 2][hPosition] + curPrint == 0x03)
		curPrint = 0x03;
	line[vPosition / 2][hPosition] = curPrint;
}
