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
#include "stm32f4xx_hal_gpio.h"

//default variables
const uint8_t DC_const_dev_ip_addr[] = DC_DEF_DEV_IP_ADDR;
const uint8_t DC_const_gw_ip_addr[] = DC_DEF_GW_IP_ADDR;
const uint8_t DC_const_net_mask[] = DC_DEF_NET_MASK;
const uint8_t DC_const_MQTT_ip_broc_1[] = DC_DEF_MQTT_BROC_IP_1;
const uint8_t DC_const_MQTT_ip_broc_2[] = DC_DEF_MQTT_BROC_IP_2;
const uint8_t DC_const_MQTT_ip_broc_3[] = DC_DEF_MQTT_BROC_IP_3;
const char DC_const_MQTT_name_broc_1[] = DC_DEF_MQTT_BROC_NAME_1;
const char DC_const_MQTT_name_broc_2[] = DC_DEF_MQTT_BROC_NAME_2;
const char DC_const_MQTT_name_broc_3[] = DC_DEF_MQTT_BROC_NAME_2;
const char DC_const_NTP_server_name[] = DC_DEF_NTP_SERVER;
const char DC_const_serverDNS1[] = DC_DEF_DNS1;
const char DC_const_serverDNS2[] = DC_DEF_DNS2;

//Var
DC_set_t DC_set; //Device settings
uint32_t DC_unicID[3]; //Unic ID
char DC_unic_idef[36]; //Unic idef
char DC_unic_idStr[13]; //Unic id str
osMessageQId *DC_eventQueue; //Event queue
char strBuffer[1024];

//Extern
extern I2C_HandleTypeDef hi2c1;
extern SRAM_HandleTypeDef hsram1;
extern SRAM_HandleTypeDef hsram2;
extern NAND_HandleTypeDef hnand1;

