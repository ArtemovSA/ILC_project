
#ifndef NETW_H
#define NETW_H

#include "stm32f4xx_hal.h"

//Get ip from domen address
HAL_StatusTypeDef NW_getIP_byDomen(char* domen, uint8_t* ip);
//Convert int ip ti array
void NW_convIntIPtoArray(uint8_t* arrIP, uint32_t intIP);

#endif