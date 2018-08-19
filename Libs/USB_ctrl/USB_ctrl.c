
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
//Send payload
void USBC_sendPayload(uint8_t* payload, uint16_t len)
{
  uint16_t crc_val; //Значение CRC
  //Расчет CRC
  crc_val = crc16(payload, len);
  payload[len] = (crc_val & 0x00FF);//CRC16
  payload[len+1] = (crc_val & 0xFF00) >> 8; //CRC16
  
  payload[len+2] = USBC_STOP1_BYTE; //Стоповый байт
  payload[len+3] = USBC_STOP2_BYTE; //Стоповый байт        
  
  USBP_Send(payload,len+4); //Отправить ответ
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
      
      //Connection check
    case USBC_CMD_CONN_CHECK:
      cmdData[0] = command; //Команда
      cmdData[1] = USBC_RET_OK;
      
      //Send payload
      USBC_sendPayload(cmdData, 2);
      break;
      
      //Write
    case USBC_CMD_FLASH_WRITE:
      
      addrNAND.Block = (cmdData[1]<<8) | cmdData[2];
      addrNAND.Page = (cmdData[3]<<8) | cmdData[4];
      addrNAND.Plane = 0;
      offset = (cmdData[5]<<8) | cmdData[6];
      len = (cmdData[7]<<8) | cmdData[8];
      
      if ( xSemaphoreTake(muxNAND, 100) == pdTRUE ) {
        
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
        
      }else{
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_ERROR; 
      }
      
      USBC_sendPayload(cmdData, 2); //Send payload
      break;
      
      //Read
    case USBC_CMD_FLASH_READ:
      
      addrNAND.Block = (cmdData[1]<<8) | cmdData[2];
      addrNAND.Page = (cmdData[3]<<8) | cmdData[4];
      addrNAND.Plane = 0;
      offset = (cmdData[5]<<8) | cmdData[6];
      len = (cmdData[7]<<8) | cmdData[8];
      
      if ( xSemaphoreTake(muxNAND, 100) == pdTRUE ) {
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
        
        USBC_sendPayload(cmdData, len+2);//Send payload
        
      }else{
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_ERROR;
        
        USBC_sendPayload(cmdData, len+2);//Send payload
      }
      
      break;
      
      //Change mode
    case USBC_CMD_CHANGE_MODE:
      
      USBP_mode = (USBP_mode_t)cmdData[1]; //Change mode
      
      cmdData[0] = command; //Команда
      cmdData[1] = USBC_RET_OK;
      
      USBC_sendPayload(cmdData, 2);//Send payload
      
      break;
      
      //Load script
    case USBC_CMD_SCRYPT_LOAD:
      
      uint16_t partNum = ADD(cmdData[0], cmdData[1]);
      uint16_t partLen = ADD(cmdData[2], cmdData[3]);
      
      addrNAND.Block = (cmdData[1]<<8) | cmdData[2];
      addrNAND.Page = (cmdData[3]<<8) | cmdData[4];
      addrNAND.Plane = 0;
      
      USBC_LOAD_PART_LEN
      
      if ( xSemaphoreTake(muxNAND, 100) == pdTRUE ) {
        
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
      }
      
      USBC_sendPayload(cmdData, 2);//Send payload
      
      break;
      
      //Start Scrypt
    case USBC_CMD_SCRYPT_START:
      
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