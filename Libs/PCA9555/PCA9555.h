
#ifndef PCA9555_H
#define PCA9555_H

#define PCA9555_I2C_TIMEOUT 500

#define PCA9555_IO_INPUT      0
#define PCA9555_IO_OUTPUT     1

//Init
void PCA9555_init(I2C_HandleTypeDef *p_i2cH);
//Set pin mode
HAL_StatusTypeDef PCA9555_setPinMode(uint8_t address, uint8_t pin, uint8_t IOMode);
//Read digital
HAL_StatusTypeDef PCA9555_digitalRead(uint8_t address, uint8_t pin, uint8_t *state);
//Write digital
HAL_StatusTypeDef PCA9555_digitalWrite(uint8_t address, uint8_t pin, uint8_t state);

#endif