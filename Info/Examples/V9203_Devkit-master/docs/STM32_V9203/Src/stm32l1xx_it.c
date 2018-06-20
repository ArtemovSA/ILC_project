/**
  ******************************************************************************
  * @file    Project/STM32L1xx_StdPeriph_Template/stm32l1xx_it.c
  * @author  MCD Application Team
  * @version V1.0.0RC1
  * @date    07/02/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
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
#include "stm32l1xx_it.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
  
  
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}
/******************************************************************************/
/*                 STM32L1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l1xx_md.s).                                            */
/******************************************************************************/
//External Interrupts
void WWDG_IRQHandler(void)           // Window Watchdog
{
}

void PVD_IRQHandler(void)           //PVD through EXTI Line detect
{
}
void  TAMPER_STAMP_IRQHandler(void)   // Tamper and Time Stamp
{
}
//void RTC_WKUP_IRQHandler(void)       // RTC Wakeup
//{
//}
void FLASH_IRQHandler(void)          // FLASH
{
}
void RCC_IRQHandler(void)            // RCC
{
}
//void EXTI0_IRQHandler(void)          // EXTI Line 0
//{
//}
void EXTI1_IRQHandler(void)          // EXTI Line 1
{
}
//void EXTI2_IRQHandler(void)          // EXTI Line 2
//{
//}
void EXTI3_IRQHandler(void)          // EXTI Line 3
{
}
//void EXTI4_IRQHandler(void)          // EXTI Line 4
//{
//}
void DMA1_Channel1_IRQHandler(void)  // DMA1 Channel 1
{
}
//void DMA1_Channel2_IRQHandler(void)  // DMA1 Channel 2
//{
//}
void DMA1_Channel3_IRQHandler(void)  // DMA1 Channel 3
{
}
void DMA1_Channel4_IRQHandler(void)  // DMA1 Channel 4
{
}
void DMA1_Channel5_IRQHandler(void)  // DMA1 Channel 5
{
}
void DMA1_Channel6_IRQHandler(void)  // DMA1 Channel 6
{
}
void DMA1_Channel7_IRQHandler(void)  // DMA1 Channel 7
{
}
void ADC1_IRQHandler(void)           // ADC1
{
}
void USB_HP_IRQHandler(void)         // USB High Priority
{
}
void USB_LP_IRQHandler(void)         // USB Low  Priority
{
}
void DAC_IRQHandler(void)            // DAC
{
}
//void COMP_IRQHandler(void)           // COMP through EXTI Line
//{
//}
//void EXTI9_5_IRQHandler(void)        // EXTI Line 9..5
//{
//}
void LCD_IRQHandler(void)            // LCD
{
}
void TIM9_IRQHandler(void)           // TIM9
{
}
void TIM10_IRQHandler(void)          // TIM10
{
}
void TIM11_IRQHandler(void)          // TIM11
{
}
//void TIM2_IRQHandler(void)           // TIM2
//{
//}
//void TIM3_IRQHandler(void)           // TIM3
//{
//}
//void TIM4_IRQHandler(void)           // TIM4
//{
//}
void I2C1_EV_IRQHandler(void)        // I2C1 Event
{
}
void I2C1_ER_IRQHandler(void)        // I2C1 Error
{
}
void I2C2_EV_IRQHandler(void)        // I2C2 Event
{
}
void I2C2_ER_IRQHandler(void)        // I2C2 Error
{
}
void SPI1_IRQHandler(void)           // SPI1
{
}
void SPI2_IRQHandler(void)           // SPI2
{
}
void USART1_IRQHandler(void)         // USART1
{
}
//void USART2_IRQHandler(void)         // USART2
//{
//}
//void USART3_IRQHandler(void)         // USART3
//{
//}
void EXTI15_10_IRQHandler(void)      // EXTI Line 15..10
{
}
void RTC_Alarm_IRQHandler(void)      // RTC Alarm through EXTI Line
{
}
void USB_FS_WKUP_IRQHandler(void)    // USB FS Wakeup from suspend
{
}
//void TIM6_IRQHandler(void)           // TIM6
//{
//}
void TIM7_IRQHandler(void)           // TIM7
{
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
