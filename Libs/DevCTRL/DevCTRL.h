#ifndef DEVCTRL_H
#define DEVCTRL_H

#include "stm32f4xx_hal.h"
#include "EXP.h"
#include "V9203.h"

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

//Unic ID
#define UNIC_ID_PREFIX  "aaaaaaaa-1234-1234-1234-"
extern uint32_t DC_unicID[3]; //Unic ID
extern char DC_unic_idef[36]; //Unic idef + prefix
extern char DC_unic_idStr[13]; //Unic id str

//***********************************Default settings***********************************************

//TCP/IP Ethernet
#define DC_DEF_DEV_IP_ADDR              {192,168,1,50}
#define DC_DEF_GW_IP_ADDR               {192,168,1,1}
#define DC_DEF_NET_MASK                 {255,255,255,0}
#define DC_DEF_NTP_SERVER               "ntp1.stratum2.ru"
#define DC_DEF_DNS1                     {8,8,8,8}
#define DC_DEF_DNS2                     {77,88,8,8}

//MQTT
#define DC_COUNT_IP_BROCK_ADDR          3
#define DC_COUNT_NAME_BROCK_ADDR        3
#define DC_COUNT_BROCKS                 DC_COUNT_NAME_BROCK_ADDR+DC_COUNT_IP_BROCK_ADDR
#define DC_DEF_MQTT_BROC_IP_1           {52,58,123,29}
#define DC_DEF_MQTT_BROC_IP_2           {37,187,106,16}
#define DC_DEF_MQTT_BROC_IP_3           {35,157,158,75}
#define DC_DEF_MQTT_BROC_NAME_1         "ems.insyte.ru"
#define DC_DEF_MQTT_BROC_NAME_2         "broker.mqtt-dashboard.com"
#define DC_DEF_MQTT_BROC_NAME_3         "test.mosquitto.org"
#define DC_DEF_MQTT_PORT                1883
#define DC_DEF_MQTT_USER                "user"
#define DC_DEF_MQTT_PASS                "123"
#define DC_DEF_MQTT_QOS                 1

//EMS
#define DC_DEF_EMS_OUT_PERIOD           5 //sec

//Calibration cofficients
//Channel 1
#define DC_CAL_CH1_UOFFSET      0
#define DC_CAL_CH1_IOFFSET      0x21A8301BUL
#define DC_CAL_CH1_P1OFFSET     0x21E51894UL
#define DC_CAL_CH1_P2OFFSET     0x00000000UL
#define DC_CAL_CH1_R2POFFSET    0x00000000UL

//Proportiona coefficients
#define DC_CAL_COEFF_P          0x10BUL
#define DC_CAL_COEFF_U          0x513bUL
#define DC_CAL_COEFF_I          0x1A2C0UL

//**********************************Settings*****************************************************

#define DC_SET_MAGICKEY 0x01

typedef struct
{
  uint8_t magicKey;
  
  //TCP/IP Ethernet
  uint8_t net_dev_ip_addr[4];
  uint8_t net_gw_ip_addr[4];
  uint8_t net_mask[4];
  char netNTP_server[40];
  uint8_t serverDNS1[4];
  uint8_t serverDNS2[4];
  
  //MQTT
  uint8_t MQTT_broc_ip[DC_COUNT_IP_BROCK_ADDR][4]; //Brocker ip list
  char MQTT_broc_name[DC_COUNT_NAME_BROCK_ADDR][40]; //Brocker net name list
  uint8_t MQTT_activeBrock; //Active brocker
  uint16_t MQTT_port; //Port
  char MQTT_user[20];
  char MQTT_pass[20];
  uint8_t MQTT_qos;
  
  //EMS
  uint16_t EMS_out_period;
  
  //Calibration struct
  V9203_calChannel_t V9203_ch1_cal;
  V9203_calChannel_t V9203_ch2_cal;
  V9203_calChannel_t V9203_ch3_cal;
  V9203_calChannel_t V9203_ch4_cal;
  
}DC_set_t;

extern DC_set_t DC_set; //Device settings
//Write settings
HAL_StatusTypeDef DC_writeSet(DC_set_t *settings, NAND_AddressTypeDef addr);

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
HAL_StatusTypeDef DC_load_settings();
//User function Get Current task ID 
uint8_t DC_getCurrentTaskID();

//**********************************Digital IO******************************************************

//LED blink
void DC_LedBlink(uint8_t led, uint16_t rate_Hz, uint16_t count);
//LED out
void DC_LedOut(uint8_t led, uint8_t state);
//Relay out
HAL_StatusTypeDef DC_relayOut(uint8_t relNum, uint8_t state);


#endif