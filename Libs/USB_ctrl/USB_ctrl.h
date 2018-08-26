
#ifndef USB_CTRL_H
#define USB_CTRL_H

#include "stdint.h"

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
  USBC_CMD_SET_SETTINGS,        //Set settings
  USBC_CMD_SET_CALIBRATE,       //Set CALIBRATE
};

//Command status
enum{
  USBC_STAT_EXEC_END = 0,       //End command
  USBC_STAT_EXEC_START,         //Start command
  USBC_STAT_EXEC_CONT           //Continue command
};

//Settings List
enum{
  USBC_SET_NET_DEV_IP = 1,
  USBC_SET_NET_GW_IP,
  USBC_SET_NET_MASK,
  USBC_SET_NTP_DOMEN,
  USBC_SET_NET_DNS_IP,
  USBC_SET_MQTT_IP,
  USBC_SET_MQTT_DOMEN,
  USBC_SET_MQTT_CH,
  USBC_SET_MQTT_PORT,
  USBC_SET_MQTT_USER,
  USBC_SET_MQTT_PASS,
  USBC_SET_MQTT_QOS,
  USBC_SET_EMS_PERIOD,
  USBC_SET_EMS_AUTO_SEND,
  USBC_SET_VM_AUTO_START
};

//Returned values
enum{
  USBC_RET_ERROR = 0,
  USBC_RET_OK,
  USBC_RET_ADDR_ERR,
  USBC_RET_OVERF,
  USBC_RET_NANS,
  USBC_RET_NEXIST
};

//USB command init
void USBC_init(uint8_t priority);
//Recive commnd process
void USBC_Receive_proc(uint8_t *data, uint16_t len);

#endif