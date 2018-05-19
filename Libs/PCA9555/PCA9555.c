
#include "PCA9555.h"
#include "DevCTRL.h"

I2C_HandleTypeDef *p_PCA9555_i2cH;

//Get reg val
HAL_StatusTypeDef PCA9555_regGetValue(uint8_t address, uint8_t regAddr, uint16_t *regVal);
//Set reg val
HAL_StatusTypeDef PCA9555_regSetValue(uint8_t address, uint8_t regAddr, uint16_t regVal);

//--------------------------------------------------------------------------------------------------
//Init
void PCA9555_init(I2C_HandleTypeDef *p_i2cH)
{
  p_PCA9555_i2cH = p_i2cH;
  
}
//--------------------------------------------------------------------------------------------------
//Set pin mode
HAL_StatusTypeDef PCA9555_setPinMode(uint8_t address, uint8_t pin, uint8_t IOMode)
{
  HAL_StatusTypeDef stat;
  uint16_t regVal;
    
  if (pin > 15 ){
    return HAL_ERROR;
  }
  
  //Read
  if ((stat = PCA9555_regGetValue(address,  PCA9555_REG_CONFIG, &regVal)) != HAL_OK)
    return stat;
  
  if (IOMode == PCA9555_IO_OUTPUT) {
    regVal = regVal & ~(1 << pin);
  } else {
    regVal = regVal | (1 << pin);
  }
  
  //Write
  if ((stat = PCA9555_regSetValue(address,  PCA9555_REG_CONFIG, regVal)) != HAL_OK)
    return stat;
  
  return stat;
}
//--------------------------------------------------------------------------------------------------
//Read digital
HAL_StatusTypeDef PCA9555_digitalRead(uint8_t address, uint8_t pin, uint8_t *state) 
{
  HAL_StatusTypeDef stat;
  uint16_t regVal;
  
  if (pin > 15 ){
    return HAL_ERROR;
  }
  
  //Read
  if ((stat = PCA9555_regGetValue(address,  PCA9555_REG_INPUT, &regVal)) != HAL_OK)
    return stat;
  
  if ((regVal & (1 << pin)) > 0){
    *state = 1;
  } else {
    *state = 0;
  }
  
  return stat;
}
//--------------------------------------------------------------------------------------------------
//Write digital
HAL_StatusTypeDef PCA9555_digitalWrite(uint8_t address, uint8_t pin, uint8_t state) 
{
  HAL_StatusTypeDef stat;
  uint16_t regVal;
  
  if (pin > 15 ){
    return HAL_ERROR;
  }
  
   //Read
  if ((stat = PCA9555_regGetValue(address,  PCA9555_REG_OUTPUT, &regVal)) != HAL_OK)
    return stat;
  
  if (state == 1) {
    regVal = regVal | (1 << pin);    // and OR bit in register
  } else {
    regVal = regVal & ~(1 << pin);    // AND all bits
  }
  
   //Write
  if ((stat = PCA9555_regSetValue(address,  PCA9555_REG_OUTPUT, regVal)) != HAL_OK)
    return stat;
  
  return stat;
}
//--------------------------------------------------------------------------------------------------
//Get reg val
HAL_StatusTypeDef PCA9555_regGetValue(uint8_t address, uint8_t regAddr, uint16_t *regVal) 
{ 
  return HAL_I2C_Mem_Read(p_PCA9555_i2cH, address, regAddr, I2C_MEMADD_SIZE_16BIT, (uint8_t*)regVal, 2,PCA9555_I2C_TIMEOUT);
}
//--------------------------------------------------------------------------------------------------
//Set reg val
HAL_StatusTypeDef PCA9555_regSetValue(uint8_t address, uint8_t regAddr, uint16_t regVal)
{
  return HAL_I2C_Mem_Write(p_PCA9555_i2cH, address, regAddr, I2C_MEMADD_SIZE_16BIT, (uint8_t*)&regVal, 2,PCA9555_I2C_TIMEOUT);
}