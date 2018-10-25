/*
 * ScoreBoard.hpp
 *
 *  Created on: Oct 20, 2018
 *      Author: choeecs27
 */

#ifndef SCOREBOARD_HPP_
#define SCOREBOARD_HPP_

typedef struct
{
	char name[4];
	int score;
} rankedPlayer;

typedef struct
{
	rankedPlayer rankedPlayers[5];
}sScoreBoard;

class ScoreBoard {
public:

	~ScoreBoard(){delete pInst; pInst = 0;}

	int IsScoreRanked(int score);
	void SetScoreRank(int score, const char name[4]);
	int GetScoreRank(int rank, char** namePtr);

	static ScoreBoard* GetInstance(void);
	static ScoreBoard* pInst;

	static const int PLAYER_CT = 5;

private:
	ScoreBoard();
	void init(void);

	sScoreBoard* pData;
};

#endif /* SCOREBOARD_HPP_ */
