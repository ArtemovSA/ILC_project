
#include "EMS_protocol.h"
#include "devMQTT.h"
#include "DevCTRL.h"
#include "V9203.h"
#include "cJSON.h"

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
  // topic: varibles/id/FREQ
  emsTopics[EMS_TOPID_VAR_FREQ].sub_pub = MQTT_PUB;
  sprintf(emsTopics[EMS_TOPID_VAR_FREQ].name, "%s/%s/%s", EMS_TOPIC_VAR_PREFIX, DC_unic_idef, "FREQ");
  
  // topic: varibles/id/RMSU
  emsTopics[EMS_TOPID_VAR_RMSU].sub_pub = MQTT_PUB;
  sprintf(emsTopics[EMS_TOPID_VAR_RMSU].name, "%s/%s/%s", EMS_TOPIC_VAR_PREFIX, DC_unic_idef, "RMSU");
  
  // topic: varibles/id/RMSI
  emsTopics[EMS_TOPID_VAR_RMSI].sub_pub = MQTT_PUB;
  sprintf(emsTopics[EMS_TOPID_VAR_RMSI].name, "%s/%s/%s", EMS_TOPIC_VAR_PREFIX, DC_unic_idef, "RMSI");
  
  // topic: varibles/id/RMSP
  emsTopics[EMS_TOPID_VAR_RMSP].sub_pub = MQTT_PUB;
  sprintf(emsTopics[EMS_TOPID_VAR_RMSP].name, "%s/%s/%s", EMS_TOPIC_VAR_PREFIX, DC_unic_idef, "RMSP");
  
  // topic: varibles/id/RMSRP
  emsTopics[EMS_TOPID_VAR_RMSRP].sub_pub = MQTT_PUB;
  sprintf(emsTopics[EMS_TOPID_VAR_RMSRP].name, "%s/%s/%s", EMS_TOPIC_VAR_PREFIX, DC_unic_idef, "RMSRP");

  // topic: attributes/id/OUT_PERIOD
  emsTopics[EMS_TOPID_ATTR_PERIOD].sub_pub = MQTT_SUB;
  sprintf(emsTopics[EMS_TOPID_ATTR_PERIOD].name, "%s/%s/%s", EMS_TOPIC_ATR_PREFIX, DC_unic_idef, "OUT_PERIOD");
  
  // topic: debug/id
  emsTopics[EMS_TOPID_DEBUG].sub_pub = MQTT_PUB_SUB;
  sprintf(emsTopics[EMS_TOPID_DEBUG].name, "%s/%s" ,EMS_TOPIC_DEB_PREFIX, DC_unic_idef);
  
  //devMQTT_init(emsTopics, EMS_TOPID_COUNT, &EMS_callBack); //Init MQTT
  //devMQTT_connect(DC_set.MQTT_broc_ip, DC_set.MQTT_port, DC_set.MQTT_clintID, DC_set.MQTT_user, DC_set.MQTT_pass); //Connect

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
    case EMS_TOPID_ATTR_PERIOD: DC_debugOut("# CALL ATTR PERIOD\r\n");
    case EMS_TOPID_DEBUG: DC_debugOut("# CALL DEBUG\r\n");
    }
}

