/*
 * halShockInterrupt.hpp
 *
 *  Created on: Oct 7, 2018
 *      Author: choeecs27
 */

#ifndef HALSHOCKINTERRUPT_HPP_
#define HALSHOCKINTERRUPT_HPP_

void ISRShockIntIn(void);
bool CheckTapOccurred(void);
void ShockIntInit(void);

#endif /* HALSHOCKINTERRUPT_HPP_ */
