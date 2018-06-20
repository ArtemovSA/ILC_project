/**
  ******************************************************************************
  * @file    stm32l1xx_flash.c
  * @author  MCD Application Team
  * @version V1.0.0RC1
  * @date    07/02/2010
  * @brief   This file provides all the Flash firmware functions. These functions
  *          can be executed from the FLASH memory. The functions that should be
  *          called from SRAM are defined inside the "stm32l1xx_flash_ramfunc.c"
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
  
/* FLASH Mask */
#define RDPRT_MASK                 ((uint32_t)0x00000002)
#define WRP01_MASK                 ((uint32_t)0x0000FFFF)
#define WRP23_MASK                 ((uint32_t)0xFFFF0000)

/* Delay definition */
#define ERASE_TIMEOUT               ((uint32_t)0x000B0000)
#define PROGRAM_TIMEOUT             ((uint32_t)0x00002000)

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

/**
  * @}
  */ 
 
/** @defgroup FLASH_Private_Functions
  * @{
  */ 

/**
  * @brief  Sets the code latency value.
  * @param  FLASH_Latency: specifies the FLASH Latency value.
  *   This parameter can be one of the following values:
  *     @arg FLASH_Latency_0: FLASH Zero Latency cycle
  *     @arg FLASH_Latency_1: FLASH One Latency cycle
  * @retval None
  */
void FLASH_SetLatency(uint32_t FLASH_Latency)
{
   uint32_t tmpreg = 0;
  
  /* Check the parameters */
  assert_param(IS_FLASH_LATENCY(FLASH_Latency));
  
  /* Read the ACR register */
  tmpreg = FLASH->ACR;  
  
  /* Sets the Latency value */
  tmpreg &= (uint32_t) (~((uint32_t)FLASH_ACR_LATENCY));
  tmpreg |= FLASH_Latency;
  
  /* Write the ACR register */
  FLASH->ACR = tmpreg;
}

/**
  * @brief  Enables or disables the Prefetch Buffer.
  * @param  FLASH_PrefetchBuffer: specifies the prefetch buffer status.
  *   This parameter can be one of the following values:
  *     @arg FLASH_PrefetchBuffer_Enable: FLASH Prefetch Buffer Enable
  *     @arg FLASH_PrefetchBuffer_Disable: FLASH Prefetch Buffer Disable
  * @retval None
  */
void FLASH_PrefetchBufferCmd(uint32_t FLASH_PrefetchBuffer)
{
  /* Check the parameters */
  assert_param(IS_FLASH_PREFETCHBUFFER_STATE(FLASH_PrefetchBuffer));
  
  /* Enable or disable the Prefetch Buffer */
  FLASH->ACR &= (uint32_t)(~((uint32_t)FLASH_ACR_PRFTEN));
  FLASH->ACR |= FLASH_PrefetchBuffer;
}

/**
  * @brief  Enables or disables read access to flash by 64 bits.
  * @param  NewState: new state of the FLASH read access mode.
  *              This parameter can be: ENABLE or DISABLE.  
  * @note1  - If this bit is set, the Read access 64 bit is used.
  *         - If this bit is reset, the Read access 32 bit is used.
  * @note2  - This bit cannot be written at the same time as the LATENCY and 
  *           PRFTEN bits. 
  *         - To reset this bit, the LATENCY should be zero wait state and the 
  *           prefetch off.
  * @retval None
  */
void FLASH_ReadAccess64Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if(NewState != DISABLE)
  {
    FLASH->ACR |= FLASH_ACR_ACC64;
  }
  else
  {
    FLASH->ACR &= (uint32_t)(~((uint32_t)FLASH_ACR_ACC64));
  }
}

/**
  * @brief  Enable or disable the power down mode during RUN mode .
  * @param  NewState: new state of the power down mode during RUN mode.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void FLASH_RUNPowerDownCmd(FunctionalState NewState)
{
    /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
     /* Unlock the RUN_PD bit */
     FLASH->PDKEYR = FLASH_PDKEY1;
     FLASH->PDKEYR = FLASH_PDKEY2;
     
     /* Set the RUN_PD bit in  FLASH_ACR register to put Flash in power down mode */    
     FLASH->ACR |= (uint32_t)FLASH_ACR_RUN_PD;
  }
  else
  {
    /* Clear the RUN_PD bit in  FLASH_ACR register to put Flash in idle  mode */
    FLASH->ACR &= (uint32_t)(~(uint32_t)FLASH_ACR_RUN_PD);
  }
}

