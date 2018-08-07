
#include "USB_ctrl.h"
#include "cmsis_os.h"
#include "Memory.h"
#include "deviceDefs.h"
#include "crc16.h"
#include "string.h"
#include "USB_port.h"
#include "DevCTRL.h"

//Variables
uint8_t USBC_cmd_buf[USBC_CMD_BUF_LEN]; //CMD buffer
uint8_t USBC_cmd_len; //Длина команды
uint8_t USBC_state = USBC_state_WAIT_STOP1; //state

//Semaphores
extern xSemaphoreHandle muxNAND;
//Task handle
xTaskHandle USBC_handle;

//Command process
void USBC_cmd_proc(uint8_t* cmdData, uint16_t cmdLen);
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
      USBC_cmd_proc(USBC_cmd_buf, USBC_cmd_len); //Process command
      USBC_state = USBC_state_WAIT_STOP1;
      //Zero cmd buffer
      memset(USBC_cmd_buf, 0, USBC_CMD_BUF_LEN);
      USBC_cmd_len = 0;
    }
    
    vTaskDelay(100);
  }
}
//--------------------------------------------------------------------------------------------------
//Command process
void USBC_cmd_proc(uint8_t* cmdData, uint16_t cmdLen)
{
  uint16_t crc_val; //Значение CRC
  uint16_t block; //Адрес блока
  uint16_t page; //Адрес страницы
  uint16_t offset; //Смещение
  uint16_t len; //Длина
  uint8_t command; //Command current
  NAND_AddressTypeDef addrNAND; //Nand address
  
  //Calc CRC16
  crc_val = crc16(cmdData, cmdLen-2);
  
  //If CRC OK
  if (crc_val == 0x0000) {
    
    command = cmdData[0]; //Get cmd
    
    switch(command) {
      
      //Debug
    case USBC_CMD_DEBUG:
      DC_debugOut((char*)&cmdData[1]);
      break;
      
      //Write
    case USBC_CMD_FLASH_WRITE:
      
      block = (cmdData[1]<<8) | cmdData[2];
      page = (cmdData[3]<<8) | cmdData[4];
      offset = (cmdData[5]<<8) | cmdData[6];
      len = (cmdData[7]<<8) | cmdData[8];
      
      if ( xSemaphoreTake(muxNAND, 100) == pdTRUE ) {
        
        addrNAND.Plane = 0;
        addrNAND.Block = block;
        addrNAND.Page = page;
        
        //Try write
        if (MEM_NAND_writeData(addrNAND, offset, &cmdData[9], len) == DEV_OK)
        {
          cmdData[0] = command; //Команда
          cmdData[1] = USBC_RET_OK;
        }else{
          cmdData[0] = command; //Команда
          cmdData[1] = USBC_RET_ERROR; 
        }
        
        xSemaphoreGive(muxNAND);
        
        //Расчет CRC
        crc_val = crc16(cmdData, 2);
        cmdData[2] = (crc_val & 0x00FF);//CRC16
        cmdData[3] = (crc_val & 0xFF00) >> 8; //CRC16
        
        cmdData[4] = USBC_STOP1_BYTE; //Стоповый байт
        cmdData[5] = USBC_STOP2_BYTE; //Стоповый байт        
        
        USBP_Send(cmdData,6); //Отправить ответ
        
      }
      break;
      
      //Read
    case USBC_CMD_FLASH_READ:
      
      block = (cmdData[1]<<8) | cmdData[2];
      page = (cmdData[3]<<8) | cmdData[4];
      offset = (cmdData[5]<<8) | cmdData[6];
      len = (cmdData[7]<<8) | cmdData[8];
      
      //Try read
      if (MEM_NAND_readData(addrNAND, offset, &cmdData[2], len) == DEV_OK)
      {
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_OK;
      }else{
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_ERROR; 
      }
      
      xSemaphoreGive(muxNAND);
      
      //Расчет CRC
      crc_val = crc16(cmdData, len+2);
      cmdData[len+2] = (crc_val & 0x00FF);//CRC16
      cmdData[len+3] = (crc_val & 0xFF00) >> 8; //CRC16
      
      cmdData[len+4] = USBC_STOP1_BYTE; //Стоповый байт
      cmdData[len+5] = USBC_STOP2_BYTE; //Стоповый байт        
      
      USBP_Send(cmdData,len+6); //Отправить ответ
      
      break;
      
      //Change mode
    case USBC_CMD_CHANGE_MODE:
            
      USBP_mode = cmdData[1]; //Change mode
      
      cmdData[0] = command; //Команда
      cmdData[1] = USBC_RET_OK;
      
      crc_val = crc16(cmdData, 2);
      cmdData[2] = (crc_val & 0x00FF);//CRC16
      cmdData[3] = (crc_val & 0xFF00) >> 8; //CRC16
      
      cmdData[4] = USBC_STOP1_BYTE; //Стоповый байт
      cmdData[5] = USBC_STOP2_BYTE; //Стоповый байт   
      
      break;
        
    }
  }else{
    
  }
}
//--------------------------------------------------------------------------------------------------
//Recive commnd process
void USBC_Receive_proc(uint8_t *data, uint16_t len)
{
  //Check current state
  if (USBC_state != USBC_state_MSG_PROCESS)
  {
    //Overflow protection
    if (USBC_cmd_len + len > USBC_CMD_BUF_LEN)
      USBC_cmd_len = 0;
    
    //Command buffer copy
    memcpy(&USBC_cmd_buf[USBC_cmd_len], data, len);
    USBC_cmd_len += len;
    
    //Check end stop bytes in command
    for (int i=0; i<len; i++)
    {
      if (USBC_state == USBC_state_WAIT_STOP2)
        if (data[i] == USBC_STOP2_BYTE)
        {
          USBC_state = USBC_state_MSG_PROCESS;
        }else{
          USBC_state = USBC_state_WAIT_STOP1;
        }
      
      if (USBC_state == USBC_state_WAIT_STOP1)
        if (data[i] == USBC_STOP1_BYTE)
        {
          USBC_state = USBC_state_WAIT_STOP2;
        }else{
          USBC_state = USBC_state_WAIT_STOP1;
        }
    }
  }
}