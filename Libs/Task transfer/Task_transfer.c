
#include "Task_transfer.h"
#include "String.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

TT_tasksDesc_t tasksDesc[TT_TASK_COUNT]; //Tasks description

//Queues connection
extern osMessageQId debug_TTqueueHandle;
extern osMessageQId WM_TTqueueHandle;
extern osMessageQId PW_TTqueueHandle;
extern osMessageQId CL_TTqueueHandle;

//--------------------------------------------------------------------------------------------------
//Send query
TT_stat_t TT_send_query(TT_taskID_t taskID, QueueHandle_t* queue_send, QueueHandle_t* queue_ret, void *mes, void (*p_func)()) {
  
  TT_mes_t query;
  TT_mes_t ansver;
  
  //Prepare query
  query.mesType = REQ_MSG;
  query.taskID = taskID;
  query.p_retQueue = *queue_ret;
  query.p_retMes = mes;
  query.p_func = p_func;

  //Send queue
  if( xQueueSend( queue_send, &query, ( TickType_t ) 1000 ) != pdPASS )
  {
    return EXEC_ERROR;
  }
  
  //Wait message
  if ( xQueueReceive( (QueueHandle_t)(*queue_ret), &ansver, 15000 ) == pdTRUE )
  {
    return ansver.execStat;
  }
  
  return EXEC_ERROR;
}
//--------------------------------------------------------------------------------------------------
//Task transfer inint
void TT_init() {
  
  //Create task_id queues task_name struct
  tasksDesc[TT_DEBUG_TASK].conQueue = &debug_TTqueueHandle;
  strcpy(tasksDesc[TT_DEBUG_TASK].taskName, "Debug_Task");
  
  tasksDesc[TT_WM_TASK].conQueue = &WM_TTqueueHandle;
  strcpy(tasksDesc[TT_WM_TASK].taskName, "WM_Task");
  
  tasksDesc[TT_PW_TASK].conQueue = &PW_TTqueueHandle;
  strcpy(tasksDesc[TT_PW_TASK].taskName, "PW_Task");
  
  tasksDesc[TT_CL_TASK].conQueue = &CL_TTqueueHandle;
  strcpy(tasksDesc[TT_CL_TASK].taskName, "CL_Task");
 
}