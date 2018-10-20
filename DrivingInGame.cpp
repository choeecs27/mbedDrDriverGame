/*
 * DrivingInGame.cpp
 *
 *  Created on: Oct 7, 2018
 *      Author: choeecs27
 */
#include "mbed.h"
#include "drDriver.hpp"
#include "DrivingInGame.hpp"
#include "roadObject.hpp"
#include "halADXL345.hpp"
#include "TextLCD.h"
#include <string.h>
#include <cstdlib>

extern TextLCD lcd;
extern Timer gameTimer;
extern Serial pc;
extern DigitalIn steer;

float DrivingInGame::Start(void)
{
	hal_ADXL345* acce = hal_ADXL345::GetInstance();

	gameTimer.reset();
	gameTimer.start();

	pRoadObject[0] = new obstacle(0, 0);
	pRoadObject[1] = new roadCar(3, 1, 0);
	//pRoadObject[1] = new obstacle(2, 0);
	pRoadObject[2] = 0;
	Hayato = new drDriver();
	srand(gameTimer.read_ms());

	int steerDownsampleCt = 0;
	float score = 0;
	while(1)
	{
		unsigned int currentTime = gameTimer.read_ms();

		//
		// Update obstacle position
		//
		updateObstaclePosition(currentTime);

		//
		// Update Dr Driver position
		//
		updateDrDriverPosition();

		//
		// Collision checking
		//
		bool collision = isCollision();

		//
		// Randomly generate obstacle
		//
		generateObstacle();

		//
		// Refresh LCD display
		//
		refreshLcd();

		//
		// Update every 20 ms (50 Hz)
		//
		wait_ms(REFRESH_PERIOD);
		score += REFRESH_PERIOD * Hayato->GetSpeed();
		//pc.printf("score: (%f)\r\n",score);
		steerDownsampleCt++;
		if (steerDownsampleCt == 12)
		{
			int16_t data[3];
			steerDownsampleCt = 0;
			acce->ReadXYZ(data);
			int adj = 0;
			if (data[1] > 50+defaultYPos)
				adj = 1;
			else if (data[1] < -50+defaultYPos)
				adj = -1;
			pc.printf("x: (%+4d), y: (%+4d), z: (%+4d)\r\n",data[0],data[1], data[2]);
			Hayato->AdjustVPosition(adj);
		}

		if(collision == true)
		{
			wait_ms(2000);
			break;
		}
	}

	//
	// Delete all road objects
	//
	for (unsigned int i = 0; i < MAX_OBSTACLE_COUNT; i++)
	{
		if (pRoadObject[i] != 0)
		{
			deleteRoadObject(i);
		}
	}
	delete Hayato;

	return score;
}

bool DrivingInGame::isCollision(void)
{
	unsigned int drDriverVPos = Hayato->GetVPosition();
	unsigned int drDriverHPos = Hayato->GetHPosition();
	bool collision = false;

	for (unsigned int i = 0; i < MAX_OBSTACLE_COUNT; i++)
	{
		if (pRoadObject[i] != 0)
		{
			if (pRoadObject[i]->DoIBlockDrDriver(drDriverVPos, drDriverHPos) == true)
			{
				switch(pRoadObject[i]->GetObjType())
				{
				case OBSTACLE:
				case ROAD_CAR:
					collision = true;
					break;
				default:
					break;
				}

				if (collision == true)
				{
					break;
				}
			}
		}
	}
	return collision;
}

void DrivingInGame::generateObstacle(void)
{
	unsigned int hPosSum = 0;
	int pVacuumRoadObjSlot = -1;

	for (unsigned int i = 0; i < MAX_OBSTACLE_COUNT; i++)
	{
		if (pRoadObject[i] != 0)
		{
			hPosSum += pRoadObject[i]->GetHPosition();
		}
		else
		{
			pVacuumRoadObjSlot = i;
		}
	}

	if ((hPosSum < 12) && (pVacuumRoadObjSlot >= 0))
	{
		//
		// Magic number to start considering generating obstacle
		//
		int r = rand();
		if (r % 17 == 0)
		{
				pRoadObject[pVacuumRoadObjSlot] = new obstacle((r % 2)*2, gameTimer.read_ms());
		}
	}
}

void DrivingInGame::updateObstaclePosition(unsigned int currentTime)
{
	for (unsigned int i = 0; i < MAX_OBSTACLE_COUNT; i++)
	{
		if (pRoadObject[i] != 0)
		{
			//pc.printf("Updating position for obj %d!\r\n", i);
			if(pRoadObject[i]->UpdatePosition(3, currentTime) == true)
			{
				deleteRoadObject(i);
			}

		}
	}
}

void DrivingInGame::updateDrDriverPosition(void)
{
	//
	// (WIP) Read accelerometer to update horizontal movement and speed (accel / decel)
	//

}

void DrivingInGame::refreshLcd(void)
{
	char line[2][17];
	strcpy((char*)line[0], "                ");
	strcpy((char*)line[1], "                ");

	//
	// Print Dr Driver
	//
	Hayato->PrintOnLcd(line);

	//
	// Print road objects
	//
	for (unsigned int i = 0; i < MAX_OBSTACLE_COUNT; i++)
	{
		if (pRoadObject[i] != 0)
		{
			pRoadObject[i]->PrintOnLcd(line);
		}
	}
	lcd.cls();
	lcd.printf(line[0]);
	lcd.locate(0,1);
	lcd.printf(line[1]);
}

void DrivingInGame::deleteRoadObject(unsigned int i)
{
	//
	// delete road object
	//
	delete pRoadObject[i];
	pRoadObject[i] = 0;
}


