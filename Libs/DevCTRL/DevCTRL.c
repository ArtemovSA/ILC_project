#include "DevCTRL.h"
#include <stdarg.h>
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "Memory.h"
#include "FreeRTOS.h"
#include "task.h"
#include "PCA9555.h"
#include "deviceDefs.h"
#include "Task_transfer.h"
#include "fatfs.h"
#include "stm32f4xx_hal_gpio.h"
#include "Clock.h"
#include "USB_port.h"

//default variables
const uint8_t DC_const_dev_mac_addr[] = DC_DEF_DEV_MAC_ADDR;
const uint8_t DC_const_dev_ip_addr[] = DC_DEF_DEV_IP_ADDR;
const uint8_t DC_const_gw_ip_addr[] = DC_DEF_GW_IP_ADDR;
const uint8_t DC_const_net_mask[] = DC_DEF_NET_MASK;
const uint8_t DC_const_MQTT_ip_broc[] = DC_DEF_MQTT_BROC_IP;
const char DC_const_NTP_server_name[] = DC_DEF_NTP_SERVER;
const char DC_const_serverDNS[] = DC_DEF_DNS;

//Var
DC_set_t DC_set;        //Device settings
DC_set_t DC_tempSet;    //Temp data settings
DC_state_t DC_state;    //state
uint32_t DC_unicID[3];  //Unic ID
char DC_unic_idef[36];  //Unic idef
char DC_unic_idStr[13]; //Unic id str
osMessageQId *DC_eventQueue; //Event queue
char strBuffer[512];
uint8_t *pDevMAC; //Device MAC address point
DC_calibr_t DC_calibr; //Calibrate struct

//Extern
extern I2C_HandleTypeDef hi2c1;
extern SRAM_HandleTypeDef hsram1;
extern SRAM_HandleTypeDef hsram2;
extern NAND_HandleTypeDef hnand1;
extern SemaphoreHandle_t muxV9203;

//Get unic ID
void DC_getUnicID();

//LED task handle 
xTaskHandle ledTask_handle;
//LED task
void vTASK_led(void *pvParameters);
//Led status
volatile ledState_t linkState = LED_OFF;
volatile ledState_t stateState = LED_OFF;
volatile ledState_t runState = LED_OFF;
//Fat variables
FRESULT FATFS_res;
FATFS FATFS_Obj;
FIL LOG_file;

//--------------------------------------------------------------------------------------------------
//Init
void DC_init(osMessageQId *eventQueue)
{
  HAL_StatusTypeDef stat;
  
  DC_debugOut("\r\n# Start dev ILC\r\n");
  
  //Get unic ID
  DC_getUnicID();
  
  //Flash
  //Init memory
  MEM_init(&hsram1, &hsram2, &hnand1);
  if (MEM_NAND_checkID() == DEV_OK)
  {
    DC_debugOut("# Nand check OK\r\n");
    if (DC_load_settings() == DEV_OK)
      DC_debugOut("# Load settings OK\r\n");
  }else{
    DC_debugOut("# Nand check ERROR\r\n");
  }
  
  //PCA9555
  PCA9555_init(&hi2c1);
  
  //Set pin mode
  if ((stat = PCA9555_regSetValue(PCA9555_DEF_ADDR, PCA9555_REG_CONFIG, PCA9555_PIN_MODE_DEF)) == HAL_OK)
  {
    //Set default out
    if (PCA9555_regSetValue(PCA9555_DEF_ADDR, PCA9555_REG_OUTPUT, PCA9555_PIN_OUT_DEF) == HAL_OK)
    {
      DC_debugOut("# PCA9555 INIT OK\r\n");
    }else{
      DC_debugOut("# PCA9555 OUT ERROR\r\n"); 
    }
    
  }else{
    
    if (stat == HAL_ERROR)
    {
      DC_state.errorFlags |= DC_ERR_PCA9555;
      DC_debugOut("# PCA9555 ERROR\r\n");
    }
    
    if (stat == HAL_TIMEOUT)
      DC_debugOut("# PCA9555 TIMEOUT\r\n");
  }
  
  //Log init
  //  FATFS_res = f_mount(&FATFS_Obj, "0", 1);
  //  if (FATFS_res != FR_OK)
  //  {
  //    DC_debugOut("# Mount error %d\r\n", FATFS_res);
  //    DC_state.discMount = 0;
  //  }else{
  //    DC_debugOut("# Mount drive OK\r\n");
  //    DC_state.discMount = 1;    
  //  }
  
  //Start led task
  xTaskCreate(vTASK_led,(char*)"TASK_led", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+5, &ledTask_handle);
}
//--------------------------------------------------------------------------------------------------
//Log data
void DC_log(char *str, ...)
{
  char datetime[50];
  va_list args;
  va_start(args, str);
  va_end(args);
  
  vsprintf(strBuffer, str, args);
  va_end(args);
  
  
  //Get format date time
  if (CL_getFormat_DateTime(datetime) == DEV_OK)
  {
    strcat(strBuffer,";");
    strcat(strBuffer, datetime);
    strcat(strBuffer, "\r\n");
    
    FATFS_res = f_open(&LOG_file, "LOG.txt", FA_WRITE | FA_OPEN_ALWAYS);
    if (FATFS_res != FR_OK)
    {
      DC_debugOut("# File log error%d\r\n", FATFS_res);
    }
    FATFS_res = f_lseek(&LOG_file, f_size(&LOG_file));
    
    f_printf(&LOG_file, strBuffer);
    f_close(&LOG_file);
  }
  
  va_end(args);
}

