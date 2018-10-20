/*
 * AudioOut.cpp
 *
 *  Created on: Oct 14, 2018
 *      Author: choeecs27
 */
#include "mbed.h"
#include "AudioOut.hpp"

AudioOut* AudioOut::pInst = 0;

PwmOut audio(p26);
Ticker tk1;

AudioOut* AudioOut::GetInstance(void)
{
	if (pInst == 0)
		pInst = new AudioOut();
	return pInst;
}

void AudioOut::StartMusic(void)
{
	isPause = false;
	noteIndex = 0;
	tk1.attach(&isrPlayNote, (float) 60 / bpm);
}

void AudioOut::StopMusic(void)
{
	audio.write(0);
	tk1.attach(0, 10.);
}

void AudioOut::SetScriptPointer(const float *pScript, unsigned int scriptLength)
{
	this->pScript = pScript;
	this->scriptLength = scriptLength;
}


float AudioOut::GetCurrentNote(void)
{
	float ret;
	if (isPause == true)
	{
		ret = 0.;
		pauseDowncounter--;
		if (pauseDowncounter == 0)
			isPause = false;
	}
	else
	{
		if(noteIndex < scriptLength)
		{
			ret = pScript[noteIndex];
			noteIndex++;

			if (noteIndex == scriptLength && GetPlayMode() == PLAY_LOOP)
			{
				noteIndex = 0;
			}
		}
		else
		{
			ret = 0.;
			//
			// Finished playing script once
			// Deciding what to do next
			//
			switch (GetPlayMode())
			{
				case PLAY_LOOP:
					//
					// Its not supposed to come here
					//
					StartMusic();
					break;
				case PLAY_LOOP_GAP:
					isPause = true;
					pauseDowncounter = bpm / 60;
				case PLAY_ONCE:
				default:
					StopMusic();
					break;
			}
		}
	}
	return ret;
}

void isrPlayNote(void)
{
	AudioOut* pAudio = AudioOut::GetInstance();
	float currentNote = pAudio->GetCurrentNote();

	if (currentNote != 0.)
	{
		//
		// Music playing
		//
		audio.period_us(1000000 / currentNote);
		audio.write(0.50);
	}
	else
	{
		audio.write(0);
	}
}
