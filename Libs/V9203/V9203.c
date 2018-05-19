
#include "V9203.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"
#include "String.h"
#include "DevCTRL.h"
#include "PCA9555.h"

//Var
SPI_HandleTypeDef *V9203_hspi;

//--------------------------------------------------------------------------------------------------
//Init
void V9203_init(SPI_HandleTypeDef *hspi)
{
  V9203_hspi = hspi;
}
//--------------------------------------------------------------------------------------------------
//Set chip select
HAL_StatusTypeDef V9203_set_CS(uint8_t channel, uint8_t state)
{
  uint8_t pin;
  
  switch(channel)
  {
  case 1: pin = PCA9555_PIN_CS1; break;
  case 2: pin = PCA9555_PIN_CS2; break;
  case 3: pin = PCA9555_PIN_CS3; break;
  case 4: pin = PCA9555_PIN_CS4; break;
  default: DC_debugOut("# V9203 CS channel num ERROR\r\n"); return HAL_ERROR;
  };
  
  return PCA9555_digitalWrite(PCA9555_DEF_ADDR, pin, state);
}
//--------------------------------------------------------------------------------------------------
//Set data|cmd
HAL_StatusTypeDef V9203_data_cmd_flash(uint8_t channel, uint8_t cmd, uint16_t dataTx, uint16_t *dataRx)
{
  HAL_StatusTypeDef state;
  uint8_t txBuf[4];
  uint8_t rxBuf[4];

  txBuf[0] = (0x3f & cmd) | 0x80;
  txBuf[1] = HI(dataTx);
  txBuf[2] = LO(dataTx);
  txBuf[3] = ~(LO(dataTx) + HI(dataTx) + txBuf[0]);
  
  if ((state = V9203_set_CS(channel, LOW_LEVEL)) != HAL_OK)
    return state;
  
  if ((state = HAL_SPI_TransmitReceive(V9203_hspi, txBuf, rxBuf, 4, V9203_SPI_TIMEOUT)) != HAL_OK)
     return state;
  
  if ((state = V9203_set_CS(channel, HIGH_LEVEL)) != HAL_OK)
    return state;
  
  if (dataRx != NULL)
    memcpy(dataRx, &rxBuf[1], 2); //Copy
  
  return state;
}
//--------------------------------------------------------------------------------------------------
//Write flash
HAL_StatusTypeDef V9203_wr_flash(uint8_t channel, uint16_t addrReg, uint32_t data)
{
  HAL_StatusTypeDef state;
  
  if ((state = V9203_data_cmd_flash(channel, V9203_CMD_WRITE_LO_WORD, LO16(data), NULL)) != HAL_OK)
    return state;
  
  if ((state = V9203_data_cmd_flash(channel, V9203_CMD_WRITE_HI_WORD, HI16(data), NULL)) != HAL_OK)
    return state;
  
  if ((state = V9203_data_cmd_flash(channel, V9203_CMD_WRITE_TO_REG, addrReg, NULL)) != HAL_OK)
    return state;
  
  return state;
}
//--------------------------------------------------------------------------------------------------
//Read flash
HAL_StatusTypeDef spi_rd_flash(uint8_t channel, uint16_t addrReg, uint32_t* data)
{
  unsigned long mt_dout;
  
  set_data_cmd_flash(0x10, addr); 
  set_data_cmd_flash(0x12, addr);
  mt_dout = (unsigned long)tdo_d;
  if((unsigned char)(tdo_m+(unsigned char)(tdo_d&0x00ff)+(unsigned char)(tdo_d>>8))!=(unsigned char)(~tdo_c))
  {
    spi_err = 1;
  }
  else
  {
    spi_err = 0;
  }
  set_data_cmd_flash(0x14, addr);
  mt_dout += (((unsigned long)tdo_d)<<16);
  if((unsigned char)(tdo_m+(unsigned char)(tdo_d&0x00ff)+(unsigned char)(tdo_d>>8))!=(unsigned char)(~tdo_c))
  {
    spi_err = 1;
  }
  else
  {
    spi_err = 0;
  }
  
  return mt_dout;
}