//--------------------------------------------------------------------------------------------------
//LED task
void vTASK_led(void *pvParameters)
{
  uint8_t slowCnt;
  uint8_t linkLedState = GPIO_PIN_RESET;
  uint8_t statLedState = GPIO_PIN_RESET;
  uint8_t runLedState = GPIO_PIN_RESET;
  
  while(1)
  {
    if (linkState == LED_PROC_ERROR)
    {
      DC_LedOut(LED_LINK, (GPIO_PinState)linkLedState);
      linkLedState = !linkLedState;
    }
    
    if (stateState == LED_PROC_ERROR)
    {
      DC_LedOut(LED_STATUS, (GPIO_PinState)statLedState);
      statLedState = !statLedState;
    }
    
    if (runState == LED_PROC_ERROR)
    {
      DC_LedOut(LED_RUN, (GPIO_PinState)runLedState);
      runLedState = !runLedState;
    }
    
    if (slowCnt >= 2)
    {
      if (linkState == LED_PROC_OK)
      {
        DC_LedOut(LED_LINK, (GPIO_PinState)linkLedState);
        linkLedState = !linkLedState;
      }
      
      if (stateState == LED_PROC_OK)
      {
        DC_LedOut(LED_STATUS, (GPIO_PinState)statLedState);
        statLedState = !statLedState;
      }
      
      if (runState == LED_PROC_OK)
      {
        DC_LedOut(LED_RUN, (GPIO_PinState)runLedState);
        runLedState = !runLedState;
      }
      
      
    }else{
      slowCnt++;
    }
    
    vTaskDelay(500);
  }
}
//--------------------------------------------------------------------------------------------------
//LED out
void DC_LedOut(LED_t led, uint8_t state)
{
  switch (led)
  {
  case LED_LINK: HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, (GPIO_PinState)state); break;
  case LED_STATUS: HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, (GPIO_PinState)state); break;
  case LED_RUN: HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, (GPIO_PinState)state); break;
  }
}
//--------------------------------------------------------------------------------------------------
//LED blink
void DC_LedBlink(LED_t led, uint16_t rate_Hz, uint16_t count)
{
  uint8_t state;
  uint16_t delay = 1/rate_Hz/2;
  count = count*2;
  
  while (count--)
  {
    if (state)
    {
      state = 0;
    }else{
      state = 1;
    }
    
    DC_LedOut(led, state);
    vTaskDelay(delay);
  }
}
//--------------------------------------------------------------------------------------------------
//Get unic ID
void DC_getUnicID()
{
  HAL_GetUID(DC_unicID);
  // char uinicID[12];
  //  memcpy(uinicID, DC_unicID, 12);
  sprintf(DC_unic_idStr, "%02x%02x%02x", DC_unicID[0], DC_unicID[1], DC_unicID[2]);
  DC_unic_idStr[12] = 0;
  sprintf(DC_unic_idef, "%s%s", UNIC_ID_PREFIX, DC_unic_idStr);
  
  DC_debugOut("# UNIC ID: %s\r\n", DC_unic_idef);
}
//--------------------------------------------------------------------------------------------------
//Out debug data
//arg: str - string for out
void DC_debugOut(char *str, ...)
{  
  va_list args;
  va_start(args, str);
  va_end(args);
  
  vsprintf(strBuffer, str, args);
  va_end(args);
  
  if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) ? true : false)
  {
    //taskENTER_CRITICAL();
    printf(strBuffer);
    //taskEXIT_CRITICAL();
  }
  
  //Debug mode
  if (USBP_mode == USBP_MODE_DEBUG)
  {
    //Отправить данные
    USBP_Send((uint8_t*)strBuffer, strlen(strBuffer));
  }
  
  va_end(args);
}
//--------------------------------------------------------------------------------------------------
//IP out
void DC_debug_ipAdrrOut(char *text, uint8_t *ip)
{
  DC_debugOut("%s %d:%d:%d:%d\r\n", text, *ip, *(ip+1), *(ip+2), *(ip+3));
}
//--------------------------------------------------------------------------------------------------
//Out settings
void DC_debug_settingsOut()
{
  DC_debugOut("# --Network Settings--\r\n");
  DC_debug_ipAdrrOut("# NET DEV IP: ", DC_set.net_dev_ip_addr);
  DC_debug_ipAdrrOut("# NET GATEWAY IP: ", DC_set.net_gw_ip_addr);
  DC_debug_ipAdrrOut("# NET MASK: ", DC_set.net_mask);
  DC_debugOut("# --MQTT Settings--\r\n");
  DC_debugOut("# MQTT PORT %d\r\n", DC_set.MQTT_port);
  DC_debugOut("# MQTT USER %s\r\n", DC_set.MQTT_user);
  DC_debugOut("# MQTT PASS %s\r\n", DC_set.MQTT_pass);
  DC_debugOut("# MQTT QOS %d\r\n", DC_set.MQTT_qos);
}
//--------------------------------------------------------------------------------------------------
//Write settings
DEV_Status_t DC_writeSet(DC_set_t *settings, NAND_AddressTypeDef addr)
{
  DEV_Status_t stat;
  
  //Write
  if ((stat = MEM_NAND_writeData(addr, 0, (uint8_t*)settings, sizeof(DC_set_t))) != DEV_OK)
  {
    DC_debugOut("# Write settings ERROR\r\n");
  }else{
    DC_debugOut("# Write settings OK\r\n");
  }
  
  return stat;
}
//--------------------------------------------------------------------------------------------------
//Write settings
DEV_Status_t DC_writeCalibrate(DC_calibr_t *calibr, NAND_AddressTypeDef addr)
{
  DEV_Status_t stat;
  
  //Write
  if ((stat = MEM_NAND_writeData(addr, 0, (uint8_t*)calibr, sizeof(DC_calibr_t))) != DEV_OK)
  {
    DC_debugOut("# Write calibration ERROR\r\n");
  }else{
    DC_debugOut("# Write calibration OK\r\n");
  }
  
  return stat;
}
//--------------------------------------------------------------------------------------------------
//Load calibrate
DEV_Status_t DC_load_calibrate()
{
  DEV_Status_t stat;
  
  NAND_AddressTypeDef addr = MEM_NAND_ADDR_CALIBRATE;
  
  //read
  if ((stat = MEM_NAND_readData(addr, 0, (uint8_t*)&DC_calibr, sizeof(DC_calibr))) != DEV_OK)
  {
    DC_debugOut("# NAND IO ERROR\r\n");
    return stat;
  }
  
  if (DC_calibr.magicKey == DC_CALIBR_MAGICKEY)
  {
    DC_debugOut("# Calibration load OK\r\n");
    return DEV_OK;
  }
  
  //V9203 set settings
  for (int i=0; i<V9203_COUNT_CHANNELS; i++)
    V9203_setDefaultReg(i, &DC_calibr.channel_cal[i]);
  DC_debugOut("# Calibration set default\r\n");
  
  DC_calibr.magicKey = DC_CALIBR_MAGICKEY;
  DC_writeCalibrate(&DC_calibr, addr);
  
  return DEV_OK;
}  
//--------------------------------------------------------------------------------------------------
//Load settings
DEV_Status_t DC_load_settings()
{
  DEV_Status_t stat;
  
  NAND_AddressTypeDef addr = MEM_NAND_ADDR_SETTINGS;
  
  //read
  if ((stat = MEM_NAND_readData(addr, 0, (uint8_t*)&DC_set, sizeof(DC_set))) != DEV_OK)
  {
    DC_debugOut("# NAND IO ERROR\r\n");
    return stat;
  }
  
  if (DC_set.magicKey == DC_SET_MAGICKEY)
  {
    pDevMAC = DC_set.devMAC;
    DC_debugOut("# Settings load OK\r\n");
    //DC_debug_settingsOut();
    return DEV_OK;
  }
  
  //Clear settings
  memset(&DC_set, 0, sizeof(DC_set));
  
  //Set default
  //Network
  memcpy(DC_set.devMAC, DC_const_dev_mac_addr, 6);
  DC_set.net_DHCP_en = DC_DEF_DEV_DHCP_EN;
  memcpy(DC_set.net_dev_ip_addr, DC_const_dev_ip_addr, 4);
  memcpy(DC_set.net_gw_ip_addr, DC_const_gw_ip_addr, 4);
  memcpy(DC_set.net_mask, DC_const_net_mask, 4);
  memcpy(DC_set.netNTP_server, DC_const_NTP_server_name, strlen(DC_const_NTP_server_name));
  memcpy(DC_set.serverDNS, DC_const_serverDNS, 4);
  
  //MQTT
  memcpy(DC_set.MQTT_broc_ip, DC_const_MQTT_ip_broc, 4);
  memcpy(DC_set.MQTT_broc_domen, DC_DEF_MQTT_BROC_DOMEN, strlen(DC_DEF_MQTT_BROC_DOMEN));
  DC_set.MQTT_port = DC_DEF_MQTT_PORT;
  DC_set.MQTT_broc_ch = DC_DEF_MQTT_BROC_CH;
  
  memcpy(DC_set.MQTT_user, DC_DEF_MQTT_USER, sizeof(DC_DEF_MQTT_USER));
  memcpy(DC_set.MQTT_pass, DC_DEF_MQTT_PASS, sizeof(DC_DEF_MQTT_PASS));
  DC_set.MQTT_qos = DC_DEF_MQTT_QOS;
  
  //EMS
  DC_set.EMS_out_period = DC_DEF_EMS_OUT_PERIOD;
  DC_set.EMS_autoSendEn = DC_DEF_EMS_SEND_EN;
  DC_set.EMS_channelEn = DC_DEF_EMS_CH_EN;
  
  //Py
  DC_set.PY_autoStartEn = DC_DEF_PY_AUTOSTART;
  strcpy(DC_set.PY_scryptData.Name, DC_DEF_PY_NAME);
  DC_set.PY_scryptData.memID = DC_DEF_PY_MEM;
  
  //Set magic key
  DC_set.magicKey = DC_SET_MAGICKEY;
  
  //Write settings
  DC_writeSet(&DC_set ,addr);
  
  pDevMAC = DC_set.devMAC;
  DC_debugOut("# Settings set default\r\n");
  //DC_debug_settingsOut();
  
  return DEV_OK;
}
//--------------------------------------------------------------------------------------------------
//Assign setting parametr
DEV_Status_t DC_assignSettings()
{
  NAND_AddressTypeDef addr = MEM_NAND_ADDR_SETTINGS;
  DC_tempSet.magicKey = DC_SET_MAGICKEY;
  return DC_writeSet(&DC_tempSet ,addr); //Write settings
}
//--------------------------------------------------------------------------------------------------
//Reset setting key
DEV_Status_t DC_setResetSettingKey()
{
  NAND_AddressTypeDef addr = MEM_NAND_ADDR_SETTINGS;
  
  //Set magic key
  DC_set.magicKey = DC_SET_MAGICKEY+1;
  return DC_writeSet(&DC_set ,addr); //Write settings
}
//--------------------------------------------------------------------------------------------------
//System reset
void DC_systemReset()
{
  DC_LedOut(LED_LINK, 1);
  DC_LedOut(LED_STATUS, 1);
  DC_LedOut(LED_RUN, 1);
  vTaskDelay(500);
  NVIC_SystemReset();
}
//--------------------------------------------------------------------------------------------------
//Set setting parametr
DEV_Status_t DC_setSetParam(DC_settingID_t setID, uint8_t* data, uint8_t len)
{
  switch(setID)
  {
  case DC_SET_NET_MAC_ADR:
    if (len == 6)
    {
      memcpy(DC_tempSet.devMAC, data, 6);
      DC_debug_ipAdrrOut("* Witten MAC addr: ", DC_tempSet.devMAC);
      return DEV_OK;
    }
    break;
    
  case DC_SET_NET_DHCP_EN:
    DC_tempSet.net_DHCP_en = *data;
    DC_debugOut("* Witten DHCP enable: %d\r\n", DC_tempSet.net_DHCP_en);
    return DEV_OK;
    break;
    
  case DC_SET_NET_DEV_IP:
    if (len == 4)
    {
      memcpy(DC_tempSet.net_dev_ip_addr, data, 4);
      DC_debug_ipAdrrOut("* Witten dev IP: ", DC_tempSet.net_dev_ip_addr);
      return DEV_OK;
    }
    break;
    
  case DC_SET_NET_GW_IP:
    if (len == 4)
    {
      memcpy(DC_tempSet.net_gw_ip_addr, data, 4);
      DC_debug_ipAdrrOut("* Witten gateway IP: ", DC_tempSet.net_dev_ip_addr);
      return DEV_OK;
    }
    break;
    
  case DC_SET_NET_MASK:
    if (len == 4)
    {
      memcpy(DC_tempSet.net_mask, data, 4);
      DC_debug_ipAdrrOut("* Witten net mask: ", DC_tempSet.net_dev_ip_addr);
      return DEV_OK;
    }
    break;
    
  case DC_SET_NTP_DOMEN:
    if (len <= sizeof(DC_tempSet.netNTP_server))
    {
      memcpy(DC_tempSet.netNTP_server, data, len);
      DC_debugOut("* Witten NTP domen: %s\r\n", DC_tempSet.netNTP_server);
      return DEV_OK;
    }
    break;
    
  case DC_SET_NET_DNS_IP:
    if (len == 4)
    {
      memcpy(DC_tempSet.serverDNS, data, 4);
      DC_debug_ipAdrrOut("* Witten DNS server: ", DC_tempSet.net_dev_ip_addr);
      return DEV_OK;
    }
    break;
    
  case DC_SET_MQTT_IP:
    if (len == 4)
    {
      memcpy(DC_tempSet.MQTT_broc_ip, data, 4);
      DC_debug_ipAdrrOut("* Witten MQTT ip: ", DC_tempSet.MQTT_broc_ip);
      return DEV_OK;
    }
    break;
    
  case DC_SET_MQTT_DOMEN:
    if (len <= sizeof(DC_tempSet.MQTT_broc_domen))
    {
      memcpy(DC_tempSet.MQTT_broc_domen, data, len);
      DC_debugOut("* Witten MQTT domen: %s\r\n", DC_tempSet.MQTT_broc_domen);
      return DEV_OK;
    }
    break;
    
  case DC_SET_MQTT_CH:
    DC_tempSet.MQTT_broc_ch = *data;
    DC_debugOut("* Witten channel: %d\r\n", DC_tempSet.MQTT_broc_ch);
    return DEV_OK;
    break;
    
  case DC_SET_MQTT_PORT:
    if (len == 2)
    {
      memcpy((uint8_t*)DC_tempSet.MQTT_port, data, 2);
      DC_debugOut("* Witten MQTT port: %d\r\n", DC_tempSet.MQTT_port);
      return DEV_OK;
    }
    break;
    
  case DC_SET_MQTT_USER:
    if (len <= sizeof(DC_tempSet.MQTT_user))
    {
      memcpy(DC_tempSet.MQTT_user, data, len);
      DC_debugOut("* Witten MQTT user: %s\r\n", DC_tempSet.MQTT_user);
      return DEV_OK;
    }
    break;
    
  case DC_SET_MQTT_PASS:
    if (len <= sizeof(DC_tempSet.MQTT_pass))
    {
      memcpy(DC_tempSet.MQTT_pass, data, len);
      DC_debugOut("* Witten MQTT pass: %s\r\n", DC_tempSet.MQTT_pass);
      return DEV_OK;
    }
    break;
    
  case DC_SET_MQTT_QOS:
    if (len == 1)
    {
      DC_tempSet.MQTT_qos = *data;
      DC_debugOut("* Witten MQTT QoS: %d\r\n", DC_tempSet.MQTT_qos);
      return DEV_OK;
    }
    break;
    
  case DC_SET_EMS_PERIOD:
    if (len == 2)
    {
      memcpy((uint8_t*)DC_tempSet.EMS_out_period, data, 2);
      DC_debugOut("* Witten EMS period: %d\r\n", DC_tempSet.EMS_out_period);
      return DEV_OK;
    }
    break;
    
  case DC_SET_EMS_AUTO_SEND:
    if (len == 1)
    {
      DC_tempSet.EMS_autoSendEn = *data;
      DC_debugOut("* Witten EMS enable: %d\r\n", DC_tempSet.EMS_autoSendEn);
      return DEV_OK;
    }
    break;
    
  case DC_SET_EMS_CHANNEL_EN:
    if (len == 1)
    {
      DC_tempSet.EMS_channelEn = *data;
      DC_debugOut("* Witten EMS channel en: %d\r\n", DC_tempSet.EMS_channelEn);
      return DEV_OK;
    }
    break;
    
  case DC_SET_VM_AUTO_START:
    if (len == 1)
    {
      DC_tempSet.PY_autoStartEn = *data;
      DC_debugOut("* Witten PY autostart: %d\r\n", DC_tempSet.PY_autoStartEn);
      return DEV_OK;
    }
    break;
    
  }
  return DEV_ERROR;
}
//--------------------------------------------------------------------------------------------------
//Get setting param
DEV_Status_t DC_getSetParam(DC_settingID_t setID, uint8_t* data, uint8_t* len)
{
  switch(setID)
  {
  case DC_SET_NET_MAC_ADR:
    *len = 6;
    memcpy(data, DC_set.devMAC, 6);
    break;
    
  case DC_SET_NET_DHCP_EN:
    *len = 1;
    *data = DC_set.net_DHCP_en;
    break;
    
  case DC_SET_NET_DEV_IP:
    *len = 4;
    memcpy(data, DC_set.net_dev_ip_addr, 4);
    break;
    
  case DC_SET_NET_GW_IP:
    *len = 4;
    memcpy(data, DC_set.net_gw_ip_addr, 4);
    break;
    
  case DC_SET_NET_MASK:
    *len = 4;
    memcpy(data, DC_set.net_mask, 4);
    break;
    
  case DC_SET_NTP_DOMEN:
    *len = sizeof(DC_set.netNTP_server);
    memcpy(data, DC_set.netNTP_server, sizeof(DC_set.netNTP_server));
    break;
    
  case DC_SET_NET_DNS_IP:
    *len = 4;
    memcpy(data, DC_set.serverDNS, 4);
    break;
    
  case DC_SET_MQTT_IP:
    *len = 4;
    memcpy(data, DC_set.MQTT_broc_ip, 4);
    break;
    
  case DC_SET_MQTT_DOMEN:
    *len = sizeof(DC_set.MQTT_broc_domen);
    memcpy(data, DC_set.MQTT_broc_domen, sizeof(DC_set.MQTT_broc_domen));
    break;
    
  case DC_SET_MQTT_CH:
    *len = 1;
    *data = DC_set.MQTT_broc_ch;
    break;
    
  case DC_SET_MQTT_PORT:
    *len = 2;
    memcpy(data, (uint8_t*)DC_set.MQTT_port, 2);
    break;
    
  case DC_SET_MQTT_USER:
    *len = sizeof(DC_set.MQTT_user);
    memcpy(data, DC_set.MQTT_user, sizeof(DC_set.MQTT_user));
    break;
    
  case DC_SET_MQTT_PASS:
    *len = sizeof(DC_set.MQTT_pass);
    memcpy(data, DC_set.MQTT_pass, sizeof(DC_set.MQTT_pass));
    break;
    
  case DC_SET_MQTT_QOS:
    *len = 1;
    *data = DC_set.MQTT_qos;
    break;
    
  case DC_SET_EMS_PERIOD:
    *len = 2;
    memcpy(data, (uint8_t*)DC_set.EMS_out_period, 2);
    break;
    
  case DC_SET_EMS_AUTO_SEND:
    *len = 1;
    *data = DC_set.EMS_autoSendEn;
    break;
    
  case DC_SET_EMS_CHANNEL_EN:
    *len = 1;
    *data = DC_set.EMS_channelEn;
    break;
    
  case DC_SET_VM_AUTO_START:
    *len = 1;
    *data = DC_set.PY_autoStartEn;
    break;
  }
  
  return DEV_OK;  
}
//--------------------------------------------------------------------------------------------------
//Set calibrate parametr
DEV_Status_t DC_setCalParam(uint8_t channel, V9203_line_t line, DC_calibrID_t calID, uint8_t* data, uint8_t len)
{
  switch(calID)
  {
    
  }
  
  return DEV_OK;
}
//--------------------------------------------------------------------------------------------------
//Get calibrate param
DEV_Status_t DC_getCalParam(uint8_t channel, V9203_line_t line, DC_calibrID_t calID, uint8_t* data, uint8_t* len)
{
  *len = 4;
  
  switch(calID)
  {
  case DC_CAL_REG_CTHH:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].V9203_defSet.CTHH, data, *len); 
    break;
    
  case DC_CAL_REG_CTHL:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].V9203_defSet.CTHL, data, *len);
    break;
    
  case DC_CAL_REG_WAEC0:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].V9203_defSet.WAEC0, data, *len);
    break;
    
  case DC_CAL_REG_MTPARA0:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].V9203_defSet.MTPARA0, data, *len);
    break;
    
  case DC_CAL_REG_MTPARA1:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].V9203_defSet.MTPARA1, data, *len);
    break;    
    
  case DC_CAL_REG_MTPARA2:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].V9203_defSet.MTPARA2, data, *len);
    break;  
    
  case DC_CAL_REG_ANCtrl0:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].V9203_defSet.ANCtrl0, data, *len);
    break;
    
  case DC_CAL_REG_ANCtrl1:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].V9203_defSet.ANCtrl1, data, *len);
    break;    
    
  case DC_CAL_REG_ANCtrl2:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].V9203_defSet.ANCtrl2, data, *len);
    break;  
    
  case DC_CAL_REG_ANCtrl3:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].V9203_defSet.ANCtrl3, data, *len);
    break;  
    
  case DC_CAL_REG_WARTU:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].calTotalPhase[line].Cal_WARTU, data, *len);
    break;
    
  case DC_CAL_REG_WARTI:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].calTotalPhase[line].Cal_WARTI, data, *len);
    break;
    
  case DC_CAL_REG_WAPT:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].calTotalPhase[line].Cal_WAPT, data, *len);
    break;
    
  case DC_CAL_REG_WAQT:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].calTotalPhase[line].Cal_WAQT, data, *len);
    break;
    
  case DC_CAL_REG_WWARTU:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].calTotalPhase[line].Cal_WWARTU, data, *len);
    break;
    
  case DC_CAL_REG_WWARTI:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].calTotalPhase[line].Cal_WARTI, data, *len);
    break;
    
  case DC_CAL_REG_WWAPT:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].calTotalPhase[line].Cal_WWAPT, data, *len);
    break;
    
  case DC_CAL_REG_WWAQT:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].calTotalPhase[line].Cal_WWAQT, data, *len);
    break;
    
  case DC_CAL_REG_WARTIN:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].WARTIN, data, *len); break;
    break;
    
  case DC_CAL_REG_WWARTIN:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].WBRTIN, data, *len); break;
    break;
    
  case DC_CAL_PROP_P:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].calPropPower, data, *len); break;
    break;

  case DC_CAL_PROP_U:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].calPropVoltage, data, *len); break;
    break;
    
  case DC_CAL_PROP_I:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].calPropCurrent, data, *len); break;
    break;
    
  case DC_CAL_PROP_FREQ:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].calPropFreq, data, *len); break;
    break;
    
  case DC_CAL_THRDI_DETECT:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].cal_currThrdDetect, data, *len); break;
    break;
    
  case DC_CAL_THRDM_DETECT:
    memcpy((uint8_t*)DC_calibr.channel_cal[channel].cal_energyThrdDetect, data, *len); break;
    break;
    
  default:
    return DEV_ERROR;
  }
  return DEV_OK;
}
//--------------------------------------------------------------------------------------------------
//Relay out
DEV_Status_t DC_getValues(uint8_t channel, V9203_line_t line, DC_valueID_t id, uint8_t *data, uint8_t *len)
{
  float floatBuf;
  uint64_t uBuf;
  
  if( xSemaphoreTake( muxV9203, ( TickType_t ) 1000 ) == pdTRUE )
  {
    switch(id)
    {
    case DC_VAL_RMSV:
      floatBuf = V9203_getRMS_Voltage(channel, line);
      memcpy(data, (uint8_t*)&floatBuf, sizeof(floatBuf));
      *len = 4;
      break;
    case DC_VAL_RMSI:
      floatBuf = V9203_getRMS_Current(channel, line);
      memcpy(data, (uint8_t*)&floatBuf, sizeof(floatBuf));
      *len = 4;
      break; 
    case DC_VAL_RMSP:
      floatBuf = V9203_getRMS_Power(channel, line);
      memcpy(data, (uint8_t*)&floatBuf, sizeof(floatBuf));
      *len = 4;
      break;
    case DC_VAL_RMSRP:
      floatBuf = V9203_getRMS_reactivePower(channel, line);
      memcpy(data, (uint8_t*)&floatBuf, sizeof(floatBuf));
      *len = 4;
      break; 
    case DC_VAL_COSFI:
      floatBuf = V9203_getCOSfi(channel, line);
      memcpy(data, (uint8_t*)&floatBuf, sizeof(floatBuf));
      *len = 4;
      break; 
     case DC_VAL_CONSSP:
      uBuf = V9203_getSCons(channel, line);
      memcpy(data, (uint8_t*)&uBuf, sizeof(uBuf));
      *len = 8;
      break;  
    case DC_VAL_CONSP:
      uBuf = V9203_getPCons(channel, line);
      memcpy(data, (uint8_t*)&uBuf, sizeof(uBuf));
      *len = 8;
      break;
    case DC_VAL_CONSRP:
      uBuf = V9203_getQCons(channel, line);
      memcpy(data, (uint8_t*)&uBuf, sizeof(uBuf));
      *len = 8;
      break;  
    case DC_VAL_FREQ:
      floatBuf = V9203_getFreq(channel, line);
      memcpy(data, (uint8_t*)&floatBuf, sizeof(floatBuf));
      *len = 4;
      break;
    }
    xSemaphoreGive( muxV9203 );
  }
  
  return DEV_OK;
}
//--------------------------------------------------------------------------------------------------
//Relay out
HAL_StatusTypeDef DC_relayOut(uint8_t relNum, uint8_t state)
{
  uint8_t pin;
  
  switch (relNum)
  {
  case 1: pin = PCA9555_PIN_K1; break;
  case 2: pin = PCA9555_PIN_K2; break;
  case 3: pin = PCA9555_PIN_K3; break;
  case 4: pin = PCA9555_PIN_K4; break;
  default: DC_debugOut("# Relay num ERROR\r\n"); return HAL_ERROR;
  }
  
  return PCA9555_digitalWrite(PCA9555_DEF_ADDR, pin, state);
}
//*********************************************IRQ**************************************************
//GPIO EXTI callback
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  //PCA9555
  if(GPIO_Pin == nPW_INT_Pin)
  {    
    TT_sendEventIRQ(EVENT_IRQ_PCA9555, NULL, DC_eventQueue); //Send event from IRQ
    DC_debugOut("@ PCA9555 IRQ\r\n");
  } 
}