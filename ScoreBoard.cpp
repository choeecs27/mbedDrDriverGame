/*
 * ScoreBoard.cpp
 *
 *  Created on: Oct 20, 2018
 *      Author: choeecs27
 */
#include "mbed.h"
#include "ScoreBoard.hpp"
#include "FlashData.hpp"

ScoreBoard* ScoreBoard::pInst = 0;

ScoreBoard* ScoreBoard::GetInstance(void)
{
	if (pInst == 0)
		pInst = new ScoreBoard();
	return pInst;
}

ScoreBoard::ScoreBoard()
{
	init();
}

void ScoreBoard::init(void)
{
	//
	// Read from Flash
	//
	FlashData* pFlash = FlashData::GetInstance();
	pData = pFlash->GetScoreBoardData();

	if (pData == 0)
	{
		//
		// Scoreboard is empty, create a dummy scoreBoard
		//
		sScoreBoard highScore = {};

		for(int i=0; i < PLAYER_CT; i++)
		{
			highScore.rankedPlayers[i].score = 0;
			//highScore.rankedPlayers[i].name = "AAA";
			memcpy(highScore.rankedPlayers[i].name, "AAA", sizeof(char)*4);
		}

		pFlash->WriteScoreBoardData(&highScore);
	}
	pData = pFlash->GetScoreBoardData();
}

int ScoreBoard::IsScoreRanked(int score)
{
	int ret = 0;

	for(int i=0; i < PLAYER_CT; i++)
	{
		if(score > pData->rankedPlayers[i].score)
		{
			ret = i+1;
			break;
		}
	}

	return ret;
}

void ScoreBoard::SetScoreRank(int score, const char name[4])
{
	int curRanking = -1;

	if ((curRanking = ScoreBoard::IsScoreRanked(score)) > 0)
	{
		curRanking--;
		sScoreBoard scoreBoardForUpdate = {};


		for(int i = 0; i < PLAYER_CT; i++)
		{
			if (i < curRanking)
			{
				memcpy(&(scoreBoardForUpdate.rankedPlayers[i]), &(pData->rankedPlayers[i]), sizeof(rankedPlayer));
			}
			else if (i == curRanking)
			{
				scoreBoardForUpdate.rankedPlayers[i].score = score;
				memcpy(scoreBoardForUpdate.rankedPlayers[i].name, name, sizeof(char)*4);
			}
			else
			{
				memcpy(&(scoreBoardForUpdate.rankedPlayers[i]), &(pData->rankedPlayers[i-1]), sizeof(rankedPlayer));
			}
		}
		FlashData::GetInstance()->WriteScoreBoardData(&scoreBoardForUpdate);
	}
}

int ScoreBoard::GetScoreRank(int rank, char** namePtr)
{
	*namePtr = pData->rankedPlayers[rank].name;
	return pData->rankedPlayers[rank].score;
}


