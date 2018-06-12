
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
  
  devMQTT_init(emsTopics, EMS_TOPID_COUNT, &EMS_callBack); //Init MQTT
  devMQTT_connect(DC_set.MQTT_broc_ip, DC_set.MQTT_port, DC_set.MQTT_clintID, DC_set.MQTT_user, DC_set.MQTT_pass); //Connect
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
void EMS_sendVars()
{
  char *out;
  char strBuf[15];
  cJSON *root = cJSON_CreateObject();
  cJSON *vars = cJSON_CreateArray();
  cJSON *channels = cJSON_CreateArray();
  
  cJSON_AddItemToObject(root, "varibles", vars);
  cJSON_AddItemToObject(vars, "channels", channels);
  cJSON_AddNumberToObject(root, "time", 111111111);
  
  for (int i=0; i < V9203_COUNT_CHANNELS; i++)
  {

    cJSON *phaseA = NULL;
    cJSON *phaseB = NULL;
    cJSON *phaseC = NULL;
    cJSON *channel = NULL;
    
    phaseA = cJSON_CreateArray();
    cJSON_AddNumberToObject(phaseA, "freq", meshChan[i].phaseA.freq);
    cJSON_AddNumberToObject(phaseA, "RMSV", meshChan[i].phaseA.RMSV);
    cJSON_AddNumberToObject(phaseA, "RMSI", meshChan[i].phaseA.RMSI);
    cJSON_AddNumberToObject(phaseA, "RMSP", meshChan[i].phaseA.RMSP);
    cJSON_AddNumberToObject(phaseA, "RMSRP", meshChan[i].phaseA.RMSRP);
    
    phaseB = cJSON_CreateArray();
    cJSON_AddNumberToObject(phaseB, "freq", meshChan[i].phaseB.freq);
    cJSON_AddNumberToObject(phaseB, "RMSV", meshChan[i].phaseB.RMSV);
    cJSON_AddNumberToObject(phaseB, "RMSI", meshChan[i].phaseB.RMSI);
    cJSON_AddNumberToObject(phaseB, "RMSP", meshChan[i].phaseB.RMSP);
    cJSON_AddNumberToObject(phaseB, "RMSRP", meshChan[i].phaseB.RMSRP);
    
    phaseC = cJSON_CreateArray();
    cJSON_AddNumberToObject(phaseC, "freq", meshChan[i].phaseC.freq);
    cJSON_AddNumberToObject(phaseC, "RMSV", meshChan[i].phaseC.RMSV);
    cJSON_AddNumberToObject(phaseC, "RMSI", meshChan[i].phaseC.RMSI);
    cJSON_AddNumberToObject(phaseC, "RMSP", meshChan[i].phaseC.RMSP);
    cJSON_AddNumberToObject(phaseC, "RMSRP", meshChan[i].phaseC.RMSRP);
    
    channel = cJSON_CreateArray();
    cJSON_AddItemToObject(channel, "phaseA", phaseA);
    cJSON_AddItemToObject(channel, "phaseB", phaseB);
    cJSON_AddItemToObject(channel, "phaseC", phaseC);
    sprintf(strBuf, "channel_%d", i);
    
    cJSON_AddItemToObject(channels, strBuf, channel);
  }
  
  out = cJSON_Print(root);
  DC_debugOut(out);
}
//******************************************************************************
// startEMS_task function
void startEMS_task(void const * argument)
{
  vTaskDelay(2000);
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
    }
    
    //Send vars
    EMS_sendVars();

    osDelay(DC_set.EMS_out_period);
  }
}