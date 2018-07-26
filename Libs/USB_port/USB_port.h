
#ifndef USB_PORT_H
#define USB_PORT_H

#include "stdint.h"

//Buffers len
#define USBP_RX_BUF_LEN  50
#define USBP_TX_BUF_LEN  50


//USB modes
typedef enum{
  USB_MODE_CMD = 0,
  USB_MODE_SCRIPT
}USBP_mode_t;  

//Returned values
enum{
  USB_RET_OK = 1,
  USB_RET_ERROR,
  USB_RET_ADDR_ERR,
  USB_RET_OVERF,
  USB_RET_NANS,
  USB_RET_NEXIST
};

extern uint8_t USBP_state;
extern USBP_mode_t USB_mode; //USB modes


#endif