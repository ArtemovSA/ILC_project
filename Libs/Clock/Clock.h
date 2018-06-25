
#ifndef CLOCK_H
#define CLOCK_H

#include "stm32f4xx_hal.h"
#include <time.h>

#define CL_USE_NTP 1 //Enable NTP client
#define SNTP_SET_SYSTEM_TIME(sec) CL_setSystem_Timestamp(sec)

//Init clock
HAL_StatusTypeDef CL_init();
//Get system timestamp
void CL_setSystem_Timestamp(uint32_t sec);
//Set system timestamp
HAL_StatusTypeDef CL_getSystem_Timestamp(time_t *timestamp);

#endif