/**
  * @brief  Enable or disable the power down mode during Sleep mode.
  * @param  NewState: new state of the power down mode during sleep mode.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void FLASH_SLEEPPowerDownCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Set the SLEEP_PD bit to put Flash in power down mode during sleep mode */
    FLASH->ACR |= FLASH_ACR_SLEEP_PD;
  }
  else
  {
    /* Clear the SLEEP_PD bit in to put Flash in idle mode during sleep mode */
    FLASH->ACR &= (uint32_t)(~((uint32_t)FLASH_ACR_SLEEP_PD));
  }
}

/**
  * @brief  Unlocks the data memory and FLASH_PECR register access.
  * @param  None
  * @retval None
  */
void FLASH_UnlockData(void)
{
  if((FLASH->PECR & FLASH_PECR_PELOCK) != RESET)
  {  
    /* Unlocking the Data memory and FLASH_PECR register access*/
    FLASH->PEKEYR = FLASH_PEKEY1;
    FLASH->PEKEYR = FLASH_PEKEY2;
  }
}

/**
  * @brief  Unlocks the program memory access.
  * @param  None
  * @retval None
  */
void FLASH_UnlockProgram(void)
{
  if((FLASH->PECR & FLASH_PECR_PRGLOCK) != RESET)
  {
    /* Unlocking the data memory and FLASH_PECR register access */
    FLASH_UnlockData();
  
    /* Unlocking the program memory access */
    FLASH->PRGKEYR = FLASH_PRGKEY1;
    FLASH->PRGKEYR = FLASH_PRGKEY2;  
  }
}

/**
  * @brief  Unlocks the option bytes block access.
  * @param  None
  * @retval None
  */
void FLASH_UnlockOptionByte(void)
{
  if((FLASH->PECR & FLASH_PECR_OPTLOCK) != RESET)
  {
    /* Unlocking the data memory and FLASH_PECR register access */
    FLASH_UnlockData();
  
    /* Unlocking the option bytes block access */
    FLASH->OPTKEYR = FLASH_OPTKEY1;
    FLASH->OPTKEYR = FLASH_OPTKEY2;
  }
}

/**
  * @brief  Locks the Data memory and FLASH_PECR register access.
  * @param  None
  * @retval None
  */
void FLASH_LockData(void)
{
  /* Set the PELOCK Bit to lock the data memory and FLASH_PECR register access */
  FLASH->PECR |= FLASH_PECR_PELOCK;
}

/**
  * @brief  Locks the Program memory access.
  * @param  None
  * @retval None
  */
void FLASH_LockProgram(void)
{
  /* Set the PRGLOCK Bit to lock the program memory access */
  FLASH->PECR |= FLASH_PECR_PRGLOCK;
}

/**
  * @brief  Locks the option bytes block access.
  * @param  None
  * @retval None
  */
void FLASH_LockOptionByte(void)
{
  /* Set the OPTLOCK Bit to lock the option bytes block access */
  FLASH->PECR |= FLASH_PECR_OPTLOCK;
}

/**
  * @brief  Launch the option byte loading.
  * @param  None
  * @retval None
  */
void FLASH_OBLLaunch(void)
{
  /* Set the OBL_Launch bit to lauch the option byte loading */
  FLASH->PECR |= FLASH_PECR_OBL_LAUNCH;
}

/**
  * @brief  Erase a word in data memory.
  * @param  Address: specifies the address to be erased
  * @note   A data memory word is erased in the data memory only if the address 
  *         to load is the start address of a word (multiple of a word).  
  * @retval FLASH Status: The returned value can be: 
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseDataWord(uint32_t Address)
{
  FLASH_Status status = FLASH_COMPLETE;
  
  /* Check the parameters */
  assert_param(IS_FLASH_DATA_ADDRESS(Address));
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ERASE_TIMEOUT);
  
  if(status == FLASH_COMPLETE)
  {
    /* Write "00000000h" to valid address in the data memory" */
    *(__IO uint32_t *) Address = 0x00000000;
  }
   
  /* Return the erase status */
  return status;
}

