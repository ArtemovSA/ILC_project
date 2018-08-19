
#include "TASK_script.h"
#include "PY_usr.h"
#include "DevCTRL.h"
#include "py_plat.h"

TimerHandle_t PY_timer; //������
xTaskHandle script_handle; //����� �������
xQueueHandle queue_script_commands; //������� �������
uint8_t PY_task; //���������� ������ �������
uint32_t PY_script_addr; //����� �������
NAND_AddressTypeDef PY_script_struct_addr; //���������� ����������� ������
PY_scryptData_t PY_scryptData; //�������� �������

uint8_t PY_callback_queue[PY_COUNT_CALLBACKS]; //������� callback
uint8_t PY_callback_queue_len; //����� �������
uint8_t PY_heap[PM_HEAP_SIZE]; //Python heap

//���������� ���������� ������������ � callback
PY_var_SMS_CALLBACK_t PY_var_SMS_CALLBACK;
PY_var_MQTT_CALLBACK_t PY_var_MQTT_CALLBACK;
PY_var_GPIO_CALLBACK_t PY_var_GPIO_CALLBACK;
PY_var_MODEM_CALLBACK_t PY_var_MODEM_CALLBACK;

//��������� ������� Callback
PY_callback_t PY_callback[PY_COUNT_CALLBACKS] = {
  {0 , DEF_PY_SMS_CALLBACK, PY_CALLBACK_EMPTY, (void *)&PY_var_SMS_CALLBACK},
  {0 , DEF_PY_MQTT_CALLBACK, PY_CALLBACK_EMPTY, (void *)&PY_var_MQTT_CALLBACK},
  {0 , DEF_PY_GPIO_CALLBACK, PY_CALLBACK_EMPTY, (void *)&PY_var_GPIO_CALLBACK},
  {0 , DEF_PY_MODEM_CALLBACK, PY_CALLBACK_EMPTY, (void *)&PY_var_MODEM_CALLBACK},
};

static void PY_TimerHandler( TimerHandle_t xTimer ); //������

//----------------------------------------------------------------------------------------------------
void vTASK_script(void *pvParameters) {
  
  PmReturn_t retval = PM_RET_OK; //Return value
  PY_task = PY_SCRIPT_NLOAD;
  NAND_AddressTypeDef PY_VM_data_addr = MEM_NAND_ADDR_VM_DATA;

  while (1) {
    
    //Task pause
    vTaskSuspend(script_handle);
      
    //Read description data
    if (MEM_NAND_readData(PY_VM_data_addr, 0, (uint8_t*)&PY_scryptData, sizeof(PY_scryptData_t)) != DEV_OK)
    {
      DC_debugOut("Can't read VM descr from NAND\r\n");
      vTaskResume( script_handle );
    }else{
      
      //Copy from NAND to SRAM
      if (MEM_NAND_to_SRAM(PY_scryptData.memoryID, MEM_SRAM2_SCRYPT_ADDR, PY_VM_data_addr, sizeof(PY_scryptData_t), PY_scryptData.len) != DEV_OK)
      {
        DC_debugOut("Can't read VM script from NAND\r\n");
        vTaskResume( script_handle );
      }
      
      //Check CRC in SRAM
      if ( MEM_checkCRC8_SRAM(PY_scryptData.memoryID, PY_scryptData.crc, MEM_SRAM2_SCRYPT_ADDR, PY_scryptData.len) != DEV_OK)
      {
        DC_debugOut("CRC VM ERROR\r\n");
        vTaskResume( script_handle );
      }
    }

    
    xTimerStart( PY_timer, 0 ); //��������� ������
    //���������������� ������
    retval = pm_init(PY_heap, PM_HEAP_SIZE, (PmMemSpace_t)PY_scryptData.memoryID, MEM_SRAM2_SCRYPT_ADDR);
    retval = pm_run((uint8_t*)PY_scryptData.ModuleName);
    
    vTaskDelay(50); //�������� �� ���������
    xTimerStop( PY_timer, 0 ); //���������� ������
    
    //���������� ������, ���� ������
    if (retval != PM_RET_OK) {
      
      //��������� ��������� �� ������
      for (int i=0; i<PM_ERROR_CODE_LEN; i++){
        if (PM_error_codes[i].error_code == retval) {
          DC_debugOut("PY Error #%02X - %s", retval, PM_error_codes[i].description);
          break;
        }
      }
      
      DC_debugOut("Script stopped");
    }
    
    //�������� ������ callback
    for (int i=0; i<PY_COUNT_CALLBACKS; i++) {
      PY_callback[i].callback_status = PY_CALLBACK_EMPTY;
    }

  }
}
//----------------------------------------------------------------------------------------------------
//������
static void PY_TimerHandler( TimerHandle_t xTimer )
{
  pm_vmPeriodic(1000); //1 ms
}
//----------------------------------------------------------------------------------------------------
//������������� ������
void TASK_script_init(uint8_t priority) {  
  PY_timer = xTimerCreate( "PY_Timer", 1, pdTRUE, (void*)0, PY_TimerHandler );
  xTaskCreate(vTASK_script,(char*)"TASK_script", configMINIMAL_STACK_SIZE+256, NULL, tskIDLE_PRIORITY + priority, &script_handle);
}
//----------------------------------------------------------------------------------------------------
//�������� ������� 
void PY_add_callbackID(uint8_t id_callback) {
  if (PY_callback_queue_len < PY_COUNT_CALLBACKS) {
    PY_callback_queue[PY_callback_queue_len++] = id_callback;
  }
}
//----------------------------------------------------------------------------------------------------
//�������� �� ������� 
int PY_get_callbackID() {
  if (PY_callback_queue_len > 0)
      return PY_callback_queue[--PY_callback_queue_len];
  return -1;
}