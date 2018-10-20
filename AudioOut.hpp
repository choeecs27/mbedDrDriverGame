/*
 * AudioOut.hpp
 *
 *  Created on: Oct 14, 2018
 *      Author: choeecs27
 */

#ifndef AUDIOOUT_HPP_
#define AUDIOOUT_HPP_

#define C4 261.626
#define D4 293.665
#define E4 329.668
#define F4 349.228
#define G4 391.995
#define A4 440
#define B4 493.883

enum{
	PLAY_ONCE,
	PLAY_LOOP,
	PLAY_LOOP_GAP
};

void isrPlayNote(void);

class AudioOut {
	public:
	~AudioOut(){ delete pInst; pInst = 0; }

	void SetScriptPointer(const float *pScript, unsigned int scriptLength);
	void StartMusic(void);
	void StopMusic(void);
	void SetPlayMode(unsigned int playMode) {this->playMode = playMode;}
	void SetPlayTempo(unsigned int bpm) {this->bpm = bpm;}
	float GetCurrentNote(void);
	unsigned int GetPlayMode(void){return playMode;}



	static AudioOut* GetInstance(void);

	static AudioOut* pInst;
private:

	AudioOut(){}

	const float *pScript;
	unsigned int scriptLength;
	unsigned int noteIndex;
	unsigned int playMode;
	unsigned int bpm;
	unsigned int pauseDowncounter;
	bool isPause;
};

#endif /* AUDIOOUT_HPP_ */
