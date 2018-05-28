#ifndef DEVCTRL_H
#define DEVCTRL_H

#include "stm32f4xx_hal.h"
#include "EXP.h"

//***********************************Device data****************************************************

//PCA9555
#define PCA9555_DEF_ADDR           0x40    //Address

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

extern uint32_t DC_unicID[3]; //Unic ID


//***********************************Default settings***********************************************

//TCP/IP Ethernet
#define DC_DEF_DEV_IP_ADDR              {192,168,1,50}
#define DC_DEF_GW_IP_ADDR               {192,168,1,1}
#define DC_DEF_NET_MASK                 {255,255,255,0}

//MQTT
#define DC_DEF_MQTT_BROC_IP             {192,168,1,1}
#define DC_DEF_MQTT_PORT                1883
#define DC_DEF_MQTT_CLINETID_PFX        "INS_C1"
#define DC_DEF_MQTT_USER                "user"
#define DC_DEF_MQTT_PASS                "123"
#define DC_DEF_MQTT_QOS                 1
#define DC_DEF_MQTT_CMD_TOPIC           "INS_CMD"
#define DC_DEF_MQTT_DATA_TOPIC          "INS_DATA"
#define DC_DEF_MQTT_DEBUG_TOPIC         "INS_DEBUG"

//**********************************Settings*****************************************************

#define DC_SET_MAGICKEY 0x01

typedef struct
{
  uint8_t magicKey;
  
  //TCP/IP Ethernet
  uint8_t net_dev_ip_addr[4];
  uint8_t net_gw_ip_addr[4];
  uint8_t net_mask[4];
  
  //MQTT
  uint8_t MQTT_broc_ip[4];
  uint16_t MQTT_port;
  char MQTT_clintID[20];
  char MQTT_user[20];
  char MQTT_pass[20];
  uint8_t MQTT_qos;
  char MQTT_cmd_topic[20];
  char MQTT_data_topic[20];

}DC_set_t;

extern DC_set_t DC_set; //Device settings

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
//IP addr out
void DC_debug_ipAdrrOut(char *text, uint8_t* ip);
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