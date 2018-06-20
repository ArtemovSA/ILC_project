/**
  ******************************************************************************
  * @file    stm32l1xx_flash.h
  * @author  MCD Application Team
  * @version V1.0.0RC1
  * @date    07/02/2010
  * @brief   This file contains all the functions prototypes for the FLASH 
  *          firmware library.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L1xx_FLASH_H
#define __STM32L1xx_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

/** @addtogroup STM32L1xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup FLASH
  * @{
  */

/** @defgroup FLASH_Exported_Types
  * @{
  */

/** 
  * @brief  FLASH __RAM_FUNC definition  
  */ 
#if defined ( __CC_ARM   )
/* With ARM Compiler, RAM functions are defined using the toolchain options. For
more details please refer to your toolchain documentation. */
#define __RAM_FUNC
#elif defined ( __ICCARM__ )
#define __RAM_FUNC __ramfunc
#elif defined   (  __GNUC__  )
#define __RAM_FUNC
#endif 

/** 
  * @brief  FLASH Status  
  */ 
typedef enum
{ 
  FLASH_BUSY = 1,
  FLASH_ERROR_WRP,
  FLASH_ERROR_PROGRAM,
  FLASH_COMPLETE,
  FLASH_TIMEOUT
}FLASH_Status;

/**
  * @}
  */ 
  
/** @defgroup FLASH_Exported_Constants
  * @{
  */ 
  
/** @defgroup FLASH_Latency 
  * @{
  */ 
#define FLASH_Latency_0                ((uint8_t)0x0000)  /*!< FLASH Zero Latency cycle */
#define FLASH_Latency_1                ((uint8_t)0x0001)  /*!< FLASH One Latency cycle */

#define IS_FLASH_LATENCY(LATENCY) (((LATENCY) == FLASH_Latency_0) || \
                                   ((LATENCY) == FLASH_Latency_1))
/**
  * @}
  */ 


/** @defgroup Prefetch_Buffer_Enable_Disable 
  * @{
  */ 
#define FLASH_PrefetchBuffer_Enable    ((uint32_t)0x00000002)  /*!< FLASH Prefetch Buffer Enable */
#define FLASH_PrefetchBuffer_Disable   ((uint32_t)0x00000000)  /*!< FLASH Prefetch Buffer Disable */

#define IS_FLASH_PREFETCHBUFFER_STATE(STATE) (((STATE) == FLASH_PrefetchBuffer_Enable) || \
                                              ((STATE) == FLASH_PrefetchBuffer_Disable)) 
/**
  * @}
  */ 


/** @defgroup Option_Bytes_Read_Protection 
  * @{
  */ 

/** 
  * @brief  Read Protection Level  
  */ 
#define FLASH_ReadProtection_Level_0   ((uint8_t)0xAA)
#define FLASH_ReadProtection_Level_1   ((uint8_t)0xBB)
#define FLASH_ReadProtection_Level_2   ((uint8_t)0xCC)

#define IS_FLASH_READPROTECTION_LEVEL(LEVEL) (((LEVEL) == FLASH_ReadProtection_Level_0)||\
                                              ((LEVEL) == FLASH_ReadProtection_Level_1)||\
                                              ((LEVEL) == FLASH_ReadProtection_Level_2))

/**
  * @}
  */ 

/** @defgroup FLASH_Interrupts 
  * @{
  */
   
#define FLASH_IT_EOP               FLASH_PECR_EOPIE  /*!< End of programming interrupt source */
#define FLASH_IT_ERR               FLASH_PECR_ERRIE  /*!< Error interrupt source */
#define IS_FLASH_IT(IT) ((((IT) & (uint32_t)0xFFFCFFFF) == 0x00000000) && (((IT) != 0x00000000)))
/**
  * @}
  */ 

/** @defgroup FLASH_Flags 
  * @{
  */ 