/**
  * @brief  Erases a specified page in program memory.
  * @param  Page_Address: The page address in program memory to be erased.
  * @note   A Page is erased in the Program memory only if the address to load 
  *         is the start address of a page (multiple of 256 bytes).  
  * @retval FLASH Status: The returned value can be: 
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseProgramPage(uint32_t Page_Address)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters */
  assert_param(IS_FLASH_PROGRAM_ADDRESS(Page_Address));
 
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ERASE_TIMEOUT);
  
  if(status == FLASH_COMPLETE)
  {
    /* If the previous operation is completed, proceed to erase the page */

    /* Set the ERASE bit */
    FLASH->PECR |= FLASH_PECR_ERASE;

    /* Set PROG bit */
    FLASH->PECR |= FLASH_PECR_PROG;
  
    /* Write 00000000h to the first word of the program page to erase */
    *(__IO uint32_t *)Page_Address = 0x00000000;
 
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ERASE_TIMEOUT);

    if(status != FLASH_TIMEOUT)
    {
      /* If the erase operation is completed, disable the ERASE and PROG bits */
      FLASH->PECR &= (uint32_t)(~FLASH_PECR_PROG);
      FLASH->PECR &= (uint32_t)(~FLASH_PECR_ERASE);
    }    
  }     
  /* Return the Erase Status */
  return status;
}

/**
  * @brief  Programs a word at a specified address in program memory.
  * @param  Address: specifies the address to be written.
  * @param  Data: specifies the data to be written.
  * @retval FLASH Status: The returned value can be:  
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_FastWriteProgramWord(uint32_t Address, uint32_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters */
  assert_param(IS_FLASH_PROGRAM_ADDRESS(Address));
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);
  
  if(status == FLASH_COMPLETE)
  {
    /* If the previous operation is completed, proceed to program the new  word */  
    *(__IO uint32_t *)Address = Data;
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);       
  }
  /* Return the Write Status */
  return status;
}

/**
  * @brief  Write a Byte at a specified address in data memory.
  * @param  Address: specifies the address to be written.
  * @param  Data: specifies the data to be written.
  * @note This function assumes that the is data word is already erased.  
  * @retval FLASH Status: The returned value can be:  
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_FastWriteDataByte(uint32_t Address, uint8_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  uint32_t tmp = 0, tmpaddr = 0;
  
  /* Check the parameters */
  assert_param(IS_FLASH_DATA_ADDRESS(Address)); 

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);
  
  if(status == FLASH_COMPLETE)
  {
    if(Data != (uint8_t)0x00) 
    {
      /* If the previous operation is completed, proceed to write the new Data */        
      *(__IO uint8_t *)Address = Data;
            
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);           
    }
    else
    {
      tmpaddr = Address & 0xFFFFFFFC;
      tmp = * (__IO uint32_t *) tmpaddr;
      tmpaddr = 0xFF << ((uint32_t) (0x8 * (Address & 0x3)));
      tmp &= ~tmpaddr;
      status = FLASH_EraseDataWord(Address & 0xFFFFFFFC);
      status = FLASH_FastWriteDataWord((Address & 0xFFFFFFFC), tmp);         
    }       
  }
  /* Return the Write Status */
  return status;
}

/**
  * @brief  Writes a half word at a specified address in data memory.
  * @param  Address: specifies the address to be written.
  * @param  Data: specifies the data to be written.
  * @note This function assumes that the is data word is already erased.  
  * @retval FLASH Status: The returned value can be: 
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or  FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_FastWriteDataHalfWord(uint32_t Address, uint16_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  uint32_t tmp = 0, tmpaddr = 0;
  
  /* Check the parameters */
  assert_param(IS_FLASH_DATA_ADDRESS(Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);
  
  if(status == FLASH_COMPLETE)
  {
    if(Data != (uint16_t)0x0000) 
    {
      /* If the previous operation is completed, proceed to write the new data */   
      *(__IO uint16_t *)Address = Data;
  
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);
    }
    else
    {
      if((Address & 0x3) != 0x3)
      {        
        tmpaddr = Address & 0xFFFFFFFC;
        tmp = * (__IO uint32_t *) tmpaddr;
        tmpaddr = 0xFFFF << ((uint32_t) (0x8 * (Address & 0x3)));
        tmp &= ~tmpaddr;        
        status = FLASH_EraseDataWord(Address & 0xFFFFFFFC);
        status = FLASH_FastWriteDataWord((Address & 0xFFFFFFFC), tmp);      
      }
      else
      {
        FLASH_FastWriteDataByte(Address, 0x00);
        FLASH_FastWriteDataByte(Address + 1, 0x00);          
      }      
    }      
  } 
  /* Return the Write Status */
  return status;
}

