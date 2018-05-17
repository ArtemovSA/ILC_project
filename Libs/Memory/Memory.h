#ifndef MEMORY_H
#define MEMORY_H

#include "stm32f4xx_hal.h"

#define MEM_EN_SRAM1_ON    HAL_GPIO_WritePin(SRAM_CE2_1_GPIO_Port, SRAM_CE2_1_Pin, GPIO_PIN_SET)
#define MEM_EN_SRAM1_OFF   HAL_GPIO_WritePin(SRAM_CE2_1_GPIO_Port, SRAM_CE2_1_Pin, GPIO_PIN_SET)
#define MEM_EN_SRAM2_ON    HAL_GPIO_WritePin(SRAM_CE2_2_GPIO_Port, SRAM_CE2_2_Pin, GPIO_PIN_SET)
#define MEM_EN_SRAM2_OFF   HAL_GPIO_WritePin(SRAM_CE2_2_GPIO_Port, SRAM_CE2_2_Pin, GPIO_PIN_SET)

//***************************************Memory work************************************************
typedef enum{
  MEM_ID_NAND = 0,
  MEM_ID_SRAM1,
  MEM_ID_SRAM2
}MEM_ID_t;



//***************************************NAND mem map***********************************************
#define MEM_NAND_PAGE_SIZE      0x800
#define MEM_NAND_BLOCK_SIZE     0x1000
#define MEM_NAND_PLANE_SIZE     0x400000
#define MEM_NAND_MEMORY_SIZE    0x400000
#define MEM_NAND_DEV_ID         0xF1

                                 //plane, block, page
#define MEM_NAND_ADDR_SETTINGS  (NAND_AddressTypeDef) {0, 0, 0}
#define MEM_NAND_ADDR_LOG       (NAND_AddressTypeDef) {0, 1, 0}

//***************************************NAND functions*********************************************

//Check nand ID
HAL_StatusTypeDef MEM_NAND_checkID();
//Write data nand
HAL_StatusTypeDef MEM_NAND_writeData(NAND_AddressTypeDef address, uint8_t *data, uint16_t len);
//Read data nand
HAL_StatusTypeDef MEM_NAND_readData(NAND_AddressTypeDef address, uint8_t *data, uint16_t len);

//***************************************SRAM1 mem map**********************************************
#define MEM_SRAM1_MEMORY_SIZE    0xFA000

#define MEM_SRAM1_ADDR_SCRIPT1   0x0000

//***************************************SRAM2 mem map**********************************************
#define MEM_SRAM2_MEMORY_SIZE    0xFA000

//***************************************SRAM functions*********************************************

//Write data SRAM
HAL_StatusTypeDef MEM_SRAM_writeData(MEM_ID_t memID, uint32_t address, uint8_t *data, uint32_t len);
//Read data SRAM
HAL_StatusTypeDef MEM_SRAM_readData(MEM_ID_t memID, uint32_t address, uint8_t *data, uint32_t len);


#endif