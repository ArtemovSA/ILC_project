
#ifndef USB_CTRL_H
#define USB_CTRL_H

#include "stdint.h"

#define USBC_CMD_BUF_LEN        128
#define USBC_STOP1_BYTE         0x54
#define USBC_STOP2_BYTE         0x55

//USB states
enum{
  USBC_state_WAIT_STOP1 = 0,
  USBC_state_WAIT_STOP2,
  USBC_state_MSG_PROCESS,
};

//Commands
enum{
  USBC_CMD_DEBUG = 0,
  USBC_CMD_FLASH_WRITE,
  USBC_CMD_FLASH_READ,
  USBC_CMD_CHANGE_MODE
};

//Returned values
enum{
  USBC_RET_OK = 1,
  USBC_RET_ERROR,
  USBC_RET_ADDR_ERR,
  USBC_RET_OVERF,
  USBC_RET_NANS,
  USBC_RET_NEXIST
};

//Recive commnd process
void USBC_Receive_proc(uint8_t *data, uint16_t len);

#endif