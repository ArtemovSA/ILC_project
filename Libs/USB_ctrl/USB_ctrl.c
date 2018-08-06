
#include "USB_ctrl.h"
#include "cmsis_os.h"
#include "Memory.h"
#include "deviceDefs.h"
#include "crc16.h"
#include "string.h"
#include "USB_port.h"

uint8_t USB_cmd_buf[USBC_CMD_BUF_LEN]; //CMD buffer
uint8_t USB_cmd_len; //Длина команды
uint8_t USB_command;
uint8_t USBC_state = USBC_state_WAIT_STOP1;
const char USB_STOP_1 = 0x54;
const char USB_STOP_2 = 0x55;

//Semaphores
extern xSemaphoreHandle muxNAND;

xTaskHandle USBC_handle;

//Message process
void USBC_msg_proc();
//USB command task
void vUSBC_task(void *pvParameters);

//--------------------------------------------------------------------------------------------------
//USB command init
void USBC_init(uint8_t priority)
{
  xTaskCreate(vUSBC_task,(char*)"USBC_task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + priority, &USBC_handle);
}
//--------------------------------------------------------------------------------------------------
//USB command task
void vUSBC_task(void *pvParameters) 
{
  while(1)
  {
    if (USBC_state == USBC_state_MSG_PROCESS)
    {
      USBC_msg_proc();
      USBC_state = USBC_state_WAIT_STOP1;
      memset(USB_cmd_buf, 0, USBC_CMD_BUF_LEN);
      USB_cmd_len = 0;
    }
    
    vTaskDelay(100);
  }
}
//--------------------------------------------------------------------------------------------------
//Message process
void USBC_msg_proc()
{
  uint16_t crc_val; //Значение CRC
  uint16_t block; //Адрес блока
  uint16_t page; //Адрес страницы
  uint16_t offset; //Смещение
  uint16_t len; //Длина
  NAND_AddressTypeDef addrNAND;
  
  crc_val = crc16(USB_cmd_buf,USB_cmd_len-2);
  
  if (crc_val == 0x0000) {
    
    USB_command = USB_cmd_buf[0];
    
     switch(USB_command) {
     
     //Write array
     case USBC_CMD_FLASH_ARR_WRITE:
       block = (USB_cmd_buf[1]<<8) | USB_cmd_buf[2];
       page = (USB_cmd_buf[3]<<8) | USB_cmd_buf[4];
       offset = (USB_cmd_buf[5]<<8) | USB_cmd_buf[6];
       len = (USB_cmd_buf[7]<<8) | USB_cmd_buf[8];
       
       if ( xSemaphoreTake(muxNAND, 100) == pdTRUE ) {
         
         addrNAND.Plane = 0;
         addrNAND.Block = block;
         addrNAND.Page = page;
         
         if (MEM_NAND_writeData(addrNAND, offset, &USB_cmd_buf[7], len) == DEV_OK)
         {
           USB_cmd_buf[0] = USB_cmd_buf[0]; //Команда
           USB_cmd_buf[1] = USBP_RET_OK;
         }else{
           USB_cmd_buf[0] = USB_cmd_buf[0]; //Команда
           USB_cmd_buf[1] = USBP_RET_ERROR; 
         }
         
         xSemaphoreGive(muxNAND);
         
         //Расчет CRC
         crc_val = crc16(USB_cmd_buf,2);
         USB_cmd_buf[2] = (crc_val & 0x00FF);//CRC16
         USB_cmd_buf[3] = (crc_val & 0xFF00) >> 8; //CRC16
         
         USB_cmd_buf[4] = USB_STOP_1; //Стоповый байт
         USB_cmd_buf[5] = USB_STOP_2; //Стоповый байт        
         
         USBP_Send(USB_cmd_buf,6); //Отправить ответ
         
       }
       break;
       
     case USBC_CMD_FLASH_ARR_READ:
       
//      addr = (USB_cmd_buf[1]<<16) | (USB_cmd_buf[2]<<8) | USB_cmd_buf[3];
//      len = ADD(USB_cmd_buf[5],USB_cmd_buf[4]);
//      
//      memset(USB_tx_buf,0,sizeof(USB_tx_buf));
//      
//      if ( xSemaphoreTake(extFlash_mutex, 100) == pdTRUE ) {
//        EXT_Flash_readData(addr, &USB_tx_buf[6], len); //Читать данные из флеш
//        xSemaphoreGive(extFlash_mutex);
        
        //Формирование ответа
//        memcpy(USB_tx_buf, USB_cmd_buf, 5);
//        USB_tx_buf[0] = USB_cmd_buf[0]; //Команда
//        USB_tx_buf[1] = USB_cmd_buf[1]; //Адрес
//        USB_tx_buf[2] = USB_cmd_buf[2]; //Адрес
//        USB_tx_buf[3] = USB_cmd_buf[3]; //Адрес
//        USB_tx_buf[4] = USB_cmd_buf[4]; //Длина
//        USB_tx_buf[5] = USB_cmd_buf[5]; //Длина
//        
//        
//        //Расчет CRC
//        crc_val = crc16(USB_tx_buf,len+6);
//        USB_tx_buf[len+6] = (crc_val & 0x00FF); //CRC16
//        USB_tx_buf[len+7] = (crc_val & 0xFF00) >> 8; //CRC16
//        
//        USB_tx_buf[len+8] = USB_STOP_1; //Стоповый байт
//        USB_tx_buf[len+9] = USB_STOP_2; //Стоповый байт        
//      
//        USB_Send(USB_tx_buf,len+10); //Отправить ответ
//
//      }
      
      break;
     case USBC_CMD_FLASH_BYTE_WRITE:
//       
//      addr = (USB_tx_buf[1]<<16) | (USB_tx_buf[2]<<8) | USB_tx_buf[3];
//
//      if ( xSemaphoreTake(extFlash_mutex, 100) == pdTRUE ) {
//        EXT_Flash_ReWriteData(addr,&USB_cmd_buf[4], 1); //Записать данные   
//        xSemaphoreGive(extFlash_mutex);
        
        //Формирование ответа
//        memcpy(USB_tx_buf, USB_cmd_buf, 3);
//        USB_tx_buf[0] = USB_cmd_buf[0]; //Команда
//        USB_tx_buf[1] = USB_cmd_buf[1]; //Адрес
//        USB_tx_buf[2] = USB_cmd_buf[2]; //Адрес
//        USB_tx_buf[3] = USB_cmd_buf[3]; //Адрес
        
//        //Расчет CRC
//        crc_val = crc16(USB_tx_buf,4);
//        USB_tx_buf[4] = (crc_val & 0x00FF); //CRC16
//        USB_tx_buf[5] = (crc_val & 0xFF00) >> 8; //CRC16
//        
//        USB_tx_buf[6] = USB_STOP_1; //Стоповый байт
//        USB_tx_buf[7] = USB_STOP_2; //Стоповый байт        
//        
//        USB_Send(USB_tx_buf,8); //Отправить ответ
//      }
      
       break;
     case USBC_CMD_FLASH_BYTE_READ:
       
//      addr = (USB_cmd_buf[1]<<16) | (USB_cmd_buf[2]<<8) | USB_cmd_buf[3];
//
//      if ( xSemaphoreTake(extFlash_mutex, 100) == pdTRUE ) {
//        EXT_Flash_readData(addr, &USB_tx_buf[4], 1); //Читать данные из флеш
//        xSemaphoreGive(extFlash_mutex);
//        
//        //Формирование ответа
//        memcpy(USB_tx_buf, USB_cmd_buf, 3);
////        USB_tx_buf[0] = USB_cmd_buf[0]; //Команда
////        USB_tx_buf[1] = USB_cmd_buf[1]; //Адрес
////        USB_tx_buf[2] = USB_cmd_buf[2]; //Адрес
////        USB_tx_buf[3] = USB_cmd_buf[3]; //Адрес
//        
//        //Расчет CRC
//        crc_val = crc16(USB_tx_buf,5);
//        USB_tx_buf[5] = (crc_val & 0x00FF); //CRC16
//        USB_tx_buf[6] = (crc_val & 0xFF00) >> 8; //CRC16
//        
//        USB_tx_buf[7] = USB_STOP_1; //Стоповый байт
//        USB_tx_buf[8] = USB_STOP_2; //Стоповый байт        
//        
//        USB_Send(USB_tx_buf,9); //Отправить ответ
//      }
      
      break;
     }

  }else{
    
  }
}
//--------------------------------------------------------------------------------------------------
//Recive commnd process
void USBC_Receive_proc(uint8_t *data, uint16_t len)
{
  if (USB_cmd_len <= USBC_CMD_BUF_LEN)
    USB_cmd_len = 0;
  
  memcpy(&USB_cmd_buf[USB_cmd_len], data, len);
  USB_cmd_len += len;
  
  for (int i=0; i<len; i++)
  {
    if (USBC_state == USBC_state_WAIT_STOP2)
      if (data[i] == USB_STOP_2)
      {
        USBC_state = USBC_state_MSG_PROCESS;
      }else{
        USBC_state = USBC_state_WAIT_STOP1;
      }
    
    if (USBC_state == USBC_state_WAIT_STOP1)
      if (data[i] == USB_STOP_1)
      {
        USBC_state = USBC_state_WAIT_STOP2;
      }else{
        USBC_state = USBC_state_WAIT_STOP1;
      }
  }
}