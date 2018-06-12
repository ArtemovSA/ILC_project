
#ifndef PCA9555_H
#define PCA9555_H

#define PCA9555_REG_INPUT      0
#define PCA9555_REG_OUTPUT     2
#define PCA9555_REG_INVERT     4
#define PCA9555_REG_CONFIG     6

#define PCA9555_I2C_TIMEOUT  500

#define PCA9555_IO_INPUT      0
#define PCA9555_IO_OUTPUT     1

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

//Init
void PCA9555_init(I2C_HandleTypeDef *p_i2cH);
//Set pin mode
HAL_StatusTypeDef PCA9555_setPinMode(uint8_t address, uint8_t pin, uint8_t IOMode);
//Read digital
HAL_StatusTypeDef PCA9555_digitalRead(uint8_t address, uint8_t pin, uint8_t *state);
//Write digital
HAL_StatusTypeDef PCA9555_digitalWrite(uint8_t address, uint8_t pin, uint8_t state);
//Set reg val
HAL_StatusTypeDef PCA9555_regSetValue(uint8_t address, uint8_t regAddr, uint16_t regVal);
//Get reg val
HAL_StatusTypeDef PCA9555_regGetValue(uint8_t address, uint8_t regAddr, uint16_t *regVal);

#endif