/**
  * @brief  Programs a word at a specified address in data memory.
  * @param  Address: specifies the address to be written.
  * @param  Data: specifies the data to be written.
  * @note This function assumes that the is data word is already erased.    
  * @retval FLASH Status: The returned value can be: 
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_FastWriteDataWord(uint32_t Address, uint32_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters */
  assert_param(IS_FLASH_DATA_ADDRESS(Address));
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);
  
  if(status == FLASH_COMPLETE)
  {
    /* If the previous operation is completed, proceed to program the new data */    
    *(__IO uint32_t *)Address = Data;
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);       
  }
  /* Return the Write Status */
  return status;
}

/**
  * @brief  Write a Byte at a specified address in data memory without erase.
  * @param  Address: specifies the address to be written.
  * @param  Data: specifies the data to be written.
  * @retval FLASH Status: The returned value can be: 
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_WriteDataByte(uint32_t Address, uint8_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  uint32_t tmp = 0, tmpaddr = 0;
  
  /* Check the parameters */
  assert_param(IS_FLASH_DATA_ADDRESS(Address)); 

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);
  
  if(status == FLASH_COMPLETE)
  {
    if(Data != (uint8_t) 0x00)
    {
      /* If the previous operation is completed, proceed to write the new Data */     
      FLASH->PECR |= FLASH_PECR_FTDW;
    
      *(__IO uint8_t *)Address = Data;
    
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);
  
      if(status != FLASH_TIMEOUT)
      {
        /* If the write operation is completed, disable the FTDW bit */
        FLASH->PECR &= (uint32_t)(~FLASH_PECR_FTDW);
      }    
    }
    else
    {
      tmpaddr = Address & 0xFFFFFFFC;
      tmp = * (__IO uint32_t *) tmpaddr;
      tmpaddr = 0xFF << ((uint32_t) (0x8 * (Address & 0x3)));
      tmp &= ~tmpaddr;        
      status = FLASH_EraseDataWord(Address & 0xFFFFFFFC);
      status = FLASH_FastWriteDataWord((Address & 0xFFFFFFFC), tmp);        
    }   
  }
  /* Return the Write Status */
  return status;
}

/**
  * @brief  Writes a half word at a specified address in data memory without erase.
  * @param  Address: specifies the address to be written.
  * @param  Data: specifies the data to be written.
  * @retval FLASH Status: The returned value can be:  
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_WriteDataHalfWord(uint32_t Address, uint16_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  uint32_t tmp = 0, tmpaddr = 0;
  
  /* Check the parameters */
  assert_param(IS_FLASH_DATA_ADDRESS(Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);
  
  if(status == FLASH_COMPLETE)
  {
    if(Data != (uint16_t)0x0000)
    {
      /* If the previous operation is completed, proceed to write the new data */   
      FLASH->PECR |= FLASH_PECR_FTDW;  

      *(__IO uint16_t *)Address = Data;
   
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);
    
      if(status != FLASH_TIMEOUT)
      {
        /* If the write operation is completed, disable the FTDW  bit */
        FLASH->PECR &= (uint32_t)(~FLASH_PECR_FTDW);
      }      
    }
    else
    {
      if((Address & 0x3) != 0x3)
      {
        tmpaddr = Address & 0xFFFFFFFC;
        tmp = * (__IO uint32_t *) tmpaddr;
        tmpaddr = 0xFFFF << ((uint32_t) (0x8 * (Address & 0x3)));
        tmp &= ~tmpaddr;          
        status = FLASH_EraseDataWord(Address & 0xFFFFFFFC);
        status = FLASH_FastWriteDataWord((Address & 0xFFFFFFFC), tmp);  
      }
      else
      {
        FLASH_FastWriteDataByte(Address, 0x00);
        FLASH_FastWriteDataByte(Address + 1, 0x00);          
      }              
    }    
  } 
  /* Return the Write Status */
  return status;
}

