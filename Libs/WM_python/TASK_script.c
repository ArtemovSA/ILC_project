
#include "TASK_script.h"
#include "PY_usr.h"
#include "DevCTRL.h"

TimerHandle_t PY_timer; //Таймер
xTaskHandle script_handle; //Хендл скрипта
xQueueHandle queue_script_commands; //Очередь комманд
uint8_t PY_task; //Выполнение задачи скрипта
uint32_t PY_script_addr; //Адрес скрипта
NAND_AddressTypeDef PY_script_struct_addr; //Метаданные виртуальной машины
PY_scryptData_t PY_scryptData; //Описание скрипта
PY_error_t PY_error; //Ошибка скрипта

uint8_t PY_callback_queue[PY_COUNT_CALLBACKS]; //Очередь callback
uint8_t PY_callback_queue_len; //Длина очереди

//Глабальные переменные возвращаемые в callback
PY_var_SMS_CALLBACK_t PY_var_SMS_CALLBACK;
PY_var_MQTT_CALLBACK_t PY_var_MQTT_CALLBACK;
PY_var_GPIO_CALLBACK_t PY_var_GPIO_CALLBACK;
PY_var_MODEM_CALLBACK_t PY_var_MODEM_CALLBACK;
PY_var_RF_CALLBACK_t PY_var_RF_CALLBACK;

//Структура функций Callback
PY_callback_t PY_callback[PY_COUNT_CALLBACKS] = {
  {0 , DEF_PY_SMS_CALLBACK, PY_CALLBACK_EMPTY, (void *)&PY_var_SMS_CALLBACK},
  {0 , DEF_PY_MQTT_CALLBACK, PY_CALLBACK_EMPTY, (void *)&PY_var_MQTT_CALLBACK},
  {0 , DEF_PY_GPIO_CALLBACK, PY_CALLBACK_EMPTY, (void *)&PY_var_GPIO_CALLBACK},
  {0 , DEF_PY_MODEM_CALLBACK, PY_CALLBACK_EMPTY, (void *)&PY_var_MODEM_CALLBACK},
  {0 , DEF_PY_RF_CALLBACK, PY_CALLBACK_EMPTY, (void *)&PY_var_RF_CALLBACK}
};

static void PY_TimerHandler( TimerHandle_t xTimer ); //Таймер

//----------------------------------------------------------------------------------------------------
void vTASK_script(void *pvParameters) {
  
  PmReturn_t retval = PM_RET_OK; //Return value
  PY_task = PY_SCRIPT_NLOAD;
  NAND_AddressTypeDef PY_WM_data_addr = MEM_NAND_ADDR_WM_DATA;
  
  //Read description data
  if (MEM_NAND_readData(PY_WM_data_addr, 0, (uint8_t*)&PY_scryptData, sizeof(PY_scryptData_t)) != DEV_OK)
  {
    DC_debugOut("Can't read WM descr from NAND\r\n");
    vTaskResume( script_handle );
  }else{
    //Copy from NAND to SRAM
    if (MEM_NAND_to_SRAM(PY_scryptData.memoryID, 0, PY_WM_data_addr, sizeof(PY_scryptData_t), PY_scryptData.len) != DEV_OK)
    {
      DC_debugOut("Can't read WM script from NAND\r\n");
      vTaskResume( script_handle );
    }
    
    //Check CRC in SRAM
    if ( MEM_checkCRC8_SRAM(MEM_ID_t memID, uint8_t crc, uint32_t addr, uint32_t len) != DEV_OK)
    {
      DC_debugOut("CRC WM ERROR\r\n");
      vTaskResume( script_handle );
    }
  }
  
  while (1) {
    
    
    
    
    //Вычисляется адрес скрипта в памяти относительно его номера
    PY_script_struct_addr = FADR_SCRIPT_START + num * PY_SCRIPT_LEN;
    
    //Читать структуру из памяти
    if ( xSemaphoreTake(xMutexFlash, MUTEX_FLASH_DELAY) == pdTRUE ) {
      EXT_Flash_readData(PY_script_struct_addr, (uint8_t*) &script_buf, sizeof(script_buf)); //Читать данные из флеш
      xSemaphoreGive(xMutexFlash);
      
      //Если скрипт не используется или это первый запуск
      if ((script_buf.state == PY_SCRIPT_NUSE)||(PY_main_script.state == PY_SCRIPT_NLOAD)) {
        
        PY_main_script = script_buf; //Копировать параметры скрипта
        
        PY_script_addr = PY_script_struct_addr + EXT_FLASH_PACK_SIZE; //Вычисляю адрес начала скрипта
        
        //Записать статус
        PY_main_script.state = PY_SCRIPT_USE;
        EXT_Flash_ReWriteData(PY_script_struct_addr, (uint8_t*) &PY_main_script, sizeof(PY_main_script));
        
        //PY_task = PY_SCRIPT_START; //Запустить
      }
    }else{
      PY_task = PY_SCRIPT_STOP;
    }
    
    if (PY_task == PY_SCRIPT_START) {
      xTimerStart( PY_timer, 0 ); //Запустить таймер
      //Инициализировать скрипт
      retval = pm_init(MEMSPACE_FLASH, (uint8_t const*)PY_script_addr);
      retval = pm_run((uint8_t*)PY_main_script.name);
    }
    
    vTaskDelay(50); //Задержка на обработку
    xTimerStop( PY_timer, 0 ); //Остановить таймер
    
    //Остановить скрипт, если ошибка
    if (retval != PM_RET_OK) {
      
      //Записать статус как ошибочный скрипт
      PY_main_script.state = PY_ERROR_START;
      xSemaphoreTake(xMutexFlash, MUTEX_FLASH_DELAY);
      EXT_Flash_ReWriteData(PY_script_struct_addr, (uint8_t*) &PY_main_script, sizeof(PY_main_script));
      xSemaphoreGive(xMutexFlash);
      
      //Отправить сообщение об ошибке
      for (int i=0; i<sizeof(PY_error_codes); i++){
        if (PY_error_codes[i].error_code == retval) {
          sprintf(str, "\n\rError #%02X - %s",retval, PY_error_codes[i].description);
          break;
        }
      }
      sprintf(str, "%s\n\rScript stopped",str);
      TM_USB_VCP_Puts(str);
    }else{
      TM_USB_VCP_Puts("\n\rScript stopped");
    }
  
    //Очистить список callback
    for (int i=0; i<PY_COUNT_CALLBACKS; i++) {
      PY_callback[i].callback_status = PY_CALLBACK_EMPTY;
    }
    
    //Изменить статус скрипта
    PY_task = PY_SCRIPT_STOP;
    
    while (PY_task != PY_SCRIPT_START) {//Ждать
      vTaskDelay(100);
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
//Запустить скрипт
void PY_StartScript(uint8_t nub_script) {
  
  uint32_t script_addr; //Адрес скрипта
  PY_script_type script; //Пременная описывающая скрипт
  
  //Если это другой скрипт
  if (PY_main_script.num != nub_script) {
  
    //Вычисляется адрес пакета скрипта в памяти
    script_addr = FADR_SCRIPT_START + nub_script * PY_SCRIPT_LEN;
    
    EXT_Flash_readData(script_addr, (uint8_t*) &script, sizeof(script)); //Читать данные из флеш
    if (script.state != 0x00) { //Если скрипт был загружен
      PY_task = PY_SCRIPT_STOP; //Останосить предыдущий скрипт
      PY_main_script = script; //Скопировать параметры
    }
  }
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