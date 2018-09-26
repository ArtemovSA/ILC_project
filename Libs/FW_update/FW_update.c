
#include "FW_update.h"
#include "fatfs.h"
#include "string.h"
#include "stdio.h"
#include "CRC8.h"

//SD Card
FATFS filesystem;
FIL fileInf, fileBin;
FILINFO fno;
FRESULT ret;
uint32_t n;
uint32_t num;
uint8_t crc;
uint64_t fwSize;

extern void debugOut(char *str, ...);
//--------------------------------------------------------------------------------------------------
//Erace Flash
DEV_Status_t FW_eraceFlash()
{
  uint32_t SectorError = 0;
  FLASH_EraseInitTypeDef EraseInitStruct;
  
  HAL_FLASH_Unlock(); //Unlock
  
  //Erace flash
  EraseInitStruct.TypeErase = TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange = VOLTAGE_RANGE_3;
  EraseInitStruct.Sector = FW_IMAGE_START_SECTOR;
  EraseInitStruct.NbSectors = FW_IMAGE_END_SECTOR;
  
  //Try erace sector
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
  { 
    debugOut("@ FLASH erace error\r\n");
    return DEV_ERROR;
  }
  
  return DEV_OK;
}
//--------------------------------------------------------------------------------------------------
//Read infodata
DEV_Status_t FW_readInfodata(DEV_info_t* info)
{
  NAND_AddressTypeDef addr = MEM_NAND_ADDR_INFO;
  
  //read infodata
  if (MEM_NAND_readData(addr, 0, (uint8_t*)info, sizeof(DEV_info_t)) != DEV_OK)
    return DEV_ERROR;
  
  return DEV_OK;
}
char* var;
uint16_t strLEn;
//--------------------------------------------------------------------------------------------------
//Read Next str
static DEV_Status_t FW_readNextStr(FIL* fp, char* str_p, uint16_t len)
{  
  
  var = f_gets(str_p, len, fp);// read current line
  strLEn = strlen(str_p);
  //if (f_lseek(fp, strLEn) != FR_OK)// move to the next line
  //  return DEV_ERROR;
  
  return DEV_OK;
}
//--------------------------------------------------------------------------------------------------
//Read card metadata
DEV_Status_t FW_readCardMetadata(FW_metadata_t* metadata)
{    
  char str[128];
  
  //Mount FS
  if (f_mount(&filesystem, SDPath, 1) != FR_OK) {
    debugOut("@ SD card not found \n\r");
    return DEV_NEXIST;
  }
  
  //Открыть файл информации
  ret = f_open(&fileInf, FW_SD_INF_FILE_NAME, FA_OPEN_EXISTING | FA_READ);
  
  ret = f_open(&fileInf, FW_SD_INF_FILE_NAME, FA_CREATE_ALWAYS | FA_WRITE );
  f_printf(&fileInf, "Hello");
    f_close(&fileInf);
  ret =  f_mount(&filesystem, SDPath, 0);
    
  //If inf file exist
  if (ret == FR_OK) {

      //Read cmd
      if (FW_readNextStr(&fileInf, str, sizeof(str)) != DEV_OK)
        debugOut("@ Can't read inf \n\r");
      
      sscanf(str, FW_CMD_PARCE, &num, &n);
      if (n > 0)
        metadata->FW_cmd = num;
      
      //Read version
      if (FW_readNextStr(&fileInf, str, sizeof(str)) != DEV_OK)
        debugOut("@ Can't read inf \n\r");
      
      sscanf(str, FW_VERSION_PARCE, &num, &n);
      if (n > 0)
        metadata->FW_new_ver = num;
      
      //Read size
      if (FW_readNextStr(&fileInf, str, sizeof(str)) != DEV_OK)
        debugOut("@ Can't read inf \n\r");
      
      sscanf(str, FW_SIZE_PARCE, &num, &n);
      if (n > 0)
        metadata->FW_size = num;
      
      //Read crc
      if (FW_readNextStr(&fileInf, str, sizeof(str)) != DEV_OK)
        debugOut("@ Can't read inf \n\r");
      
      sscanf(str, FW_CRC_PARCE, &num, &n);
      if (n > 0)
        metadata->FW_CRC = num;
      
      ret = f_open(&fileBin, FW_SD_BIN_FILE_NAME, FA_OPEN_EXISTING | FA_READ);
      
      if (ret == FR_OK) {
        return DEV_OK;
      }else{
        debugOut("@ File FW.bin not found \n\r");
      return DEV_ERROR;
      }
      
  }else{
    debugOut("@ File FW.inf not found \n\r");
    return DEV_ERROR;
  }
}
//--------------------------------------------------------------------------------------------------
//SD card update
DEV_Status_t FW_SDcardUpdate(FW_metadata_t* metadata)
{
  uint8_t crc_val = 0;
  uint8_t FW_buf[MEM_NAND_PAGE_SIZE];
  uint32_t readBytes;
  uint64_t fwWritePoint = 0;
  
  //Открыть файл информации
  ret = f_open(&fileBin, FW_SD_BIN_FILE_NAME, FA_READ);
  
  //If file exist
  if (ret == FR_OK) {
    
    ret = f_open(&fileBin, FW_SD_BIN_FILE_NAME, FA_READ);
    
    if (ret == FR_OK) {
      
      while (f_read(&fileBin, FW_buf, sizeof(FW_buf), &readBytes))
      {
        crc8calc(&crc_val, FW_buf, readBytes);
      }
      
      if (crc_val != metadata->FW_CRC)
      {
        debugOut("@ CRC error\r\n");
        return DEV_ERROR;
      }
    }
    
    f_lseek(&fileBin, 0); //Move to start
    
    while (f_read(&fileBin, FW_buf, sizeof(FW_buf), &readBytes))
    {
      for (int i=0; i<readBytes; i++)
      {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, fwWritePoint, FW_buf[i]) != HAL_OK)
        {
          debugOut("@ FLASH WRITE ERROR\r\n");
          return DEV_ERROR;
        }
        
        if (fwWritePoint > metadata->FW_size)
        {
          debugOut("@ BIG FILE\r\n");
          return DEV_ERROR;
        }
        
        fwWritePoint++;
      }
    }
    
    return DEV_OK;
  }

  return DEV_ERROR;  
}
//--------------------------------------------------------------------------------------------------
//Read nand metadata
DEV_Status_t FW_readNandMetadata(FW_metadata_t* metadata)
{
  NAND_AddressTypeDef addr = MEM_NAND_ADDR_FW_META;
  
  //read metadata
  if (MEM_NAND_readData(addr, 0, (uint8_t*)metadata, sizeof(FW_metadata_t)) != DEV_OK)
  {
    debugOut("@ NAND IO ERROR\r\n");
    return DEV_ERROR;
  }
  
  return DEV_OK;
}
//--------------------------------------------------------------------------------------------------
//Nand update
DEV_Status_t FW_nandUpdate(FW_metadata_t* metadata)
{
  uint16_t pages = metadata->FW_size/MEM_NAND_PAGE_SIZE;
  uint16_t tail = metadata->FW_size- pages*MEM_NAND_PAGE_SIZE;
  uint8_t crc_val = 0;
  uint8_t FW_buf[MEM_NAND_PAGE_SIZE];
  NAND_AddressTypeDef addr = MEM_NAND_ADDR_FW;
  
  //Full pages
  for ( int pageNum = 0; pageNum < pages; pageNum++)
  {
    if (MEM_NAND_readData(addr, 0, FW_buf, MEM_NAND_PAGE_SIZE) != DEV_OK)
    {
      debugOut("@ NAND IO ERROR\r\n");
      return DEV_ERROR;
    }
    
    crc8calc(&crc_val, FW_buf, MEM_NAND_PAGE_SIZE);
  }
  
  //Tail
  if (MEM_NAND_readData(addr, 0, FW_buf, tail) != DEV_OK)
  {
    debugOut("@ NAND IO ERROR\r\n");
    return DEV_ERROR;
  }
  
  crc8calc(&crc_val, FW_buf, tail);
  
  //Check CRC
  if (crc_val != metadata->FW_CRC)
  {
    debugOut("@ CRC New FW inccorect\r\n");
    return DEV_ERROR;
  }
  
  //Erace Flash
  if (FW_eraceFlash() != DEV_OK)
  {
    debugOut("@ Can't erace\r\n");
    return DEV_ERROR;
  }
  
  //Write FW in memory
  //Full pages
  for ( int pageNum = 0; pageNum < pages; pageNum++)
  {
    if (MEM_NAND_readData(addr, 0, FW_buf, MEM_NAND_PAGE_SIZE) != DEV_OK)
    {
      debugOut("@ NAND IO ERROR\r\n");
      return DEV_ERROR;
    }
    
    for (int i=0; i<MEM_NAND_PAGE_SIZE; i++)
    {
      if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, FW_IMAGE_START_ADDRESS+pageNum*MEM_NAND_PAGE_SIZE+i, FW_buf[i]) != HAL_OK)
      {
        debugOut("@ FLASH WRITE ERROR\r\n");
        return DEV_ERROR;
      }
    }
  }
  
  //Tail
  if (MEM_NAND_readData(addr, 0,  FW_buf, tail) != DEV_OK)
  {
    debugOut("@ NAND IO ERROR\r\n");
    return DEV_ERROR;
  }
  
  for (int i=0; i<tail; i++)
  {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, FW_IMAGE_START_ADDRESS+pages*MEM_NAND_PAGE_SIZE+i, FW_buf[i]) != HAL_OK)
    {
      debugOut("@ FLASH WRITE ERROR\r\n");
      return DEV_ERROR;
    }
  }
  
  return DEV_OK;
}