/**
  * @brief  Programs a word at a specified address in data memory without erase.
  * @param  Address: specifies the address to be written.
  * @param  Data: specifies the data to be written.
  * @retval FLASH Status: The returned value can be:  
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or  FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_WriteDataWord(uint32_t Address, uint32_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  
  /* Check the parameters */
  assert_param(IS_FLASH_DATA_ADDRESS(Address));
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);
  
  if(status == FLASH_COMPLETE)
  {
    /* If the previous operation is completed, proceed to program the data */    
    FLASH->PECR |= FLASH_PECR_FTDW; 
    
    *(__IO uint32_t *)Address = Data;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);
    
    if(status != FLASH_TIMEOUT)
    {
      /* If the write operation is completed, disable the FTDW  bit */
      FLASH->PECR &= (uint32_t)(~FLASH_PECR_FTDW);
    }         
  }
  /* Return the Write Status */
  return status;
}

/**
  * @brief  Write protects the desired pages
  * @param  FLASH_Pages: specifies the address of the pages to be write protected.
  *   This parameter can be:
  *     @arg  value between FLASH_WRProt_Pages0to15 and FLASH_WRProt_Pages496to511
  *     @arg FLASH_WRProt_AllPages
  * @param  NewState: new state of the specified FLASH Pages Wtite protection.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval FLASH Status: The returned value can be: 
  * FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_WriteProtection(uint32_t FLASH_Pages, FunctionalState NewState)
{
  uint32_t WRP01_Data = 0, WRP23_Data = 0;
  
  FLASH_Status status = FLASH_COMPLETE;
  uint32_t tmp1 = 0, tmp2 = 0;
  
  /* Check the parameters */
  assert_param(IS_FLASH_WRPROT_PAGE(FLASH_Pages));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
     
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);
 
  if(status == FLASH_COMPLETE)
  {
    if (NewState != DISABLE)
    {
      WRP01_Data = (uint16_t)(((FLASH_Pages & WRP01_MASK) | OB->WRP01));
      WRP23_Data = (uint16_t)((((FLASH_Pages & WRP23_MASK)>>16 | OB->WRP23))); 
      tmp1 = (uint32_t)(~(WRP01_Data) << 16)|(WRP01_Data);
      OB->WRP01 = tmp1;
      
      tmp2 = (uint32_t)(~(WRP23_Data) << 16)|(WRP23_Data);
      OB->WRP23 = tmp2;      
    }             
    
    else
    {
      WRP01_Data = (uint16_t)(~FLASH_Pages & (WRP01_MASK & OB->WRP01));
      WRP23_Data = (uint16_t)((((~FLASH_Pages & WRP23_MASK)>>16 & OB->WRP23))); 

      tmp1 = (uint32_t)((~WRP01_Data) << 16)|(WRP01_Data);
      OB->WRP01 = tmp1;
      
      tmp2 = (uint32_t)((~WRP23_Data) << 16)|(WRP23_Data);
      OB->WRP23 = tmp2;
    }
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);
  }

  /* Return the write protection operation Status */
  return status;      
}

/**
  * @brief  Enables or disables the read out protection.
  * @param  FLASH_ReadProtection_Level: specifies the read protection level. 
  *   This parameter can be:
  *     @arg FLASH_ReadProtection_Level_0: No protection
  *     @arg FLASH_ReadProtection_Level_1: Read protection of the memory                     
  *     @arg FLASH_ReadProtection_Level_2: Chip protection
  *     @retval FLASH Status: The returned value can be: 
  * FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ReadProtection(uint8_t FLASH_ReadProtection_Level)
{
  FLASH_Status status = FLASH_COMPLETE;
  uint8_t tmp1 = 0;
  uint32_t tmp2 = 0;
  
  /* Check the parameters */
  assert_param(IS_FLASH_READPROTECTION_LEVEL(FLASH_ReadProtection_Level));
  status = FLASH_WaitForLastOperation(ERASE_TIMEOUT);
  
  /* calculate the option byte to write */
  tmp1 = (uint8_t)(~(FLASH_ReadProtection_Level ));
  tmp2 = (uint32_t)(((uint32_t)((uint32_t)(tmp1) << 16)) | ((uint32_t)FLASH_ReadProtection_Level));
  
  if(status == FLASH_COMPLETE)
  {         
   /* program read protection level */
    OB->RDP = tmp2;
  } 
  /* Return the Read protection operation Status */
  return status;            
}