//------------------------------------------------------------------------------
//Send vars
void EMS_sendVars(uint8_t channel_num)
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
  cJSON_AddNumberToObject(phaseA, "freq", meshChan[channel_num].phaseA.freq);
  cJSON_AddNumberToObject(phaseA, "RMSV", meshChan[channel_num].phaseA.RMSV);
  cJSON_AddNumberToObject(phaseA, "RMSI", meshChan[channel_num].phaseA.RMSI);
  cJSON_AddNumberToObject(phaseA, "RMSP", meshChan[channel_num].phaseA.RMSP);
  cJSON_AddNumberToObject(phaseA, "RMSRP", meshChan[channel_num].phaseA.RMSRP);
  
  phaseB = cJSON_CreateObject();
  cJSON_AddNumberToObject(phaseB, "freq", meshChan[channel_num].phaseB.freq);
  cJSON_AddNumberToObject(phaseB, "RMSV", meshChan[channel_num].phaseB.RMSV);
  cJSON_AddNumberToObject(phaseB, "RMSI", meshChan[channel_num].phaseB.RMSI);
  cJSON_AddNumberToObject(phaseB, "RMSP", meshChan[channel_num].phaseB.RMSP);
  cJSON_AddNumberToObject(phaseB, "RMSRP", meshChan[channel_num].phaseB.RMSRP);
  
  phaseC = cJSON_CreateObject();
  cJSON_AddNumberToObject(phaseC, "freq", meshChan[channel_num].phaseC.freq);
  cJSON_AddNumberToObject(phaseC, "RMSV", meshChan[channel_num].phaseC.RMSV);
  cJSON_AddNumberToObject(phaseC, "RMSI", meshChan[channel_num].phaseC.RMSI);
  cJSON_AddNumberToObject(phaseC, "RMSP", meshChan[channel_num].phaseC.RMSP);
  cJSON_AddNumberToObject(phaseC, "RMSRP", meshChan[channel_num].phaseC.RMSRP);
  
  channel = cJSON_CreateObject();
  cJSON_AddItemToObject(channel, "phaseA", phaseA);
  cJSON_AddItemToObject(channel, "phaseB", phaseB);
  cJSON_AddItemToObject(channel, "phaseC", phaseC);
  sprintf(strBuf, "channel_%d", channel_num);
  
  cJSON_AddItemToObject(vars, strBuf, channel);
  cJSON_AddItemToObject(root, "varibles", vars);
  cJSON_AddNumberToObject(root, "time", 111111111);
  
  out = cJSON_Print(root);
  cJSON_Delete(root);
  DC_debugOut(out);
  DC_debugOut("out len:%d", strlen(out));

  devMQTT_publish("qq", (uint8_t*)out, strlen(out));
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
      
      DC_debugOut("@ ch %d FREQ A: %.3f | FREQ B: %.3f | FREQ C: %.3f\r\n", i, meshChan[i].phaseA.freq, meshChan[i].phaseB.freq, meshChan[i].phaseC.freq);
      
      //Get RMS voltage
      meshChan[i].phaseA.RMSV = V9203_getRMS_Voltage(i, LINE_A);
      meshChan[i].phaseB.RMSV = V9203_getRMS_Voltage(i, LINE_B);
      meshChan[i].phaseC.RMSV = V9203_getRMS_Voltage(i, LINE_C);
      
      DC_debugOut("@ ch %d RMSU A: %.3f | RMSU B: %.3f | RMSU C: %.3f\r\n", i, meshChan[i].phaseA.RMSV, meshChan[i].phaseB.RMSV, meshChan[i].phaseC.RMSV);
      
      //Get RMS current
      meshChan[i].phaseA.RMSI = V9203_getRMS_Current(i, LINE_A);
      meshChan[i].phaseB.RMSI = V9203_getRMS_Current(i, LINE_B);
      meshChan[i].phaseC.RMSI = V9203_getRMS_Current(i, LINE_C);
      
      DC_debugOut("@ ch %d RMSI A: %.3f | RMSI B: %.3f | RMSI C: %.3f\r\n", i, meshChan[i].phaseA.RMSI, meshChan[i].phaseB.RMSI, meshChan[i].phaseC.RMSI);
      
      //Get RMS power
      meshChan[i].phaseA.RMSP = V9203_getRMS_Power(i, LINE_A);
      meshChan[i].phaseB.RMSP = V9203_getRMS_Power(i, LINE_B);
      meshChan[i].phaseC.RMSP = V9203_getRMS_Power(i, LINE_C);
      
      DC_debugOut("@ ch %d RMSP A: %.3f | RMSP B: %.3f | RMSP C: %.3f\r\n", i, meshChan[i].phaseA.RMSP, meshChan[i].phaseB.RMSP, meshChan[i].phaseC.RMSP);
      
      //Get RMSRP power
      meshChan[i].phaseA.RMSRP = V9203_getRMS_reactivePower(i, LINE_A);
      meshChan[i].phaseB.RMSRP = V9203_getRMS_reactivePower(i, LINE_B);
      meshChan[i].phaseC.RMSRP = V9203_getRMS_reactivePower(i, LINE_C);
      
      DC_debugOut("@ ch %d RMSRP A: %.3f | RMSRP B: %.3f | RMSRP C: %.3f\r\n\r\n", i, meshChan[i].phaseA.RMSRP, meshChan[i].phaseB.RMSRP, meshChan[i].phaseC.RMSRP);
      
      //Send vars
      //EMS_sendVars(i);
    }

    vTaskDelay(DC_set.EMS_out_period);
  }
}