#dev_sys.py
#
#Artemov Sergey
#19.08.2018

"""__NATIVE__
#include "stdint.h"
#include "string.h"
#include "TASK_script.h"
#include "DevCTRL.h"
#include "stm32f4xx_hal.h"

char str_buf[256]; //Global buffer

//Get uint8_t value by index
uint8_t py_get_uint8_t(uint8_t id) {
	pPmObj_t val_in;
	val_in = NATIVE_GET_LOCAL(id);
	return ((pPmInt_t)val_in)->val;
}

//Get float value by index
float py_get_float(uint8_t id) {
	pPmObj_t val_in;
	val_in = NATIVE_GET_LOCAL(id);
	return ((pPmFloat_t)val_in)->val;
}

//Get string value by index
void py_get_string(uint8_t id, char *str, uint16_t *len) {
	pPmObj_t val_in;
	
	val_in = NATIVE_GET_LOCAL(id);
	memset(str,0,sizeof(str));
	memcpy(str, ((pPmString_t)val_in)->val, ((pPmString_t)val_in)->length);
	*len = ((pPmString_t)val_in)->length;
}

//Get function name
PmReturn_t py_get_functionName(char *name) {
	pPmObj_t pframe;
	pPmObj_t pstr;
	PmReturn_t retval = PM_RET_OK;
	
	/* Get the top frame */
	pframe = (pPmObj_t)gVmGlobal.pthread->pframe;
	
	retval = tuple_getItem((pPmObj_t)((pPmFrame_t)pframe)->fo_func->f_co->co_names, -1, &pstr);	
	sprintf(name,"%s",((pPmString_t)pstr)->val);

	return retval;
}

//Add int to list
PmReturn_t py_add_int_toList(int val, pPmObj_t *list) {
	pPmObj_t pi;
	PmReturn_t retval = PM_RET_OK;

	retval = int_new(val, &pi);
	if (retval == PM_RET_OK)
		retval = list_append(*list, pi);

	return retval;
}

//Add string to list
PmReturn_t py_add_str_toList(char* str, pPmObj_t *list) {
	pPmObj_t ps;
	PmReturn_t retval = PM_RET_OK;

	retval = string_new(str, &ps);

	if (retval == PM_RET_OK)
		retval = list_append(*list, ps);

	return retval;
}

//Write error
void py_error_write(uint8_t error_code, uint8_t source) {

	pPmObj_t pframe;
	pPmObj_t pstr;
		
	PM_error.source = source;
	PM_error.error_code = error_code;

	/* Get the top frame */
	pframe = (pPmObj_t)gVmGlobal.pthread->pframe;
	tuple_getItem((pPmObj_t)((pPmFrame_t)pframe)->fo_func->f_co->co_names, -1, &pstr);	
	memcpy(PM_error.thread_name,((pPmString_t)pstr)->val,sizeof(PM_error.thread_name));
}
"""

#Delay ms
def delay_ms(ms):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	pPmObj_t ms_val;
	ms_val = NATIVE_GET_LOCAL(0);
	vTaskDelay(((pPmInt_t)ms_val)->val);
	NATIVE_SET_TOS(PM_NONE);
	return retval;
     """
	pass

#Остановить поток
def kill_Thread():
	"""__NATIVE__
	PmReturn_t retval = PM_RET_EX;
	return retval;
	"""

#Получить имя потока
def func_name():
	"""__NATIVE__
	char buf[30];
	pPmObj_t ps;
	pPmObj_t pframe;
	pPmObj_t pstr;
	
	PmReturn_t retval = PM_RET_OK;
	
	/* Get the top frame */
	pframe = (pPmObj_t)gVmGlobal.pthread->pframe;
	
	retval = tuple_getItem((pPmObj_t)((pPmFrame_t)pframe)->fo_func->f_co->co_names, -1, &pstr);	
	sprintf(buf,"%s",((pPmString_t)pstr)->val);	
	if (retval == PM_RET_OK) {
		retval = string_new(buf, &ps);
    		NATIVE_SET_TOS(ps);
    	}
    	
	return retval;
	"""
	pass

#Получить описание ошибки
def get_error():
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	pPmObj_t pl;
	uint8_t const *str = (uint8_t const *)PM_error.thread_name;

	uint8_t const *pstr = str;
	
	retval = list_new(&pl);
	py_add_int_toList(PM_error.source, &pl);
	py_add_int_toList(PM_error.error_code, &pl);
	py_add_str_toList((char*)pstr, &pl);

	NATIVE_SET_TOS(pl);
	return retval;
	"""
	pass

