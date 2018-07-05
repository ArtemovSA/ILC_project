
#include "EMS_protocol.h"
#include "devMQTT.h"
#include "DevCTRL.h"
#include "V9203.h"
#include "cJSON.h"
#include "Clock.h"
#include "Memory.h"
#include "Network.h"

//topic list
devMQTT_topic emsTopics[EMS_TOPID_COUNT];
extern SPI_HandleTypeDef hspi1;

//Measure
EMC_chan_mesh_t meshChan[DC_V9203_COUNT_CHANNELS];

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
  
  // topic: ctrl/id/
  emsTopics[EMS_TOPID_CTRL].sub_pub = MQTT_PUB_SUB;
  sprintf(emsTopics[EMS_TOPID_CTRL].name, "%s/%s", EMS_TOPIC_CTRL_PREFIX, DC_unic_idef);
  
  // topic: debug/id
  emsTopics[EMS_TOPID_DEBUG].sub_pub = MQTT_PUB_SUB;
  sprintf(emsTopics[EMS_TOPID_DEBUG].name, "%s/%s" ,EMS_TOPIC_DEB_PREFIX, DC_unic_idef);
  
  //MQTT connection
  devMQTT_init(emsTopics, EMS_TOPID_COUNT, &EMS_callBack); //Init MQTT
  devMQTT_connect(DC_set.MQTT_broc_ip[DC_set.MQTT_activeBrock], DC_set.MQTT_port, DC_unic_idStr, DC_set.MQTT_user, DC_set.MQTT_pass); //Connect

  //JSON init
  cJSON_Hooks hooks;
  hooks.malloc_fn = pvPortMalloc;
  hooks.free_fn = vPortFree;
  cJSON_InitHooks(&hooks);
}
//------------------------------------------------------------------------------
//Get ip addres by name in JSON
HAL_StatusTypeDef EMS_JSON_getIPAdrr(const cJSON* objectJSON, char *JSON_name, uint8_t* addrIP)
{
  cJSON *jNet_ip = cJSON_GetObjectItemCaseSensitive(objectJSON, JSON_name);
  if (jNet_ip != NULL)
  {
    if (cJSON_IsNumber(jNet_ip) && (jNet_ip->valueint != NULL))
    {
      NW_convIntIPtoArray(addrIP, jNet_ip->valueint);//Convert int ip ti array
      return HAL_OK;
    }
  }
  
  return HAL_ERROR;
}
//------------------------------------------------------------------------------
//Get str Val by name in JSON
HAL_StatusTypeDef EMS_JSON_getStr(const cJSON* objectJSON, char *JSON_name, char* str, uint8_t maxLen)
{
  cJSON *jNet_ip = cJSON_GetObjectItemCaseSensitive(objectJSON, JSON_name);
  if (jNet_ip != NULL)
  {
    if (cJSON_IsString(jNet_ip) && (jNet_ip->valuestring != NULL))
    {
      if (strlen(jNet_ip->valuestring) <= maxLen)
      {
        strcpy(str, jNet_ip->valuestring);
        return HAL_OK;
      }
    }
  }
  
  return HAL_ERROR;
}
//------------------------------------------------------------------------------
//Get int Val by name in JSON
HAL_StatusTypeDef EMS_JSON_getInt(const cJSON* objectJSON, char *JSON_name, uint16_t* val)
{
  cJSON *jNet_ip = cJSON_GetObjectItemCaseSensitive(objectJSON, JSON_name);
  if (jNet_ip != NULL)
  {
    if (cJSON_IsNumber(jNet_ip) && (jNet_ip->valueint != NULL))
    {
      *val = (uint16_t) jNet_ip->valueint;
      return HAL_OK;
    }
  }
  
  return HAL_ERROR;
}
//------------------------------------------------------------------------------
//Ctrl callback
HAL_StatusTypeDef EMS_ctrlCallback(uint8_t* data, uint16_t len)
{
  //Main struct
  cJSON *ctrl_json = cJSON_Parse((char*)data);
  
  if (set_json == NULL)
  {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL)
    {
      DC_debugOut("# JSON parce Error before: %s\n", error_ptr);
    }
    return HAL_ERROR;
  }
  
  //Check child empty
  if (set_json->child == NULL)
  {
    DC_debugOut("# JSON message empty\r\n");
    return HAL_ERROR;
  }    
  
  
}
//------------------------------------------------------------------------------
//Set main settings
HAL_StatusTypeDef EMS_setMain_set(uint8_t* data, uint16_t len)
{
  //Create and copy settings
  DC_set_t settings = DC_set;
  
  //Main struct
  cJSON *set_json = cJSON_Parse((char*)data);
  
  if (set_json == NULL)
  {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL)
    {
      DC_debugOut("# JSON parce Error before: %s\n", error_ptr);
    }
    return HAL_ERROR;
  }
  
  //Check child empty
  if (set_json->child == NULL)
  {
    DC_debugOut("# JSON message empty\r\n");
    return HAL_ERROR;
  }    
  
  //***********************************Network struct********************************************
  //Get cJSON net set section
  cJSON *set_net_json = cJSON_GetObjectItemCaseSensitive(set_json, EMS_JSON_SET_NET_SET);
  
  if (set_net_json != NULL)
  {
    //SET NET IP
    EMS_JSON_getIPAdrr(set_net_json, EMS_JSON_SET_NET_IP, settings.net_dev_ip_addr);
    
    //SET NET GATEWAY
    EMS_JSON_getIPAdrr(set_net_json, EMS_JSON_SET_NET_GATEIP, settings.net_gw_ip_addr);
    
    //SET NET MASK
    EMS_JSON_getIPAdrr(set_net_json, EMS_JSON_SET_NET_MASK, settings.net_mask);
    
    //SET_NTP_NAME
    EMS_JSON_getStr(set_net_json, EMS_JSON_SET_NTP_NAME, settings.netNTP_server, sizeof(settings.netNTP_server));
    
    //SET_DNS_IP
    EMS_JSON_getIPAdrr(set_net_json, EMS_JSON_SET_DNS_IP(1), settings.serverDNS1);
    EMS_JSON_getIPAdrr(set_net_json, EMS_JSON_SET_DNS_IP(2), settings.serverDNS2);
  }
  
  //***********************************MQTT struct**********************************************
  //Get cJSON MQTT set section
  cJSON *set_mqtt_json = cJSON_GetObjectItemCaseSensitive(set_json, EMS_JSON_SET_MQTT_SET);
  
  if (set_mqtt_json != NULL)
  {
    //Set MQTT ip
    for (int i=0; i<DC_COUNT_IP_BROCK_ADDR; i++)
      EMS_JSON_getIPAdrr(set_mqtt_json, EMS_JSON_SET_MQTT_IP(i), settings.MQTT_broc_ip[i]);
    
    //Set MQTT name
    for (int i=0; i<DC_COUNT_IP_BROCK_ADDR; i++)
      EMS_JSON_getStr(set_mqtt_json, EMS_JSON_SET_MQTT_NAME(i), settings.MQTT_broc_name[i], sizeof(settings.MQTT_broc_name[i]));
    
    //Set active brocker
    uint16_t brockID;
    EMS_JSON_getInt(set_mqtt_json, EMS_JSON_SET_ACT_BROCK, &brockID);
    if (brockID <= DC_COUNT_BROCKS)
      settings.MQTT_activeBrock = (uint8_t)brockID;
    
    //Set MQTT port
    EMS_JSON_getInt(set_mqtt_json, EMS_JSON_SET_MQTT_PORT, &settings.MQTT_port);
    
    //Set MQTT user
    EMS_JSON_getStr(set_mqtt_json, EMS_JSON_SET_MQTT_USER, settings.MQTT_user, sizeof(settings.MQTT_user));
    
    //Set MQTT pass
    EMS_JSON_getStr(set_mqtt_json, EMS_JSON_SET_MQTT_PASS, settings.MQTT_pass, sizeof(settings.MQTT_pass));
    
  }
  
  //***********************************EMS struct**********************************************
  //Get cJSON EMS set section
  cJSON *set_ems_json = cJSON_GetObjectItemCaseSensitive(set_json, EMS_JSON_SET_EMS_SET);
  
  if (set_ems_json != NULL)
  {
    //Set out period
    uint16_t outPeriod;
    EMS_JSON_getInt(set_ems_json, EMS_JSON_SET_EMS_OUT_PER, &outPeriod);
    if (outPeriod > 5)
      settings.EMS_out_period = outPeriod;
  }  
  
  //***********************************Settings copy*******************************************
  //Write settings
  if (DC_writeSet(&settings, MEM_NAND_ADDR_SETTINGS) == HAL_OK)
  {
    DC_set = settings; //Copy new settings
    DC_debugOut(" # JSON settings set OK\r\n");
  }else{
    DC_debugOut(" # JSON settings set ERROR\r\n");
  }
  
  return HAL_OK;
}
//------------------------------------------------------------------------------
//Set phase calibrate
HAL_StatusTypeDef EMS_setPahseCalibrate(cJSON* cal_json, char* jsonName, JBRE_t* phaseCalVal)
{
  cJSON *phase_cal_json = cJSON_GetObjectItemCaseSensitive(cal_json, jsonName);
  uint16_t calVal;
  
  if (phase_cal_json != NULL)
  {
    //Get WARTU
    EMS_JSON_getInt(phase_cal_json, EMS_JSON_CAL_WARTU, &calVal);
    phaseCalVal->RacWARTU = calVal;
  }
  
  return HAL_OK;
}
//------------------------------------------------------------------------------
//Set calibrate coefficients
HAL_StatusTypeDef EMS_setCalibrate(uint8_t* data, uint16_t len)
{  
  //Calibrate struct
  cJSON *cal_json = cJSON_Parse((char*)data);
  
  if (cal_json == NULL)
  {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL)
    {
      DC_debugOut("# JSON parce Error before: %s\n", error_ptr);
    }
    return HAL_ERROR;
  }
  
  //Check child empty
  if (cal_json->child == NULL)
  {
    DC_debugOut("# JSON message empty\r\n");
    return HAL_ERROR;
  }
  
  //Get channel num
  uint16_t channelNum;
  V9203_calChannel_t channelCal;
  
  EMS_JSON_getInt(cal_json, EMS_JSON_CAL_CHANNEL, &channelNum);
  
  //Check channel num
  if ((channelNum > DC_V9203_COUNT_CHANNELS) || (channelNum == 0))
  {
    DC_debugOut("# Channel number out of range\r\n");
    return HAL_ERROR;
  }

  //***********************************Calibrate phase********************************************
  
  //Set phaseA calibrate
  if (EMS_setPahseCalibrate(cal_json, EMS_JSON_CAL_PHASE(A), &channelCal.calPhaseA) != HAL_OK)
  {
    DC_debugOut("# Phase A calibrate error\r\n");
    return HAL_ERROR;
  }
  
  //Set phaseB calibrate
  if (EMS_setPahseCalibrate(cal_json, EMS_JSON_CAL_PHASE(B), &channelCal.calPhaseB) != HAL_OK)
  {
    DC_debugOut("# Phase B calibrate error\r\n");
    return HAL_ERROR;
  }
  
  //Set phaseC calibrate
  if (EMS_setPahseCalibrate(cal_json, EMS_JSON_CAL_PHASE(C), &channelCal.calPhaseC) != HAL_OK)
  {
    DC_debugOut("# Phase C calibrate error\r\n");
    return HAL_ERROR;
  }
  
  //Get phase gains
  uint16_t gainCal;
  EMS_JSON_getInt(cal_json, EMS_JSON_CAL_GAIN_U, &gainCal);
  channelCal.gainKoef_U = gainCal;
  EMS_JSON_getInt(cal_json, EMS_JSON_CAL_GAIN_I, &gainCal);
  channelCal.gainKoef_I = gainCal;
  EMS_JSON_getInt(cal_json, EMS_JSON_CAL_GAIN_P, &gainCal);
  channelCal.gainKoef_P = gainCal;
  
  //Change params
  switch(channelNum)
  {
  case 1: DC_set.V9203_ch1_cal = channelCal; DC_debugOut(" # Getted calirb for ch: %d\r\n", channelNum); break;
  case 2: DC_set.V9203_ch2_cal = channelCal; DC_debugOut(" # Getted calirb for ch: %d\r\n", channelNum); break;
  case 3: DC_set.V9203_ch3_cal = channelCal; DC_debugOut(" # Getted calirb for ch: %d\r\n", channelNum); break;
  case 4: DC_set.V9203_ch4_cal = channelCal; DC_debugOut(" # Getted calirb for ch: %d\r\n", channelNum); break;
  default: DC_debugOut("# Channel number out of range\r\n");
  }
    
  return HAL_OK;
}
//------------------------------------------------------------------------------
//Callback
void EMS_callBack(uint16_t topic_ID, uint8_t* data, uint16_t len)
{
    switch (topic_ID)
    {
    case EMS_TOPID_ATTR_MAIN_SET: DC_debugOut("# CALL ATTR MAIN\r\n"); taskENTER_CRITICAL(); EMS_setMain_set(data, len); taskEXIT_CRITICAL(); break;
    case EMS_TOPID_ATTR_CALIBR: DC_debugOut("# CALL ATTR CALIBRATE\r\n"); taskENTER_CRITICAL(); EMS_setCalibrate(data, len); taskEXIT_CRITICAL(); break;
    case EMS_TOPID_CTRL: DC_debugOut("# CALL CTRL\r\n"); taskENTER_CRITICAL(); EMS_ctrlCallback(data, len); taskEXIT_CRITICAL(); break;
    case EMS_TOPID_DEBUG: DC_debugOut("# CALL DEBUG\r\n"); break;
    default: DC_debugOut("# CALL NON TOPIC\r\n");
    }
}
//------------------------------------------------------------------------------
//Send Channel vars
void EMS_sendChannelVars(uint8_t channel_num)
{
  char *out;
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
  
  cJSON_AddItemToObject(vars, "channel", channel);
  cJSON_AddNumberToObject(vars, "channel_NUM", channel_num);
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
//------------------------------------------------------------------------------
//Debug out counter outputs variables
void EMS_ChannelDebugOut(uint8_t channel)
{
  DC_debugOut("@ ch %d FREQ A: %2f | FREQ B: %2f | FREQ C: %2f\r\n", channel, meshChan[channel].phaseA.freq, meshChan[channel].phaseB.freq, meshChan[channel].phaseC.freq);
  DC_debugOut("@ ch %d RMSU A: %2f | RMSU B: %2f | RMSU C: %2f\r\n", channel, meshChan[channel].phaseA.RMSV, meshChan[channel].phaseB.RMSV, meshChan[channel].phaseC.RMSV);
  DC_debugOut("@ ch %d RMSI A: %2f | RMSI B: %2f | RMSI C: %2f\r\n", channel, meshChan[channel].phaseA.RMSI, meshChan[channel].phaseB.RMSI, meshChan[channel].phaseC.RMSI);
  DC_debugOut("@ ch %d RMSP A: %2f | RMSP B: %2f | RMSP C: %2f\r\n", channel, meshChan[channel].phaseA.RMSP, meshChan[channel].phaseB.RMSP, meshChan[channel].phaseC.RMSP);
  DC_debugOut("@ ch %d RMSRP A: %2f | RMSRP B: %2f | RMSRP C: %2f\r\n", channel, meshChan[channel].phaseA.RMSRP, meshChan[channel].phaseB.RMSRP, meshChan[channel].phaseC.RMSRP);
}
//******************************************************************************
// startEMS_task function
void startEMS_task(void const * argument)
{ 
  V9203_init(&hspi1);
  EMS_init();
  
  while(1)
  {
    for (int i=0; i < DC_V9203_COUNT_CHANNELS; i++)
    {
      //Get frequency
      meshChan[i].phaseA.freq = V9203_getFreq(i, LINE_A);
      meshChan[i].phaseB.freq = V9203_getFreq(i, LINE_B);
      meshChan[i].phaseC.freq = V9203_getFreq(i, LINE_C);
      
      //Get RMS voltage
      meshChan[i].phaseA.RMSV = V9203_getRMS_Voltage(i, LINE_A);
      meshChan[i].phaseB.RMSV = V9203_getRMS_Voltage(i, LINE_B);
      meshChan[i].phaseC.RMSV = V9203_getRMS_Voltage(i, LINE_C);
      
      //Get RMS current
      meshChan[i].phaseA.RMSI = V9203_getRMS_Current(i, LINE_A);
      meshChan[i].phaseB.RMSI = V9203_getRMS_Current(i, LINE_B);
      meshChan[i].phaseC.RMSI = V9203_getRMS_Current(i, LINE_C);

      //Get RMS power
      meshChan[i].phaseA.RMSP = V9203_getRMS_Power(i, LINE_A);
      meshChan[i].phaseB.RMSP = V9203_getRMS_Power(i, LINE_B);
      meshChan[i].phaseC.RMSP = V9203_getRMS_Power(i, LINE_C);
      
      //Get RMSRP power
      meshChan[i].phaseA.RMSRP = V9203_getRMS_reactivePower(i, LINE_A);
      meshChan[i].phaseB.RMSRP = V9203_getRMS_reactivePower(i, LINE_B);
      meshChan[i].phaseC.RMSRP = V9203_getRMS_reactivePower(i, LINE_C);
      
      //Debug out counter outputs variables
      EMS_ChannelDebugOut(i);
      
      //Send vars
      EMS_sendChannelVars(i);
    }
    
    vTaskDelay(DC_set.EMS_out_period*1000);
  }
}