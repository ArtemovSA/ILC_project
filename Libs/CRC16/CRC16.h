
#ifndef CRC16_H
#define CRC16_H

#include "stdint.h"

void crc16calc(uint16_t *crc, const uint8_t *addr, uint16_t len);

#endif

