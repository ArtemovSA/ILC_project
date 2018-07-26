
#include "USB_port.h"
#include "cmsis_os.h"
#include "usbd_cdc_if.h"

//USBP modes
USBP_mode_t USBP_mode;

//Recive buffers
uint8_t USBP_rx_buf[USBP_RX_BUF_LEN];
uint8_t USBP_tx_buf[USBP_TX_BUF_LEN];
uint8_t USBP_rx_count = 0;

//Таймер
TimerHandle_t USBP_timer;

//Semaphores
extern xSemaphoreHandle muxUSBP;

//Recive runtime
static void USBP_runtime( TimerHandle_t xTimer );

//--------------------------------------------------------------------------------------------------
//init
void USBP_init()
{
  USBP_timer = xTimerCreate( "USBP_Timer", 1, pdTRUE, (void*)0, USBP_runtime );
}
//--------------------------------------------------------------------------------------------------
//Отправить данные
uint16_t USBP_Send(uint8_t* data, uint16_t len)
{
  return VCP_write(data, len);
}
//--------------------------------------------------------------------------------------------------
//Получить данные
uint16_t USBP_Recive(uint8_t* data, uint16_t len)
{
  return VCP_read(data, len);
}
//--------------------------------------------------------------------------------------------------
//Recive runtime
static void USBP_runtime( TimerHandle_t xTimer )
{      
  uint16_t len = USBP_Recive(USBP_rx_buf, 10);
  
  if (len > 0)
  {
    if (USBP_rx_count <= USBP_RX_BUF_LEN)
      USBP_rx_count = 0;
    
    USBP_rx_count += len;
    
    if (USB_mode == USB_MODE_CMD)
    {
      
    }
    
    if (USB_mode == USB_MODE_SCRIPT)
    {
      
    }
    
  }

}