/**
  * @brief  Programs the FLASH User Option Byte: IWDG_SW / RST_STOP / RST_STDBY.
  * @param  OB_IWDG: Selects the WDG mode
  *   This parameter can be one of the following values:
  *     @arg OB_IWDG_SW: Software WDG selected
  *     @arg OB_IWDG_HW: Hardware WDG selected
  * @param  OB_STOP: Reset event when entering STOP mode.
  *   This parameter can be one of the following values:
  *     @arg OB_STOP_NoRST: No reset generated when entering in STOP
  *     @arg OB_STOP_RST: Reset generated when entering in STOP
  * @param  OB_STDBY: Reset event when entering Standby mode.
  *   This parameter can be one of the following values:
  *     @arg OB_STDBY_NoRST: No reset generated when entering in STANDBY
  *     @arg OB_STDBY_RST: Reset generated when entering in STANDBY
  * @retval FLASH Status: The returned value can be: 
  * FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_UserOptionByteConfig(uint8_t OB_IWDG, uint8_t OB_STOP, uint8_t OB_STDBY)
{
  FLASH_Status status = FLASH_COMPLETE; 
  uint32_t tmp = 0, tmp1 = 0;

  /* Check the parameters */
  assert_param(IS_OB_IWDG_SOURCE(OB_IWDG));
  assert_param(IS_OB_STOP_SOURCE(OB_STOP));
  assert_param(IS_OB_STDBY_SOURCE(OB_STDBY));

  /* Get the User Option byte register */
  tmp1 = (FLASH->OBR & 0x000F0000) >> 16;
    
  /* Calculate the user option byte to write */ 
  tmp = (uint32_t)(((uint32_t)~((uint32_t)((uint32_t)(OB_IWDG) | (uint32_t)(OB_STOP) | (uint32_t)(OB_STDBY) | tmp1))) << ((uint32_t)0x10));
  tmp |= ((uint32_t)(OB_IWDG) | ((uint32_t)OB_STOP) | (uint32_t)(OB_STDBY) | tmp1);
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);
  
  if(status == FLASH_COMPLETE)
  {  
    /* Write the User Option Byte */              
    OB->USER = tmp; 
  }    
  /* Return the Option Byte program Status */
  return status;
}

/**
  * @brief  Programs the FLASH brownout reset threshold level Option Byte.
  * @param  OB_BOR: Selects the brownout reset threshold level
  *   This parameter can be one of the following values:
  *     @arg OB_BOR_OFF: Reset threshold level for the 1.45 V-1.55 V voltage 
              range (power down only).
  *     @arg OB_BOR_LEVEL1: Reset threshold level for 1.69 V-1.8 V voltage 
  *          range (power on).
  *     @arg OB_BOR_LEVEL2: Reset threshold level for 1.94 V-2.1 V voltage 
  *          range (power on).
  *     @arg OB_BOR_LEVEL3: Reset threshold level for 2.3 V-2.49 V voltage 
  *          range (power on).
  *     @arg OB_BOR_LEVEL4: Reset threshold level for 2.54 V-2.74 voltage 
  *          range (power on).
  *     @arg OB_BOR_LEVEL5: Reset threshold level for 2.77 V-3.0 V voltage 
  *          range (power on).
  * @retval FLASH Status: The returned value can be: 
  * FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_SetBORResetLevel(uint8_t OB_BOR)
{
  FLASH_Status status = FLASH_COMPLETE;
  uint32_t tmp = 0, tmp1 = 0;

  /* Check the parameters */
  assert_param(IS_OB_BOR_LEVEL(OB_BOR));

  /* Get the User Option byte register */
  tmp1 = (FLASH->OBR & 0x00700000) >> 16;
     
  /* Calculate the option byte to write */
  tmp = (uint32_t)~(OB_BOR | tmp1)<<16;
  tmp |= (OB_BOR | tmp1);
    
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);
  
  if(status == FLASH_COMPLETE)
  {  
    /* Write the BOR Option Byte */            
    OB->USER = tmp; 
  }    
  /* Return the Option Byte program Status */
  return status;
}

/**
  * @brief  Returns the FLASH User Option Bytes values.
  * @param  None
  * @retval The FLASH User Option Bytes .
  */
uint32_t FLASH_GetUserOptionByte(void)
{
  /* Return the User Option Byte */
  return (uint32_t)(FLASH->OBR >> 16);
}

