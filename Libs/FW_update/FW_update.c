
#include "FW_update.h"
#include "fatfs.h"
#include "string.h"
#include "stdio.h"

//FW buffer
uint8_t FW_buf[MEM_NAND_PAGE_SIZE];

//SD Card
FATFS filesystem;
FIL fileInf, fileBin;
FILINFO fno;
FRESULT ret;
uint32_t n;
uint32_t num;
uint8_t crc;
uint64_t fwSize;
char* str_p;

//--------------------------------------------------------------------------------------------------
//Read infodata
DEV_Status_t FW_readInfodata(DEV_info_t* info)
{
  NAND_AddressTypeDef addr = MEM_NAND_ADDR_INFO;
  
  //read infodata
  if (MEM_NAND_readData(addr, (uint8_t*)info, sizeof(DEV_info_t)) != HAL_OK)
  {
    return DEV_OK;
  }
  
  return DEV_ERROR;
}
//--------------------------------------------------------------------------------------------------
//Read Next str
static DEV_Status_t FW_readNextStr(char* str_p)
{
  str_p = f_gets((char*)FW_buf, sizeof(FW_buf), &fileInf);// read current line
  if (f_lseek(&fileInf,strlen(str_p) ) != FR_OK)// move to the next line
    return DEV_ERROR;
  
  return DEV_OK;
}
//--------------------------------------------------------------------------------------------------
//Read card metadata
DEV_Status_t FW_readCardMetadata(FW_metadata_t* metadata)
{
  //Mount FS
  if (f_mount(&filesystem, 0, 1) != FR_OK) {
    printf("@ SD card not found \n\r");
    return DEV_NEXIST;
  }
  
  //Открыть файл информации
  ret = f_open(&fileInf, FW_SD_INF_FILE_NAME, FA_READ);
  
  //If inf file exist
  if (ret == FR_OK) {
    
    ret = f_open(&fileBin, FW_SD_INF_FILE_NAME, FA_READ);
    if (ret == FR_OK) {
     
       //Read cmd
      if (FW_readNextStr(str_p) != DEV_OK)
        printf("@ Can't read inf \n\r");
      
      sscanf(str_p, "#Cmd: %d%n", &num, &n);
      if (n > 0)
        metadata->FW_cmd = num;
      
      //Read version
      if (FW_readNextStr(str_p) != DEV_OK)
        printf("@ Can't read inf \n\r");
      
      sscanf(str_p, "#Version: %d%n", &num, &n);
      if (n > 0)
        metadata->FW_new_ver = num;
      
      //Read size
      if (FW_readNextStr(str_p) != DEV_OK)
        printf("@ Can't read inf \n\r");
      
      sscanf(str_p, "#SizeFW: %ld%n", &num, &n);
      if (n > 0)
        metadata->FW_size = num;
      
      //Read crc
      if (FW_readNextStr(str_p) != DEV_OK)
        printf("@ Can't read inf \n\r");
      
      sscanf(str_p, "#CRC: %x02%n", &num, &n);
      if (n > 0)
        metadata->FW_CRC = num;
      
      return DEV_OK;
    }else{
      printf("@ File FW.bin not found \n\r");
      return DEV_ERROR;
    }
  }else{
    printf("@ File FW.inf not found \n\r");
    return DEV_ERROR;
  }
}
//--------------------------------------------------------------------------------------------------
//Read nand metadata
DEV_Status_t FW_readNandMetadata(FW_metadata_t* metadata)
{
  NAND_AddressTypeDef addr = MEM_NAND_ADDR_FW;
  
  //read metadata
  if (MEM_NAND_readData(addr, (uint8_t*)metadata, sizeof(FW_metadata_t)) != HAL_OK)
  {
    printf("@ NAND IO ERROR\r\n");
    return DEV_ERROR;
  }
  
  return DEV_OK;
}