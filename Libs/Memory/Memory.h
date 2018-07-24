#ifndef MEMORY_H
#define MEMORY_H

#include "stm32f4xx_hal.h"
#include "deviceDefs.h"

//Gpio macros
#define MEM_EN_SRAM1_ON    HAL_GPIO_WritePin(SRAM_CE2_1_GPIO_Port, SRAM_CE2_1_Pin, GPIO_PIN_SET)
#define MEM_EN_SRAM1_OFF   HAL_GPIO_WritePin(SRAM_CE2_1_GPIO_Port, SRAM_CE2_1_Pin, GPIO_PIN_RESET)
#define MEM_EN_SRAM2_ON    HAL_GPIO_WritePin(SRAM_CE2_2_GPIO_Port, SRAM_CE2_2_Pin, GPIO_PIN_SET)
#define MEM_EN_SRAM2_OFF   HAL_GPIO_WritePin(SRAM_CE2_2_GPIO_Port, SRAM_CE2_2_Pin, GPIO_PIN_RESET)

//***************************************Memory work************************************************

//Id memoryes
typedef enum{
  MEM_ID_NAND = 0,
  MEM_ID_SRAM1,
  MEM_ID_SRAM2
}MEM_ID_t;

//Init memory
void MEM_init(SRAM_HandleTypeDef *sram1, SRAM_HandleTypeDef *sram2, NAND_HandleTypeDef *nand);

//***************************************NAND mem map***********************************************

#define MEM_NAND_PAGE_SIZE      0x800UL                 //2048 bytes
#define MEM_NAND_BLOCK_SIZE     0x1000UL                // in pages
#define MEM_NAND_PLANE_SIZE     0x400000UL              // in blocks
#define MEM_NAND_MEMORY_SIZE    0x400000UL              // in blocks
#define MEM_NAND_DEV_ID         0xDA                    // default id

                                                      //plane, block, page
#define MEM_NAND_ADDR_INFO      (NAND_AddressTypeDef) {0, 0, 0}         //Info page
#define MEM_NAND_ADDR_SETTINGS  (NAND_AddressTypeDef) {0, 0, 1}         //Main settings
#define MEM_NAND_ADDR_LOG       (NAND_AddressTypeDef) {0, 0, 2}         //Start log data
#define MEM_NAND_ADDR_FW_META   (NAND_AddressTypeDef) {0, 0, 10}        //Nand FW metadata
#define MEM_NAND_ADDR_FW        (NAND_AddressTypeDef) {0, 0, 11}        //FW space 8 blocks by 128kB on 1024kB
#define MEM_NAND_ADDR_WM_DATA   (NAND_AddressTypeDef) {0, 1, 0}         //Wirtual mashine data

//***************************************NAND functions*********************************************

//Check nand ID
DEV_Status_t MEM_NAND_checkID();
//Write data NAND
DEV_Status_t MEM_NAND_writeData(NAND_AddressTypeDef address, uint32_t offset_addr, uint8_t *data, uint16_t len);
//Read data NAND
DEV_Status_t MEM_NAND_readData(NAND_AddressTypeDef address, uint32_t offset_addr, uint8_t *data, uint16_t len);

//***************************************SRAM1 mem map**********************************************

#define MEM_SRAM1_MEMORY_SIZE    0xFA000
#define MEM_SRAM1_ADDR_SCRIPT1   0x0000

//***************************************SRAM2 mem map**********************************************

#define MEM_SRAM2_MEMORY_SIZE    0xFA000
#define MEM_SRAM1_ADDR_SCRIPT2   0x0000

//***************************************SRAM functions*********************************************

//Write data SRAM
DEV_Status_t MEM_SRAM_writeData(MEM_ID_t memID, uint32_t address, uint8_t *data, uint32_t len);
//Read data SRAM
DEV_Status_t MEM_SRAM_readData(MEM_ID_t memID, uint32_t address, uint8_t *data, uint32_t len);
//Copy from NAND to SRAM
DEV_Status_t MEM_NAND_to_SRAM(MEM_ID_t memoryID, uint32_t addrSRAM, NAND_AddressTypeDef addrNAND, uint32_t offsetNAND, uint32_t len);
//Check CRC in SRAM
DEV_Status_t MEM_checkCRC8_SRAM(MEM_ID_t memID, uint8_t crc, uint32_t addr, uint32_t len);

#endif