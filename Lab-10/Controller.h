/*
 * Controller.h
 *
 *  Created on: Nov 11, 2021
 *      Author: pulkitmahajan
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

//This module will drive all interactions relating to the Network (Blynk)


//Init ESP Chip
// Connect to Wifi
void InitController(void);

//Connect to Blynk
void ConnectToServer(void);

//Get Inputs to set PWM
void RecieveData(void);

//Send values to be displayed
void SendData(void);



#endif /* CONTROLLER_H_ */

