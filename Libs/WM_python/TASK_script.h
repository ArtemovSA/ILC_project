
#ifndef TASK_SCRIPT_H
#define TASK_SCRIPT_H

#include "cmsis_os.h"
#include "PY_pm.h"
#include "PY_mem.h"
#include "PY_obj.h"
#include "Memory.h"

#define PY_SCRIPT_SIZE 0x20000 //128 Кбайт

//Режимы работы отладки
typedef enum {
  PY_SCRIPT_NLOAD = 0,  //Скрипт не загружен
  PY_SCRIPT_PAUSE,      //Скрипт приостановлен
  PY_SCRIPT_STOP,       //Скрипт остановлен
  PY_SCRIPT_START,      //Скрипт запущен 
  PY_SCRIPT_ERROR       //Ошибочный скрипт
} PY_scryptMode;

//Структура описания скрипта в памяти
typedef struct{
  uint32_t len;         //Длина скрипта
  uint16_t crc;         //CRC16
  MEM_ID_t memoryID;    //Тип использованной памяти
  char ModuleName[20];  //Start module name
} PY_scryptData_t;

//Настройки названий callback
#define DEF_PY_SMS_CALLBACK             "SMS_callback"                  //SMS callback
#define DEF_PY_MQTT_CALLBACK            "MQTT_callback"                 //MQTT callback
#define DEF_PY_GPIO_CALLBACK            "GPIO_callback"                 //GPIO callback
#define DEF_PY_MODEM_CALLBACK           "MODEM_callback"                //MODEM callback
#define DEF_PY_RF_CALLBACK              "RF_callback"                   //RF callback

//----------------------------------Callback------------------------------------

//Список Callback
typedef enum{
  PY_SMS_CALLBACK = 0,
  PY_MQTT_CALLBACK,
  PY_GPIO_CALLBACK,
  PY_MODEM_CALLBACK,
  PY_RF_CALLBACK,
  PY_COUNT_CALLBACKS
} PY_callback_ID;

//Статус callback
typedef enum{
  PY_CALLBACK_EMPTY = 0,
  PY_CALLBACK_STOP,
  PY_CALLBACK_RUN,
} PY_callback_stat;

//Callbacks decription
typedef struct{
  pPmObj_t PY_func;
  char callback_name[20];
  uint8_t callback_status;
  void *callback_val;
} PY_callback_t; 

//Возвращаемые переменные в callback
//SMS_CALLBACK
typedef struct{
  char Phone[20];
  char SMS_text[80];
} PY_var_SMS_CALLBACK_t;

//MQTT_CALLBACK
typedef struct{
  char ip_addr[15];
  char port[6];
  char topic[20];
  char message[128];
  uint8_t len;
} PY_var_MQTT_CALLBACK_t;

//GPIO_CALLBACK
typedef struct{
  uint8_t type_dev;
  uint8_t adr_sw;
  uint8_t adr_port;
  uint8_t value;
} PY_var_GPIO_CALLBACK_t;

//MODEM_CALLBACK
typedef struct{
  char rx_buf[256];
  uint8_t len;
} PY_var_MODEM_CALLBACK_t;

//------------------------------------------------------------------------------

extern PY_scryptData_t PY_main_script; //Основной скрипт
extern uint8_t PY_task; //Выполнение задачи скрипта
extern uint32_t PY_script_addr; //Адрес скрипта во flash
extern PY_callback_t PY_callback[PY_COUNT_CALLBACKS]; //Название функций Callback

void TASK_script_init(uint8_t priority); //Инициализация задачи
void PY_StartScript(uint8_t nub_script); //Запустить скрипт
int PY_get_callbackID(); //Получить из очереди 
void PY_add_callbackID(uint8_t id_callback); //Добавить очередь 

#endif