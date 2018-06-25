
#include "EMS_protocol.h"
#include "devMQTT.h"
#include "DevCTRL.h"
#include "V9203.h"
#include "cJSON.h"
#include "Clock.h"

//topic list
devMQTT_topic emsTopics[EMS_TOPID_COUNT];
extern SPI_HandleTypeDef hspi1;

//Measure
EMC_chan_mesh_t meshChan[V9203_COUNT_CHANNELS];

//Task
osThreadId EMS_taskHandle;
osMessageQId EMS_TTqueueHandle;

//Callback
void EMS_callBack(uint16_t topic_ID, uint8_t* data, uint16_t len);

//------------------------------------------------------------------------------
//Init EMS
void EMS_init()
{   
  // topic: varibles/id/channel
  emsTopics[EMS_TOPID_VAR_CHAN].sub_pub = MQTT_PUB;
  sprintf(emsTopics[EMS_TOPID_VAR_CHAN].name, "%s/%s/%s", EMS_TOPIC_VAR_PREFIX, DC_unic_idef, EMS_TOPIC_CHANNEL);
  
  // topic: attributes/id/main_set
  emsTopics[EMS_TOPID_ATTR_MAIN_SET].sub_pub = MQTT_PUB_SUB;
  sprintf(emsTopics[EMS_TOPID_ATTR_MAIN_SET].name, "%s/%s/%s", EMS_TOPIC_ATR_PREFIX, DC_unic_idef, EMS_TOPIC_MAIN_SETTINGS);
    
  // topic: attributes/id/calibrate
  emsTopics[EMS_TOPID_ATTR_CALIBR].sub_pub = MQTT_PUB_SUB;
  sprintf(emsTopics[EMS_TOPID_ATTR_CALIBR].name, "%s/%s/%s", EMS_TOPIC_ATR_PREFIX, DC_unic_idef, EMS_TOPIC_CALIBRATE);
  
  // topic: debug/id
  emsTopics[EMS_TOPID_DEBUG].sub_pub = MQTT_PUB_SUB;
  sprintf(emsTopics[EMS_TOPID_DEBUG].name, "%s/%s" ,EMS_TOPIC_DEB_PREFIX, DC_unic_idef);
  
  //MQTT connection
  devMQTT_init(emsTopics, EMS_TOPID_COUNT, &EMS_callBack); //Init MQTT
  devMQTT_connect(DC_set.MQTT_broc_ip[DC_set.MQTT_activeBrock], DC_set.MQTT_port, DC_set.MQTT_clintID, DC_set.MQTT_user, DC_set.MQTT_pass); //Connect

  //JSON init
  cJSON_Hooks hooks;
  hooks.malloc_fn = pvPortMalloc;
  hooks.free_fn = vPortFree;
  cJSON_InitHooks(&hooks);
}
//------------------------------------------------------------------------------
//Callback
void EMS_callBack(uint16_t topic_ID, uint8_t* data, uint16_t len)
{
    switch (topic_ID)
    {
    case EMS_TOPID_ATTR_MAIN_SET: DC_debugOut("# CALL ATTR MAIN\r\n"); break;
    case EMS_TOPID_ATTR_CALIBR: DC_debugOut("# CALL ATTR CALIBRATE\r\n"); break;
    case EMS_TOPID_DEBUG: DC_debugOut("# CALL DEBUG\r\n"); break;
    default: DC_debugOut("# CALL NON TOPIC\r\n");
    }
}

