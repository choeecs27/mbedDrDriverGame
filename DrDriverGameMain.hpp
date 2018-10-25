/*
 * DrDriverGameMain.h
 *
 *  Created on: Oct 6, 2018
 *      Author: choeecs27
 */

#ifndef DRDRIVERGAMEMAIN_HPP_
#define DRDRIVERGAMEMAIN_HPP_

#include "ScoreBoard.hpp"

class DrDriverGameMain {
public:
	DrDriverGameMain(void){}
	virtual ~DrDriverGameMain(void){}

	void MainStateMachine(void);
	void Init(void);

private:
	enum eGameState
	{
		ST_INIT,
		ST_WELCOME,
		ST_TUTORIAL,
		ST_GAME_WAIT,
		ST_IN_GAME,
		ST_GAMEOVER,
		ST_ENTERNAME,
		ST_END,
	};

	typedef struct
	{
		char nameStr[4] = {};
		char tempChar;
		int score;
	}sEnterNameVars;

	static const int NUM_CUSTOM_CHARS = 5;
	unsigned int customChars[NUM_CUSTOM_CHARS][8] = {
			{0,0,0,0,0x12, 0x1F, 0x1F, 0x12},           // 0x01 Car low
			{0x12, 0x1F, 0x1F, 0x12, 0,0,0,0},          // 0x02 Car high
			{0x12, 0x1F, 0x1F, 0x12,0x12, 0x1F, 0x1F, 0x12}, // 0x03 two Cars
			{0, 0x04, 0x1F, 0x0E, 0x0E, 0x0A, 0x11, 0}, // 0x04 Star
			{0, 0x0A, 0x1F, 0x1F, 0x1F, 0x0E, 0x04, 0}  // 0x05Heart
	};

	Timer tm1;
	int subState;
	sEnterNameVars sEnterName;
	ScoreBoard* pSB;

	void programCustomChars(void);
	void displayStringsOnLcd(const char *sLine1, const char *sLine2);
	void stWelcome(void);
	unsigned int stGameWait(void);
	bool stEnterName(void);
};

#endif /* DRDRIVERGAMEMAIN_HPP_ */
