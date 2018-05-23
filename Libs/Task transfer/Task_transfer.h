
#ifndef TASK_TRANS_H
#define TASK_TRANS_H

#include "cmsis_os.h"

//Tasks
typedef enum {
  TT_DEBUG_TASK = 0,
  TT_WM_TASK,
  TT_PW_TASK,
  TT_CL_TASK,
  TT_TASK_COUNT
}TT_taskID_t;

//Messages types
typedef enum {
  REQ_MSG = 0,
  EVENT_IRQ_PCA9555,
  EVENT_MQTT_MSG,
  EVENT_TCP_MSG,
  EVENT_TCP_CLOSE,
}TT_mesID_t;

//Status types
typedef enum {
  EXEC_ERROR = 0,
  EXEC_WAIT_ER,
  EXEC_OK
}TT_stat_t;

//Tasks discription struct
typedef struct {
  osMessageQId *conQueue;
  char taskName[20];
}TT_tasksDesc_t;

typedef struct {
  uint8_t payload[10];
} TT_sendMes_t;

//Struct for tasks
typedef struct {
  TT_mesID_t mesType;           //Type of message
  TT_taskID_t taskID;           //Task id
  TT_sendMes_t sendMes;         //Send message
  void* p_retQueue;             //Return queue p
  void* p_retMes;               //Message p
  void (*p_func)();             //Exec func p
  TT_stat_t execStat;           //Exec status
}TT_mes_t;

extern TT_tasksDesc_t tasksDesc[TT_TASK_COUNT]; //Tasks description

void TT_init(); //Task transfer inint
TT_stat_t TT_send_query(TT_taskID_t taskID, QueueHandle_t* queue_send, QueueHandle_t* queue_ret, void *mes, void (*p_func)()); //Send query
TT_stat_t TT_sendEvent(TT_mesID_t eventID, TT_sendMes_t *mes, QueueHandle_t *eventQueue); //Send event
TT_stat_t TT_sendEventIRQ(TT_mesID_t eventID, TT_sendMes_t *mes, QueueHandle_t *eventQueue); //Send event from IRQ

#endif