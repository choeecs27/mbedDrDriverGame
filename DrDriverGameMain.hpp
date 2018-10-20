/*
 * DrDriverGameMain.h
 *
 *  Created on: Oct 6, 2018
 *      Author: choeecs27
 */

#ifndef DRDRIVERGAMEMAIN_HPP_
#define DRDRIVERGAMEMAIN_HPP_

class DrDriverGameMain {
public:
	DrDriverGameMain(void){}
	virtual ~DrDriverGameMain(void){}

	void MainStateMachine(void);

private:
	enum eGameState
	{
		ST_WELCOME,
		ST_TUTORIAL,
		ST_CALIBRATION,
		ST_GAME_WAIT,
		ST_IN_GAME,
		ST_GAMEOVER,
	};

	static const int NUM_CUSTOM_CHARS = 5;
	unsigned int customChars[NUM_CUSTOM_CHARS][8] = {
			{0,0,0,0,0x12, 0x1F, 0x1F, 0x12},           // 0x01 Car low
			{0x12, 0x1F, 0x1F, 0x12, 0,0,0,0},          // 0x02 Car high
			{0x12, 0x1F, 0x1F, 0x12,0x12, 0x1F, 0x1F, 0x12}, // 0x03 two Cars
			{0, 0x04, 0x1F, 0x0E, 0x0E, 0x0A, 0x11, 0}, // 0x04 Star
			{0, 0x0A, 0x1F, 0x1F, 0x1F, 0x0E, 0x04, 0}  // 0x05Heart
	};

	void programCustomChars(void);
	void displayStringsOnLcd(const char *sLine1, const char *sLine2);
	unsigned int stGameWait(void);
};

#endif /* DRDRIVERGAMEMAIN_HPP_ */
