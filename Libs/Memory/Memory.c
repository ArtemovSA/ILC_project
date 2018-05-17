#include "Memory.h"

#include "stm32f4xx_hal_nand.h"
#include "stm32f4xx_hal_sram.h"
#include "math.h"

//Handles
SRAM_HandleTypeDef *MEM_hSRAM1;
SRAM_HandleTypeDef *MEM_hSRAM2;
NAND_HandleTypeDef *MEM_hNAND1;

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
//***************************************NAND*******************************************************
//Check nand ID
HAL_StatusTypeDef MEM_NAND_checkID()
{
  HAL_StatusTypeDef stat;
  NAND_IDTypeDef NAND_id;
  
  MEM_selectMem(MEM_ID_NAND); //Select memory
  
  stat = HAL_NAND_Read_ID(MEM_hNAND1, &NAND_id);
  
  if (stat != HAL_OK)
    return stat;
  
  if (NAND_id.Device_Id == MEM_NAND_DEV_ID)
    return HAL_OK;
  
  return HAL_ERROR;
}
//--------------------------------------------------------------------------------------------------
//Write data nand
HAL_StatusTypeDef MEM_NAND_writeData(NAND_AddressTypeDef address, uint8_t *data, uint16_t len)
{
  HAL_StatusTypeDef stat;
  MEM_selectMem(MEM_ID_NAND); //Select memory
  
  stat = HAL_NAND_Erase_Block(MEM_hNAND1, &address);
  
  stat = HAL_NAND_Write_Page_8b(MEM_hNAND1, &address, data, len);
  
  if (stat != HAL_OK)
    return stat;
  
  return stat; 
}
//--------------------------------------------------------------------------------------------------
//Read data nand
HAL_StatusTypeDef MEM_NAND_readData(NAND_AddressTypeDef address, uint8_t *data, uint16_t len)
{
  HAL_StatusTypeDef stat;
  MEM_selectMem(MEM_ID_NAND); //Select memory
  
  stat = HAL_NAND_Read_Page_8b(MEM_hNAND1, &address, data, len);
  
  if (stat != HAL_OK)
    return stat;
  
  return stat; 
}
//***************************************SRAM******************************************************
//Write data SRAM
HAL_StatusTypeDef MEM_SRAM_writeData(MEM_ID_t memID, uint32_t address, uint8_t *data, uint32_t len)
{
  HAL_StatusTypeDef stat;
  SRAM_HandleTypeDef *sramHandle;
  
  MEM_selectMem(memID); //Select memory
  
  if (memID == MEM_ID_SRAM1)
  {
    sramHandle = MEM_hSRAM1;
  }else if(memID == MEM_ID_SRAM2) {
    sramHandle = MEM_hSRAM1;
  }

  stat = HAL_SRAM_WriteOperation_Enable(sramHandle);

  if (stat != HAL_OK)
    return stat;
  
  stat = HAL_SRAM_Write_8b(sramHandle, &address, data, len);
  
  if (stat != HAL_OK)
    return stat;
  
  stat =  HAL_SRAM_WriteOperation_Disable(sramHandle);
  
  return stat;
}
//--------------------------------------------------------------------------------------------------
//Read data SRAM
HAL_StatusTypeDef MEM_SRAM_readData(MEM_ID_t memID, uint32_t address, uint8_t *data, uint32_t len)
{
  HAL_StatusTypeDef stat;
  SRAM_HandleTypeDef *sramHandle;
  
  MEM_selectMem(memID); //Select memory
  
  if (memID == MEM_ID_SRAM1)
  {
    sramHandle = MEM_hSRAM1;
  }else if(memID == MEM_ID_SRAM2) {
    sramHandle = MEM_hSRAM1;
  }
  
  stat =  HAL_SRAM_Read_8b(sramHandle, &address, data, len);
  
  return stat;
}
