
#ifndef FW_UPDATE_H
#define FW_UPDATE_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_flash_ex.h"

//**********************************FW NAND**************************************************

#define FW_IMAGE_START_SECTOR           FLASH_SECTOR_2 // 32KBytes for bootloader
#define FW_IMAGE_END_SECTOR             FLASH_SECTOR_TOTAL

#define FW_IMAGE_START_ADDRESS          0x00008000UL
#define FW_IMAGE_END_ADDRESS            0x080FFFFFUL

#define FW_GOTO_FW                      0x00
#define FW_NEW_FW                       0x01

typedef struct
{
  uint8_t FW_mKey; //FW magic key
  uint16_t FW_curent_ver; //FW current version
  uint16_t FW_new_ver; //FW new version
  uint64_t FW_size; //SizeFW in bytes
  uint8_t FW_CRC; //Firmware CRC
  
}FW_metadata_t;

//**********************************FW SD CARD***********************************************

#define FW_SD_BIN_FILE_NAME     "FW.bin"
#define FW_SD_INF_FILE_NAME     "FW.inf"


#endif