#define FLASH_FLAG_BSY                 ((uint32_t)0x00000001)  /*!< FLASH Busy flag */
#define FLASH_FLAG_EOP                 ((uint32_t)0x00000002)  /*!< FLASH End of Programming flag */
#define FLASH_FLAG_ENDHV               ((uint32_t)0x00000004)  /*!< FLASH End of High Voltage flag */
#define FLASH_FLAG_READY               ((uint32_t)0x00000008)  /*!< FLASH Ready flag after low power mode */
#define FLASH_FLAG_WRPERR              ((uint32_t)0x00000100)  /*!< FLASH Write protected error flag */
#define FLASH_FLAG_PGAERR              ((uint32_t)0x00000200)  /*!< FLASH Programming Alignment error flag */
#define FLASH_FLAG_SIZERR              ((uint32_t)0x00000400)  /*!< FLASH Size error flag  */
#define FLASH_FLAG_OPTVERR             ((uint32_t)0x00000800)  /*!< FLASH Option Validity error flag  */

 
#define IS_FLASH_CLEAR_FLAG(FLAG) ((((FLAG) & (uint32_t)0xFFFFF0FD) == 0x00000000) && ((FLAG) != 0x00000000))

#define IS_FLASH_GET_FLAG(FLAG)  (((FLAG) == FLASH_FLAG_BSY) || ((FLAG) == FLASH_FLAG_EOP) || \
                                  ((FLAG) == FLASH_FLAG_ENDHV) || ((FLAG) == FLASH_FLAG_READY ) || \
                                  ((FLAG) ==  FLASH_FLAG_WRPERR) || ((FLAG) == FLASH_FLAG_PGAERR ) || \
                                  ((FLAG) ==  FLASH_FLAG_SIZERR) || ((FLAG) == FLASH_FLAG_OPTVERR))
/**
  * @}
  */ 

/** @defgroup FLASH_Keys 
  * @{
  */ 

#define FLASH_PDKEY1               ((uint32_t)0x04152637)
#define FLASH_PDKEY2               ((uint32_t)0xFAFBFCFD)
#define FLASH_PEKEY1               ((uint32_t)0x89ABCDEF)
#define FLASH_PEKEY2               ((uint32_t)0x02030405)
#define FLASH_PRGKEY1              ((uint32_t)0x8C9DAEBF)
#define FLASH_PRGKEY2              ((uint32_t)0x13141516)
#define FLASH_OPTKEY1              ((uint32_t)0xFBEAD9C8)
#define FLASH_OPTKEY2              ((uint32_t)0x24252627)
								 
/**
  * @}
  */ 

/** @defgroup FLASH_Address 
  * @{
  */
  
#define IS_FLASH_DATA_ADDRESS(ADDRESS) (((ADDRESS) >= 0x08080000) && ((ADDRESS) <= 0x08080FFF))
#define IS_FLASH_PROGRAM_ADDRESS(ADDRESS) (((ADDRESS) >= 0x08000000) && ((ADDRESS) <= 0x0801FFFF))  

/**
  * @}
  */ 

/** @defgroup Option_Bytes_Write_Protection 
  * @{
  */
  

