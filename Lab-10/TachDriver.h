/*
 * TachDriver.h
 *
 *  Created on: Nov 11, 2021
 *      Author: pulkitmahajan
 */

#ifndef TACHDRIVER_H_
#define TACHDRIVER_H_

#include <stdio.h>

void InitTachDriver();

//1.    Calculate error as the difference between actual and desired speed
void CalcError();

void ReadSpeed();





#endif /* TACHDRIVER_H_ */
