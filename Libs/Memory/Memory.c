#include "Memory.h"
#include "crc8.h"

#include "stm32f4xx_hal_nand.h"
#include "stm32f4xx_hal_sram.h"
#include "math.h"
#include "string.h"

//Handles
SRAM_HandleTypeDef *MEM_hSRAM1;
SRAM_HandleTypeDef *MEM_hSRAM2;
NAND_HandleTypeDef *MEM_hNAND1;

uint8_t MEM_dataBuf[MEM_NAND_PAGE_SIZE];

//--------------------------------------------------------------------------------------------------
//Init memory
void MEM_init(SRAM_HandleTypeDef *sram1, SRAM_HandleTypeDef *sram2, NAND_HandleTypeDef *nand)
{
  //Get point
  MEM_hSRAM1 = sram1;
  MEM_hSRAM2 = sram2;
  MEM_hNAND1 = nand; 
}
//--------------------------------------------------------------------------------------------------
//Select memory
void MEM_selectMem(MEM_ID_t ID)
{
  switch(ID)
  {
  case MEM_ID_NAND:
    MEM_EN_SRAM1_OFF;
    MEM_EN_SRAM2_OFF;
    break;
  case MEM_ID_SRAM1:
    MEM_EN_SRAM1_ON;
    MEM_EN_SRAM2_OFF;
    break;
  case MEM_ID_SRAM2:
    MEM_EN_SRAM1_OFF;
    MEM_EN_SRAM2_ON;
    break;
  }
}
//**************************************************************************************************
//                                      NAND
//**************************************************************************************************
//Check nand ID
DEV_Status_t MEM_NAND_checkID()
{
  DEV_Status_t stat;
  NAND_IDTypeDef NAND_id;
  
  MEM_selectMem(MEM_ID_NAND); //Select memory
  
  stat = (DEV_Status_t)HAL_NAND_Read_ID(MEM_hNAND1, &NAND_id);
  
  if (stat != DEV_OK)
    return stat;
  
  if (NAND_id.Device_Id == MEM_NAND_DEV_ID)
    return DEV_OK;
  
  return DEV_ERROR;
}
//--------------------------------------------------------------------------------------------------
//Write data NAND
DEV_Status_t MEM_NAND_writeData(NAND_AddressTypeDef address, uint16_t offset_addr, uint8_t *data, uint16_t len)
{
  DEV_Status_t stat;
  NAND_AddressTypeDef startBlockAddress = address;
  uint32_t SRAMaddress = MEM_SRAM1_ADDR_BLOCK_BUF;
  
  startBlockAddress.Page = 0;
  
  //Enable sram write
  if ((stat = (DEV_Status_t)HAL_SRAM_WriteOperation_Enable(MEM_hSRAM1)) != DEV_OK)
    return stat;
  
  //Copy block to sram buffer
  for (int i=0; i<MEM_NAND_BLOCK_SIZE; i++)
  {
    MEM_selectMem(MEM_ID_NAND); //Select memory
    if ((stat = (DEV_Status_t)HAL_NAND_Read_Page_8b(MEM_hNAND1, &startBlockAddress, MEM_dataBuf, 1)) != DEV_OK)
      return stat;
    
    MEM_selectMem(MEM_ID_SRAM1); //Select memory
    if ((stat = (DEV_Status_t)HAL_SRAM_Write_8b(MEM_hSRAM1, &SRAMaddress, MEM_dataBuf, MEM_NAND_PAGE_SIZE)) != DEV_OK)
      return stat;
    SRAMaddress += MEM_NAND_BLOCK_SIZE;
  }  

  //Erace block NAND
  MEM_selectMem(MEM_ID_NAND); //Select memory
  if ((stat = (DEV_Status_t)HAL_NAND_Erase_Block(MEM_hNAND1, &startBlockAddress)) != DEV_OK)
    return stat;
  
  //Write data
  MEM_selectMem(MEM_ID_SRAM1); //Select memory
  SRAMaddress += address.Page * MEM_NAND_PAGE_SIZE + offset_addr;
  if ((stat = (DEV_Status_t)HAL_SRAM_Write_8b(MEM_hSRAM1, &SRAMaddress, data, len)) != DEV_OK)
      return stat;
  
  SRAMaddress = MEM_SRAM1_ADDR_BLOCK_BUF;
    
  //Copy block to nand
  for (int i=0; i<MEM_NAND_BLOCK_SIZE; i++)
  {
    MEM_selectMem(MEM_ID_SRAM1); //Select memory
    if ((stat = (DEV_Status_t)HAL_SRAM_Read_8b(MEM_hSRAM1, &SRAMaddress, MEM_dataBuf, MEM_NAND_PAGE_SIZE)) != DEV_OK)
      return stat;
    
    MEM_selectMem(MEM_ID_NAND); //Select memory
    if ((stat = (DEV_Status_t)HAL_NAND_Write_Page_8b(MEM_hNAND1, &startBlockAddress, MEM_dataBuf, 1)) != DEV_OK)
      return stat;
    SRAMaddress += MEM_NAND_BLOCK_SIZE;
  }
  
  return DEV_OK; 
}
//--------------------------------------------------------------------------------------------------
//Read data NAND
DEV_Status_t MEM_NAND_readData(NAND_AddressTypeDef address, uint16_t offset_addr, uint8_t *data, uint16_t len)
{
  DEV_Status_t stat;
  MEM_selectMem(MEM_ID_NAND); //Select memory
  
  stat = (DEV_Status_t)HAL_NAND_Read_Page_8b(MEM_hNAND1, &address, MEM_dataBuf, 1);
  
  if (stat != DEV_OK)
    return stat;
  
  memcpy((data+offset_addr), MEM_dataBuf, len);
  
  return DEV_OK; 
}
//**************************************************************************************************
//                                      SRAM
//**************************************************************************************************
//Write data SRAM
DEV_Status_t MEM_SRAM_writeData(MEM_ID_t memID, uint32_t address, uint8_t *data, uint32_t len)
{
  DEV_Status_t stat;
  SRAM_HandleTypeDef *sramHandle;
  
  MEM_selectMem(memID); //Select memory
  
  if (memID == MEM_ID_SRAM1)
  {
    sramHandle = MEM_hSRAM1;
  }else if(memID == MEM_ID_SRAM2) {
    sramHandle = MEM_hSRAM2;
  }

  stat = (DEV_Status_t)HAL_SRAM_WriteOperation_Enable(sramHandle);

  if (stat != DEV_OK)
    return stat;
  
  stat = (DEV_Status_t)HAL_SRAM_Write_8b(sramHandle, &address, data, len);
  
  if (stat != DEV_OK)
    return stat;
  
  stat =  (DEV_Status_t)HAL_SRAM_WriteOperation_Disable(sramHandle);
  
  return stat;
}
//--------------------------------------------------------------------------------------------------
//Read data SRAM
DEV_Status_t MEM_SRAM_readData(MEM_ID_t memID, uint32_t address, uint8_t *data, uint32_t len)
{
  DEV_Status_t stat;
  SRAM_HandleTypeDef *sramHandle;
  
  MEM_selectMem(memID); //Select memory
  
  if (memID == MEM_ID_SRAM1)
  {
    sramHandle = MEM_hSRAM1;
  }else if(memID == MEM_ID_SRAM2) {
    sramHandle = MEM_hSRAM1;
  }
  
  stat = (DEV_Status_t)HAL_SRAM_Read_8b(sramHandle, &address, data, len);
  
  return stat;
}
//--------------------------------------------------------------------------------------------------
//Check CRC in SRAM
DEV_Status_t MEM_checkCRC8_SRAM(MEM_ID_t memID, uint8_t crc, uint32_t addr, uint32_t len)
{
  SRAM_HandleTypeDef *sramHandle;
  uint8_t buffers = (len/MEM_NAND_PAGE_SIZE); //Count full pages
  uint32_t tail = len-buffers*MEM_NAND_PAGE_SIZE;  //tail in bytes
  DEV_Status_t stat; //Return Status
  uint8_t crcVal = 0;
  
  MEM_selectMem(memID); //Select memory
  
  if (memID == MEM_ID_SRAM1)
  {
    sramHandle = MEM_hSRAM1;
  }else if(memID == MEM_ID_SRAM2) {
    sramHandle = MEM_hSRAM1;
  }
  
  //Calc for full buffers
  for (int i=0; i<buffers; i++)
  {    
    if ((stat = (DEV_Status_t)HAL_SRAM_Read_8b(sramHandle, &addr, MEM_dataBuf, MEM_NAND_PAGE_SIZE)) != DEV_OK) //Read buffer
        return stat;
         
    crcVal = crc8(MEM_dataBuf, MEM_NAND_PAGE_SIZE, crcVal);
    addr += MEM_NAND_PAGE_SIZE;
  }
  
  //Tail
  if ((stat = (DEV_Status_t)HAL_SRAM_Read_8b(sramHandle, &addr, MEM_dataBuf, tail)) != DEV_OK) //Read buffer
        return stat;
         
  crcVal = crc8(MEM_dataBuf, tail, crcVal); 
  
  if (crcVal != crc)
  {
    return DEV_VAL_ERROR;
  }else{
    return DEV_OK;
  }
  
}
//--------------------------------------------------------------------------------------------------
//Copy from NAND to SRAM
DEV_Status_t MEM_NAND_to_SRAM(MEM_ID_t memoryID, uint32_t addrSRAM, NAND_AddressTypeDef addrNAND, uint32_t offsetNAND, uint32_t len)
{
  uint8_t pages = (len/MEM_NAND_PAGE_SIZE); //Count full pages
  uint32_t tail = len-pages*MEM_NAND_PAGE_SIZE;  //tail in bytes
  NAND_AddressTypeDef relativeAddrNAND = addrNAND; //Relative address in NAND
  DEV_Status_t stat; //Return Status
  
  //Copy full pages
  for (int i=0; i<pages; i++)
  {
    if ((stat = MEM_NAND_readData(relativeAddrNAND, 0, MEM_dataBuf, MEM_NAND_PAGE_SIZE)) != DEV_OK) //Read page
      return stat;
    
    if ((stat = MEM_SRAM_writeData(memoryID, addrSRAM, MEM_dataBuf, MEM_NAND_PAGE_SIZE)) != DEV_OK) //Write data SRAM
      return stat;
    
    relativeAddrNAND.Page = addrNAND.Page + i;
    addrSRAM += MEM_NAND_PAGE_SIZE*i;
  }
  
  //Copy tail
  if ((stat = MEM_NAND_readData(relativeAddrNAND, 0, MEM_dataBuf, tail)) != DEV_OK) //Read page
      return stat;
  
  if ((stat = MEM_SRAM_writeData(memoryID, addrSRAM, MEM_dataBuf, tail)) != DEV_OK) //Write data SRAM
      return stat;  
  
  return DEV_OK; 
}
