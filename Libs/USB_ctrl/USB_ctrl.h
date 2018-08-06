
#ifndef USB_CTRL_H
#define USB_CTRL_H

#include "stdint.h"

#define USBC_CMD_BUF_LEN  128

//USB states
enum{
  USBC_state_WAIT_STOP1 = 0,
  USBC_state_WAIT_STOP2,
  USBC_state_MSG_PROCESS,
};

//Commands
enum{
  USBC_CMD_FLASH_ARR_WRITE = 1,
  USBC_CMD_FLASH_BYTE_WRITE,
  USBC_CMD_FLASH_ARR_READ,
  USBC_CMD_FLASH_BYTE_READ
};
//Recive commnd process
void USBC_Receive_proc(uint8_t *data, uint16_t len);

#endif