//Func
HAL_StatusTypeDef DC_load_settings(); //Load settings
//Get unic ID
void DC_getUnicID();

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
  if (MEM_NAND_checkID() == HAL_OK)
  {
    DC_debugOut("# Nand check OK\r\n");
    if (DC_load_settings() == HAL_OK)
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
      DC_debugOut("# PCA9555 ERROR\r\n");
    
    if (stat == HAL_TIMEOUT)
      DC_debugOut("# PCA9555 TIMEOUT\r\n");
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

  va_end(args);
}
//--------------------------------------------------------------------------------------------------
//IP out
void DC_debug_ipAdrrOut(char *text, uint8_t *ip)
{
  DC_debugOut("%s %d:%d:%d:%d\r\n", text, *ip, *(ip+1), *(ip+2), *(ip+3));
}
//--------------------------------------------------------------------------------------------------
//IP active out
void DC_debug_ipActiveAdrrOut(char *text, uint8_t activeBrock)
{
  if (activeBrock < 3)
  {
    DC_debug_ipAdrrOut(text, DC_set.MQTT_broc_ip[activeBrock]);
  }
  
  if ((activeBrock > 3) && (activeBrock < 7))
  {
    DC_debugOut("%s %s", text, DC_set.MQTT_broc_name[activeBrock-3]);
  }
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
  DC_debug_ipActiveAdrrOut("# MQTT ACTIVE BROCKER: ", DC_set.MQTT_activeBrock);
  DC_debugOut("# MQTT PORT %d\r\n", DC_set.MQTT_port);
  DC_debugOut("# MQTT USER %s\r\n", DC_set.MQTT_user);
  DC_debugOut("# MQTT PASS %s\r\n", DC_set.MQTT_pass);
  DC_debugOut("# MQTT QOS %d\r\n", DC_set.MQTT_qos);
}
//--------------------------------------------------------------------------------------------------
//Write settings
HAL_StatusTypeDef DC_writeSet(DC_set_t *settings, NAND_AddressTypeDef addr)
{
  HAL_StatusTypeDef stat;
  
  //Write
  if ((stat = MEM_NAND_writeData(addr, (uint8_t*)settings, sizeof(DC_set_t))) != HAL_OK)
  {
    DC_debugOut("# Write settings ERROR\r\n");
  }
  
  return stat;
}
//--------------------------------------------------------------------------------------------------
//Load settings
HAL_StatusTypeDef DC_load_settings()
{
  HAL_StatusTypeDef stat;
  
  NAND_AddressTypeDef addr = MEM_NAND_ADDR_SETTINGS;
  
  //read
  if ((stat = MEM_NAND_readData(addr, (uint8_t*)&DC_set, sizeof(DC_set))) != HAL_OK)
  {
    DC_debugOut("# NAND IO ERROR\r\n");
    return stat;
  }
  
  if (DC_set.magicKey == DC_SET_MAGICKEY)
  {
    DC_debugOut("# Settings load OK\r\n");
    //DC_debug_settingsOut();
    return HAL_OK;
  }
  
  //Clear settings
  memset(&DC_set, 0, sizeof(DC_set));
  
  //Set default
  //Network
  memcpy(DC_set.net_dev_ip_addr, DC_const_dev_ip_addr, 4);
  memcpy(DC_set.net_gw_ip_addr, DC_const_gw_ip_addr, 4);
  memcpy(DC_set.net_mask, DC_const_net_mask, 4);
  memcpy(DC_set.netNTP_server, DC_const_NTP_server_name, strlen(DC_const_NTP_server_name));
  memcpy(DC_set.serverDNS1, DC_const_serverDNS1, 4);
  memcpy(DC_set.serverDNS1, DC_const_serverDNS2, 4);

  //MQTT
  memcpy(DC_set.MQTT_broc_ip[0], DC_const_MQTT_ip_broc_1, 4);
  memcpy(DC_set.MQTT_broc_ip[1], DC_const_MQTT_ip_broc_2, 4);
  memcpy(DC_set.MQTT_broc_ip[2], DC_const_MQTT_ip_broc_3, 4);
  memcpy(DC_set.MQTT_broc_name[0], DC_const_MQTT_name_broc_1, strlen(DC_const_MQTT_name_broc_1));
  memcpy(DC_set.MQTT_broc_name[1], DC_const_MQTT_name_broc_2, strlen(DC_const_MQTT_name_broc_2));
  memcpy(DC_set.MQTT_broc_name[2], DC_const_MQTT_name_broc_3, strlen(DC_const_MQTT_name_broc_3));
  DC_set.MQTT_port = DC_DEF_MQTT_PORT;

  memcpy(DC_set.MQTT_user, DC_DEF_MQTT_USER, sizeof(DC_DEF_MQTT_USER));
  memcpy(DC_set.MQTT_pass, DC_DEF_MQTT_PASS, sizeof(DC_DEF_MQTT_PASS));
  DC_set.MQTT_qos = DC_DEF_MQTT_QOS;
  
  //EMS
  DC_set.EMS_out_period = DC_DEF_EMS_OUT_PERIOD;
  
  //V9203 set settings
  for (int i=0; i<DC_V9203_COUNT_CHANNELS; i++)
    V9203_setDefaultReg(i, &DC_set.V9203_ch_set[i]);
  
  //Set magic key
  DC_set.magicKey = DC_SET_MAGICKEY;
  
  //Write settings
  DC_writeSet(&DC_set ,addr);
  
  DC_debugOut("# Settings set default\r\n");
  //DC_debug_settingsOut();
  
  return HAL_OK;
}
//--------------------------------------------------------------------------------------------------
//LED out
void DC_LedOut(uint8_t led, uint8_t state)
{
  switch (led)
  {
  case 0: HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, (GPIO_PinState)state); break;
  case 1: HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, (GPIO_PinState)state); break;
  case 2: HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, (GPIO_PinState)state); break;
  default: DC_debugOut("# Led num ERROR\r\n");
  }
}
//--------------------------------------------------------------------------------------------------
//LED blink
void DC_LedBlink(uint8_t led, uint16_t rate_Hz, uint16_t count)
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