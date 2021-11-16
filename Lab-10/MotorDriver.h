/*
 * MotorDriver.h
 *
 *  Created on: Nov 11, 2021
 *      Author: pulkitmahajan
 */

#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_

#include <stdio.h>
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/Timer1A.h"
#include "UARTLab4.h"
#include "DisplayDriver.h"

void InitMotor(uint16_t period, uint16_t duty);

void InitMotorTimer(long period);

void MotorTimerHandler(void);

void CalculatePWM(void);

void SetDuty(uint16_t duty);

void PI_Loop(void);


#endif /* MOTORDRIVER_H_ */
