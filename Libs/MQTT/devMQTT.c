
#include "devMQTT.h"
#include "DevCTRL.h"

static int inpub_id; //topic ID
mqtt_client_t *mqttMainClient;
struct mqtt_connect_client_info_t ci;

//*******************************************Callback***********************************************
//Publish cb
static void mqtt_pub_request_cb(void *arg, err_t result) {
  if(result != ERR_OK) {
    DC_debugOut("# MQTT Publish result: %d\r\n", result);
  }
}
//--------------------------------------------------------------------------------------------------
//Incoming publish cb
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) {
  printf("Incoming publish at topic %s with total length %u\n", topic, (unsigned int)tot_len);
  
  // Decode topic string into a user defined reference
  if( strcmp(topic, DC_set.MQTT_cmd_topic) == 0) {
    inpub_id = MQTT_CMD_TOPIC;
  } else if( strcmp(topic, DC_set.MQTT_data_topic) == 0) {
    inpub_id = MQTT_DATA_TOPIC;
  } else if( strcmp(topic, DC_DEF_MQTT_DEBUG_TOPIC) == 0) { //Debug topic
    // For all other topics
    inpub_id = MQTT_DEBUG_TOPIC;
  }else{
    inpub_id = MQTT_OTHER_TOPIC;
  }
}
//--------------------------------------------------------------------------------------------------
//Incoming data cb
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
  
  DC_debugOut("# Incoming message len: %d, flags %u\n", len, (unsigned int)flags);
  
  if (flags & MQTT_DATA_FLAG_LAST) {
    
    // Call function or do action depending on reference, in this case inpub_id
    if(inpub_id == MQTT_CMD_TOPIC) {
      DC_debugOut("# MQTT CMD message\r\n");
    }else if(inpub_id == MQTT_DATA_TOPIC) {
      DC_debugOut("# MQTT DATA message\r\n");
    }else if(inpub_id == MQTT_DEBUG_TOPIC) {
      DC_debugOut("# MQTT Debug message\r\n");
    } else {
      DC_debugOut("# MQTT incoming_data_cb: Ignoring payload...\r\n");
    }
    
    
  } else {
    // Handle fragmented payload, store in buffer, write to file or whatever
  }
}
//--------------------------------------------------------------------------------------------------
//Subscribe result cb
static void mqtt_sub_request_cb(void *arg, err_t result) {
  DC_debugOut("# MQTT Subscribe result: %d\n", result);
}
//--------------------------------------------------------------------------------------------------
//Connection cb
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) 
{
  err_t err;
  
  if(status == MQTT_CONNECT_ACCEPTED) {
    
    DC_debugOut("# MQTT connection_cb: Successfully connected\n");
    
    // Setup callback for incoming publish requests
    mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);
    
    // Subscribe to CMD topic
    err = mqtt_subscribe(client, DC_set.MQTT_cmd_topic, DC_set.MQTT_qos, mqtt_sub_request_cb, arg);
    
    if(err != ERR_OK) {
      DC_debugOut("# MQTT subscribe CMD return: %d\n", err);
    }
    
    // Subscribe to DATA topic
    err = mqtt_subscribe(client, DC_set.MQTT_data_topic, DC_set.MQTT_qos, mqtt_sub_request_cb, arg);
    
    if(err != ERR_OK) {
      DC_debugOut("# MQTT subscribe DATA return: %d\n", err);
    }   
    
    // Subscribe to Debug topic
    err = mqtt_subscribe(client, DC_DEF_MQTT_DEBUG_TOPIC, DC_set.MQTT_qos, mqtt_sub_request_cb, arg);
    
    if(err != ERR_OK) {
      DC_debugOut("# MQTT subscribe Debug return: %d\n", err);
    } 
    
  } else {
    DC_debugOut("# MQTT connection cb: Disconnected, reason: %d\n", status);
    
    // Its more nice to be connected, so try to reconnect
    devMQTT_connect(DC_set.MQTT_broc_ip, DC_set.MQTT_port, DC_set.MQTT_clintID, DC_set.MQTT_user, DC_set.MQTT_pass);
  }
}
//******************************************API*****************************************************
//MQTT connection
HAL_StatusTypeDef devMQTT_connect(uint8_t* MQTT_IP, uint16_t MQTT_port, char* MQTT_clintID, char* MQTT_user, char* MQTT_pass)
{
  ip4_addr_t mqttIPaddr;
  
  mqttMainClient = mqtt_client_new();
  IP4_ADDR(&mqttIPaddr, *MQTT_IP, *(MQTT_IP+1), *(MQTT_IP+2), *(MQTT_IP+3));
  
  //Settings
  strcpy((char*)ci.client_id, MQTT_clintID);
  strcpy((char*)ci.client_user, MQTT_user);
  strcpy((char*)ci.client_pass, MQTT_pass);

  if (ERR_OK == mqtt_client_connect(mqttMainClient, &mqttIPaddr, MQTT_port, mqtt_connection_cb, 0, &ci))
  {
    DC_debug_ipAdrrOut("# MQTT connection OK: ", MQTT_IP);
    return HAL_OK;    
  }
  
  return HAL_ERROR;
}
//--------------------------------------------------------------------------------------------------
//Public message
HAL_StatusTypeDef devMQTT_publish(char *topicName, uint8_t *payload, uint16_t len) {

  err_t err;
  u8_t qos = DC_set.MQTT_qos; /* 0 1 or 2, see MQTT specification */
  u8_t retain = 0; // No don't retain such crappy payload
  
  err = mqtt_publish(mqttMainClient, topicName, (const char *)payload, len, qos, retain, mqtt_pub_request_cb, 0);
  
  if(err != ERR_OK) {
    DC_debugOut("# MQTT Publish err: %d\n", err);
    return HAL_ERROR;
  }
  
  return HAL_OK;
}