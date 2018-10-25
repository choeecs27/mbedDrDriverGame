/*
 * halShockInterrupt.hpp
 *
 *  Created on: Oct 7, 2018
 *      Author: choeecs27
 */

#ifndef HALACCELEROMETERCTRL_HPP_
#define HALACCELEROMETERCTRL_HPP_

void ISRShockIntIn(void);
bool CheckTapOccurred(void);
void ShockIntInit(void);
int CheckTiltDirection(void);

#endif /* HALACCELEROMETERCTRL_HPP_ */
