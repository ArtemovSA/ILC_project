
#ifndef USB_PORT_H
#define USB_PORT_H

#include "stdint.h"

//Buffers len
#define USBP_RX_BUF_LEN  64
#define USBP_TX_BUF_LEN  64

//USB modes
typedef enum{
  USBP_MODE_CMD = 0,
  USBP_MODE_SCRIPT
}USBP_mode_t;  

//USBP mode
extern volatile USBP_mode_t USBP_mode;

//init
void USBP_init();
//Отправить данные
uint16_t USBP_Send(uint8_t* data, uint16_t len);
//Получить данные
uint16_t USBP_Recive(uint8_t* data, uint16_t len);

#endif