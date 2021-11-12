/*
 * SwitchDriver.h
 *
 *  Created on: Nov 11, 2021
 *      Author: anuvgupta
 */


#ifndef _SWITCHDRIVER_H_
#define _SWITCHDRIVER_H_

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "CortexM.h"


/* constants */
#define NVIC_EN0_INT2           0x00000004  // Interrupt 2 enable
#define NVIC_EN0_INT4           0x00000010  // Interrupt 4 enable
#define PF3                     (*((volatile uint32_t *)0x40025020))
#define PF2                     (*((volatile uint32_t *)0x40025010))
#define PF1                     (*((volatile uint32_t *)0x40025008))
#define Pin0Mask 0x01
#define Pin1Mask 0x02
#define Pin2Mask 0x04
#define Pin3Mask 0x08
#define Pin4Mask 0x10
#define Pin5Mask 0x20
#define Pin6Mask 0x40
#define Pin7Mask 0x80


/* methods */

void Switches_Init(void);

void Switches_Loop(void);

void Switches_Handler(uint32_t id);



#endif
