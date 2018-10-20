/*
 * halShockInterrupt.cpp
 *
 *  Created on: Oct 7, 2018
 *      Author: choeecs27
 */

#include "mbed.h"
#include "halShockInterrupt.hpp"
#include "halADXL345.hpp"

extern Timer shockIntTimer;
extern InterruptIn shockIntIn;

//
// For debug. remove when done
//
extern Serial pc;

bool TapOccurred;
unsigned int TapOccurTime;

void ISRShockIntIn(void)
{
	unsigned int curTapOccurTime = shockIntTimer.read_ms();

	if (curTapOccurTime > 100)
	{
		pc.printf("ISR - Tap occurred!\r\n");
		TapOccurTime = curTapOccurTime;
		shockIntTimer.reset();
		TapOccurred = true;
	}

	wait_ms(50);
	hal_ADXL345 *acce = hal_ADXL345::GetInstance();
	unsigned char intSource = acce->GetInterruptSource();
	pc.printf("ISR - serviced! Int src: 0x%02x\r\n", intSource);
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
		if (shockIntTimer.read_ms() > 100)
		{
			//
			// If Tap is consumed 100 ms after the event, discard the event
			//
			TapOccurred = false;
		}
	}
	return TapOccurred;
}
