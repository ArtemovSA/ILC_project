
#include "TASK_script.h"
#include "PY_usr.h"
#include "DevCTRL.h"
#include "py_plat.h"

TimerHandle_t PY_timer; //Таймер
xTaskHandle script_handle; //Хендл скрипта
xQueueHandle queue_script_commands; //Очередь комманд
uint8_t PY_task; //Выполнение задачи скрипта
uint32_t PY_script_addr; //Адрес скрипта
NAND_AddressTypeDef PY_script_struct_addr; //Метаданные виртуальной машины
bool volatile scryptStop = false; //Stop flag

uint8_t PY_callback_queue[PY_COUNT_CALLBACKS]; //Очередь callback
uint8_t PY_callback_queue_len; //Длина очереди
uint8_t PY_heap[PM_HEAP_SIZE]; //Python heap

//Глабальные переменные возвращаемые в callback
PY_var_SMS_CALLBACK_t PY_var_SMS_CALLBACK;
PY_var_MQTT_CALLBACK_t PY_var_MQTT_CALLBACK;
PY_var_GPIO_CALLBACK_t PY_var_GPIO_CALLBACK;
PY_var_MODEM_CALLBACK_t PY_var_MODEM_CALLBACK;

//Структура функций Callback
PY_callback_t PY_callback[PY_COUNT_CALLBACKS] = {
  {0 , DEF_PY_SMS_CALLBACK, PY_CALLBACK_EMPTY, (void *)&PY_var_SMS_CALLBACK},
  {0 , DEF_PY_MQTT_CALLBACK, PY_CALLBACK_EMPTY, (void *)&PY_var_MQTT_CALLBACK},
  {0 , DEF_PY_GPIO_CALLBACK, PY_CALLBACK_EMPTY, (void *)&PY_var_GPIO_CALLBACK},
  {0 , DEF_PY_MODEM_CALLBACK, PY_CALLBACK_EMPTY, (void *)&PY_var_MODEM_CALLBACK},
};

static void PY_TimerHandler( TimerHandle_t xTimer ); //Таймер