#Разрешить callback
def addCallback(func):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	pPmObj_t pf;
	pPmObj_t ps;
	char buf[20];

	/* If wrong number of args, raise TypeError */
	if (NATIVE_GET_NUM_ARGS() != 1)
	{
	   PM_RAISE(retval, PM_RET_EX_TYPE);
	   return retval;
	}

	//Check type
	pf = NATIVE_GET_LOCAL(0);
	if (OBJ_GET_TYPE(pf) != OBJ_TYPE_FXN)
	{
	   PM_RAISE(retval, PM_RET_EX_TYPE);
	   return retval;
	}

	//Get function name
	retval = tuple_getItem((pPmObj_t)((pPmFunc_t)pf)->f_co->co_names, -1, &ps);	
	sprintf(buf,"%s",((pPmString_t)ps)->val);	

	//Search function by name
	for (int i=0; i<PY_COUNT_CALLBACKS; i++) {
		if (strstr(PY_callback[i].callback_name, buf) !=0){
			PY_callback[i].PY_func = pf; //Record function
			PY_callback[i].callback_status = PY_CALLBACK_STOP; //Record status
			NATIVE_SET_TOS(PM_NONE);
			retval = PM_RET_OK;
			return retval;
		}
	}
	
	NATIVE_SET_TOS(PM_NONE);
	retval = PM_RET_NO;
	return retval;
	"""
	pass

#Очистить выполнение callback
def ClearCallback():
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	char name[20];
	
	//Get function name
     py_get_functionName(name); 	

	//Search function by name
	for (int i=0; i<PY_COUNT_CALLBACKS; i++) {
		if (strstr(PY_callback[i].callback_name, name) !=0){
			PY_callback[i].callback_status = PY_CALLBACK_STOP; //Record status
			NATIVE_SET_TOS(PM_NONE);
			retval = PM_RET_OK;
			return retval;
		}
	}
	
	NATIVE_SET_TOS(PM_NONE);
	retval = PM_RET_NO;
	return retval;
	"""
	pass
	
#Получить значение callback
def getCallbackValue():
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	char name[20];
	pPmObj_t pl;
	
	//Get function name
     py_get_functionName(name); 	

     //Search function by name
	for (int i=0; i<PY_COUNT_CALLBACKS; i++) {
		if (strstr(PY_callback[i].callback_name, name) !=0){
			retval = list_new(&pl);

			//Make params
			switch(i) {
			case PY_MQTT_CALLBACK:
				break;
			case PY_GPIO_CALLBACK:	
				break;
			case PY_MODEM_CALLBACK:
				break;
			}
			
			NATIVE_SET_TOS(pl);
			retval = PM_RET_OK;
			return retval;
		}
	}
	
	NATIVE_SET_TOS(PM_NONE);
	retval = PM_RET_NO;
	return retval;
	"""
	pass

#Обработать ошибку если она есть
def error_proc():
	error = dev_sys.get_error() #Получить ошибку
	error_code = error[1]
	if (error_code != ER_OK):
		source = error[0]
		thread_name = error[3]
		print "Error code:", error_code
		print "Error source:", source
		print "Name:", thread_name
		print "Thread kill"
		dev_sys.delay_ms(100)
		dev_sys.kill_Thread() #Остановить поток