//------------------------------------------------------------------------------
//Send Channel vars
void EMS_sendChannelVars(uint8_t channel_num)
{
  char *out;
  char strBuf[15];
  cJSON *root = cJSON_CreateObject();
  cJSON *vars = cJSON_CreateObject();
  cJSON *phaseA = NULL;
  cJSON *phaseB = NULL;
  cJSON *phaseC = NULL;
  cJSON *channel = NULL;
  
  phaseA = cJSON_CreateObject();
  cJSON_AddNumberToObject(phaseA, EMS_JSON_NAME_FREQ, meshChan[channel_num].phaseA.freq);
  cJSON_AddNumberToObject(phaseA, EMS_JSON_NAME_RMSV, meshChan[channel_num].phaseA.RMSV);
  cJSON_AddNumberToObject(phaseA, EMS_JSON_NAME_RMSI, meshChan[channel_num].phaseA.RMSI);
  cJSON_AddNumberToObject(phaseA, EMS_JSON_NAME_RMSP, meshChan[channel_num].phaseA.RMSP);
  cJSON_AddNumberToObject(phaseA, EMS_JSON_NAME_RMSRP, meshChan[channel_num].phaseA.RMSRP);
  
  phaseB = cJSON_CreateObject();
  cJSON_AddNumberToObject(phaseB, EMS_JSON_NAME_FREQ, meshChan[channel_num].phaseB.freq);
  cJSON_AddNumberToObject(phaseB, EMS_JSON_NAME_RMSV, meshChan[channel_num].phaseB.RMSV);
  cJSON_AddNumberToObject(phaseB, EMS_JSON_NAME_RMSI, meshChan[channel_num].phaseB.RMSI);
  cJSON_AddNumberToObject(phaseB, EMS_JSON_NAME_RMSP, meshChan[channel_num].phaseB.RMSP);
  cJSON_AddNumberToObject(phaseB, EMS_JSON_NAME_RMSRP, meshChan[channel_num].phaseB.RMSRP);
  
  phaseC = cJSON_CreateObject();
  cJSON_AddNumberToObject(phaseC, EMS_JSON_NAME_FREQ, meshChan[channel_num].phaseC.freq);
  cJSON_AddNumberToObject(phaseC, EMS_JSON_NAME_RMSV, meshChan[channel_num].phaseC.RMSV);
  cJSON_AddNumberToObject(phaseC, EMS_JSON_NAME_RMSI, meshChan[channel_num].phaseC.RMSI);
  cJSON_AddNumberToObject(phaseC, EMS_JSON_NAME_RMSP, meshChan[channel_num].phaseC.RMSP);
  cJSON_AddNumberToObject(phaseC, EMS_JSON_NAME_RMSRP, meshChan[channel_num].phaseC.RMSRP);
  
  channel = cJSON_CreateObject();
  cJSON_AddItemToObject(channel, "phaseA", phaseA);
  cJSON_AddItemToObject(channel, "phaseB", phaseB);
  cJSON_AddItemToObject(channel, "phaseC", phaseC);
  sprintf(strBuf, "channel_%d", channel_num);
  
  cJSON_AddItemToObject(vars, strBuf, channel);
  cJSON_AddItemToObject(root, "varibles", vars);
  
  time_t timestamp;
  if ( CL_getSystem_Timestamp(&timestamp) == HAL_OK)//Get system timestamp
  {
    cJSON_AddNumberToObject(root, "time", (uint32_t)timestamp);
  }else{
    cJSON_AddNumberToObject(root, "time", 0);
  }
  
  out = cJSON_Print(root);
  cJSON_Delete(root);
  //DC_debugOut(out);
  
  devMQTT_publish(emsTopics[EMS_TOPID_VAR_CHAN].name, (uint8_t*)out, strlen(out), DC_set.MQTT_qos);
  vPortFree(out);
}
//******************************************************************************
// startEMS_task function
void startEMS_task(void const * argument)
{ 
  V9203_init(&hspi1);
  EMS_init();
  
  while(1)
  {
    for (int i=0; i < V9203_COUNT_CHANNELS; i++)
    {
      //Get frequency
      meshChan[i].phaseA.freq = V9203_getFreq(i, LINE_A);
      meshChan[i].phaseB.freq = V9203_getFreq(i, LINE_B);
      meshChan[i].phaseC.freq = V9203_getFreq(i, LINE_C);
      
      DC_debugOut("@ ch %d FREQ A: %2f | FREQ B: %2f | FREQ C: %2f\r\n", i, meshChan[i].phaseA.freq, meshChan[i].phaseB.freq, meshChan[i].phaseC.freq);
      
      //Get RMS voltage
      meshChan[i].phaseA.RMSV = V9203_getRMS_Voltage(i, LINE_A);
      meshChan[i].phaseB.RMSV = V9203_getRMS_Voltage(i, LINE_B);
      meshChan[i].phaseC.RMSV = V9203_getRMS_Voltage(i, LINE_C);
      
      DC_debugOut("@ ch %d RMSU A: %2f | RMSU B: %2f | RMSU C: %2f\r\n", i, meshChan[i].phaseA.RMSV, meshChan[i].phaseB.RMSV, meshChan[i].phaseC.RMSV);
      
      //Get RMS current
      meshChan[i].phaseA.RMSI = V9203_getRMS_Current(i, LINE_A);
      meshChan[i].phaseB.RMSI = V9203_getRMS_Current(i, LINE_B);
      meshChan[i].phaseC.RMSI = V9203_getRMS_Current(i, LINE_C);
      
      DC_debugOut("@ ch %d RMSI A: %2f | RMSI B: %2f | RMSI C: %2f\r\n", i, meshChan[i].phaseA.RMSI, meshChan[i].phaseB.RMSI, meshChan[i].phaseC.RMSI);
      
      //Get RMS power
      meshChan[i].phaseA.RMSP = V9203_getRMS_Power(i, LINE_A);
      meshChan[i].phaseB.RMSP = V9203_getRMS_Power(i, LINE_B);
      meshChan[i].phaseC.RMSP = V9203_getRMS_Power(i, LINE_C);
      
      DC_debugOut("@ ch %d RMSP A: %2f | RMSP B: %2f | RMSP C: %2f\r\n", i, meshChan[i].phaseA.RMSP, meshChan[i].phaseB.RMSP, meshChan[i].phaseC.RMSP);
      
      //Get RMSRP power
      meshChan[i].phaseA.RMSRP = V9203_getRMS_reactivePower(i, LINE_A);
      meshChan[i].phaseB.RMSRP = V9203_getRMS_reactivePower(i, LINE_B);
      meshChan[i].phaseC.RMSRP = V9203_getRMS_reactivePower(i, LINE_C);
      
      DC_debugOut("@ ch %d RMSRP A: %2f | RMSRP B: %2f | RMSRP C: %2f\r\n", i, meshChan[i].phaseA.RMSRP, meshChan[i].phaseB.RMSRP, meshChan[i].phaseC.RMSRP);
      
      //Send vars
      EMS_sendChannelVars(i);
    }
    
    vTaskDelay(DC_set.EMS_out_period*1000);
  }
}