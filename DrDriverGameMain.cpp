/*
 * DrDriverGameMain.cpp
 *
 *  Created on: Oct 6, 2018
 *      Author: choeecs27
 */
#include "mbed.h"
#include "DrDriverGameMain.hpp"
#include "drDriver.hpp"
#include "DrivingInGame.hpp"
#include "halShockInterrupt.hpp"
#include "TextLCD.h"
#include "AudioOut.hpp"

extern TextLCD lcd;

const unsigned int SCRIPT_LENGTH = 32;
const float script1[SCRIPT_LENGTH] = {C4, C4, C4, D4, E4, E4, C4, C4, D4, D4, E4, E4, C4, C4, C4, C4,
						  D4, D4, D4, E4, G4, G4, E4, E4, D4, D4, E4, E4, G4, G4, G4, G4};

void DrDriverGameMain::MainStateMachine(void)
{
	AudioOut* pAudio = AudioOut::GetInstance();
	unsigned int state = ST_WELCOME;
	bool firstTimeInThisState = true;

	programCustomChars();

	pAudio->SetScriptPointer(script1, SCRIPT_LENGTH);
	pAudio->SetPlayMode(PLAY_LOOP);
	pAudio->SetPlayTempo(280);
	pAudio->StartMusic();
	int score = 0;
	while(1)
	{

		switch(state)
		{
			case ST_WELCOME:

				if (firstTimeInThisState == true)
				{
					displayStringsOnLcd("Dr. Driver!!!",
										"Tap to Start ...");
					firstTimeInThisState = false;
				}
				if (CheckTapOccurred() == true)
				{
					state = ST_TUTORIAL;
					firstTimeInThisState = true;
				}
				break;

			case ST_TUTORIAL:
				displayStringsOnLcd("Tilt UP/DOWN",
									"to change lane");
				wait_ms(1000);
				state = ST_CALIBRATION;
				break;

			case ST_CALIBRATION:
				displayStringsOnLcd("Calibrating",
									"Current position");

				wait_ms(1000);
				state = ST_GAME_WAIT;
				break;

			case ST_GAME_WAIT:
				state = stGameWait();
				break;

			case ST_IN_GAME:
				score = 0;
				displayStringsOnLcd("Gaming!!",
									"");
				wait_ms(1000);

				{
					DrivingInGame gameOn;
					gameOn.SetDefaultYPos(128);

					score = (int)(gameOn.Start()/1000);

				}
				state = ST_GAMEOVER;
				break;

			case ST_GAMEOVER:
				char dispScore[17] = {};
				sprintf(dispScore, "Score:%10d", (int)(score));
				displayStringsOnLcd("Game Over...",
									dispScore);
				wait_ms(3000);
				return;
		}
	}
}

unsigned int DrDriverGameMain::stGameWait(void)
{
	for (int i = 3; i > 0; i--)
	{
		char disp1[17] = {};
		sprintf(disp1, "Start in: %d", i);
		displayStringsOnLcd(disp1,
									"");
		wait_ms(500);
	}

	return ST_IN_GAME;
}

void DrDriverGameMain::displayStringsOnLcd(const char *sLine1, const char *sLine2)
{
	lcd.cls();
	lcd.printf(sLine1);
	lcd.locate(0,1);
	lcd.printf(sLine2);
}

void DrDriverGameMain::programCustomChars(void)
{
	for (int i = 0; i < NUM_CUSTOM_CHARS; i++)
	{
		//
		// Purposely avoid char 0x00 to avoid confusion with null character
		//
		lcd.programCharacter(i+1, customChars[i]);
	}
}
