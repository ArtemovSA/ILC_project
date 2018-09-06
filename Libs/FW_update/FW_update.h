
#ifndef FW_UPDATE_H
#define FW_UPDATE_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_flash_ex.h"
#include "Memory.h"
#include "deviceDefs.h"

//**********************************FW prams***********************************************

//CMD list
#define FW_GOTO_FW                      0x00    //Not use
#define FW_UDATE_FW                     0x01    //Check versions and write new
#define FW_REWRITE_FW                   0x02    //Update without check version

//FW metadata
typedef struct
{
  uint8_t FW_cmd; //FW cmd
  uint16_t FW_new_ver; //FW new version
  uint64_t FW_size; //SizeFW in bytes
  uint8_t FW_CRC; //Firmware CRC
}FW_metadata_t;

//Read infodata
DEV_Status_t FW_readInfodata(DEV_info_t* info);

//**********************************FW NAND**************************************************

//Nand struct
#define FW_IMAGE_START_SECTOR           FLASH_SECTOR_2 // 32KBytes for bootloader
#define FW_IMAGE_END_SECTOR             FLASH_SECTOR_TOTAL

#define FW_IMAGE_START_ADDRESS          0x08008000UL
#define FW_IMAGE_END_ADDRESS            0x080FFFFFUL

//Read nand metadata
DEV_Status_t FW_readNandMetadata(FW_metadata_t* metadata);
//Nand update
DEV_Status_t FW_nandUpdate(FW_metadata_t* metadata);

//**********************************FW SD CARD***********************************************

#define FW_CMD_PARCE            "#Cmd: %d%n"
#define FW_VERSION_PARCE        "#Version: %d%n"
#define FW_SIZE_PARCE           "#SizeFW: %ld%n"
#define FW_CRC_PARCE            "#CRC: %d%n"

#define FW_SD_BIN_FILE_NAME     "FW.bin"        //FW file
#define FW_SD_INF_FILE_NAME     "FW.inf"        //FW metadata file

//Read card metadata
DEV_Status_t FW_readCardMetadata(FW_metadata_t* metadata);
//SD card update
DEV_Status_t FW_SDcardUpdate(FW_metadata_t* metadata);


#endif