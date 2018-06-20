/**
  ******************************************************************************
  * @file    stm32l1xx_flash_ramfunc.c
  * @author  MCD Application Team
  * @version V1.0.0RC1
  * @date    07/02/2010
  * @brief   This file provides all the Flash firmware functions which should be
  *          executed from the internal SRAM. Other FLASH memory functions that
  *          can be used from the FLASH are defined in the "stm32l1xx_flash.c"
  *          file.    
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

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_flash.h"

/** @addtogroup STM32L1xx_StdPeriph_Driver
  * @{
  */

/** @defgroup FLASH 
  * @brief FLASH driver modules
  * @{
  */ 

/** @defgroup FLASH_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */

/** @defgroup FLASH_Private_Defines
  * @{
  */
  
#define HALF_PROGRAM_TIMEOUT             ((uint32_t)0x00002000)

/**
  * @}
  */ 

/** @defgroup FLASH_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup FLASH_Private_Variables
  * @{
  */
   
/**
  * @}
  */ 

/** @defgroup FLASH_Private_FunctionPrototypes
  * @{
  */ 
__RAM_FUNC static FLASH_Status GetStatus(void);
__RAM_FUNC static FLASH_Status WaitForLastOperation(uint32_t Timeout);
/**
  * @}
  */ 
 
/** @defgroup FLASH_Private_Functions
  * @{
  */ 

/**
  * @brief  Programs a half page in program memory.
  * @param  Address: specifies the address to be written.
  * @param  pBuffer: pointer to the buffer  containing the data to be  written to 
  *         the half page.
  * @note   Half page write is possible only from SRAM.
  * @note   If there are more than 32 words to write, after 32 words another 
  *         Half Page programming operation starts and has to be finished.
  * @note   A half page is written to the program memory only if the first 
  *         address to load is the start address of a half page (multiple of 128 
  *         bytes) and the 31 remaining words to load are in the same half page.
  * @note   During the Program memory half page write all read operations are 
  *         forbidden (this includes DMA read operations and debugger read 
  *         operations such as breakpoints, periodic updates, etc.)
  * @note   If a PGAERR is set during a Program memory half page write, the 
  *         complete write operation is aborted. Software should then reset the 
  *         FPRG and PROG/DATA bits and restart the write operation from the 
  *         beginning.                             
  * @retval FLASH Status: The returned value can be:  
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
__RAM_FUNC FLASH_Status FLASH_WriteProgramHalfPage(uint32_t Address, uint32_t* pBuffer)
{
  uint32_t count = 0; 
   
  FLASH_Status status = FLASH_COMPLETE;

  /* Wait for last operation to be completed */
  status = WaitForLastOperation(HALF_PROGRAM_TIMEOUT);
  
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new  
    half page */
    FLASH->PECR |= FLASH_PECR_FPRG;
    FLASH->PECR |= FLASH_PECR_PROG;
    
    /* Write one half page directly with 32 different words */
    while(count < 32)
    {
      *(__IO uint32_t*) (Address + (4 * count)) = *(pBuffer++);
      count ++;  
    }
    /* Wait for last operation to be completed */
    status = WaitForLastOperation(HALF_PROGRAM_TIMEOUT);
 
    if(status != FLASH_TIMEOUT)
    {
      /* if the write operation is completed, disable the PROG and FPRG bits */
      FLASH->PECR &= (uint32_t)(~FLASH_PECR_PROG);
      FLASH->PECR &= (uint32_t)(~FLASH_PECR_FPRG);
    }       
  }
  /* Return the Write Status */
  return status;
}

/**
  * @brief  Erase a double word in data memory.
  * @param  Address: specifies the address to be erased
  * @note   Data memory double word erase is possible only from SRAM.
  * @note   A double word is erased to the data memory only if the first address 
  *         to load is the start address of a double word (multiple of 8 bytes)  
  * @note   During the Data memory double word erase, all read operations are 
  *         forbidden (this includes DMA read operations and debugger read 
  *         operations such as breakpoints, periodic updates, etc.)  
  * @retval FLASH Status: The returned value can be: 
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */

