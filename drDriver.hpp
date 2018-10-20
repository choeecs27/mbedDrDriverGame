/*
 * drDriver.hpp
 *
 *  Created on: Oct 7, 2018
 *      Author: choeecs27
 */

#ifndef DRDRIVER_HPP_
#define DRDRIVER_HPP_

#include "roadObject.hpp"

class drDriver: public roadCar {
public:
	drDriver();
	virtual ~drDriver() {};
	bool DoIBlockDrDriver(unsigned int vPos, unsigned int hPos) { return false; }
	bool UpdatePosition(float extSpeed, unsigned int currentTime);
	bool AdjustVPosition(int step);

private:
	static const int DR_DRIVER_HPOSITION = 2;
};

#endif /* DRDRIVER_HPP_ */
