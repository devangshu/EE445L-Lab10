#ifndef _ESPDRIVER_H_
#define _ESPDRIVER_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "../inc/ADCSWTrigger.h"
#include "UARTLab4.h"
#include "esp8266Lab4.h"
#include "tm4c123gh6pm.h"
#include "ST7735Lab4.h"

#include "Timer2.h"
#include "Timer3.h"

#define DEBUG1

void ESP_Init(void);

void ESP_Request_Time(void);

#endif
