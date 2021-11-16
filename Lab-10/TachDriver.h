/*
 * TachDriver.h
 *
 *  Created on: Nov 11, 2021
 *      Author: pulkitmahajan
 */

#ifndef TACHDRIVER_H_
#define TACHDRIVER_H_


#include <stdint.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"

void PeriodMeasure_Init(void);

void InitTachDriver(void);

//1.    Calculate error as the difference between actual and desired speed
void CalcError(void);

void ReadSpeed(void);





#endif /* TACHDRIVER_H_ */
