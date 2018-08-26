#ifndef DEVCTRL_H
#define DEVCTRL_H

#include "stm32f4xx_hal.h"
#include "deviceDefs.h"
#include "V9203.h"
#include "TASK_script.h"

//***********************************Device data****************************************************

//PCA9555
#define PCA9555_DEF_ADDR          0x40    //Address

//Init mode mask 1-input mode
#define PCA9555_PIN_MODE_DEF      0x000F
//Defaul output
#define PCA9555_PIN_OUT_DEF       0x0F00

//PCA9555 PIN MAP
#define PCA9555_PIN_DIN1           0       //
#define PCA9555_PIN_DIN2           1       //
#define PCA9555_PIN_DIN3           2       //
#define PCA9555_PIN_DIN4           3       //

#define PCA9555_PIN_CS1            8       //
#define PCA9555_PIN_CS2            9       //
#define PCA9555_PIN_CS3            10      //
#define PCA9555_PIN_CS4            11      //

#define PCA9555_PIN_K1             12      //
#define PCA9555_PIN_K2             13      //
#define PCA9555_PIN_K3             14      //
#define PCA9555_PIN_K4             15      //

#define DC_V9203_COUNT_CHANNELS    1      //Count channels

//Unic ID
#define UNIC_ID_PREFIX  "aaaaaaaa-1234-1234-1234-"
extern uint32_t DC_unicID[3]; //Unic ID
extern char DC_unic_idef[36]; //Unic idef + prefix
extern char DC_unic_idStr[13]; //Unic id str

//***********************************Default settings***********************************************

//TCP/IP Ethernet
#define DC_DEF_DEV_IP_ADDR              {192,168,1,55}
#define DC_DEF_GW_IP_ADDR               {192,168,1,1}
#define DC_DEF_NET_MASK                 {255,255,255,0}
#define DC_DEF_NTP_SERVER               "ntp1.stratum2.ru"
#define DC_DEF_DNS                      {8,8,8,8}

//MQTT
#define DC_DEF_MQTT_BROC_CH             0 //0 - IP, 1 - DOMEN
#define DC_DEF_MQTT_BROC_IP             {82,202,249,110}
#define DC_DEF_MQTT_BROC_DOMEN          "ems.insyte.ru"
#define DC_DEF_MQTT_PORT                1883
#define DC_DEF_MQTT_USER                "user"
#define DC_DEF_MQTT_PASS                "123"
#define DC_DEF_MQTT_QOS                 1

//EMS
#define DC_DEF_EMS_OUT_PERIOD           5 //sec
#define DC_DEF_EMS_SEND_EN              1 //Разрешить передачу данных

//Python
#define DC_DEF_PY_NAME                  "main"          //Имя старта скрипта
#define DC_DEF_PY_MEM                   MEM_ID_SRAM1    //Память скрипта

//**********************************Settings*****************************************************

#define DC_SET_MAGICKEY 0x02

typedef struct {
  
  uint8_t magicKey;
  
  //TCP/IP Ethernet
  uint8_t net_dev_ip_addr[4];
  uint8_t net_gw_ip_addr[4];
  uint8_t net_mask[4];
  char netNTP_server[40];
  uint8_t serverDNS[4];
  
  //MQTT
  uint8_t MQTT_broc_ip[4]; //Brocker ip list
  char MQTT_broc_domen[40]; //Brocker net name list
  uint8_t MQTT_broc_ch; //Active brocker channel
  uint16_t MQTT_port; //Port
  char MQTT_user[20];
  char MQTT_pass[20];
  uint8_t MQTT_qos;
  
  //EMS
  uint16_t EMS_out_period; //Send period
  uint8_t EMS_autoSendEn; //Enable periodic send
  
  //Python VM
  uint8_t VM_autoStartEn; //Enable autostart script
  PY_scryptData_t PY_scryptData; //Описание скрипта
  
  //Settings struct
  V9203_settings_t V9203_ch_set[DC_V9203_COUNT_CHANNELS];
  
}DC_set_t;

extern DC_set_t DC_set; //Device settings
//Write settings
DEV_Status_t DC_writeSet(DC_set_t *settings, NAND_AddressTypeDef addr);

//**********************************Flash log*******************************************************

typedef struct
{
  uint32_t logID;
  uint64_t UTC_timeshtamp;
  
}DC_log_t;

//**********************************Main Functions**************************************************

//Init
void DC_init();
//Out debug data
void DC_debugOut(char *str, ...);
//IP out
void DC_debug_ipAdrrOut(char *text, uint8_t *ip);
//Load settings
DEV_Status_t DC_load_settings();
//User function Get Current task ID 
uint8_t DC_getCurrentTaskID();

//**********************************Digital IO******************************************************

//LEDs
typedef enum{
  LED_LINK = 0,
  LED_STATUS,
  LED_RUN
}LED_t;

//Status out
typedef enum{
  LED_OFF = 0,
  LED_PROC_OK,
  LED_PROC_ERROR
}ledState_t;

//Led status
extern volatile ledState_t linkState;
extern volatile ledState_t stateState;
extern volatile ledState_t runState;

//Relay out
HAL_StatusTypeDef DC_relayOut(uint8_t relNum, uint8_t state);



#endif