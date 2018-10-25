/*
 * halShockInterrupt.cpp
 *
 *  Created on: Oct 7, 2018
 *      Author: choeecs27
 */

#include "halAccelerometerCtrl.hpp"

#include "mbed.h"
#include "halADXL345.hpp"


Timer shockIntTimer;

//
// shock interrupt to use pull down mode 2
//
InterruptIn shockIntIn(p10);

bool TapOccurred;

void ISRShockIntIn(void)
{
	//unsigned int curTapOccurTime = shockIntTimer.read_ms();

	//if (curTapOccurTime > 100)
	//{
	//	TapOccurTime = curTapOccurTime;
		shockIntTimer.reset();
		TapOccurred = true;
	//}

	wait_ms(50);
	hal_ADXL345 *acce = hal_ADXL345::GetInstance();
	unsigned char intSource = acce->GetInterruptSource();
	int16_t data[3];
	acce->ReadXYZ(data);
}

void ShockIntInit(void)
{
	TapOccurred = false;
	shockIntTimer.reset();
	shockIntTimer.start();
	shockIntIn.mode(PullDown);
	shockIntIn.rise(&ISRShockIntIn);
}

bool CheckTapOccurred(void)
{
	if (TapOccurred == true)
	{
		if (shockIntTimer.read_ms() > 150)
		{
			//
			// If Tap is consumed 100 ms after the event, discard the event
			//
			TapOccurred = false;
		}
	}
	return TapOccurred;
}

int CheckTiltDirection(void)
{
	const int defaultYPos = 128;

	int16_t data[3];
	hal_ADXL345::GetInstance()->ReadXYZ(data);
	int adj = 0;
	if (data[1] > 50+defaultYPos)
		adj = 1;
	else if (data[1] < -50+defaultYPos)
		adj = -1;
	return adj;
}
