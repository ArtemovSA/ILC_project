/**
  ******************************************************************************
  * @file    stm32l1xx_comp.c
  * @author  MCD Application Team
  * @version V1.0.0RC1
  * @date    07/02/2010
  * @brief   This file provides all the COMP firmware functions.
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
#include "stm32l1xx_comp.h"

/** @addtogroup STM32L1xx_StdPeriph_Driver
  * @{
  */

/** @defgroup COMP 
  * @brief COMP driver modules
  * @{
  */ 

/** @defgroup COMP_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup COMP_Private_Defines
  * @{
  */
#define COMP_CSR_RESET_VALUE    (uint32_t)0x00000000 /*!<CSR Reset value */
 

/**
  * @}
  */ 

/** @defgroup COMP_Private_Macros
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup COMP_Private_Variables
  * @{
  */ 


/**
  * @}
  */

/** @defgroup COMP_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup COMP_Private_Functions
  * @{
  */
  
  /**
  * @brief  Deinitializes COMP peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
void COMP_DeInit(void)
{
  COMP->CSR = COMP_CSR_RESET_VALUE;    /*!< Set COMP->CSR to reset value */
}

/**
  * @brief  Initializes the COMP2 peripheral according to the specified parameters
  *         in the COMP_InitStruct.
  * @note   This function configures only COMP2.
  * @param  COMP_InitStruct: pointer to an COMP_InitTypeDef structure that contains 
  *         the configuration information for the specified COMP peripheral.
  * @retval None
  */
void COMP_Init(COMP_InitTypeDef* COMP_InitStruct)
{
  uint32_t tmpreg = 0;
  
  /* Check the parameters */
  assert_param(IS_COMP_INVERTING_INPUT(COMP_InitStruct->COMP_InvertingInput));
  assert_param(IS_COMP_OUTPUT(COMP_InitStruct->COMP_OutputSelect));
  assert_param(IS_COMP_SPEED(COMP_InitStruct->COMP_Speed));

  /* Get the COMP CSR value */
  tmpreg = COMP->CSR;

  /* Clear the  INSEL[2:0], OUTSEL[1:0] and SPEED bits */ 
  tmpreg &= (uint32_t) (~ (uint32_t) (COMP_CSR_OUTSEL | COMP_CSR_INSEL | COMP_CSR_SPEED));
  
  /* Configure COMP: speed, inversion input selection and output redirection */
  /* Set SPEED bit according to COMP_InitStruct->COMP_Speed value */
  /* Set INSEL bits according to COMP_InitStruct->COMP_InvertingInput value */ 
  /* Set OUTSEL bits according to COMP_InitStruct->COMP_OutputSelect value */  
  tmpreg |= (uint32_t)((COMP_InitStruct->COMP_Speed | COMP_InitStruct->COMP_InvertingInput 
                        | COMP_InitStruct->COMP_OutputSelect));
  /* The COMP2 comparator is enabled as soon as the INSEL[2:0] bits value are 
     different from "000" */
  /* Write to COMP_CSR register */
  COMP->CSR = tmpreg;  
}

/**
  * @brief  Enable or disable the COMP1 peripheral.
  * @param  NewState: new state of the COMP1 peripheral. 
  *         This parameter can be: ENABLE or DISABLE.
  * @Note   This function enables/disables only the COMP1.
  * @retval None
  */
void COMP_Cmd(FunctionalState NewState)
{
  /* Check the parameter */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the COMP1 */
    COMP->CSR |= (uint32_t) COMP_CSR_CMP1EN;
  }
  else
  {
    /* Disable the COMP1  */
    COMP->CSR &= (uint32_t)(~COMP_CSR_CMP1EN);
  }
}

/**
  * @brief  Return the output level of the selected comparator.
  * @param  COMP_Selection: the selected comparator. This parameter can be one of
  *         the following values:
  *     @arg COMP_Selection_COMP1 : COMP1  selected
  *     @arg COMP_Selection_COMP2 : COMP2  selected  
  * @retval Returns the comparator output level.
  */
uint8_t COMP_GetOutputLevel(uint32_t COMP_Selection)
{
  uint8_t compout = 0x0;

  /* Check the parameters */
  assert_param(IS_COMP_ALL_PERIPH(COMP_Selection));

  /* Check if Comparator 1 is selected */
  if(COMP_Selection == COMP_Selection_COMP1)
  {
    /* Check if comparator 1 output level is high */
    if((COMP->CSR & COMP_CSR_CMP1OUT) != (uint8_t) RESET)
    {
      /* Get Comparator 1 output level */
      compout = (uint8_t) COMP_OutputLevel_High;
    }
    /* comparator 1 output level is low */
    else
    {
      /* Get Comparator 1 output level */
      compout = (uint8_t) COMP_OutputLevel_Low;
    }
  }
  /* Comparator 2 is selected */
  else
  {
    /* Check if comparator 2 output level is high */
    if((COMP->CSR & COMP_CSR_CMP2OUT) != (uint8_t) RESET)
    {
      /* Get Comparator output level */
      compout = (uint8_t) COMP_OutputLevel_High;
    }
    /* comparator 2 output level is low */
    else
    {
      /* Get Comparator 2 output level */
      compout = (uint8_t) COMP_OutputLevel_Low;
    }
  }
  /* Return the comparator output level */
  return (uint8_t)(compout);
}

/**
  * @brief  Enables or disables the window mode.
  * param   NewState: new state of the window mode. This parameter can
  *         be ENABLE or DISABLE.
  * @retval None
  */
void COMP_WindowCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the window mode */
    COMP->CSR |= (uint32_t) COMP_CSR_WNDWE;
  }
  else
  {
    /* Disable the window mode */
    COMP->CSR &= (uint32_t)(~COMP_CSR_WNDWE);
  }
}

/**
  * @brief  Enables or disables the output of internal reference voltage.
  * @param  NewState: new state of the Vrefint output. 
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void COMP_VrefintOutputCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the output of internal reference voltage */
    COMP->CSR |= (uint32_t) COMP_CSR_VREFOUTEN;
  }
  else
  {
    /* Disable the output of internal reference voltage */
    COMP->CSR &= (uint32_t) (~COMP_CSR_VREFOUTEN);
  }
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
