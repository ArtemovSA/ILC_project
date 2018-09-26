
#ifndef CLOCK_H
#define CLOCK_H

#include "stm32f4xx_hal.h"
#include "deviceDefs.h"
#include <time.h>

#define CL_USE_NTP 1 //Enable NTP client
#define SNTP_SET_SYSTEM_TIME(sec) CL_setSystem_Timestamp(sec)

//Init clock
DEV_Status_t CL_init();
//Get system timestamp
void CL_setSystem_Timestamp(uint32_t sec);
//Set system timestamp
DEV_Status_t CL_getSystem_Timestamp(time_t *timestamp);
//Get format date time
DEV_Status_t CL_getFormat_DateTime(char* buf);
//Get dateTime
DEV_Status_t CL_getDateTime(uint8_t* year, uint8_t* month, uint8_t* date, uint8_t* hours, uint8_t* minutes, uint8_t* sec);

#endif