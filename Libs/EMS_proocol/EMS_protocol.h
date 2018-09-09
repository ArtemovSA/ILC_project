
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

//Messages
#define EMS_DBG_MES_ALIVE               "Alive"

//**********************************Channel variables***********************************************

//JSON attributes
#define EMS_JSON_VAL_CHANNEL_NUM       "CH_NUM"
#define EMS_JSON_VAL_PHASE_NAME(n)     "PHASE#n"
#define EMS_JSON_VAL_CHANNEL_VAL       "CH_VAL"
//Main parameters
#define EMS_JSON_VAL_FREQ              "FREQ"          //������� ����
#define EMS_JSON_VAL_RMSIN             "RMSIN"         //��� ���� ��������
#define EMS_JSON_VAL_CONSSP            "CONSSP"        //������������ ������ ��������  
#define EMS_JSON_VAL_COSFIS            "COSFIS"        //����������� �������� �������
//Phase parametrs
#define EMS_JSON_VAL_RMSV              "RMSV"          //��� ����������
#define EMS_JSON_VAL_RMSI              "RMSI"          //��� ����
#define EMS_JSON_VAL_RMSP              "RMSP"          //��� �������� ��������
#define EMS_JSON_VAL_RMSRP             "RMSRP"         //��� ���������� ��������
#define EMS_JSON_VAL_COSFI             "COSFI"         //����������� ��������
#define EMS_JSON_VAL_CONSSP            "CONSSP"        //������������ ������ ��������
#define EMS_JSON_VAL_CONSP             "CONSP"         //������������ �������� ��������
#define EMS_JSON_VAL_CONSRP            "CONSRP"        //������������ ���������� ��������

//**********************************Control*********************************************************
//JSON control parametsrs
#define EMS_JSON_SET_OUT(n)             "SET_OUT_#n"     //���������� �������� ������
#define EMS_JSON_GET_IN(n)              "GET_IN_#n"      //�������� �������� �����

//**********************************Calibrate attributes********************************************

#define EMS_JSON_CAL_CHANNEL_NAME       "CHANNEL"
#define EMS_JSON_CAL_PHASEA_NAME        "PHASEA"
#define EMS_JSON_CAL_PHASEB_NAME        "PHASEB"
#define EMS_JSON_CAL_PHASEC_NAME        "PHASEC"
//Total RMS
#define EMS_JSON_CAL_WARTU              "WARTU"         //����������� �������� �� ����������
#define EMS_JSON_CAL_WARTI              "WARTI"         //����������� �������� �� ����
#define EMS_JSON_CAL_WAPT               "WAPT"          //����������� �������� �� �������� ��������
#define EMS_JSON_CAL_WAQT               "WAQT"          //����������� �������� �� ���������� ��������
#define EMS_JSON_CAL_WWARTU             "WWARTU"        //�������� �� ����������
#define EMS_JSON_CAL_WWARTI             "WWARTI"        //�������� �� ����
#define EMS_JSON_CAL_WWAPT              "WWAPT"         //�������� �� �������� ��������
#define EMS_JSON_CAL_WWAQT              "WWAQT"         //�������� �� ���������� ��������
//Fundamental RMS
#define EMS_JSON_CAL_WBRTU              "WBRTU"         //����������� �������� �� ����������
#define EMS_JSON_CAL_WBRTI              "WBRTI"         //����������� �������� �� ����
#define EMS_JSON_CAL_WBPT               "WBPT"          //����������� �������� �� �������� ��������
#define EMS_JSON_CAL_WBQT               "WBQT"          //����������� �������� �� ���������� ��������
#define EMS_JSON_CAL_WWBRTU             "WWBRTU"        //�������� �� ����������
#define EMS_JSON_CAL_WWBRTI             "WWBRTI"        //�������� �� ����
#define EMS_JSON_CAL_WWBPT              "WWBPT"         //�������� �� �������� ��������
#define EMS_JSON_CAL_WWBQT              "WWBQT"         //�������� �� ���������� ��������
//Threshold
#define EMS_JSON_CAL_ZZDCUM             "ZZDCUM"        //������� �������������� ����
#define EMS_JSON_CAL_ZZEGYTH            "ZZEGYTH"       //������� ����� ��������

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
#define EMS_JSON_SET_MQTT_IP            "SERV#n_IP"
#define EMS_JSON_SET_MQTT_DOMEN         "SERV#n_D"
#define EMS_JSON_SET_BROC_CH            "BROC_CH"
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
  float RMSV;   //�������� ����������           
  float RMSI;   //�������� ����
  float RMSP;   //�������� ��������
  float RMSRP;  //�������� ���������� ��������
  float COSFI;  //Cos Fi
  uint64_t CONSSP;      //������� ������ ��������
  uint64_t CONSP;       //������� �������� ��������
  uint64_t CONSRP;      //������� ���������� ��������
}EMS_phase_mesh_t;

//Channel mesh
typedef struct{
  float FREQ;   //������ ����
  float RMSNI;  //����� ��� ��������
  uint64_t CONSSP; //������� ������ ��������
  float COSFIS; //����������� ��������
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