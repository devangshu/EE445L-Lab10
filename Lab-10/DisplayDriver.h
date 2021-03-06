/*
 * DisplayDriver.h
 *
 *  Created on: Nov 11, 2021
 *      Author: pulkitmahajan
 */

#ifndef DISPLAYDRIVER_H_
#define DISPLAYDRIVER_H_

#include "ST7735Lab4.h"
#include "MotorDriver.h"
#include "TachDriver.h"
#include "Timer4A.h"

#define MAX_RPS 100

//This will just call ST7735_Init()
void DisplayInit(void);

//Display graph
void PlotCustomGraph(void);



#endif /* DISPLAYDRIVER_H_ */
