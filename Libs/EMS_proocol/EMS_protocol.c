
#include "EMS_protocol.h"
#include "devMQTT.h"
#include "DevCTRL.h"

//topic list
devMQTT_topic emsTopics[EMS_TOPID_COUNT];

//------------------------------------------------------------------------------
//Init EMS
void EMS_init()
{
  // topic: varibles/id
  emsTopics[EMS_TOPID_VAR_OUT].sub_pub = MQTT_PUB;
  sprintf(emsTopics[EMS_TOPID_VAR_OUT].name, "%s/%s" ,EMS_TOPIC_VAR_PREFIX, DC_unic_idef);
  // topic: debug/id
  emsTopics[EMS_TOPID_DEBUG].sub_pub = MQTT_PUB_SUB;
  sprintf(emsTopics[EMS_TOPID_DEBUG].name, "%s/%s" ,EMS_TOPIC_DEB_PREFIX, DC_unic_idef);
  
  devMQTT_init(emsTopics, EMS_TOPID_COUNT); //Init MQTT
  devMQTT_connect(DC_set.MQTT_broc_ip, DC_set.MQTT_port, DC_set.MQTT_clintID, DC_set.MQTT_user, DC_set.MQTT_pass); //Connect
}

