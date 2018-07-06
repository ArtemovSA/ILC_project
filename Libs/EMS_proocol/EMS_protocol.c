
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
  // topic: id/varibles/channel
  emsTopics[EMS_TOPID_VAR_CHAN].sub_pub = MQTT_PUB;
  sprintf(emsTopics[EMS_TOPID_VAR_CHAN].name, "%s/%s/%s", EMS_TOPIC_VAR_PREFIX, DC_unic_idef, EMS_TOPIC_CHANNEL);
  
  // topic: id/attributes/main_set
  emsTopics[EMS_TOPID_ATTR_MAIN_SET].sub_pub = MQTT_PUB_SUB;
  sprintf(emsTopics[EMS_TOPID_ATTR_MAIN_SET].name, "%s/%s/%s", EMS_TOPIC_ATR_PREFIX, DC_unic_idef, EMS_TOPIC_MAIN_SETTINGS);
    
  // topic: id/attributes/calibrate
  emsTopics[EMS_TOPID_ATTR_CALIBR].sub_pub = MQTT_PUB_SUB;
  sprintf(emsTopics[EMS_TOPID_ATTR_CALIBR].name, "%s/%s/%s", EMS_TOPIC_ATR_PREFIX, DC_unic_idef, EMS_TOPIC_CALIBRATE);
  
  // topic: id/ctrl
  emsTopics[EMS_TOPID_CTRL].sub_pub = MQTT_PUB_SUB;
  sprintf(emsTopics[EMS_TOPID_CTRL].name, "%s/%s", EMS_TOPIC_CTRL_PREFIX, DC_unic_idef);
  
  // topic: debug/id
  emsTopics[EMS_TOPID_DEBUG].sub_pub = MQTT_PUB_SUB;
  sprintf(emsTopics[EMS_TOPID_DEBUG].name, "%s/%s", EMS_TOPIC_DEB_PREFIX, DC_unic_idef);
  
  //MQTT connection
  devMQTT_init(emsTopics, EMS_TOPID_COUNT, &EMS_callBack); //Init MQTT
  
  if (DC_set.MQTT_broc_ch == 0)
  {
    devMQTT_connect(DC_set.MQTT_broc_ip, DC_set.MQTT_port, DC_unic_idStr, DC_set.MQTT_user, DC_set.MQTT_pass); //Connect
    DC_debug_ipAdrrOut("# MQTT connection server by IP#: ", DC_set.MQTT_broc_ip);
  }else{
    uint8_t broc_ip;
    NW_getIP_byDomen(DC_set.MQTT_broc_domen, &broc_ip);
    devMQTT_connect(&broc_ip, DC_set.MQTT_port, DC_unic_idStr, DC_set.MQTT_user, DC_set.MQTT_pass); //Connect
    DC_debugOut("# MQTT connection server by Domen#: %s OK\r\n", DC_set.MQTT_broc_domen);
  }

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
  
  if (ctrl_json == NULL)
  {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL)
    {
      DC_debugOut("# JSON parce Error before: %s\n", error_ptr);
    }
    return HAL_ERROR;
  }
  
  //Check child empty
  if (ctrl_json->child == NULL)
  {
    DC_debugOut("# JSON message empty\r\n");
    return HAL_ERROR;
  }    
  
  return HAL_OK;
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
  cJSON *set_net_json = cJSON_GetObjectItemCaseSensitive(set_json, EMS_JSON_SET_NET_NAME);
  
  if (set_net_json != NULL)
  {
    //SET NET IP
    EMS_JSON_getIPAdrr(set_net_json, EMS_JSON_SET_NET_IP, settings.net_dev_ip_addr);
    
    //SET NET GATEWAY
    EMS_JSON_getIPAdrr(set_net_json, EMS_JSON_SET_NET_GATEIP, settings.net_gw_ip_addr);
    
    //SET NET MASK
    EMS_JSON_getIPAdrr(set_net_json, EMS_JSON_SET_NET_MASK, settings.net_mask);
    
    //SET_NTP_NAME
    EMS_JSON_getStr(set_net_json, EMS_JSON_SET_NTP_DOMEN, settings.netNTP_server, sizeof(settings.netNTP_server));
    
    //SET_DNS_IP
    EMS_JSON_getIPAdrr(set_net_json, EMS_JSON_SET_DNS_IP(1), settings.serverDNS1);
    EMS_JSON_getIPAdrr(set_net_json, EMS_JSON_SET_DNS_IP(2), settings.serverDNS2);
  }
  
  //***********************************MQTT struct**********************************************
  //Get cJSON MQTT set section
  cJSON *set_mqtt_json = cJSON_GetObjectItemCaseSensitive(set_json, EMS_JSON_SET_MQTT_NAME);
  
  if (set_mqtt_json != NULL)
  {
    //Set MQTT ip
    EMS_JSON_getIPAdrr(set_mqtt_json, EMS_JSON_SET_MQTT_IP, settings.MQTT_broc_ip);
    
    //Set MQTT domen
    EMS_JSON_getStr(set_mqtt_json, EMS_JSON_SET_MQTT_DOMEN, settings.MQTT_broc_domen, sizeof(settings.MQTT_broc_domen));
    
    //Set active brocker
    uint16_t brockCH;
    EMS_JSON_getInt(set_mqtt_json, EMS_JSON_SET_BROC_CH, &brockCH);
    settings.MQTT_broc_ch = (uint8_t)brockCH;
    
    //Set MQTT port
    EMS_JSON_getInt(set_mqtt_json, EMS_JSON_SET_MQTT_PORT, &settings.MQTT_port);
    
    //Set MQTT user
    EMS_JSON_getStr(set_mqtt_json, EMS_JSON_SET_MQTT_USER, settings.MQTT_user, sizeof(settings.MQTT_user));
    
    //Set MQTT pass
    EMS_JSON_getStr(set_mqtt_json, EMS_JSON_SET_MQTT_PASS, settings.MQTT_pass, sizeof(settings.MQTT_pass));
    
  }
  
  //***********************************EMS struct**********************************************
  //Get cJSON EMS set section
  cJSON *set_ems_json = cJSON_GetObjectItemCaseSensitive(set_json, EMS_JSON_SET_EMS_NAME);
  
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
//Set phase total calibrate
HAL_StatusTypeDef EMS_setPahseTotalCalibrate(cJSON* cal_json, char* jsonName, V9203_Total_cal_t* totalCal_p)
{
  cJSON *phase_cal_json = cJSON_GetObjectItemCaseSensitive(cal_json, jsonName);
  uint16_t calVal;
  
  if (phase_cal_json != NULL)
  {
    //Get WARTU
    EMS_JSON_getInt(phase_cal_json, EMS_JSON_CAL_WARTU, &calVal);
    totalCal_p->Cal_WARTU = calVal;
    //Get WARTI
    EMS_JSON_getInt(phase_cal_json, EMS_JSON_CAL_WARTI, &calVal);
    totalCal_p->Cal_WARTI = calVal;
    //Get WAPT
    EMS_JSON_getInt(phase_cal_json, EMS_JSON_CAL_WAPT, &calVal);
    totalCal_p->Cal_WAPT = calVal;
    //Get WAQT
    EMS_JSON_getInt(phase_cal_json, EMS_JSON_CAL_WAQT, &calVal);
    totalCal_p->Cal_WAQT = calVal;
    //Get WWARTU
    EMS_JSON_getInt(phase_cal_json, EMS_JSON_CAL_WWARTU, &calVal);
    totalCal_p->Cal_WWARTU = calVal;
    //Get WWARTI
    EMS_JSON_getInt(phase_cal_json, EMS_JSON_CAL_WWARTI, &calVal);
    totalCal_p->Cal_WWARTI = calVal;
    //Get WWAPT
    EMS_JSON_getInt(phase_cal_json, EMS_JSON_CAL_WWAPT, &calVal);
    totalCal_p->Cal_WWAPT = calVal;
    //Get WWAQT
    EMS_JSON_getInt(phase_cal_json, EMS_JSON_CAL_WWAQT, &calVal);
    totalCal_p->Cal_WWAQT = calVal;
  }
  
  return HAL_OK;
}
//------------------------------------------------------------------------------
//Set phase Fundamental calibrate
HAL_StatusTypeDef EMS_setPahseFundamentalCalibrate(cJSON* cal_json, char* jsonName, V9203_Fund_cal_t* fundCal_p)
{
  cJSON *phase_cal_json = cJSON_GetObjectItemCaseSensitive(cal_json, jsonName);
  uint16_t calVal;
  
  if (phase_cal_json != NULL)
  {
    //Get WBRTU
    EMS_JSON_getInt(phase_cal_json, EMS_JSON_CAL_WBRTU, &calVal);
    fundCal_p->Cal_WBRTU = calVal;
    //Get WBRTI
    EMS_JSON_getInt(phase_cal_json, EMS_JSON_CAL_WBRTI, &calVal);
    fundCal_p->Cal_WBRTI = calVal;
    //Get WBPT
    EMS_JSON_getInt(phase_cal_json, EMS_JSON_CAL_WBPT, &calVal);
    fundCal_p->Cal_WBPT = calVal;
    //Get WBQT
    EMS_JSON_getInt(phase_cal_json, EMS_JSON_CAL_WBQT, &calVal);
    fundCal_p->Cal_WBQT = calVal;
    //Get WWBRTU
    EMS_JSON_getInt(phase_cal_json, EMS_JSON_CAL_WWBRTU, &calVal);
    fundCal_p->Cal_WWBRTU = calVal;
    //Get WWBRTI
    EMS_JSON_getInt(phase_cal_json, EMS_JSON_CAL_WWBRTI, &calVal);
    fundCal_p->Cal_WWBRTI = calVal;
    //Get WWBPT
    EMS_JSON_getInt(phase_cal_json, EMS_JSON_CAL_WWBPT, &calVal);
    fundCal_p->Cal_WWBPT = calVal;
    //Get WWBQT
    EMS_JSON_getInt(phase_cal_json, EMS_JSON_CAL_WWBQT, &calVal);
    fundCal_p->Cal_WWBQT = calVal;
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
  
  EMS_JSON_getInt(cal_json, EMS_JSON_CAL_CHANNEL_NAME, &channelNum);
  
  //Check channel num
  if ((channelNum > DC_V9203_COUNT_CHANNELS) || (channelNum == 0))
  {
    DC_debugOut("# Channel number out of range\r\n");
    return HAL_ERROR;
  }

  //***********************************Calibrate TOTAL phase********************************************

  //Set phaseA calibrate
  if (EMS_setPahseTotalCalibrate(cal_json, EMS_JSON_CAL_PHASE_NAME(A), &DC_set.V9203_ch_set[channelNum].calTotalPhaseA) != HAL_OK)
  {
    DC_debugOut("# Phase Total A calibrate error\r\n");
    return HAL_ERROR;
  }
  
  //Set phaseB calibrate
  if (EMS_setPahseTotalCalibrate(cal_json, EMS_JSON_CAL_PHASE_NAME(B), &DC_set.V9203_ch_set[channelNum].calTotalPhaseB) != HAL_OK)
  {
    DC_debugOut("# Phase B calibrate error\r\n");
    return HAL_ERROR;
  }
  
  //Set phaseC calibrate
  if (EMS_setPahseTotalCalibrate(cal_json, EMS_JSON_CAL_PHASE_NAME(C), &DC_set.V9203_ch_set[channelNum].calTotalPhaseC) != HAL_OK)
  {
    DC_debugOut("# Phase C calibrate error\r\n");
    return HAL_ERROR;
  }
  
  //***********************************Calibrate Fundamental Phase***************************************
  
  //Set phaseA calibrate
  if (EMS_setPahseFundamentalCalibrate(cal_json, EMS_JSON_CAL_PHASE_NAME(A), &DC_set.V9203_ch_set[channelNum].calFundPhaseA) != HAL_OK)
  {
    DC_debugOut("# Phase Total A calibrate error\r\n");
    return HAL_ERROR;
  }
  
  //Set phaseB calibrate
  if (EMS_setPahseFundamentalCalibrate(cal_json, EMS_JSON_CAL_PHASE_NAME(B), &DC_set.V9203_ch_set[channelNum].calFundPhaseB) != HAL_OK)
  {
    DC_debugOut("# Phase B calibrate error\r\n");
    return HAL_ERROR;
  }
  
  //Set phaseC calibrate
  if (EMS_setPahseFundamentalCalibrate(cal_json, EMS_JSON_CAL_PHASE_NAME(C), &DC_set.V9203_ch_set[channelNum].calFundPhaseC) != HAL_OK)
  {
    DC_debugOut("# Phase C calibrate error\r\n");
    return HAL_ERROR;
  }
  
  //***********************************Calibrate Threshold***********************************************

  uint16_t Threshold_val;
  //Граница детектирования тока
  EMS_JSON_getInt(cal_json, EMS_JSON_CAL_ZZDCUM, &Threshold_val);
  DC_set.V9203_ch_set[channelNum].cal_currThrdDetect = Threshold_val;
  //Граница счета мощности
  EMS_JSON_getInt(cal_json, EMS_JSON_CAL_ZZEGYTH, &Threshold_val);
  DC_set.V9203_ch_set[channelNum].cal_energyThrdDetect = Threshold_val;

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
  
  //Phase values
  phaseA = cJSON_CreateObject();
  cJSON_AddNumberToObject(phaseA, EMS_JSON_VAL_RMSV, meshChan[channel_num].phaseA.RMSV);
  cJSON_AddNumberToObject(phaseA, EMS_JSON_VAL_RMSI, meshChan[channel_num].phaseA.RMSI);
  cJSON_AddNumberToObject(phaseA, EMS_JSON_VAL_RMSP, meshChan[channel_num].phaseA.RMSP);
  cJSON_AddNumberToObject(phaseA, EMS_JSON_VAL_RMSRP, meshChan[channel_num].phaseA.RMSRP);
  cJSON_AddNumberToObject(phaseA, EMS_JSON_VAL_COSFI, meshChan[channel_num].phaseA.COSFI);
  cJSON_AddNumberToObject(phaseA, EMS_JSON_VAL_CONSSP, meshChan[channel_num].phaseA.CONSSP);
  cJSON_AddNumberToObject(phaseA, EMS_JSON_VAL_CONSP, meshChan[channel_num].phaseA.CONSP);
  cJSON_AddNumberToObject(phaseA, EMS_JSON_VAL_CONSRP, meshChan[channel_num].phaseA.CONSRP);
  
  phaseB = cJSON_CreateObject();
  cJSON_AddNumberToObject(phaseB, EMS_JSON_VAL_RMSV, meshChan[channel_num].phaseB.RMSV);
  cJSON_AddNumberToObject(phaseB, EMS_JSON_VAL_RMSI, meshChan[channel_num].phaseB.RMSI);
  cJSON_AddNumberToObject(phaseB, EMS_JSON_VAL_RMSP, meshChan[channel_num].phaseB.RMSP);
  cJSON_AddNumberToObject(phaseB, EMS_JSON_VAL_RMSRP, meshChan[channel_num].phaseB.RMSRP);
  cJSON_AddNumberToObject(phaseB, EMS_JSON_VAL_COSFI, meshChan[channel_num].phaseB.COSFI);
  cJSON_AddNumberToObject(phaseB, EMS_JSON_VAL_CONSSP, meshChan[channel_num].phaseB.CONSSP);
  cJSON_AddNumberToObject(phaseB, EMS_JSON_VAL_CONSP, meshChan[channel_num].phaseB.CONSP);
  cJSON_AddNumberToObject(phaseB, EMS_JSON_VAL_CONSRP, meshChan[channel_num].phaseB.CONSRP);
  
  phaseC = cJSON_CreateObject();
  cJSON_AddNumberToObject(phaseC, EMS_JSON_VAL_RMSV, meshChan[channel_num].phaseC.RMSV);
  cJSON_AddNumberToObject(phaseC, EMS_JSON_VAL_RMSI, meshChan[channel_num].phaseC.RMSI);
  cJSON_AddNumberToObject(phaseC, EMS_JSON_VAL_RMSP, meshChan[channel_num].phaseC.RMSP);
  cJSON_AddNumberToObject(phaseC, EMS_JSON_VAL_RMSRP, meshChan[channel_num].phaseC.RMSRP);
  cJSON_AddNumberToObject(phaseC, EMS_JSON_VAL_COSFI, meshChan[channel_num].phaseC.COSFI);
  cJSON_AddNumberToObject(phaseC, EMS_JSON_VAL_CONSSP, meshChan[channel_num].phaseC.CONSSP);
  cJSON_AddNumberToObject(phaseC, EMS_JSON_VAL_CONSP, meshChan[channel_num].phaseC.CONSP);
  cJSON_AddNumberToObject(phaseC, EMS_JSON_VAL_CONSRP, meshChan[channel_num].phaseC.CONSRP);
  
  //Channel values
  channel = cJSON_CreateObject();
  cJSON_AddItemToObject(channel, EMS_JSON_CAL_PHASE_NAME(A), phaseA);
  cJSON_AddItemToObject(channel, EMS_JSON_CAL_PHASE_NAME(B), phaseB);
  cJSON_AddItemToObject(channel, EMS_JSON_CAL_PHASE_NAME(C), phaseC);
  
  //Main parameters
  cJSON_AddNumberToObject(channel, EMS_JSON_VAL_FREQ, meshChan[channel_num].FREQ);
  cJSON_AddNumberToObject(channel, EMS_JSON_VAL_RMSIN, meshChan[channel_num].RMSNI);
  cJSON_AddNumberToObject(channel, EMS_JSON_VAL_CONSSP, meshChan[channel_num].CONSSP);
  cJSON_AddNumberToObject(channel, EMS_JSON_VAL_COSFIS, meshChan[channel_num].COSFIS);
  
  cJSON_AddItemToObject(vars, EMS_JSON_VAL_CHANNEL_VAL, channel);
  cJSON_AddNumberToObject(vars, EMS_JSON_VAL_CHANNEL_NUM, channel_num);
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
  DC_debugOut("@ ch %d FREQ: %2f | RMSNI: %2f | CONSSP: %2f | COSFIS: %2f\r\n" ,meshChan[channel].FREQ, meshChan[channel].RMSNI, meshChan[channel].CONSSP, meshChan[channel].COSFIS);
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
  
  for (int i=0; i<DC_V9203_COUNT_CHANNELS; i++)
    V9203_initDev(i, &DC_set.V9203_ch_set[i]); //Init dev
  
  EMS_init();
  
  while(1)
  {
    for (int i=0; i < DC_V9203_COUNT_CHANNELS; i++)
    {
      //Get frequency
      meshChan[i].FREQ = V9203_getFreq(i, LINE_A);
      
      //S power consamption
      meshChan[i].CONSSP = V9203_getSCons(i, LINE_S);
      
      //RMS N current
      meshChan[i].RMSNI = V9203_getRMS_Current(i, LINE_N);
      
      //Get cos Fi
      meshChan[i].COSFIS = V9203_getCOSfi(i, LINE_S);
      
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
      
      //Get cos Fi
      meshChan[i].phaseA.COSFI = V9203_getCOSfi(i, LINE_A);
      meshChan[i].phaseB.COSFI = V9203_getCOSfi(i, LINE_B);
      meshChan[i].phaseC.COSFI = V9203_getCOSfi(i, LINE_C);
      
      //Get full Consamption
      meshChan[i].phaseA.CONSSP = V9203_getSCons(i, LINE_A);
      meshChan[i].phaseB.CONSSP = V9203_getSCons(i, LINE_B);
      meshChan[i].phaseC.CONSSP = V9203_getSCons(i, LINE_C);
      
      //Get active Consamption
      meshChan[i].phaseA.CONSP = V9203_getPCons(i, LINE_A);
      meshChan[i].phaseB.CONSP = V9203_getPCons(i, LINE_B);
      meshChan[i].phaseC.CONSP = V9203_getPCons(i, LINE_C);
      
      //Get active Consamption
      meshChan[i].phaseA.CONSRP = V9203_getQCons(i, LINE_A);
      meshChan[i].phaseB.CONSRP = V9203_getQCons(i, LINE_B);
      meshChan[i].phaseC.CONSRP = V9203_getQCons(i, LINE_C);

      //Debug out counter outputs variables
      EMS_ChannelDebugOut(i);
      
      //Send vars
      EMS_sendChannelVars(i);
    }
    
    vTaskDelay(DC_set.EMS_out_period*1000);
  }
}