#define FLASH_WRProt_Pages0to15           ((uint32_t)0x00000001) /* Write protection of Sector0 */
#define FLASH_WRProt_Pages16to31          ((uint32_t)0x00000002) /* Write protection of Sector1 */
#define FLASH_WRProt_Pages32to47          ((uint32_t)0x00000004) /* Write protection of Sector2 */
#define FLASH_WRProt_Pages48to63          ((uint32_t)0x00000008) /* Write protection of Sector3 */
#define FLASH_WRProt_Pages64to79          ((uint32_t)0x00000010) /* Write protection of Sector4 */
#define FLASH_WRProt_Pages80to95          ((uint32_t)0x00000020) /* Write protection of Sector5 */
#define FLASH_WRProt_Pages96to111         ((uint32_t)0x00000040) /* Write protection of Sector6 */
#define FLASH_WRProt_Pages112to127        ((uint32_t)0x00000080) /* Write protection of Sector7 */
#define FLASH_WRProt_Pages128to143        ((uint32_t)0x00000100) /* Write protection of Sector8 */
#define FLASH_WRProt_Pages144to159        ((uint32_t)0x00000200) /* Write protection of Sector9 */
#define FLASH_WRProt_Pages160to175        ((uint32_t)0x00000400) /* Write protection of Sector10 */
#define FLASH_WRProt_Pages176to191        ((uint32_t)0x00000800) /* Write protection of Sector11 */
#define FLASH_WRProt_Pages192to207        ((uint32_t)0x00001000) /* Write protection of Sector12 */
#define FLASH_WRProt_Pages208to223        ((uint32_t)0x00002000) /* Write protection of Sector13 */
#define FLASH_WRProt_Pages224to239        ((uint32_t)0x00004000) /* Write protection of Sector14 */
#define FLASH_WRProt_Pages240to255        ((uint32_t)0x00008000) /* Write protection of Sector15 */
#define FLASH_WRProt_Pages256to271        ((uint32_t)0x00010000) /* Write protection of Sector16 */
#define FLASH_WRProt_Pages272to287        ((uint32_t)0x00020000) /* Write protection of Sector17 */
#define FLASH_WRProt_Pages288to303        ((uint32_t)0x00040000) /* Write protection of Sector18 */
#define FLASH_WRProt_Pages304to319        ((uint32_t)0x00080000) /* Write protection of Sector19 */
#define FLASH_WRProt_Pages320to335        ((uint32_t)0x00100000) /* Write protection of Sector20 */
#define FLASH_WRProt_Pages336to351        ((uint32_t)0x00200000) /* Write protection of Sector21 */
#define FLASH_WRProt_Pages352to367        ((uint32_t)0x00400000) /* Write protection of Sector22 */
#define FLASH_WRProt_Pages368to383        ((uint32_t)0x00800000) /* Write protection of Sector23 */
#define FLASH_WRProt_Pages384to399        ((uint32_t)0x01000000) /* Write protection of Sector24 */
#define FLASH_WRProt_Pages400to415        ((uint32_t)0x02000000) /* Write protection of Sector25 */
#define FLASH_WRProt_Pages416to431        ((uint32_t)0x04000000) /* Write protection of Sector26 */
#define FLASH_WRProt_Pages432to447        ((uint32_t)0x08000000) /* Write protection of Sector27 */
#define FLASH_WRProt_Pages448to463        ((uint32_t)0x10000000) /* Write protection of Sector28 */
#define FLASH_WRProt_Pages464to479        ((uint32_t)0x20000000) /* Write protection of Sector29 */
#define FLASH_WRProt_Pages480to495        ((uint32_t)0x40000000) /* Write protection of Sector30 */
#define FLASH_WRProt_Pages496to511        ((uint32_t)0x80000000) /* Write protection of Sector31 */

#define FLASH_WRProt_AllPages       ((uint32_t)0xFFFFFFFF) /*!< Write protection of all Sectors */

#define IS_FLASH_WRPROT_PAGE(PAGE) (((PAGE) != 0x0000000))

/**
  * @}
  */ 

/** @defgroup Option_Bytes_IWatchdog 
  * @{
  */

#define OB_IWDG_SW                     ((uint8_t)0x10)  /*!< Software WDG selected */
#define OB_IWDG_HW                     ((uint8_t)0x00)  /*!< Hardware WDG selected */
#define IS_OB_IWDG_SOURCE(SOURCE) (((SOURCE) == OB_IWDG_SW) || ((SOURCE) == OB_IWDG_HW))

/**
  * @}
  */

/** @defgroup Option_Bytes_nRST_STOP 
  * @{
  */

#define OB_STOP_NoRST                  ((uint8_t)0x20) /*!< No reset generated when entering in STOP */
#define OB_STOP_RST                    ((uint8_t)0x00) /*!< Reset generated when entering in STOP */
#define IS_OB_STOP_SOURCE(SOURCE) (((SOURCE) == OB_STOP_NoRST) || ((SOURCE) == OB_STOP_RST))

/**
  * @}
  */

/** @defgroup Option_Bytes_nRST_STDBY 
  * @{
  */

#define OB_STDBY_NoRST                 ((uint8_t)0x40) /*!< No reset generated when entering in STANDBY */
#define OB_STDBY_RST                   ((uint8_t)0x00) /*!< Reset generated when entering in STANDBY */
#define IS_OB_STDBY_SOURCE(SOURCE) (((SOURCE) == OB_STDBY_NoRST) || ((SOURCE) == OB_STDBY_RST))

/**
  * @}
  */

/** @defgroup Option_Bytes_BOR_Level 
  * @{
  */

#define OB_BOR_OFF       ((uint8_t)0x00) /*!< Reset threshold level for the 1.50 V 
                                              voltage range (power down only) */
#define OB_BOR_LEVEL1    ((uint8_t)0x08) /*!< Reset threshold level for 1.69 V-1.8 V voltage 
                                             range */
