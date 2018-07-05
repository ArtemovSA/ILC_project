
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

//JSON attributes
#define EMS_JSON_NAME_FREQ              "freq"
#define EMS_JSON_NAME_RMSV              "RMSV"
#define EMS_JSON_NAME_RMSI              "RMSI"
#define EMS_JSON_NAME_RMSP              "RMSP"
#define EMS_JSON_NAME_RMSRP             "RMSRP"

//JSON control parametsrs
#define EMS_JSON_SET_RELAY(n)           "Relay_n#n"
#define EMS_JSON_SET_RELAY(n)           "Relay_n#n"

//Settings attributes
#define EMS_JSON_SET_NET_SET            "net_set"
#define EMS_JSON_SET_NET_IP             "ip"
#define EMS_JSON_SET_NET_MASK           "mask"
#define EMS_JSON_SET_NET_GATEIP         "gateIP"
#define EMS_JSON_SET_NTP_NAME           "ntp_name"
#define EMS_JSON_SET_DNS_IP(n)          "dns#n_ip"

#define EMS_JSON_SET_MQTT_SET           "mqtt_set"
#define EMS_JSON_SET_MQTT_IP(n)         "serv#n_ip"
#define EMS_JSON_SET_MQTT_NAME(n)       "serv#n_name"
#define EMS_JSON_SET_ACT_BROCK          "serv_actn"
#define EMS_JSON_SET_MQTT_PORT          "port"
#define EMS_JSON_SET_MQTT_USER          "user"
#define EMS_JSON_SET_MQTT_PASS          "pass"

#define EMS_JSON_SET_EMS_SET            "ems_set"
#define EMS_JSON_SET_EMS_OUT_PER        "out_per"

//Calibrate attributes
#define EMS_JSON_CAL_CHANNEL            "channel"
#define EMS_JSON_CAL_PHASE(t)           "phase#t"
#define EMS_JSON_CAL_WARTU              "WARTU"
#define EMS_JSON_CAL_WARTI              "WARTI"
#define EMS_JSON_CAL_WAPT               "WAPT"
#define EMS_JSON_CAL_WWAPT              "WWAPT"
#define EMS_JSON_CAL_REWWAPT            "REWWAPT"
#define EMS_JSON_CAL_GAIN_U             "GAIN_U"
#define EMS_JSON_CAL_GAIN_I             "GAIN_I"
#define EMS_JSON_CAL_GAIN_P             "GAIN_P"


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