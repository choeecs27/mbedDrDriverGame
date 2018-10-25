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
#include "TextLCD.h"
#include "AudioOut.hpp"
#include "halAccelerometerCtrl.hpp"

extern TextLCD lcd;
extern Serial pc;

/*const unsigned int SCRIPT1_LENGTH = 32;
const float script1[SCRIPT1_LENGTH] = {
		C4, C4, C4, D4, E4, E4, C4, C4,
		D4, D4, E4, E4, C4, C4, C4, C4,
		D4, D4, D4, E4, G4, G4, E4, E4,
		D4, D4, E4, E4, G4, G4, G4, G4};*/

const unsigned int SCRIPT2_LENGTH = 64;
const float script2[SCRIPT2_LENGTH] = {
		C5, C5, D5, E5, C5, E5, D5, G4,
		C5, C5, D5, E5, C5, C5, B4, B4,
		C5, C5, D5, E5, F5, E5, D5, C5,
		B4, G4, A4, B4, C5, C5, C5, C5,
		A4, B4, A4, G4, A4, B4, C5, C5,
		G4, A4, G4, F4, E4, F4, G4, G4,
		A4, B4, A4, G4, A4, B4, C5, A4,
		G4, C5, B4, D5, C5, C5, C5, C5,
};
/*
const unsigned int SCRIPT3_LENGTH = 8;
const float script3[SCRIPT3_LENGTH] = {
		A4, F5, F5, F5, F5, E5, D5, C5,
};*/

void DrDriverGameMain::Init(void)
{
	programCustomChars();
	pSB = ScoreBoard::GetInstance();
}

void DrDriverGameMain::MainStateMachine(void)
{
	AudioOut* pAudio = AudioOut::GetInstance();
	unsigned int state = ST_INIT;

	tm1.reset();
	tm1.start();

	subState = 0;

	while(1)
	{
		switch(state)
		{
			case ST_INIT:
			{
				pAudio->SetScriptPointer(script2, SCRIPT2_LENGTH);
				pAudio->SetPlayMode(PLAY_LOOP);
				pAudio->SetPlayTempo(280);
				pAudio->StartMusic();
				displayStringsOnLcd("Dr. Driver!!!",
								"Tap to Start ...");
				state = ST_WELCOME;
				break;
			}
			case ST_WELCOME:
			{
				stWelcome();

				if (CheckTapOccurred() == true)
				{
					state = ST_TUTORIAL;
					subState = 0;
				}
				break;
			}
			case ST_TUTORIAL:
			{
				/*pAudio->StopMusic();
				pAudio->SetPlayTempo(200);
				pAudio->SetScriptPointer(script2, SCRIPT2_LENGTH);
				pAudio->SetPlayMode(PLAY_LOOP);
				pAudio->StartMusic();*/
				displayStringsOnLcd("Tilt UP/DOWN",
									"to change lane");
				wait_ms(2000);
				state = ST_GAME_WAIT;
				break;
			}
			case ST_GAME_WAIT:
			{
				state = stGameWait();
				break;
			}
			case ST_IN_GAME:
			{
				DrivingInGame gameOn;
				sEnterName.score = (int)(gameOn.Start()/1000);
				state = ST_GAMEOVER;
				break;
			}

			case ST_GAMEOVER:
			{
				/*pAudio->StopMusic();
				pAudio->SetScriptPointer(script3, SCRIPT3_LENGTH);
				pAudio->SetPlayMode(PLAY_ONCE);
				pAudio->SetPlayTempo(280);
				pAudio->StartMusic();*/

				char dispScore[17] = {};
				sprintf(dispScore, "Score:%10d", sEnterName.score);
				displayStringsOnLcd("Game Over...",
									dispScore);
				wait_ms(2000);

				if(pSB->IsScoreRanked(sEnterName.score) > 0)
				{
					state = ST_ENTERNAME;
				}
				else
				{
					state = ST_END;
				}
				break;
			}

			case ST_ENTERNAME:
			{
				if (stEnterName() == true)
					state = ST_END;
				pc.printf("State: %d!\r\n", state);
				break;
			}
			case ST_END:
			{
				tm1.reset();
				return;
			}
		}
	}
}

void DrDriverGameMain::stWelcome(void)
{
	char line1[17] = {};

	if (tm1.read_ms()>=2000)
	{
		if (subState >= 5)
			subState = 0;
		else
			subState += 1;
		tm1.reset();

		switch(subState)
		{
			case 0:
				sprintf(line1, "Dr. Driver!!!");
				break;

			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			{
				if (pSB != 0)
				{
					char* namePtr;
					int score = pSB->GetScoreRank(subState-1, &namePtr);
					sprintf(line1, "#%d %s %9d", subState, namePtr, score);

				}
				break;
			}
			default:
				break;
		}

		displayStringsOnLcd(line1, "Tap to Start ...");
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
		wait_ms(1000);
	}
	displayStringsOnLcd("Gaming!!",
						"");
	wait_ms(1000);

	return ST_IN_GAME;
}

bool DrDriverGameMain::stEnterName(void)
{
	bool ret = false;

	if (subState == 0)
	{
		subState = 1;
		sEnterName.tempChar = 'A';
		memcpy(sEnterName.nameStr, "   ", sizeof(sEnterName.nameStr));

		char line1[17] = {};
		sprintf(line1, "Rank %d: %8d", pSB->IsScoreRanked(sEnterName.score), sEnterName.score);
		lcd.cls();
		lcd.printf(line1);
		lcd.locate(0,1);
		lcd.printf("Name:  ");
		lcd.locate(8,1);
	}

	if (subState % 2 == 1)
	{
		lcd.printf("%c", sEnterName.tempChar);
		subState++;
	}
	else
	{
		lcd.printf(" ");
		subState--;
		sEnterName.tempChar += CheckTiltDirection();
	}
	lcd.locate(8+((subState-1) / 2),1);
	if (sEnterName.tempChar < 'A')
		sEnterName.tempChar = 'Z';
	else if (sEnterName.tempChar > 'Z')
		sEnterName.tempChar = 'A';

	if (CheckTapOccurred() == true)
	{
		if (subState < 7)
		{
			sEnterName.nameStr[(subState-1) / 2] = sEnterName.tempChar;
			lcd.printf("%c", sEnterName.tempChar);
			subState += 2;
		}

		if (subState >=7)
		{
			pSB->SetScoreRank(sEnterName.score, sEnterName.nameStr);
			lcd.printf(" ");
			lcd.printf("%c%c%c%c", 4,5,4,5);
			wait_ms(2000);

			ret = true;
		}
	}
	pc.printf("Substate: %d!\r\n", subState);
	wait_ms(125);

	return ret;
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
