
#ifndef USB_CTRL_H
#define USB_CTRL_H

#include "stdint.h"
#include "cmsis_os.h"

#define USBC_CMD_BUF_LEN        300
#define USBC_LOAD_PART_LEN      256
#define USBC_STOP1_BYTE         0x50
#define USBC_STOP2_BYTE         0x55

//USB states
enum{
  USBC_state_WAIT_STOP1 = 0,
  USBC_state_WAIT_STOP2,
  USBC_state_MSG_PROCESS,
};

//Commands
enum{
  USBC_CMD_DEBUG = 0,           //Debug command
  USBC_CMD_CONN_CHECK,          //Connection check
  USBC_CMD_FLASH_WRITE,         //Write data to NAND
  USBC_CMD_FLASH_READ,          //Read data from NAND
  USBC_CMD_CHANGE_MODE,         //Change mode
  USBC_CMD_SCRYPT_LOAD,         //Load script
  USBC_CMD_SCRYPT_START,        //Start script
  USBC_CMD_SCRYPT_STOP,         //Stop script
  USBC_CMD_SCRYPT_PAUSE,        //Pause script
  USBC_CMD_SET_SETTINGS,        //Set settings
  USBC_CMD_GET_SETTINGS,        //Get settings
  USBC_CMD_ASSIGN_SETTINGS,     //Assign settings
  USBC_CMD_DEFAULT_SETTINGS,    //Set default settings
  USBC_CMD_SYSTEM_RESET,        //Reset system
  USBC_CMD_SET_CALIBR,          //Set calibratings data
  USBC_CMD_GET_CALIBR,          //Get calibrate data
  USBC_CMD_ASSIGN_CALIBR,       //Assign Calibrate
  USBC_CMD_GET_VALUES           //Get values
};

//Command status
enum{
  USBC_STAT_EXEC_END = 0,       //End command
  USBC_STAT_EXEC_START,         //Start command
  USBC_STAT_EXEC_CONT           //Continue command
};

//Returned values
enum{
  USBC_RET_ERROR = 0,
  USBC_RET_OK,
  USBC_RET_ADDR_ERR,
  USBC_RET_OVERF,
  USBC_RET_NAVAL,
  USBC_RET_NANS,
  USBC_RET_NEXIST,
  USBC_RET_ALREADY
};

//USB command init
void USBC_init(osPriority priority);
//Recive commnd process
void USBC_Receive_proc(uint8_t *data, uint16_t len);

#endif