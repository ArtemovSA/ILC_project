
#ifndef EMS_PROT_H
#define EMS_PROT_H

#include "cmsis_os.h"

//Topic heads
#define EMS_TOPIC_VAR_PREFIX "variables"
#define EMS_TOPIC_ATR_PREFIX "attributes"
#define EMS_TOPIC_DEB_PREFIX "debug"

//Topics ID
enum{
  EMS_TOPID_VAR_CHAN=0, // channel topic
  EMS_TOPID_ATTR_MAIN_SET, //Main settings
  EMS_TOPID_ATTR_CALIBR, //Calibrate values
  EMS_TOPID_DEBUG,
  EMS_TOPID_COUNT
};

//Pahse mesh
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