/**
  * @brief  Returns the FLASH Write Protection Option Bytes value.
  * @param  None
  * @retval The FLASH Write Protection Option Bytes value
  */
uint32_t FLASH_GetWriteProtectionOptionByte(void)
{
  /* Return the FLASH write protection Register value */
  return (uint32_t)(FLASH->WRPR);
}

/**
  * @brief  Checks whether the FLASH Read out Protection Status is set or not.
  * @param  None
  * @retval FLASH ReadOut Protection Status(SET or RESET)
  */
FlagStatus FLASH_GetReadProtectionStatus(void)
{
  FlagStatus readstatus = RESET;
  
  if ((uint8_t)(FLASH->OBR) != (uint8_t)FLASH_ReadProtection_Level_0)
  {
    readstatus = SET;
  }
  else
  {
    readstatus = RESET;
  }
  return readstatus;
}

/**
  * @brief  Enables or disables the specified FLASH interrupts.
  * @param  FLASH_IT: specifies the FLASH interrupt sources to be enabled or 
  *         disabled.
  *   This parameter can be any combination of the following values:     
  *     @arg FLASH_IT_EOP: FLASH end of programming Interrupt
  *     @arg FLASH_IT_ERR: FLASH Error Interrupt  
  * @retval None 
  */
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FLASH_IT(FLASH_IT)); 
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if(NewState != DISABLE)
  {
    /* Enable the interrupt sources */
    FLASH->PECR |= FLASH_IT;
  }
  else
  {
    /* Disable the interrupt sources */
    FLASH->PECR &= ~(uint32_t)FLASH_IT;
  }
}

/**
  * @brief  Checks whether the specified FLASH flag is set or not.
  * @param  FLASH_FLAG: specifies the FLASH flag to check.
  *   This parameter can be one of the following values:
  *     @arg FLASH_FLAG_BSY: FLASH write/erase operations in progress flag 
  *     @arg FLASH_FLAG_EOP: FLASH End of Operation flag
  *     @arg FLASH_FLAG_READY: FLASH Ready flag after low power mode
  *     @arg FLASH_FLAG_ENDHV: FLASH End of high voltage flag
  *     @arg FLASH_FLAG_WRPERR: FLASH Write protected error flag 
  *     @arg FLASH_FLAG_PGAERR: FLASH Programming Alignment error flag
  *     @arg FLASH_FLAG_SIZERR: FLASH size error flag    
  *     @arg FLASH_FLAG_OPTVERR: FLASH Option validity error flag         
  * @retval The new state of FLASH_FLAG (SET or RESET).
  */
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters */
  assert_param(IS_FLASH_GET_FLAG(FLASH_FLAG));

  if((FLASH->SR & FLASH_FLAG) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* Return the new state of FLASH_FLAG (SET or RESET) */
  return bitstatus; 
}

/**
  * @brief  Clears the FLASH’s pending flags.
  * @param  FLASH_FLAG: specifies the FLASH flags to clear.
  *   This parameter can be any combination of the following values:
  *     @arg FLASH_FLAG_EOP: FLASH End of Operation flag
  *     @arg FLASH_FLAG_WRPERR: FLASH Write protected error flag 
  *     @arg FLASH_FLAG_PGAERR: FLASH Programming Alignment error flag 
  *     @arg FLASH_FLAG_SIZERR: FLASH size error flag    
  *     @arg FLASH_FLAG_OPTVERR: FLASH Option validity error flag              
  * @retval None
  */
void FLASH_ClearFlag(uint32_t FLASH_FLAG)
{
  /* Check the parameters */
  assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG));
  
  /* Clear the flags */
  FLASH->SR = FLASH_FLAG;
}

/**
  * @brief  Returns the FLASH Status.
  * @param  None
  * @retval FLASH Status: The returned value can be: 
  *   FLASH_BUSY, FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP or FLASH_COMPLETE.
  */
FLASH_Status FLASH_GetStatus(void)
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
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout)
{ 
  FLASH_Status status = FLASH_COMPLETE;
   
  /* Check for the FLASH Status */
  status = FLASH_GetStatus();
  
  /* Wait for a FLASH operation to complete or a TIMEOUT to occur */
  while((status == FLASH_BUSY) && (Timeout != 0x00))
  {
    status = FLASH_GetStatus();
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
