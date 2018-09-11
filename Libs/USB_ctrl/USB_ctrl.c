
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
uint16_t USBC_cmd_len; //Длина команды
uint8_t USBC_state = USBC_state_WAIT_STOP1; //state

//Semaphores
extern xSemaphoreHandle muxNAND;
extern xSemaphoreHandle muxSRAM1;
extern xSemaphoreHandle muxSRAM2;
//Task handle
xTaskHandle USBC_handle;
extern xTaskHandle script_handle; //Хендл скрипта

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

    vTaskSuspend( USBC_handle );
    //vTaskDelay(100);
  }
}
//--------------------------------------------------------------------------------------------------
//Send payload
void USBC_sendPayload(uint8_t* payload, uint16_t len)
{
  uint16_t crc_val = 0; //Значение CRC
  //Расчет CRC
  crc16calc(&crc_val, payload, len);
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
  uint16_t offset; //Смещение
  uint16_t len; //Длина
  uint8_t command; //Command current
  uint8_t status; //Command status
  NAND_AddressTypeDef addrNAND; //Nand address
  DEV_Status_t retStatus;
  
  uint16_t partNum;
  uint16_t partLen;
  
  //Calc CRC16
  crc_val = 0;
  crc16calc(&crc_val, cmdData, cmdLen-2);
  
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
      
      status = cmdData[1];
      
      //Start status pack have scrypt information
      if (status == USBC_STAT_EXEC_START)
      {
        DC_set.PY_scryptData.len = ADD(cmdData[2], cmdData[3]);
        DC_set.PY_scryptData.crc = ADD(cmdData[4], cmdData[5]);
        memcpy(DC_set.PY_scryptData.Name, &cmdData[6], 20);
        retStatus = DEV_OK;
       
      }else{
        
        partNum = ADD(cmdData[2], cmdData[3]);
        partLen = ADD(cmdData[4], cmdData[5]);
        
        if ( xSemaphoreTake(muxSRAM1, 100) == pdTRUE ) {
          
          retStatus = MEM_SRAM_writeData(MEM_ID_SRAM1, MEM_SRAM1_SCRYPT_LOAD_ADDR+partNum*USBC_LOAD_PART_LEN , &cmdData[6], partLen);
          xSemaphoreGive(muxSRAM1);
        }
        
        //Return continue command
        if(status == USBC_STAT_EXEC_END) //End command
        {
          addrNAND = MEM_NAND_ADDR_VM_DATA;
          
          if ( xSemaphoreTake(muxNAND, 100) == pdTRUE ) {
            MEM_NAND_eraceBlock(addrNAND);//Erace Block NAND
            retStatus = MEM_SRAM_to_NAND(MEM_ID_SRAM1, MEM_SRAM1_SCRYPT_LOAD_ADDR, addrNAND, partNum*USBC_LOAD_PART_LEN + partLen);//Copy from SRAM to NAND
            
            //Check CRC
            if (retStatus == DEV_OK)
              retStatus = MEM_NAND_checkCRC16(addrNAND, DC_set.PY_scryptData.crc, DC_set.PY_scryptData.len); //Check CRC16 in NAND

            xSemaphoreGive(muxNAND);
            
          }
        }
      }
      
      if (retStatus == DEV_OK)
      {
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_OK;
      }else{
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_ERROR; 
      }
      
      USBC_sendPayload(cmdData, 2);//Send payload
      
      break;
      
      //Start Scrypt
    case USBC_CMD_SCRYPT_START:
      
      //Старт скрипта
      if (PY_startScript() == DEV_OK)
      {
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_OK;
        cmdData[2] = (uint8_t)DC_set.PY_scryptData.status;
      }else{
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_ERROR;
        cmdData[2] = (uint8_t)DC_set.PY_scryptData.status;
      }
      
      USBC_sendPayload(cmdData, 3);//Send payload
      
      break;
      
    case USBC_CMD_SCRYPT_STOP:
      
      //Стоп скрипта
      if (PY_stopScript() == DEV_OK)
      {
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_OK;
        cmdData[2] = (uint8_t)DC_set.PY_scryptData.status;
      }else{
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_ERROR;
        cmdData[2] = (uint8_t)DC_set.PY_scryptData.status;
      }
      
      USBC_sendPayload(cmdData, 3);//Send payload
            
      break;
      
    case USBC_CMD_SCRYPT_PAUSE:
      
      //Пауза скрипта
      if (PY_pauseScript() == DEV_OK)
      {
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_OK;
        cmdData[2] = (uint8_t)DC_set.PY_scryptData.status;
      }else{
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_ERROR;
        cmdData[2] = (uint8_t)DC_set.PY_scryptData.status;
      }
      
      USBC_sendPayload(cmdData, 3);//Send payload
            
      break;
      
      //Установка настроек
    case USBC_CMD_SET_SETTINGS:

      //Set setting parametr
      if (DC_setSetParam((DC_settingID_t)cmdData[1], &cmdData[3], cmdData[2]) == DEV_OK)
      {
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_OK;
      }else{
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_ERROR;
      }
      
      USBC_sendPayload(cmdData, 2);//Send payload
      
      break;
      
      //Читать настройки
    case USBC_CMD_GET_SETTINGS:
      
      if (DC_getSetParam((DC_settingID_t)cmdData[1], &cmdData[3], &cmdData[2]) == DEV_OK)
      {
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_OK;
        len = cmdData[2];
        USBC_sendPayload(cmdData, len+3);//Send payload
      }else{
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_ERROR;
        USBC_sendPayload(cmdData, 2);//Send payload
      }
        
      break;
      
      //Применить настройки
    case USBC_CMD_ASSIGN_SETTINGS:
      
      //Assign setting parametr
      if (DC_assignSettings() == DEV_OK)
      {
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_OK;
      }else{
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_ERROR;
      }
      
      USBC_sendPayload(cmdData, 2);//Send payload
      
      break;
      
    case USBC_CMD_DEFAULT_SETTINGS:
      {       
        //Reset setting key
        if (DC_setResetSettingKey() == DEV_OK)
        {
          cmdData[0] = command; //Команда
          cmdData[1] = USBC_RET_OK;
        }else{
          cmdData[0] = command; //Команда
          cmdData[1] = USBC_RET_ERROR;
        }
        
        USBC_sendPayload(cmdData, 2);//Send payload
      }
      break;
      
      //Сброс
    case USBC_CMD_SYSTEM_RESET:
      
      cmdData[0] = command; //Команда
      cmdData[1] = USBC_RET_OK;
      USBC_sendPayload(cmdData, 2);//Send payload
      
      //System reset
      DC_systemReset();      
      break;
      
      //Калибровка
    case USBC_CMD_SET_CALIBR:
      
      len = cmdData[2];
      
      //Set setting parametr
      if (DC_setCalParam(cmdData[3], (DC_calibrID_t)cmdData[1], &cmdData[4], cmdData[2]) == DEV_OK)
      {
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_OK;
      }else{
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_ERROR;
      }
      
      USBC_sendPayload(cmdData, 2);//Send payload
      
      break;

    case USBC_CMD_GET_CALIBR:
      
      if (DC_getCalParam(cmdData[3], (DC_calibrID_t)cmdData[1], &cmdData[4], &cmdData[2]) == DEV_OK)
      {
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_OK;
        len = cmdData[2];
        USBC_sendPayload(cmdData, len+3);//Send payload
      }else{
        cmdData[0] = command; //Команда
        cmdData[1] = USBC_RET_ERROR;
        USBC_sendPayload(cmdData, 2);//Send payload
      }
      
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
          vTaskResume( USBC_handle );
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