//----------------------------------------------------------------------------------------------------
void vTASK_script(void *pvParameters) {

PmReturn_t retval = PM_RET_OK; //Return value
PY_task = PY_SCRIPT_NLOAD;
NAND_AddressTypeDef PY_VM_data_addr = MEM_NAND_ADDR_VM_DATA;
  PmMemSpace_t PY_memspace;
  
  while (1) {
    
    //Task pause
    vTaskSuspend(script_handle);
    
    //Copy from NAND to SRAM
    if (MEM_NAND_to_SRAM(DC_set.PY_scryptData.memID, MEM_SRAM1_SCRYPT_ADDR, PY_VM_data_addr, sizeof(PY_scryptData_t), DC_set.PY_scryptData.len) == DEV_OK)
    {
      //Check CRC in SRAM
      if (MEM_checkCRC16_SRAM(DC_set.PY_scryptData.memID, DC_set.PY_scryptData.crc, MEM_SRAM1_SCRYPT_ADDR, DC_set.PY_scryptData.len) == DEV_OK)
      {
        xTimerStart( PY_timer, 0 ); //Запустить таймер
        
        if (DC_set.PY_scryptData.memID == MEM_ID_SRAM1)
        {
          PY_memspace = MEMSPACE_SRAM1;
          
          //Инициализировать скрипт
          DC_set.PY_scryptData.status = PY_SCRIPT_START;
          uint8_t const * scrypt_p = (uint8_t const *) MEM_SRAM1_SCRYPT_ADDR;
          retval = pm_init(PY_memspace, (uint8_t const *)scrypt_p);
          
          if (retval != PM_RET_OK) {
            
            //Отправить сообщение об ошибке
            for (int i=0; i<PM_ERROR_CODE_LEN; i++){
              if (PM_error_codes[i].error_code == retval) {
                DC_debugOut("% PY init error #0x%02X - %s\r\n", retval, PM_error_codes[i].description);
                break;
              }
            }
            
            DC_set.PY_scryptData.status = PY_SCRIPT_ERROR;
            
          }else{
            
            scryptStop = false;
            retval = pm_run((uint8_t*)DC_set.PY_scryptData.Name);
            
            vTaskDelay(50); //Задержка на обработку
            xTimerStop( PY_timer, 0 ); //Остановить таймер
            
            //Остановить скрипт, если ошибка
            if (retval != PM_RET_OK) {
              
              //Отправить сообщение об ошибке
              for (int i=0; i<PM_ERROR_CODE_LEN; i++){
                if (PM_error_codes[i].error_code == retval) {
                  DC_debugOut("% PY start error #0x%02X - %s\r\n", retval, PM_error_codes[i].description);
                  break;
                }
              }
              
              DC_debugOut("% Script stopped\r\n");
              DC_set.PY_scryptData.status = PY_SCRIPT_ERROR;
            }
          }
          
          //Очистить список callback
          for (int i=0; i<PY_COUNT_CALLBACKS; i++) {
            PY_callback[i].callback_status = PY_CALLBACK_EMPTY;
          }
          
        }else{
          DC_debugOut("% Memory selection error\r\n");
        }
        
      }else{//Check CRC in SRAM
        DC_debugOut("% CRC VM ERROR\r\n");
        DC_set.PY_scryptData.status = PY_SCRIPT_ERROR;
      }
    }else{//Copy from NAND to SRAM
      DC_debugOut("% Can't read VM script from NAND\r\n");
      DC_set.PY_scryptData.status = PY_SCRIPT_ERROR;
    }
    
  }
}
//----------------------------------------------------------------------------------------------------
//Таймер
static void PY_TimerHandler( TimerHandle_t xTimer )
{
  pm_vmPeriodic(1000); //1 ms
}
//----------------------------------------------------------------------------------------------------
//Инициализация задачи
void TASK_script_init(uint8_t priority) {  
  PY_timer = xTimerCreate( "PY_Timer", 1, pdTRUE, (void*)0, PY_TimerHandler );
  xTaskCreate(vTASK_script,(char*)"TASK_script", configMINIMAL_STACK_SIZE+256, NULL, tskIDLE_PRIORITY + priority, &script_handle);
}
//----------------------------------------------------------------------------------------------------
//Добавить очередь 
void PY_add_callbackID(uint8_t id_callback) {
  if (PY_callback_queue_len < PY_COUNT_CALLBACKS) {
    PY_callback_queue[PY_callback_queue_len++] = id_callback;
  }
}
//----------------------------------------------------------------------------------------------------
//Получить из очереди 
int PY_get_callbackID() {
  if (PY_callback_queue_len > 0)
    return PY_callback_queue[--PY_callback_queue_len];
  return -1;
}
//----------------------------------------------------------------------------------------------------
//Старт скрипта
DEV_Status_t PY_startScript()
{
  uint8_t tryCounter = 100;
  
  //Task start
  vTaskResume(script_handle);
  
  while(tryCounter--)
  {
    if (DC_set.PY_scryptData.status != PY_SCRIPT_NLOAD)
      return DEV_OK;
    vTaskDelay(100);
  }
  
  return DEV_ERROR;
}
//----------------------------------------------------------------------------------------------------
//Остановка скрипта
DEV_Status_t PY_stopScript()
{
  if (DC_set.PY_scryptData.status == PY_SCRIPT_START)
  {
    scryptStop = true;    
    return DEV_OK;
  }
  
  return DEV_ERROR;
}
//----------------------------------------------------------------------------------------------------
//Приостановка скрипта
DEV_Status_t PY_pauseScript()
{
  if (DC_set.PY_scryptData.status == PY_SCRIPT_START)
  {
    //Task pause
    vTaskSuspend(script_handle);
    
    return DEV_OK;
  }
  
  return DEV_ERROR;
}