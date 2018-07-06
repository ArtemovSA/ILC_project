#ifndef DEVMQTT_H
#define DEVMQTT_H

#include "lwip.h"
#include "mqtt.h"
#include "string.h"
#include "stm32f4xx_hal_def.h"

#define MQTT_INC_BUF_SIZE 1024

#define MQTT_SUB        1
#define MQTT_PUB_SUB    2
#define MQTT_PUB        3

//Topic struct
typedef struct
{
  uint8_t sub_pub; //Subscribe - 1
  char name[70];
  /////.....
}devMQTT_topic;

//Init
void devMQTT_init(devMQTT_topic* topics, uint16_t count, void (*callBackPoint)(uint16_t, uint8_t*, uint16_t));
//MQTT connection
HAL_StatusTypeDef devMQTT_connect(uint8_t* MQTT_IP, uint16_t MQTT_port, char* MQTT_clintID, char* MQTT_user, char* MQTT_pass);
//Publish
HAL_StatusTypeDef devMQTT_publish(char *topicName, uint8_t *payload, uint16_t len, uint8_t qos);

#endif