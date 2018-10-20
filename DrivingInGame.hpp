/*
 * DrivingInGame.hpp
 *
 *  Created on: Oct 7, 2018
 *      Author: choeecs27
 */

#ifndef DRIVINGINGAME_HPP_
#define DRIVINGINGAME_HPP_

#include "roadObject.hpp"

//
// Here is the definition of the game
//
// One car, speed 3-5 frames/sec
// Up to 3 objects on the road, speed 0-2.5 frames/sec
// 16Hx4V frames.  Refresh at 60 fps
//     4 vertical positions
//
class DrivingInGame {
public:
	DrivingInGame(void){}
	virtual ~DrivingInGame(void){}
	float Start(void);
	void SetDefaultYPos(int yPos) {defaultYPos = yPos;}

private:
	static const unsigned int MAX_OBSTACLE_COUNT = 3;
	static const unsigned int REFRESH_PERIOD = 20;

	int defaultYPos;

	void updateObstaclePosition(unsigned int currentTime);
	void updateDrDriverPosition(void);
	bool isCollision(void);
	void refreshLcd(void);
	void generateObstacle(void);
	void deleteRoadObject(unsigned int i);

	roadObject *pRoadObject[MAX_OBSTACLE_COUNT];
	drDriver *Hayato;
};

#endif /* DRIVINGINGAME_HPP_ */