#define OB_BOR_LEVEL2    ((uint8_t)0x09) /*!< Reset threshold level for 1.94 V-2.1 V voltage 
                                             range */
#define OB_BOR_LEVEL3    ((uint8_t)0x0A) /*!< Reset threshold level for 2.3 V-2.49 V voltage 
                                             range */
#define OB_BOR_LEVEL4    ((uint8_t)0x0B) /*!< Reset threshold level for 2.54 V-2.74 voltage 
                                             range */
#define OB_BOR_LEVEL5    ((uint8_t)0x0C) /*!< Reset threshold level for 2.77 V-3.0 V voltage 
                                             range */

#define IS_OB_BOR_LEVEL(LEVEL)  (((LEVEL) == OB_BOR_OFF) || \
                                 ((LEVEL) == OB_BOR_LEVEL1) || \
                                 ((LEVEL) == OB_BOR_LEVEL2) || \
                                 ((LEVEL) == OB_BOR_LEVEL3) || \
                                 ((LEVEL) == OB_BOR_LEVEL4) || \
                                 ((LEVEL) == OB_BOR_LEVEL5))

/**
  * @}
  */

/**
  * @}
  */ 

/** @defgroup FLASH_Exported_Macros
  * @{
  */ 
  
/**
  * @}
  */ 

/** @defgroup FLASH_Exported_Functions
  * @{
  */ 
  
/** 
  * @brief  FLASH memory functions that can be executed from FLASH.  
  */   
void FLASH_SetLatency(uint32_t FLASH_Latency);
void FLASH_PrefetchBufferCmd(uint32_t FLASH_PrefetchBuffer);
void FLASH_ReadAccess64Cmd(FunctionalState NewState);
void FLASH_RUNPowerDownCmd(FunctionalState NewState);
void FLASH_SLEEPPowerDownCmd(FunctionalState NewState);
void FLASH_UnlockData(void);
void FLASH_UnlockProgram(void);
void FLASH_UnlockOptionByte(void);
void FLASH_LockData(void);
void FLASH_LockProgram(void);
void FLASH_LockOptionByte(void);
void FLASH_OBLLaunch(void);
FLASH_Status FLASH_EraseDataWord(uint32_t Address);
FLASH_Status FLASH_EraseProgramPage(uint32_t Page_Address);
FLASH_Status FLASH_FastWriteProgramWord(uint32_t Address, uint32_t Data);
FLASH_Status FLASH_FastWriteDataByte(uint32_t Address, uint8_t Data);
FLASH_Status FLASH_FastWriteDataHalfWord(uint32_t Address, uint16_t Data);
FLASH_Status FLASH_FastWriteDataWord(uint32_t Address, uint32_t Data);
FLASH_Status FLASH_WriteDataByte(uint32_t Address, uint8_t Data);
FLASH_Status FLASH_WriteDataHalfWord(uint32_t Address, uint16_t Data);
FLASH_Status FLASH_WriteDataWord(uint32_t Address, uint32_t Data);
FLASH_Status FLASH_WriteProtection(uint32_t FLASH_Pages, FunctionalState NewState);
FLASH_Status FLASH_ReadProtection(uint8_t FLASH_ReadProtection_Level);
FLASH_Status FLASH_UserOptionByteConfig(uint8_t OB_IWDG, uint8_t OB_STOP, uint8_t OB_STDBY);
FLASH_Status FLASH_SetBORResetLevel(uint8_t OB_BOR);
uint32_t FLASH_GetUserOptionByte(void);
uint32_t FLASH_GetWriteProtectionOptionByte(void);
FlagStatus FLASH_GetReadProtectionStatus(void);
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState);
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG);
void FLASH_ClearFlag(uint32_t FLASH_FLAG);
FLASH_Status FLASH_GetStatus(void);
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout);

/** 
  * @brief  FLASH memory functions that should be executed from internal SRAM.
  *         These functions are defined inside the "stm32l1xx_flash_ramfunc.c"
  *         file.      
  */ 
FLASH_Status FLASH_WriteProgramHalfPage(uint32_t Address, uint32_t* pBuffer);
FLASH_Status FLASH_EraseDataDoubleWord(uint32_t Address);
FLASH_Status FLASH_WriteDataDoubleWord(uint32_t Address, uint64_t Data);
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32L1xx_FLASH_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
