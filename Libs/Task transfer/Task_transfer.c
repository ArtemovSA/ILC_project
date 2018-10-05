
#include "Task_transfer.h"
#include "String.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

TT_tasksDesc_t tasksDesc[TT_TASK_COUNT]; //Tasks description

//Queues connection
osMessageQId debug_TTqueueHandle;
osMessageQId EMS_TTqueueHandle;
osMessageQId Modem_TTqueueHandle;
osMessageQId VM_TTqueueHandle;

//--------------------------------------------------------------------------------------------------
//Send event
TT_stat_t TT_sendEvent(TT_mesID_t eventID, TT_sendMes_t *mes, QueueHandle_t *eventQueue)
{
  TT_mes_t event_mes;
  event_mes.mesType = eventID;
  if (mes != NULL)
    memcpy(&event_mes.sendMes, mes, sizeof(event_mes.sendMes));
    
  //Send queue
  if( xQueueSend( eventQueue, &event_mes, ( TickType_t ) 1000 ) != pdPASS )
  {
    return EXEC_ERROR;
  }
  
  return EXEC_OK;
}
//--------------------------------------------------------------------------------------------------
//Send event from IRQ
TT_stat_t TT_sendEventIRQ(TT_mesID_t eventID, TT_sendMes_t *mes, QueueHandle_t *eventQueue)
{
  TT_mes_t event_mes;
  event_mes.mesType = eventID;
  if (mes != NULL)
    memcpy(&event_mes.sendMes, mes, sizeof(event_mes.sendMes));
  BaseType_t  xHigherPriorityTaskWoken = pdFALSE;
  
  //Send queue
  if( xQueueSendFromISR( eventQueue, &event_mes, &xHigherPriorityTaskWoken ) != pdPASS )
  {
    return EXEC_ERROR;
  }
  
  return EXEC_OK;
}
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
  
  tasksDesc[TT_MODEM_TASK].conQueue = &Modem_TTqueueHandle;
  strcpy(tasksDesc[TT_MODEM_TASK].taskName, "Modem_Task");
  
  tasksDesc[TT_EMS_TASK].conQueue = &EMS_TTqueueHandle;
  strcpy(tasksDesc[TT_EMS_TASK].taskName, "EMS_Task");
  
  tasksDesc[TT_VM_TASK].conQueue = &VM_TTqueueHandle;
  strcpy(tasksDesc[TT_VM_TASK].taskName, "VM_Task");
 
}