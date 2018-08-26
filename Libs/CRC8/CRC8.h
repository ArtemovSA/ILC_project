
#ifndef CRC8_H
#define CRC8_H

#include "stdint.h"

void crc8calc(uint8_t *crc, const uint8_t *addr, uint16_t len);

#endif