__RAM_FUNC FLASH_Status FLASH_EraseDataDoubleWord(uint32_t Address)
{
  FLASH_Status status = FLASH_COMPLETE;
    
  /* Wait for last operation to be completed */
  status = WaitForLastOperation(HALF_PROGRAM_TIMEOUT);
  
  if(status == FLASH_COMPLETE)
  {
    /* If the previous operation is completed, proceed to erase the next double word */
    /* Set the ERASE bit */
    FLASH->PECR |= FLASH_PECR_ERASE;

    /* Set DATA bit */
    FLASH->PECR |= FLASH_PECR_DATA;
   
    /* Write 00000000h to the 2 words to erase */
    *(__IO uint64_t *)Address = 0x00000000;
 
    /* Wait for last operation to be completed */
    status = WaitForLastOperation(HALF_PROGRAM_TIMEOUT);

    if(status != FLASH_TIMEOUT)
    {
      /* If the erase operation is completed, disable the ERASE and DATA bits */
      FLASH->PECR &= (uint32_t)(~FLASH_PECR_ERASE);
      FLASH->PECR &= (uint32_t)(~FLASH_PECR_DATA); 
    }    
  }  
  /* Return the erase status */
  return status;
}

/**
  * @brief  Write a double word in data memory without erase.
  * @param  Address: specifies the address to be written.
  * @param  Data: specifies the data to be written.
  * @note   Data memory double word write is possible only from SRAM.
  * @note   A data memory double word is written to the data memory only if the 
  *         first address to load is the start address of a double word (multiple 
  *         of double word).  
  * @note   During the Data memory double word write, all read operations are 
  *         forbidden (this includes DMA read operations and debugger read 
  *         operations such as breakpoints, periodic updates, etc.)    
  * @retval FLASH Status: The returned value can be: 
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */ 
__RAM_FUNC FLASH_Status FLASH_WriteDataDoubleWord(uint32_t Address, uint64_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
    
  /* Wait for last operation to be completed */
  status = WaitForLastOperation(HALF_PROGRAM_TIMEOUT);
  
  if(status == FLASH_COMPLETE)
  {
    /* If the previous operation is completed, proceed to program the new data*/
    FLASH->PECR |= FLASH_PECR_FPRG;
    FLASH->PECR |= FLASH_PECR_DATA;
    
    /* Write the 2 words */  
     *(__IO uint32_t *)Address = (uint32_t) Data;
     Address += 4;
     *(__IO uint32_t *)Address = (uint32_t) (Data >> 32);
    
    /* Wait for last operation to be completed */
    status = WaitForLastOperation(HALF_PROGRAM_TIMEOUT);
    
    if(status != FLASH_TIMEOUT)
    {
      /* If the write operation is completed, disable the FPRG and DATA bits */
      FLASH->PECR &= (uint32_t)(~FLASH_PECR_FPRG);
      FLASH->PECR &= (uint32_t)(~FLASH_PECR_DATA);
    }        
  }
  /* Return the Write Status */
  return status;
}

/**
  * @brief  Returns the FLASH Status.
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, 
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP or FLASH_COMPLETE
  */
__RAM_FUNC static FLASH_Status GetStatus(void)
{
  FLASH_Status FLASHstatus = FLASH_COMPLETE;
  
  if((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) 
  {
    FLASHstatus = FLASH_BUSY;
  }
  else 
  {  
    if((FLASH->SR & (uint32_t)FLASH_FLAG_WRPERR)!= (uint32_t)0x00)
    { 
      FLASHstatus = FLASH_ERROR_WRP;
    }
    else 
    {
      if((FLASH->SR & (uint32_t)0xFEF0) != (uint32_t)0x00)
      {
        FLASHstatus = FLASH_ERROR_PROGRAM; 
      }
      else
      {
        FLASHstatus = FLASH_COMPLETE;
      }
    }
  }
  /* Return the FLASH Status */
  return FLASHstatus;
}

/**
  * @brief  Waits for a FLASH operation to complete or a TIMEOUT to occur.
  * @param  Timeout: FLASH progamming Timeout
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, 
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or 
  *   FLASH_TIMEOUT.
  */
__RAM_FUNC static FLASH_Status WaitForLastOperation(uint32_t Timeout)
{ 
  FLASH_Status status = FLASH_COMPLETE;
   
  /* Check for the FLASH Status */
  status = GetStatus();
  
  /* Wait for a FLASH operation to complete or a TIMEOUT to occur */
  while((status == FLASH_BUSY) && (Timeout != 0x00))
  {
    status = GetStatus();
    Timeout--;
  }
  
  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }
  /* Return the operation status */
  return status;
}


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
