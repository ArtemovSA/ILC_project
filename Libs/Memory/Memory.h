#ifndef MEMORY_H
#define MEMORY_H

#include "stm32f4xx_hal.h"
#include "deviceDefs.h"

//Gpio macros
#define MEM_EN_SRAM1_ON    HAL_GPIO_WritePin(SRAM_CE2_2_GPIO_Port, SRAM_CE2_2_Pin, GPIO_PIN_SET)
#define MEM_EN_SRAM1_OFF   HAL_GPIO_WritePin(SRAM_CE2_2_GPIO_Port, SRAM_CE2_2_Pin, GPIO_PIN_RESET)
#define MEM_EN_SRAM2_ON    HAL_GPIO_WritePin(SRAM_CE2_1_GPIO_Port, SRAM_CE2_1_Pin, GPIO_PIN_SET)
#define MEM_EN_SRAM2_OFF   HAL_GPIO_WritePin(SRAM_CE2_1_GPIO_Port, SRAM_CE2_1_Pin, GPIO_PIN_RESET)

//***************************************Memory work************************************************

//Id memoryes
typedef enum{
  MEM_ID_RAM = 0,
  MEM_ID_FLASH,
  MEM_ID_SRAM1,
  MEM_ID_SRAM2,
  MEM_ID_NAND
}MEM_ID_t;

//Init memory
void MEM_init(SRAM_HandleTypeDef *sram1, SRAM_HandleTypeDef *sram2, NAND_HandleTypeDef *nand);

//***************************************Mem map***********************************************

//NAND
#define MEM_NAND_PAGE_SIZE      2048                    //2048 bytes
#define MEM_NAND_BLOCK_SIZE     64                      // in pages
#define MEM_NAND_PLANE_SIZE     1024                    // in blocks
#define MEM_NAND_MEMORY_SIZE    0x8000000UL              
#define MEM_NAND_DEV_ID         0xDA                    // default id

                                                      //page, plane, block
#define MEM_NAND_ADDR_INFO      (NAND_AddressTypeDef) {0, 0, 0}         //Info page
#define MEM_NAND_ADDR_SETTINGS  (NAND_AddressTypeDef) {0, 0, 1}         //Main settings
#define MEM_NAND_ADDR_CONDITION (NAND_AddressTypeDef) {0, 0, 2}         //Condition
#define MEM_NAND_ADDR_LOG       (NAND_AddressTypeDef) {0, 0, 3}         //Start log data
#define MEM_NAND_ADDR_FW_META   (NAND_AddressTypeDef) {0, 0, 10}        //Nand FW metadata
#define MEM_NAND_ADDR_FW        (NAND_AddressTypeDef) {0, 0, 11}        //FW space 8 blocks by 128kB on 1024kB
#define MEM_NAND_ADDR_VM_DATA   (NAND_AddressTypeDef) {0, 0, 3}         //Virtual mashine data


//***************************************NAND functions*********************************************

//Check nand ID
DEV_Status_t MEM_NAND_checkID();
//Write data NAND
DEV_Status_t MEM_NAND_writeData(NAND_AddressTypeDef address, uint16_t offset_addr, uint8_t *data, uint32_t len);
//Write data NAND
DEV_Status_t MEM_NAND_writePage(NAND_AddressTypeDef address, uint8_t *data, uint16_t len);
//Read data NAND
DEV_Status_t MEM_NAND_readData(NAND_AddressTypeDef address, uint16_t offset_addr, uint8_t *data, uint32_t len);
//Erace Block NAND
DEV_Status_t MEM_NAND_eraceBlock(NAND_AddressTypeDef address);
//Check CRC16 in NAND
DEV_Status_t MEM_NAND_checkCRC16(NAND_AddressTypeDef address, uint16_t crc16, uint32_t len);

//***************************************SRAM1 mem map**********************************************

//Bank 1
#define MEM_SRAM1_ADDR                  0x60000000ULL
#define MEM_SRAM1_MEMORY_SIZE           0xFA000UL

#define MEM_SRAM1_ADDR_BLOCK_BUF        0x600DA000ULL
#define MEM_SRAM1_ADDR_BLOCK_BUF        0x600DA000ULL
#define MEM_SRAM1_SCRYPT_ADDR           0x6009A000ULL
#define MEM_SRAM1_SCRYPT_LOAD_ADDR      0x600BA000ULL

//***************************************SRAM2 mem map**********************************************

//Bank 3
#define MEM_SRAM2_ADDR                  0x80000000ULL
#define MEM_SRAM2_MEMORY_SIZE           0xFA000

//***************************************SRAM functions*********************************************

//Write data SRAM
DEV_Status_t MEM_SRAM_writeData(MEM_ID_t memID, uint32_t address, uint8_t *data, uint32_t len);
//Read data SRAM
DEV_Status_t MEM_SRAM_readData(MEM_ID_t memID, uint32_t address, uint8_t *data, uint32_t len);
//Copy from NAND to SRAM
DEV_Status_t MEM_NAND_to_SRAM(MEM_ID_t memoryID, uint32_t addrSRAM, NAND_AddressTypeDef addrNAND, uint32_t offsetNAND, uint32_t len);
//Copy from SRAM to NAND
DEV_Status_t MEM_SRAM_to_NAND(MEM_ID_t memoryID, uint32_t addrSRAM, NAND_AddressTypeDef addrNAND, uint32_t len);
//Check CRC in SRAM
DEV_Status_t MEM_checkCRC16_SRAM(MEM_ID_t memID, uint16_t crc, uint32_t addr, uint32_t len);

#endif