#ifndef DEVTCP_H
#define DEVTCP_H

#include "lwip.h"
#include "stm32f4xx_hal_def.h"

//Set TCP settings
HAL_StatusTypeDef devTCP_set(uint8_t* devIP, uint8_t* devMASK, uint8_t* devGATEWAY);

#endif