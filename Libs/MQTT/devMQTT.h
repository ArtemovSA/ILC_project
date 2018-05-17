#ifndef DEVMQTT_H
#define DEVMQTT_H

#include "lwip.h"
#include "mqtt.h"
#include "string.h"
#include "stm32f4xx_hal_def.h"

#define MQTT_CMD_TOPIC 1
#define MQTT_DATA_TOPIC 2
#define MQTT_DEBUG_TOPIC 3
#define MQTT_OTHER_TOPIC 4

//MQTT connection
HAL_StatusTypeDef devMQTT_connect(uint8_t* MQTT_IP, uint16_t MQTT_port, char* MQTT_clintID, char* MQTT_user, char* MQTT_pass);

HAL_StatusTypeDef devMQTT_publish(char *topicName, uint8_t *payload, uint16_t len);

#endif