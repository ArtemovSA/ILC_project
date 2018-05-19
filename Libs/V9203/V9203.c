
#include "V9203.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"
#include "DevCTRL.h"
#include "PCA9555.h"

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

void set_data_cmd_flash(unsigned char cmd, unsigned int dat)
	{
	
	    unsigned char cksum,cmdb;
	    unsigned int send_dat;
	    cmdb = (0x3f&cmd)|0x80;
	    cksum = ~((dat&0x00ff) + (dat>>8) + cmdb);
	 //	cksum = 0;
		send_dat=dat;
		
	
		SPI.beginTransaction(SPISettings(200000, MSBFIRST, SPI_MODE3));
		digitalWrite(_cs,LOW);
		tdo_m=SPI.transfer(cmdb);
		tdo_d=SPI.transfer((send_dat>>8));
		tdo_d=tdo_d<<8;
		tdo_d+=SPI.transfer((send_dat));
		tdo_c=SPI.transfer(cksum);
		digitalWrite(_cs,HIGH);
		SPI.endTransaction();
	}