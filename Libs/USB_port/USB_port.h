
#ifndef USB_PORT_H
#define USB_PORT_H

#include "stdint.h"

//Buffers len
#define USBP_RX_BUF_LEN  50
#define USBP_TX_BUF_LEN  50



//USB modes
typedef enum{
  USBP_MODE_CMD = 0,
  USBP_MODE_SCRIPT
}USBP_mode_t;  

//Returned values
enum{
  USBP_RET_OK = 1,
  USBP_RET_ERROR,
  USBP_RET_ADDR_ERR,
  USBP_RET_OVERF,
  USBP_RET_NANS,
  USBP_RET_NEXIST
};

extern uint8_t USBP_state;
extern USBP_mode_t USBP_mode; //USB modes

//Отправить данные
uint16_t USBP_Send(uint8_t* data, uint16_t len);
//Получить данные
uint16_t USBP_Recive(uint8_t* data, uint16_t len);

#endif