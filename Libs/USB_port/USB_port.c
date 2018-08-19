
#include "USB_port.h"
#include "cmsis_os.h"
#include "usbd_cdc_if.h"
#include "USB_ctrl.h"

//USBP mode
volatile USBP_mode_t USBP_mode = USBP_MODE_CMD;

//Recive buffers
uint8_t USBP_rx_buf[USBP_RX_BUF_LEN];
uint8_t USBP_tx_buf[USBP_TX_BUF_LEN];

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
  xTimerStart( USBP_timer, 50);
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
  uint16_t len = USBP_Recive(USBP_rx_buf, USBP_RX_BUF_LEN);
  
  if (len > 0)
  {
    //Cmd mode
    if (USBP_mode == USBP_MODE_CMD)
    {
      USBC_Receive_proc(USBP_rx_buf, len);//Recive commnd process
    }
    
    //Scrypt mode
    if (USBP_mode == USBP_MODE_SCRIPT)
    {
      
    }
    
    //Debug mode
    if (USBP_mode == USBP_MODE_DEBUG)
    {
      
    }    
  }

}