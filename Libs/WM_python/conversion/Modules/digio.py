#digio.py
#
#Artemov Sergey
#19.08.2018
#Relay out and digital input

def relayOut_nat(relNum, state):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	HAL_StatusTypeDef retstat;
	
	//Getting values
	uint8_t relNum = py_get_uint8_t(0);
	uint8_t state = py_get_uint8_t(1);

	//Relay out
	retstat = DC_relayOut(relNum, state);
	
	if (retstat != HAL_OK)
	{
		py_error_write((uint8_t)retstat, relNum);
		retval = PM_RET_ERR;
	}
	
	NATIVE_SET_TOS(PM_NONE);
	return retval;
	"""
	pass
	
def relayOut(relNum, state):
	val = relayOut_nat(relNum, statel)
	dev_error.error_proc()
	return val
	
LED_LINK = 0,
LED_STATUS = 1
LED_RUN = 3
  
def ledOut(led, state):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	HAL_StatusTypeDef retstat;
	
	//Getting values
	uint8_t led = py_get_uint8_t(0);
	uint8_t state = py_get_uint8_t(1);

	//Relay out
	DC_LedOut((LED_t)led, state);
	
	NATIVE_SET_TOS(PM_NONE);
	return retval;
	"""
	return val