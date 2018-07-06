
#ifndef EMS_PROT_H
#define EMS_PROT_H

#include "cmsis_os.h"

//Topic heads
#define EMS_TOPIC_VAR_PREFIX "variables"
#define EMS_TOPIC_ATR_PREFIX "attributes"
#define EMS_TOPIC_CTRL_PREFIX "ctrl"
#define EMS_TOPIC_DEB_PREFIX "debug"

//Topics names
#define EMS_TOPIC_CHANNEL               "channel"
#define EMS_TOPIC_CALIBRATE             "calibrate"
#define EMS_TOPIC_MAIN_SETTINGS         "main_set"

//**********************************Channel variables***********************************************

//JSON attributes
#define EMS_JSON_VAL_CHANNEL_NAME      "CHANNEL"
#define EMS_JSON_CAL_PHASE_NAME(t)     "PHASE#t"
//Main parameters
#define EMS_JSON_VAL_FREQ              "FREQ"          //Частота сети
#define EMS_JSON_VAL_RMSIN             "RMSIN"         //СКЗ тока нейтрали
#define EMS_JSON_VAL_CONSSP            "CONSSP"        //Потребленная полная мощность       
//Phase parametrs
#define EMS_JSON_VAL_RMSV              "RMSV"          //СКЗ напряжения
#define EMS_JSON_VAL_RMSI              "RMSI"          //СКЗ тока
#define EMS_JSON_VAL_RMSP              "RMSP"          //СКЗ активной мощности
#define EMS_JSON_VAL_RMSRP             "RMSRP"         //СКЗ реактивной мощности
#define EMS_JSON_VAL_COSFI             "COSFI"         //Коэффициент мощности
#define EMS_JSON_VAL_CONSP             "CONSP"         //Потребленная активная мощность
#define EMS_JSON_VAL_CONSRP            "CONSRP"        //Потребленная реактивная мощность

//**********************************Control*********************************************************
//JSON control parametsrs
#define EMS_JSON_SET_OUT(n)             "SET_OUT_#n"     //Установить значение выхода
#define EMS_JSON_GET_IN(n)              "GET_IN_#n"      //Получить значения входа

//**********************************Calibrate attributes********************************************

#define EMS_JSON_CAL_CHANNEL_NAME       "CHANNEL"
#define EMS_JSON_CAL_PHASE_NAME(t)      "PHASE#t"
//Total RMS
#define EMS_JSON_CAL_WARTU              "WARTU"         //коэффициент усиления по напряжению
#define EMS_JSON_CAL_WARTI              "WARTI"         //коэффициент усиления по току
#define EMS_JSON_CAL_WAPT               "WAPT"          //коэффициент усиления по активной мощности
#define EMS_JSON_CAL_WAQT               "WAQT"          //коэффициент усиления по реактивной мощности
#define EMS_JSON_CAL_WWARTU             "WWARTU"        //смещение по напряжению
#define EMS_JSON_CAL_WWARTI             "WWARTI"        //смещение по току
#define EMS_JSON_CAL_WWAPT              "WWAPT"         //смещение по активной мощности
#define EMS_JSON_CAL_WWAQT              "WWAQT"         //смещение по реактивной мощности
//Fundamental RMS
#define EMS_JSON_CAL_WBRTU              "WBRTU"         //коэффициент усиления по напряжению
#define EMS_JSON_CAL_WBRTI              "WBRTI"         //коэффициент усиления по току
#define EMS_JSON_CAL_WBPT               "WBPT"          //коэффициент усиления по активной мощности
#define EMS_JSON_CAL_WBQT               "WBQT"          //коэффициент усиления по реактивной мощности
#define EMS_JSON_CAL_WWBRTU             "WWBRTU"        //смещение по напряжению
#define EMS_JSON_CAL_WWBRTI             "WWBRTI"        //смещение по току
#define EMS_JSON_CAL_WWBPT              "WWBPT"         //смещение по активной мощности
#define EMS_JSON_CAL_WWBQT              "WWBQT"         //смещение по реактивной мощности
//Threshold
#define EMS_JSON_CAL_ZZDCUM             "ZZDCUM"        //Граница детектирования тока
#define EMS_JSON_CAL_ZZEGYTH            "ZZEGYTH"       //Граница счета мощности

//**********************************Settings attributes*********************************************

//Settings attributes
//Net settings
#define EMS_JSON_SET_NET_NAME           "NET_SET"
#define EMS_JSON_SET_NET_IP             "NET_IP"
#define EMS_JSON_SET_NET_MASK           "NET_MASK"
#define EMS_JSON_SET_NET_GATEIP         "NET_GATEIP"
#define EMS_JSON_SET_NTP_DOMEN          "NET_NTPD"
#define EMS_JSON_SET_DNS_IP(n)          "DNS#n_IP"

//MQTT settings
#define EMS_JSON_SET_MQTT_NAME          "MQTT_SET"
#define EMS_JSON_SET_MQTT_IP(n)         "SERV#n_IP"
#define EMS_JSON_SET_MQTT_DOMEN(n)      "SERV#n_D"
#define EMS_JSON_SET_ACT_BROCK          "SERV_ACTN"
#define EMS_JSON_SET_MQTT_PORT          "PORT"
#define EMS_JSON_SET_MQTT_USER          "USER"
#define EMS_JSON_SET_MQTT_PASS          "PASS"

//EMS settings
#define EMS_JSON_SET_EMS_NAME           "EMS_SET"
#define EMS_JSON_SET_EMS_OUT_PER        "OUT_PER"

//**************************************************************************************************

//Topics ID
enum {
  EMS_TOPID_VAR_CHAN=0, // channel topic
  EMS_TOPID_ATTR_MAIN_SET, //Main settings
  EMS_TOPID_ATTR_CALIBR, //Calibrate values
  EMS_TOPID_CTRL, //Control
  EMS_TOPID_DEBUG,
  EMS_TOPID_COUNT
};

//Phase mesh
typedef struct 
{
  float freq;
  float RMSV;
  float RMSI;
  float RMSP;
  float RMSRP;
}EMS_phase_mesh_t;

//Channel mesh
typedef struct{
  EMS_phase_mesh_t phaseA;
  EMS_phase_mesh_t phaseB;
  EMS_phase_mesh_t phaseC;
}EMC_chan_mesh_t;

//Task
extern osThreadId EMS_taskHandle;
extern osMessageQId EMS_TTqueueHandle;

//Init EMS
void EMS_init();
//startEMS_task function
void startEMS_task(void const * argument);

#endif