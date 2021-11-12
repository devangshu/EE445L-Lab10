/*
 * MotorDriver.h
 *
 *  Created on: Nov 11, 2021
 *      Author: pulkitmahajan
 */

#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_

#include <stdio.h>

void InitMotor(void);

void InitMotorTimer(long period );

void MotorTimerHandler(void);

void CalculatePWM(void);

void SetPWM(void);



#endif /* MOTORDRIVER_H_ */
