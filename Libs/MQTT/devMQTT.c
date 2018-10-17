
#include "devMQTT.h"
#include "DevCTRL.h"
#include "Network.h"
#include "cJSON.h"

static int inpub_id; //topic ID
mqtt_client_t *mqttMainClient; //Client
devMQTT_topic* devMQTT_topics; //Topics list
uint16_t devMQTT_cntTop; //Count
void (*devMQTT_callBack)(uint16_t, uint8_t*, uint16_t); //Func id, data, len
static uint8_t incMessage[MQTT_INC_BUF_SIZE];
static uint16_t incMessageLen;

//--------------------------------------------------------------------------------------------------
//Init
// args: topics list
//       count topics
//       callBackPointer
void devMQTT_init(devMQTT_topic* topics, uint16_t count, void (*callBackPoint)(uint16_t, uint8_t*, uint16_t))
{
  devMQTT_topics = topics;
  devMQTT_cntTop = count;
  devMQTT_callBack = callBackPoint;
}
//*******************************************Callback***********************************************
//Publish cb
static void mqtt_pub_request_cb(void *arg, err_t result) {
  if(result != ERR_OK) {
    DC_debugOut("# MQTT Publish cb error result: %d\r\n", result);
    
    if (result == ERR_BUF)
    {
      mqttMainClient->output.get = 0;
      mqttMainClient->output.put = 0;
    }
    
  }else{
    DC_debugOut("# MQTT Publish cb OK \r\n");
  }
}
//--------------------------------------------------------------------------------------------------
//Incoming publish cb
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) {
  printf("Incoming publish at topic %s with total length %u\n", topic, (unsigned int)tot_len);
  
  // Decode topic string into a user defined reference
  for (int i=0; i<devMQTT_cntTop; i++)
  {
    if (strcmp(topic, devMQTT_topics[i].name) == 0)
    {
      inpub_id = i;
    }
  }
}
//--------------------------------------------------------------------------------------------------
//Incoming data cb
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
  
  DC_debugOut("# Incoming message len: %d, flags %u\n", len, (unsigned int)flags);
  
  //Check buf len
  if ((incMessageLen+len) < MQTT_INC_BUF_SIZE)
  {
    memcpy(incMessage+incMessageLen, data, len);
    incMessageLen += len;
  }else{
    incMessageLen = 0;
  }
  
  if (flags & MQTT_DATA_FLAG_LAST) {
    
    // Call function or do action depending on reference, in this case inpub_id
    devMQTT_callBack(inpub_id, incMessage, incMessageLen);
    incMessageLen = 0;
    
  } else {
    // Handle fragmented payload, store in buffer, write to file or whatever
  }
}
//--------------------------------------------------------------------------------------------------
//Subscribe result cb
static void mqtt_sub_request_cb(void *arg, err_t result) {
  DC_debugOut("# MQTT Subscribe cb result: %d\n", result);
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
    
    // Subscribe
    for (int i=0; i<devMQTT_cntTop; i++)
    {
      if ((devMQTT_topics[i].sub_pub == MQTT_SUB) || (devMQTT_topics[i].sub_pub == MQTT_PUB_SUB))
      {
        err = mqtt_subscribe(client, devMQTT_topics[i].name, DC_set.MQTT_qos, mqtt_sub_request_cb, arg);
        
        if(err != ERR_OK) {
          DC_debugOut("# MQTT subscribe error %s return: %d\n", devMQTT_topics[i].name, err);
        }else{
          DC_debugOut("# MQTT subscribe OK: %s\r\n", devMQTT_topics[i].name);
        }
        
      }
    }
    
  } else {
    
    DC_debugOut("# MQTT connection cb: Disconnected, reason: %d\n", status);
    
    if (mqttMainClient == NULL)
      mqttMainClient = mqtt_client_new();
    
    //MQTT connection by source
    devMQTT_conBySource();
    
  }
}
//******************************************API*****************************************************
//MQTT connection by source
DEV_Status_t devMQTT_conBySource()
{
  DEV_Status_t stat = DEV_ERROR;
    
  if (DC_set.MQTT_broc_ch == 0)
  {
    if ((stat = devMQTT_conByIP(DC_set.MQTT_broc_ip, DC_set.MQTT_port, DC_unic_idStr, DC_set.MQTT_user, DC_set.MQTT_pass)) == DEV_OK)
    {
      DC_state.mqttLink = 1;
      DC_debug_ipAdrrOut("# MQTT connection OK by IP#: ", DC_set.MQTT_broc_ip);
    }else{
      DC_state.mqttLink = 0;
      DC_debug_ipAdrrOut("# MQTT connection ERROR by IP#: ", DC_set.MQTT_broc_ip);
    }
    
  }else{
    uint8_t broc_ip[4];
    
    NW_getIP_byDomen(DC_set.MQTT_broc_domen, broc_ip);
    
    if ((stat = devMQTT_conByIP(broc_ip, DC_set.MQTT_port, DC_unic_idStr, DC_set.MQTT_user, DC_set.MQTT_pass)) == DEV_OK)
    {
      DC_state.mqttLink = 1;
      DC_debugOut("# MQTT connection OK by Domen#: %s OK\r\r\n", DC_set.MQTT_broc_domen);
    }else{
      DC_state.mqttLink = 0;
      DC_debugOut("# MQTT connection ERROR by Domen#: %s OK\r\r\n", DC_set.MQTT_broc_domen);
    }
  }
  
  return stat;
}
//--------------------------------------------------------------------------------------------------
//MQTT connection
DEV_Status_t devMQTT_conByIP(uint8_t* MQTT_IP, uint16_t MQTT_port, char* MQTT_clintID, char* MQTT_user, char* MQTT_pass)
{
  ip4_addr_t mqttIPaddr;
  struct mqtt_connect_client_info_t ci; //Client info
  
  if (mqttMainClient == NULL)
    mqttMainClient = mqtt_client_new();
  
  if (mqttMainClient == NULL)
  {
    DC_debugOut("# MQTT Client create error");
    return DEV_ERROR;
  }
  
  IP4_ADDR(&mqttIPaddr, *MQTT_IP, *(MQTT_IP+1), *(MQTT_IP+2), *(MQTT_IP+3));
  
  //Settings
  memset(&ci, 0, sizeof(ci));
  ci.client_id = MQTT_clintID;
  ci.client_user = MQTT_user;
  ci.client_pass = MQTT_pass;
  ci.keep_alive = 100;

  if (mqtt_client_connect(mqttMainClient, &mqttIPaddr, MQTT_port, mqtt_connection_cb, NULL, &ci) == ERR_OK)
  {
    return DEV_OK;    
  }else{
    mqtt_disconnect(mqttMainClient);
    mem_free(mqttMainClient);
    mqttMainClient = mqtt_client_new();
    if (mqttMainClient == NULL)
    {
      DC_debugOut("# MQTT Client create error");
      return DEV_ERROR;
    }
    if (mqtt_client_connect(mqttMainClient, &mqttIPaddr, MQTT_port, mqtt_connection_cb, NULL, &ci) == ERR_OK)
    {
      DC_debugOut("# recoonect OK\r\n");
      return DEV_OK;
    }
  }
  
  return DEV_ERROR;
}
//--------------------------------------------------------------------------------------------------
//Public message
HAL_StatusTypeDef devMQTT_publish(char *topicName, uint8_t *payload, uint16_t len, uint8_t qos) {

  err_t err;
  u8_t retain = 0; // No don't retain such crappy payload
  
  err = mqtt_publish(mqttMainClient, topicName, (const char *)payload, len, qos, retain, mqtt_pub_request_cb, 0);
  DC_debugOut("# MQTT Publish: %s\r\n", topicName);
  
  if(err != ERR_OK) {
    DC_debugOut("# MQTT Publish err: %d\r\n", err);
    return HAL_ERROR;
  }
  
  return HAL_OK;
}