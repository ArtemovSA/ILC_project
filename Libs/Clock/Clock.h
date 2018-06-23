
#ifndef CLOCK_H
#define CLOCK_H

#include "stm32f4xx_hal.h"
#include <time.h>

//#define CL_USE_NTP

//Init clock
HAL_StatusTypeDef CL_init();
//Get system timestamp
HAL_StatusTypeDef CL_getSystem_Timestamp(time_t *timestamp);

#endif