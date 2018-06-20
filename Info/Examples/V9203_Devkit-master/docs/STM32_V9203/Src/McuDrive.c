#include "Includes.h"

#define SYSCLK_FREQ_32MHz

#if defined SYSCLK_FREQ_8MHz
/**
  * @brief  Sets System clock frequency to 8MHz and configure HCLK, PCLK2
  *         and PCLK1 prescalers.
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */
static void SetSysClockTo8(void)
{  
   __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);   //ʹ��HSE,�ⲿ��Ƶ����

    
    //�ȴ��ⲿ�����ȶ�����ʱ�˳�
    do
    {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;            //�ⲿ�����Ƿ��ȶ�
        StartUpCounter++;
    } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != RESET)
    {
        HSEStatus = (uint32_t)0x01;
    }
    else
    {
        HSEStatus = (uint32_t)0x00;
    }
  
    //����ⲿ��������
    if (HSEStatus == (uint32_t)0x01)
    {
      //64λFLASH����ʧЧ��32λ
      FLASH->ACR &= ~FLASH_ACR_PRFTEN;

      //Ԥȡ����ʧЧ��ֻ����64λ����FLASH��Ч
      FLASH->ACR &= ~FLASH_ACR_PRFTEN;

      //��ȡFLASH�ȴ�0������
      FLASH->ACR &= ~FLASH_ACR_LATENCY;

      //ʹ��PWR APB1ʱ��
      RCC->APB1ENR |= RCC_APB1ENR_PWREN;

      //ѡ���ѹ��ΧΪ1.5V
      PWR->CR = PWR_CR_VOS_1;

      //�ȴ���Դ�ȶ�
      while((PWR->CSR & PWR_CSR_VOSF) != RESET)
      {
      }

      //HCLK = SYSCLK
      RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

      //PCLK2 = HCLK 
      RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

      //PCLK1 = HCLK
      RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;
      
      //PLL ѡ��HSE��Ϊʱ��Դ PLLCLK = HSE = 8MHz
      RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
      RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSE;
    
      //�ȴ��ⲿHSE�ȶ���Ϊϵͳʱ��
      while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
      {
      }
    }
    else
    {
      //ʱ���ڲ���ƵRC��Ϊϵͳʱ��,��д

    }   
}

#elif defined SYSCLK_FREQ_16MHz
/*=========================================================================================\n
* @function_name: SetSysClockTo16
* @function_file: Main.c
* @����: ����ϵͳʱ��Ϊ32M,���� HCLK, PCLK2��PCLK1
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
static void SetSysClockTo16(void)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);   //ʹ��HSE,�ⲿ��Ƶ����

    //�ȴ��ⲿ�����ȶ�����ʱ�˳�
    do
    {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;            //�ⲿ�����Ƿ��ȶ�
        StartUpCounter++;
    } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != RESET)
    {
        HSEStatus = (uint32_t)0x01;
    }
    else
    {
        HSEStatus = (uint32_t)0x00;
    }

    //����ⲿ��������
    if (HSEStatus == (uint32_t)0x01)
    {
      //ʹ��64λFLASH����
      FLASH->ACR |= FLASH_ACR_ACC64;

      //ʹ��Ԥȡ���棬ֻ����64λ����FLASH��Ч
      FLASH->ACR |= FLASH_ACR_PRFTEN;

      //��ȡFLASH�ȴ�1������
      FLASH->ACR |= FLASH_ACR_LATENCY;

      //ʹ��PWR APB1ʱ��
      RCC->APB1ENR |= RCC_APB1ENR_PWREN;

      //ѡ���ѹ��ΧΪ1.5V
      PWR->CR = PWR_CR_VOS_1;

      //�ȴ���Դ�ȶ�
      while((PWR->CSR & PWR_CSR_VOSF) != RESET)
      {
      }

      //HCLK = SYSCLK
      RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV2;

      //PCLK2 = HCLK 
      RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

      //PCLK1 = HCLK
      RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;

      //PLL ѡ��HSE��Ϊʱ��Դ PLLCLK = (HSE * 12) / 3 = 32MHz
      RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL |
                                        RCC_CFGR_PLLDIV));
      RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMUL12 | RCC_CFGR_PLLDIV3);

      //ʹ��PLL
      RCC->CR |= RCC_CR_PLLON;

      //�ȴ�PLL�ȶ�
      while((RCC->CR & RCC_CR_PLLRDY) == 0)
      {
      }

      //ѡ��PLL��Ϊϵͳʱ��
      RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
      RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

      //�ȴ�ϵͳʱ���л���PLL�ȶ�
      while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x0C)
      {
      }
    }
    else
    {
      //ʱ���ڲ���ƵRC��Ϊϵͳʱ��,��д

    }
}

#elif defined SYSCLK_FREQ_32MHz
/*=========================================================================================\n
* @function_name: SetSysClockTo32
* @function_file: Main.c
* @����: ����ϵͳʱ��Ϊ32M,���� HCLK, PCLK2��PCLK1
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
static void SetSysClockTo32(void)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);   //ʹ��HSE,�ⲿ��Ƶ����

    //�ȴ��ⲿ�����ȶ�����ʱ�˳�
    do
    {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;            //�ⲿ�����Ƿ��ȶ�
        StartUpCounter++;
    } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != RESET)
    {
        HSEStatus = (uint32_t)0x01;
    }
    else
    {
        HSEStatus = (uint32_t)0x00;
    }

    //����ⲿ��������
    if (HSEStatus == (uint32_t)0x01)
    {
      //ʹ��64λFLASH����
      FLASH->ACR |= FLASH_ACR_ACC64;

      //ʹ��Ԥȡ���棬ֻ����64λ����FLASH��Ч
      FLASH->ACR |= FLASH_ACR_PRFTEN;

      //��ȡFLASH�ȴ�1������
      FLASH->ACR |= FLASH_ACR_LATENCY;

      //ʹ��PWR APB1ʱ��
      RCC->APB1ENR |= RCC_APB1ENR_PWREN;

      //ѡ���ѹ��ΧΪ1.8V
      PWR->CR = PWR_CR_VOS_0;

      //�ȴ���Դ�ȶ�
      while((PWR->CSR & PWR_CSR_VOSF) != RESET)
      {
      }

      //HCLK = SYSCLK
      RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

      //PCLK2 = HCLK 
      RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

      //PCLK1 = HCLK
      RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;

      //PLL ѡ��HSE��Ϊʱ��Դ PLLCLK = (HSE * 12) / 3 = 32MHz
      RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL |
                                        RCC_CFGR_PLLDIV));
      RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMUL12 | RCC_CFGR_PLLDIV3);

      //ʹ��PLL
      RCC->CR |= RCC_CR_PLLON;

      //�ȴ�PLL�ȶ�
      while((RCC->CR & RCC_CR_PLLRDY) == 0)
      {
      }

      //ѡ��PLL��Ϊϵͳʱ��
      RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
      RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

      //�ȴ�ϵͳʱ���л���PLL�ȶ�
      while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x0C)
      {
      }
    }
    else
    {
      //ʱ���ڲ���ƵRC��Ϊϵͳʱ��,��д

    }
}
#endif

/*=========================================================================================\n
* @function_name: SetSysClock
* @function_file: Main.c
* @����: ����ϵͳʱ��
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
static void SetSysClock(void)
{
#if defined SYSCLK_FREQ_8MHz
    SetSysClockTo8();
#elif defined SYSCLK_FREQ_16MHz
    SetSysClockTo16();            //����ϵͳʱ��Ϊ16M
#elif defined SYSCLK_FREQ_32MHz
    SetSysClockTo32();            //����ϵͳʱ��Ϊ32M
#endif
}
/*=========================================================================================\n
* @function_name: SystemInit
* @function_file: Main.c
* @����: ϵͳ��ʼ��
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void SystemInit (void)
{
    //����MSI ON
    RCC->CR |= (uint32_t)0x00000100;
    
    //��λSW[1:0], HPRE[3:0], PPRE2[2:0], MCOSEL[2:0] and MCOPRE[2:0] bits
    RCC->CFGR &= (uint32_t)0x88FFC00C;
    
    //��λHSION, HSEON, CSSON and PLLON bits
    RCC->CR &= (uint32_t)0xEEFEFFFE;
    
    //��λHSEBYP bit 
    RCC->CR &= (uint32_t)0xFFFBFFFF;
    
    //��λPLLSRC, PLLMUL[3:0] and PLLDIV[1:0] bits
    RCC->CFGR &= (uint32_t)0xFF02FFFF;
    
    //�ر������ж�
    RCC->CIR = 0x00000000;
    
    //���� System clock frequency, HCLK, PCLK2 and PCLK1 prescalers 
    //����FLASH����λ�����ȴ����ں�ʹ��Ԥȡ����
    SetSysClock();
}
/*=========================================================================================\n
* @function_name: SystemInit
* @function_file: Main.c
* @����: ��ʼ��ϵͳOSCʱ��
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void SetSysOscClock(void)
{
    //Reset Backup Domain 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    /* Allow access to RTC */
    PWR_RTCAccessCmd(ENABLE);
    RCC_RTCResetCmd(ENABLE);
    RCC_RTCResetCmd(DISABLE);
    RCC_LSEConfig(RCC_LSE_ON);      //�����ⲿ��Ƶʱ��
    //�ȴ�LSE�ȶ�
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
    RCC_LSICmd(ENABLE);
}

/*=========================================================================================\n
* @function_name: SysIO_Init
* @function_file: McuDrive.c
* @����: ��ʼGPIO
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void SysIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //����GPIOA��GPIOHʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA
                          |RCC_AHBPeriph_GPIOD
                          |RCC_AHBPeriph_GPIOH
                          |RCC_AHBPeriph_GPIOE
                          |RCC_AHBPeriph_GPIOC
                           , ENABLE); //ʹ��GPIOA��GPIOH��GPIOE��ʱ��
 //����P��,������������˿��ٶ�40M
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

    /*   //��բ��

    CloseSwitchLight();
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    GPIO_Init(GPIOA, &GPIO_InitStructure);*/

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 |GPIO_Pin_4;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC, GPIO_Pin_5);
    GPIO_ResetBits(GPIOC, GPIO_Pin_4);
    //GPIO_SetBits(GPIOC, GPIO_Pin_1);
    //GPIO_SetBits(GPIOC, GPIO_Pin_2);
    //GPIO_SetBits(GPIOC, GPIO_Pin_3);

    //����
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOE, GPIO_Pin_12);

  //������0��EMC����
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_12;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC, GPIO_Pin_0);    
    GPIO_SetBits(GPIOC, GPIO_Pin_1);
    GPIO_SetBits(GPIOC, GPIO_Pin_2);
   GPIO_SetBits(GPIOC, GPIO_Pin_3);
   

   // GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 |GPIO_Pin_4;  //PD0134 spi2
  //  GPIO_Init(GPIOD, &GPIO_InitStructure);
   // GPIO_ResetBits(GPIOD, GPIO_Pin_3);    
   // GPIO_ResetBits(GPIOD, GPIO_Pin_4);
    //���� PA.12   UP PA.11 DW
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 |GPIO_Pin_11;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
   //GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    //GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    //GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
    //GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15 |GPIO_Pin_0  ;                                                  
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //GPIO_ResetBits(GPIOA, GPIO_Pin_7);    
    //GPIO_ResetBits(GPIOA, GPIO_Pin_4);   //spi1 dmaͨ��
  //  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
  //  GPIO_ResetBits(GPIOA, GPIO_Pin_6);
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);    
   // GPIO_SetBits(GPIOA, GPIO_Pin_11);
    //GPIO_ResetBits(GPIOA, GPIO_Pin_12);
    GPIO_ResetBits(GPIOA, GPIO_Pin_15);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_8 |
                                                    GPIO_Pin_10|GPIO_Pin_11 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOE, GPIO_Pin_2);    
    GPIO_ResetBits(GPIOE, GPIO_Pin_4);
    GPIO_ResetBits(GPIOE, GPIO_Pin_5);
    GPIO_ResetBits(GPIOE, GPIO_Pin_6);
    GPIO_ResetBits(GPIOE, GPIO_Pin_3);    
    GPIO_ResetBits(GPIOE, GPIO_Pin_8);
    GPIO_ResetBits(GPIOE, GPIO_Pin_12);
    GPIO_ResetBits(GPIOE, GPIO_Pin_15);
    GPIO_ResetBits(GPIOE, GPIO_Pin_10);
    
    //GPIO_ResetBits(GPIOE, GPIO_Pin_13);
    //DelayXms(500);
    GPIO_SetBits(GPIOE, GPIO_Pin_13);
    GPIO_SetBits(GPIOE, GPIO_Pin_14);

    //GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;  CF2
    //GPIO_Init(GPIOB, &GPIO_InitStructure);
    //GPIO_ResetBits(GPIOB, GPIO_Pin_2);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_Init(GPIOH, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOH, GPIO_Pin_2);

    
     
    //485����
    Uart3_RevEn();               //Ĭ�Ͻ���̬
    //GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;    //485 ���ƹܽ�
    //GPIO_Init(GPIOD, &GPIO_InitStructure);
         
    
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
     GPIO_Init(GPIOE, &GPIO_InitStructure);
     BE_I2C_Init();
     //GPIO_ResetBits(GPIOB, GPIO_Pin_11);
     //Stop_Delay();
    // GPIO_SetBits(GPIOB, GPIO_Pin_11);
     Bronco_PMCtrl(Work_normal);  //9203PM����
}
/*=========================================================================================\n
* @function_name: Sys_InitExit
* @function_file: McuDrive.c
* @����: ��ʼ���ⲿ�ж�
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-12)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Sys_InitExit(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG , ENABLE);
    RCC_APB2PeriphClockLPModeCmd(RCC_APB2Periph_SYSCFG , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE); //ʹ��GPIOE��ʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); //ʹ��GPIOB��ʱ��
    //PE7 ����Ϊ����ģʽ
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
   //PE7 CF1����
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

        //PB2 ����Ϊ����ģʽ
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
   //PB2 CF2����
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //����EXTI7�ж�    
    //�ж����ȼ�NVIC����
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;        //EXTI7�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);                             //��ʼ��NVIC�Ĵ���

       //����EXTI2�ж�    
    //�ж����ȼ�NVIC����
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;        //EXTI2�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;          //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);                             //��ʼ��NVIC�Ĵ���

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource7);
     SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource2);
    //Configure EXTI line
    EXTI_InitStructure.EXTI_Line = EXTI_Line7 |EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

//  EXTI_GenerateSWInterrupt(EXTI_Line15);

  // SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource4);
   //KeyIrqCmd(DISABLE);
   

}
/*=========================================================================================\n
* @function_name: TIM3_IRQHandler
* @function_file: McuDrive.c
* @����: 
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-08)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void TIM3_IRQHandler(void)   //TIM3�ж�
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
    {

        ComBom_10msDy();
        Uart2_Dy10ms();
        Uart3_Dy10ms();
        KeyScan();              //����
        
        //�������¼�
        guc_SecCnt++;
        if(guc_SecCnt>=100)
        {
            guc_SecCnt=0;
            guc_TimeEvent |= flgEtTim_Second;
        }       
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
    }
}

/*=========================================================================================\n
* @function_name: TIM2_IRQHandler
* @function_file: McuDrive.c
* @����: 
* 
* @����: 
* @����: 
* @����:   hjj (2013-08-01)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void TIM2_IRQHandler(void)   //TIM2�ж�
{
//    static uint8 i = 0;
//    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
//    {
//        //PowDownChk();
//        i++;
//        if(i >=15)
//        {
//            if(guc_FactoryType!=FactorMd)
//            {
//                //Bronco_massdata();
//
//                gui_SystemEvent |= flgEtGlb_ReadBronco;
//                i = 0;
//            }
//        }
//        TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
//    }
}


/*=========================================================================================\n
* @function_name: TIM3_IRQHandler
* @function_file: McuDrive.c
* @����: 
* 
* @����: 
* @����: 
* @����:   hjj (2013-09-11)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void TIM4_IRQHandler(void)   //TIM4�ж�
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx�����жϱ�־ 
    }
}

/*=========================================================================================\n
* @function_name: TIM3_Int_Init
* @function_file: McuDrive.c
* @����: ��ʱ��3��ʼ��
* 
* 
* @����: 
* @param:arr  
* @param:psc  
* @����: 
* @����:   lwb (2013-07-08)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void TIM3_Int_Init(uint16 arr,uint16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);        //ʱ��ʹ��
    
    //��ʱ��TIM3��ʼ��
    TIM_TimeBaseStructure.TIM_Period = arr;                     //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ    
    TIM_TimeBaseStructure.TIM_Prescaler =psc;                   //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);             //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );                   //ʹ��ָ����TIM3�ж�,��������ж�

    //�ж����ȼ�NVIC����
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;             //TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);                             //��ʼ��NVIC�Ĵ���

    TIM_Cmd(TIM3, ENABLE);                                      //ʹ��TIMx                   
}


/*=========================================================================================\n
* @function_name: TIM2_Int_Init
* @function_file: McuDrive.c
* @����: ��ʱ��2��ʼ��
* @����: 
* @param:arr  ����ֵ
* @param:psc  ʱ��ֵ
//---------------------------------------------------------------
        TIM2 Configuration: Output Compare Timing Mode:
        TIM2 counter clock at 32 MHz
        hz = (TIM2 counter clock/psc) / arr
//---------------------------------------------------------------   
* @����: 
* @����:   hjj (2013-07-08)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void TIM2_Int_Init(uint16 arr,uint16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);        //ʱ��ʹ��
    
    //��ʱ��TIM2��ʼ��
    TIM_TimeBaseStructure.TIM_Period = arr;                     //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ    
    TIM_TimeBaseStructure.TIM_Prescaler =psc;                   //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);             //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE );                  //ʹ��ָ����TIM2�ж�,��������ж�

    //�ж����ȼ�NVIC����
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             //TIM2�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;          //�����ȼ�4��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);                             //��ʼ��NVIC�Ĵ���

    TIM_Cmd(TIM2, ENABLE);                                     //ʹ��TIMx                   
}

/*=========================================================================================\n
* @function_name: TIM4_Int_Init
* @function_file: McuDrive.c
* @����: ��ʱ��4��ʼ��
* @����: 
* @param:arr  ����ֵ
* @param:psc  ʱ��ֵ
//---------------------------------------------------------------
        TIM4 Configuration: Output Compare Timing Mode:
        TIM4 counter clock at 32 MHz
        hz = (TIM2 counter clock/psc) / arr
//---------------------------------------------------------------   
* @����: 
* @����:   hjj (2013-09-11)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void TIM4_Int_Init(uint16 arr,uint16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);        //ʱ��ʹ��
    
    //��ʱ��TIM2��ʼ��
    TIM_TimeBaseStructure.TIM_Period = arr;                     //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ    
    TIM_TimeBaseStructure.TIM_Prescaler =psc;                   //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);             //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE );                  //ʹ��ָ����TIM4�ж�,��������ж�

    //�ж����ȼ�NVIC����
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;             //TIM4�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //�����ȼ�5��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);                             //��ʼ��NVIC�Ĵ���

    TIM_Cmd(TIM4, ENABLE);                                     //ʹ��TIMx                   
}

/*=========================================================================================\n
* @function_name: TIM6_Int_Init
* @function_file: McuDrive.c
* @����: ��ʱ��6��ʼ��
* @����: 
* @param:arr  ����ֵ
* @param:psc  ʱ��ֵ
//---------------------------------------------------------------
        TIM4 Configuration: Output Compare Timing Mode:
        TIM4 counter clock at 32 MHz
        hz = (TIM2 counter clock/psc) / arr
//---------------------------------------------------------------   
* @����: 
* @����:   hjj (2013-09-11)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
/*void TIM6_Int_Init(uint16 arr,uint16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);        //ʱ��ʹ��
    
    //��ʱ��TIM6��ʼ��
    TIM_TimeBaseStructure.TIM_Period = arr;                     //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ    
    TIM_TimeBaseStructure.TIM_Prescaler =psc;                   //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);             //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE );                  //ʹ��ָ����TIM4�ж�,��������ж�

    //�ж����ȼ�NVIC����
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;             //TIM4�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;          //�����ȼ�5��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);                             //��ʼ��NVIC�Ĵ���

    TIM_Cmd(TIM6, ENABLE);                                     //ʹ��TIMx                   
}
*/

/*=========================================================================================\n
* @function_name: ChangeBodeProc
* @function_file: McuDrive.c
* @����: �޸Ĳ����ʴ���
* 
* @����: 
* @����: 
* @����:   lwb (2012-05-31)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void ChangeBodeProc(void)
{
    uint8 ucMode;

    SysE2ReadData(EEP_UARTBDM,&ucMode,1);

    if(ucMode>=0x40)
    {
        ucMode=0x08;
    }

    if(ucMode==0x02)
    {
        gui_485Bode=600;
    }else if(ucMode==0x04)
    {
        gui_485Bode=1200;
    }else if(ucMode==0x08)
    {
        gui_485Bode=2400;
    }else if(ucMode==0x10)
    {
        gui_485Bode=4800;
    }else if(ucMode==0x20)
    {
        gui_485Bode=9600;
    }else
    {
        gui_485Bode=2400;
    }

    //Sys_InitUart3(gui_485Bode);
    Sys_InitUart3(9600);
    Sys_InitUart2(2400);
    ComBom_Init(ComIndex_Uart3);    //��uart4 �˿ڶ�Ӧ�� COM��ʼ��
    ComBom_Init(ComIndex_Uart2);    //��uart4 �˿ڶ�Ӧ�� COM��ʼ��
}
/*=========================================================================================\n
* @function_name: SysUart_Init
* @function_file: McuDrive.c
* @����: ��ʼ������
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-04)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void SysUart_Init(void)
{
    ChangeBodeProc();
}
/*=========================================================================================\n
* @function_name: TZ_GPIOCtrl
* @function_file: McuDrive.c
* @����: 
* 
* 
* @����: 
* @param:State  
* @����: 
* @����:   lwb (2013-07-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void TZ_GPIOCtrl(bool State)
{
    if(State)
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_12);
    }
    else
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_12);
    }
}
/*=========================================================================================\n
* @function_name: BgCtrFun
* @function_file: McuDrive.c
* @����: �������
* 
* @����: 
* @����: 
* @����:   lwb (2012-06-07)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void BgCtrFun(void)
{
    uint8 flag;

    flag=0;

    if(guc_DyBgLed>0)
    {
        guc_DyBgLed--;
        if(guc_DyBgLed==0)
        {
            if(gut_SysWarn.byte==0                      //û�и澯��
//               &&gs_LoadSwichStatus.ControlWarning==0   //û�б�������
               &&flag==0)            
            {
                CloseBeiGuang();//�رձ���
            }
        }
        else
        {
            OpenBeiGuang();
        }
    }
    else
    {
        if(gut_SysWarn.byte==0                      //û�и澯��
//           &&gs_LoadSwichStatus.ControlWarning==0   //û�б�������
           &&flag==0
           )  
        {
            CloseBeiGuang();                        //�رձ���
        }
    }

    if(guc_DyLed>0)
    {
        guc_DyLed--;
        if(guc_DyLed==0)
        {
            CloseSwitchLight();
        }
    }else 
    {

        if(gut_SysWarn.byte>0)              //�д����֣���������͸澯��
        {
            OpenBeiGuang();
        }

    }
}

/*=========================================================================================\n
* @function_name: SwichPluseOutType
* @function_file: McuDrive.c
* @����: �л����ӹ���
* 
* 
* @����: 
* @param:type  0-RTC  2-ʱ��Ͷ��
* @����: 
* @����:   lwb (2012-06-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void SwichPluseOutType(uint8 type)
{
    if(type==0)
    {
//        P9FC|=BIT3;                                                     //���RTC
//        P9OE&=(~BIT3);
    }
    if(type==2)
    {
//        P9FC&=(~BIT3);
//        P9OE&=(~BIT3);
    }
}

/*=========================================================================================\n
* @function_name: RTC_Configuration
* @function_file: McuDrive.c
* @����: RTC����
* 
* @����: 
* @����: 
* @����:  hjj (2013-08-05)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void RTC_Configuration(void)
{
    RTC_TimeTypeDef   RTC_TimeStructure;
    RTC_DateTypeDef   RTC_DateStructure;
    RTC_InitTypeDef   RTC_InitStructure;
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    // Allow access to RTC 
   // PWR_RTCAccessCmd(ENABLE);
    // Reset Backup Domain 
   // RCC_RTCResetCmd(ENABLE);
   // RCC_RTCResetCmd(DISABLE);
    //!< LSE Enable 
    //RCC_LSEConfig(RCC_LSE_ON);
    //!< Wait till LSE is ready 
    // while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    //{}
    //!< LCD Clock Source Selection 
    //RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    // Enable the RTC Clock 
    RCC_RTCCLKCmd(ENABLE);
    // Wait for RTC APB registers synchronisation 
    RTC_WaitForSynchro();     

    /* Set the Time */
 RTC_TimeStructure.RTC_Hours   = 0x08;
  RTC_TimeStructure.RTC_Minutes = 0x00;
 RTC_TimeStructure.RTC_Seconds = 0x57;
	
  /* Set the Date */
  RTC_DateStructure.RTC_Month = RTC_Month_July;
 RTC_DateStructure.RTC_Date = 0x02;
  RTC_DateStructure.RTC_Year = 0x10;
RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Friday;
		
  /* Calender Configuartion */
 RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
  RTC_InitStructure.RTC_SynchPrediv	=  0xFF;
  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
  RTC_Init(&RTC_InitStructure);

 RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
  RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
	
}


/*=========================================================================================\n
* @function_name: RTC_Configuration
* @function_file: McuDrive.c
* @����: RTCӲʱ�ӻ�ȡ
* 
* @����: 
* @����: 
* @����:  hjj (2013-08-05)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void GetExtRTC(void)           
{
    RTC_TimeTypeDef   RTC_TimeStructure;
    RTC_DateTypeDef   RTC_DateStructure;
    RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);
    RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure);
    gs_ClkTmp.ucSecond = RTC_TimeStructure.RTC_Seconds;
    gs_ClkTmp.ucMinute = RTC_TimeStructure.RTC_Minutes ;
    gs_ClkTmp.ucHour = RTC_TimeStructure.RTC_Hours;
    gs_ClkTmp.ucDay = RTC_DateStructure.RTC_Date;
    gs_ClkTmp.ucMonth = RTC_DateStructure.RTC_Month;
    gs_ClkTmp.ucWeek = RTC_DateStructure.RTC_WeekDay;
    gs_ClkTmp.ucYear = RTC_DateStructure.RTC_Year;  
}

/*=========================================================================================\n
* @function_name: RTC_Configuration
* @function_file: McuDrive.c
* @����: RTCӲʱ������
* 
* @����: 
* @����: 
* @����:  hjj (2013-08-05)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void SetExtRTC(void)           //����Ӳʱ��
{
    RTC_TimeTypeDef   RTC_TimeStructure;
    RTC_DateTypeDef   RTC_DateStructure;
    //Set the Time 
    RTC_TimeStructure.RTC_Hours   = gs_DateTime.ucHour;
    RTC_TimeStructure.RTC_Minutes = gs_DateTime.ucMinute;
    RTC_TimeStructure.RTC_Seconds = gs_DateTime.ucSecond;   
    // Set the Date 
    RTC_DateStructure.RTC_Month = gs_DateTime.ucMonth;
    RTC_DateStructure.RTC_Date = gs_DateTime.ucDay;
    RTC_DateStructure.RTC_Year = gs_DateTime.ucWeek;
    RTC_DateStructure.RTC_WeekDay = gs_DateTime.ucWeek;
    
    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
    RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
}

/*=========================================================================================\n
* @function_name: ADC_Configuration
* @function_file: McuDrive.c
* @����: ADC����
* 
* @����: 
* @����: 
* @����:  hjj (2013-08-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void ADC_Configuration(void)
{  
    ADC_InitTypeDef ADC_InitStructure;
    /* Enable ADC1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    /* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    /* ADC1 regular channel18 configuration */ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_48Cycles);
    /* Enable the temperature sensor and vref internal channel*/
    ADC_TempSensorVrefintCmd(ENABLE);
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
    /* Start ADC1 Software Conversion */ 
    //ADC_SoftwareStartConv(ADC1);
}


uint16 T_Get_Adc(void) 
    {
        ADC_SoftwareStartConv(ADC1);      //ʹ��ָ����ADC1�����ת����������
        while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
        return ADC_GetConversionValue(ADC1);    //�������һ��ADC1�������ת�����
    }

uint16 T_Get_Temp(void)
    {
    uint16 temp_val=0;
    uint16 t;
    for(t=0;t<10;t++)
        {
        temp_val+=T_Get_Adc();     //TampSensor
        DelayXms(10);
        }
    return temp_val/10;
    }

/*=========================================================================================\n
* @function_name: MCUForPowerOff_Adc
* @function_file: McuDrive.c
* @����: ����͹���ǰIO�ڣ����账��������STOPģʽ
*        ͨ��ADC����
* 
* @����: 
* @param:type 
* @����: 
* @����:   hjj (2013-07-31)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void MCUForPowerOff_Adc(void)
{
//    IWDG_ReloadCounter(); 
//    /* Set MSI clock range to 1MHz */
//    RCC_MSIRangeConfig(RCC_MSIRange_4MHz);
//    /* Select MSI as system clock source */
//    RCC_SYSCLKConfig(RCC_SYSCLKSource_MSI);
//    /* Wait till PLL is used as system clock source */
//    while (RCC_GetSYSCLKSource() != 0x00)
//    {} 
//     IWDG_ReloadCounter();  
//    GPIO_InitTypeDef GPIO_InitStructure;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;        
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_All;
//    
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//    GPIO_Init(GPIOC, &GPIO_InitStructure);
//    //GPIO_Init(GPIOD, &GPIO_InitStructure);
//    GPIO_Init(GPIOE, &GPIO_InitStructure);
//    GPIO_Init(GPIOH, &GPIO_InitStructure);
//    
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;        
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2 | GPIO_Pin_3 
//                                          |GPIO_Pin_5|GPIO_Pin_6| GPIO_Pin_7|GPIO_Pin_8 |GPIO_Pin_4
//                                          |GPIO_Pin_9 |GPIO_Pin_10 | GPIO_Pin_11|GPIO_Pin_12
//                                          |GPIO_Pin_15 ;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//    
//
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;        
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 |GPIO_Pin_2 | GPIO_Pin_3 
//                                          |GPIO_Pin_5|GPIO_Pin_6| GPIO_Pin_7|GPIO_Pin_8 |GPIO_Pin_4
//                                          |GPIO_Pin_9 |GPIO_Pin_10 | GPIO_Pin_11|GPIO_Pin_12 |GPIO_Pin_13 |
//                                         GPIO_Pin_14 |GPIO_Pin_15 ;
//    GPIO_Init(GPIOD, &GPIO_InitStructure);
//    
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
//    GPIO_Init(GPIOD, &GPIO_InitStructure);
//    
//    //GPIO_ResetBits(GPIOA,GPIO_Pin_14);
//    
//    /*GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//    GPIO_SetBits(GPIOB,GPIO_Pin_4);*/
//    
//    DAC_Cmd(DAC_Channel_1,DISABLE);
//    DAC_Cmd(DAC_Channel_2,DISABLE);
//    LCD_Cmd(DISABLE);
//    ADC_TempSensorVrefintCmd(DISABLE); 
//    DMA_Cmd(DMA1_Channel1, DISABLE);
//    ADC_Cmd(ADC1, DISABLE); 
//    RCC->AHBENR = 0x00000000;
//    RCC->APB2ENR = 0x00000000;
//    RCC->APB1ENR = 0x00000000; 
//    RCC->AHBLPENR = 0x00000000;
//    RCC->APB2LPENR = 0x00000000;
//    RCC->APB1LPENR = 0x00000000; 
//    RCC_HSICmd(DISABLE);
//    PWR_PVDCmd(DISABLE);
//    /* Enable The ultra Low Power Mode */
//    PWR_UltraLowPowerCmd(ENABLE);
//    /* Enable The power down mode during Sleep mode */
//    FLASH_SLEEPPowerDownCmd(ENABLE);
//    /* Request to enter SLEEP mode with regulator in low power mode */
//    //PWR_EnterSleepMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
//    PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
}

/*=========================================================================================\n
* @function_name: MCUForPowerOff_Adc
* @function_file: McuDrive.c
* @����: ����͹���ǰIO�ڣ����账��������STOPģʽ
*        ͨ��Key���Ѷ�ʱ
* 
* @����: 
* @param:type 
* @����: 
* @����:   hjj (2013-07-31)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void MCUForPowerOff_Key(void)
{
//    IWDG_ReloadCounter(); 
//
//    GPIO_InitTypeDef GPIO_InitStructure;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
//    GPIO_Init(GPIOD, &GPIO_InitStructure);
//    IWDG_ReloadCounter();  
//    //LCD->CR = 0X00; 
//    DAC_Cmd(DAC_Channel_1,DISABLE);
//    DAC_Cmd(DAC_Channel_2,DISABLE);
//    ADC_TempSensorVrefintCmd(DISABLE); 
//    DMA_Cmd(DMA1_Channel1, DISABLE);
//    ADC_Cmd(ADC1, DISABLE); 
//    RCC->AHBENR = 0x00000000;
//    RCC->APB2ENR = 0x00000000;
//    RCC->APB1ENR = 0x00000000; 
//    RCC->AHBLPENR = 0x00000000;
//    RCC->APB2LPENR = 0x00000000;
//    RCC->APB1LPENR = 0x00000000; 
//    RCC_HSICmd(DISABLE);
//    /* Enable The ultra Low Power Mode */
//    PWR_PVDCmd(DISABLE);
//    PWR_UltraLowPowerCmd(ENABLE);
//    /* Enable The power down mode during Sleep mode */
//    FLASH_SLEEPPowerDownCmd(ENABLE);
//    /* Request to enter SLEEP mode with regulator in low power mode */
//    //PWR_EnterSleepMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
//    PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
}


void MCUForPowerOff_Rtc(void)
{
//    IWDG_ReloadCounter(); 
//
//    GPIO_InitTypeDef GPIO_InitStructure;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;        
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   
//    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_All;
//    GPIO_Init(GPIOE, &GPIO_InitStructure);
//    GPIO_Init(GPIOH, &GPIO_InitStructure);          
//    
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
//    GPIO_Init(GPIOD, &GPIO_InitStructure);
//    IWDG_ReloadCounter();  
//  /*  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//    GPIO_SetBits(GPIOB,GPIO_Pin_4);*/
//    //LCD->CR = 0X00; 
//    DAC->CR = 0X00;
//    ADC_TempSensorVrefintCmd(DISABLE); 
//    DMA_Cmd(DMA1_Channel1, DISABLE);
//    ADC_Cmd(ADC1, DISABLE); 
//    //RCC->AHBENR = 0x00000000;
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE,DISABLE);
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH,DISABLE);
//    RCC->APB2ENR = 0x00000000;
//    //RCC->APB1ENR = 0x00000000; 
//    RCC->AHBLPENR = 0x00000000;
//    RCC->APB2LPENR = 0x00000000;
//    RCC->APB1LPENR = 0x00000000; 
//    RCC_HSICmd(DISABLE);
//    PWR_PVDCmd(DISABLE);
//    /* Enable The ultra Low Power Mode */
//    PWR_UltraLowPowerCmd(ENABLE);
//    /* Enable The power down mode during Sleep mode */
//    FLASH_SLEEPPowerDownCmd(ENABLE);
//    /* Request to enter SLEEP mode with regulator in low power mode */
//    //PWR_EnterSleepMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
//    PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
}


/*=========================================================================================\n
* @function_name: MCUForPowerOff
* @function_file: McuDrive.c
* @����: key���ѣ�ͨ����ʱ�ڽ���˯�ߣ�����ҪTIM2��ʱ���ADC��ͬʱ���Ƚ����������ϵ绽��
* 
* @����: 
* @param:type 
* @����: 
* @����:   hjj (2013-07-31)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void MCUForWakeUp_Key(void)
{
    __disable_interrupt();  //�ر����ж�
    //SystemInit();           //����ϵͳ��ʱ��
    //SetSysOscClock();       //����ϵͳOSCʱ�� 
    // RCC_HSICmd(ENABLE);   //�ڲ�����ʱ��
    //��ʼ��NVIC�ж�ϵͳ
    //�����ж����������ʼ��ַΪ0x08000000
    //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    // Init_Var();
    // DataSecurityInit();
    //SysIO_Init();               //��ʼ��ϵͳIO
    BE_I2C_Init();
    ShowInitial();
    //Init_RacCtrl();
    //Sys_InitExit();
    //RxReset_V9281();            //��λV9281
    //SysUart_Init();
    //SysLcd_Init();
    //TIM3_Int_Init(99,799);      //10Khz�ļ���Ƶ�ʣ�������100Ϊ10ms  
    //TIM2_Int_Init(99,79);      //100Khz�ļ���Ƶ�ʣ�������100Ϊ1ms  
    //ADC_Configuration();
    //COMP_Configuration();
    if(guc_WakeUpCnt_2second == true)
    {
       // gs_DispStt.uiIndex++;
        if(gs_DispStt.uiIndex >= gs_DispCont.ucJxCnt)
        {
            gs_DispStt.uiIndex = 0;
        }
    }
    else
    {
        SysLcd_Init();
        gs_DispStt.ucTmr = gs_DispCont.ucLxTm;
        gs_DispStt.ucMode = Const_DispMode_LX;
        gs_DispStt.uiIndex = 0;
    }
    
    ShowRefresh();
    RtcIrqCmd(ENABLE,(0x7ff));
    gui_SystemEvent |= flgEtGlb_PowDown;// �����µ��¼����
    guc_FromUpToSleepFlg = PowerOff_Rtc;
    IWDG_ReloadCounter();                         //ι��
    //Disp_Full();
    __enable_interrupt();    
}

/*=========================================================================================\n
* @function_name: MCUForWakeUp_Power
* @function_file: McuDrive.c
* @����: �ϵ绽�� 
* 
* 
* @����: 
* @param:type 
* @����: 
* @����:   hjj (2013-07-31)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void MCUForWakeUp_Power(void)
{
//    __disable_interrupt();  //�ر����ж�
//    SystemInit();           //����ϵͳ��ʱ��
//    SetSysOscClock();       //����ϵͳOSCʱ�� 
//    RCC_HSICmd(ENABLE);   //�ڲ�����ʱ��
//    //��ʼ��NVIC�ж�ϵͳ
//    //�����ж����������ʼ��ַΪ0x08000000
//    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//    Init_Var();
//    DataSecurityInit();
//    SysIO_Init();               //��ʼ��ϵͳIO
//    ShowInitial();
//    IWDG_ReloadCounter();                         //ι��
//    Init_RacCtrl();
//    Sys_InitExit();
//    KeyIrqCmd(DISABLE);
//    RxReset_V9281();            //��λV9281
//    SysUart_Init();
//    SysLcd_Init();
//    TIM3_Int_Init(99,1599);      //10Khz�ļ���Ƶ�ʣ�������100Ϊ10ms  
//    TIM2_Int_Init(99,159);       //100Khz�ļ���Ƶ�ʣ�������100Ϊ1ms  
//    ADC_Configuration();
//    //RTC_Configuration();        //hjj add 
//     RCC_RTCCLKCmd(ENABLE);
//    // Wait for RTC APB registers synchronisation 
//    RTC_WaitForSynchro();  
//    RtcIrqCmd(DISABLE,0);
//    Disp_Full();
//    __enable_interrupt();  
//    IWDG_ReloadCounter();                         //ι��
//     EnyB_SetMeterRule();
//    TIM4_Int_Init(199,15);      //1Mhz�ļ���Ƶ�ʣ�������120Ϊ120us 
}

/*=========================================================================================\n
* @function_name: MCUForWakeUp_Power
* @function_file: McuDrive.c
* @����:  ʵʱʱ�ӻ���
* 
* 
* @����: 
* @param:type 
* @����: 
* @����:   hjj (2013-08-05)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void MCUForWakeUp_RTC(void)
{
    __disable_interrupt();      //�ر����ж�
    //SystemInit();             //����ϵͳ��ʱ��
    //SetSysOscClock();         //����ϵͳOSCʱ�� 
    //RCC_HSICmd(ENABLE);       //�ڲ�����ʱ��
    //��ʼ��NVIC�ж�ϵͳ
    //�����ж����������ʼ��ַΪ0x08000000
    //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    //Init_Var();
    //DataSecurityInit();
    //SysIO_Init();               //��ʼ��ϵͳIO
    
    //Sys_InitExit();
    //SysLcd_Init();
    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    //PWR_RTCAccessCmd(ENABLE);
    //LCDʱ��Դѡ�� LSE  ��LCD��RTCͬһ��ʱ��Դ��
    //RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    //TIM3_Int_Init(99,799);      //10Khz�ļ���Ƶ�ʣ�������100Ϊ10ms  
    // COMP_Configuration();
    if(guc_RtcNorm == SET)
    {
        IWDG_ReloadCounter(); 

        RtcIrqCmd(ENABLE,(0xfff));  //ÿ��2S����ι��
        IWDG_ReloadCounter(); 
        guc_RtcNorm = SET;
        gui_SystemEvent |= flgEtGlb_PowDown;              // �����µ��¼����
        guc_FromUpToSleepFlg = PowerOff_Adc;
    }
    else
    {
        //SystemInit(); 
        IWDG_ReloadCounter();                         //ι��
        BE_I2C_Init();
        IWDG_ReloadCounter();                         //ι��
        ShowInitial();
        if(guc_WakeUpCnt_2second == true)                      //����
        {
            if(guc_IntRtccnt < 50)
            {
                if(gul_ShowId == 0x04000102) 
                {
                    //SleepTimeProc();
                    RTC_TimeTypeDef   RTC_TimeStructure;
                    RTC_DateTypeDef   RTC_DateStructure;
                    RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);
                    RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure);
                    gs_DateTime.ucSecond = RTC_TimeStructure.RTC_Seconds;
                    gs_DateTime.ucMinute = RTC_TimeStructure.RTC_Minutes ;
                    gs_DateTime.ucHour = RTC_TimeStructure.RTC_Hours;
                    gs_DateTime.ucDay = RTC_DateStructure.RTC_Date;
                    gs_DateTime.ucMonth = RTC_DateStructure.RTC_Month;
                    gs_DateTime.ucWeek = RTC_DateStructure.RTC_WeekDay;
                    gs_DateTime.ucYear = RTC_DateStructure.RTC_Year;  
                    gs_DispStt.ucPwrOffRefresh|= Cosnt_RefreshTime;
                }                
                ShowRefresh();
                IWDG_ReloadCounter();                         //ι��
                RtcIrqCmd(ENABLE,(0x7ff));  
                gui_SystemEvent |= flgEtGlb_PowDown;// �����µ��¼����
                guc_FromUpToSleepFlg = PowerOff_Rtc;
            }
            else
            {
                guc_SleepFlg = false;
                guc_Intkeycnt = 0;
                guc_IntRtccnt = 0;
                guc_WakeUpCnt_2second = false;
                IWDG_ReloadCounter();                         //ι��
                RtcIrqCmd(ENABLE,(0xfff));
                guc_RtcNorm = SET;
                gui_SystemEvent |= flgEtGlb_PowDown;// �����µ��¼����
                guc_FromUpToSleepFlg = PowerOff_Adc;
            }
        }
        else                                                                         //����
        {
            gs_DispStt.uiIndex ++;   
            if(gs_DispStt.uiIndex < gs_DispCont.ucLxCnt )
            {
                if(gul_ShowId == 0x04000102) 
                {
                    //SleepTimeProc();
                    RTC_TimeTypeDef   RTC_TimeStructure;
                    RTC_DateTypeDef   RTC_DateStructure;
                    RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);
                    RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure);
                    gs_DateTime.ucSecond = RTC_TimeStructure.RTC_Seconds;
                    gs_DateTime.ucMinute = RTC_TimeStructure.RTC_Minutes ;
                    gs_DateTime.ucHour = RTC_TimeStructure.RTC_Hours;
                    gs_DateTime.ucDay = RTC_DateStructure.RTC_Date;
                    gs_DateTime.ucMonth = RTC_DateStructure.RTC_Month;
                    gs_DateTime.ucWeek = RTC_DateStructure.RTC_WeekDay;
                    gs_DateTime.ucYear = RTC_DateStructure.RTC_Year;  
                    gs_DispStt.ucPwrOffRefresh|= Cosnt_RefreshTime;
                }   
                ShowRefresh();
                IWDG_ReloadCounter();                         //ι��
                RtcIrqCmd(ENABLE,(0x7ff));
                gui_SystemEvent |= flgEtGlb_PowDown;// �����µ��¼����
                guc_FromUpToSleepFlg = PowerOff_Rtc;
            }
            else
            {
                guc_Intkeycnt = 0;
                guc_IntRtccnt = 0;
                guc_SleepFlg = false;
                IWDG_ReloadCounter();                         //ι��
                RtcIrqCmd(ENABLE,(0xfff));
                guc_RtcNorm = SET;
                gui_SystemEvent |= flgEtGlb_PowDown;// �����µ��¼����
                guc_FromUpToSleepFlg = PowerOff_Adc;
            }
        }
    } 
     IWDG_ReloadCounter();                         //ι��
    __enable_interrupt();
}


/*=========================================================================================\n
* @function_name: EXTI0_IRQHandler()
* @function_file: McuDrive.c
* @����: �����ж�
*        
* 
* @����: NewState: ENABLE,DISABLE
* @param:type 
* @����: 
* @����:   hjj (2013-08-01)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void EXTI0_IRQHandler(void)
{   
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        guc_RtcNorm = RESET;
        guc_Intkeycnt++;
        if(guc_Intkeycnt == 1)
        {
            guc_SleepFlg = true;
            guc_FromSleepToUpFlg = WakeUp_Key;    
        }
        else if(guc_Intkeycnt == 2)
        {
            guc_WakeUpCnt_2second = true;
            gs_DispStt.ucMode = Const_DispMode_JX;
            gs_DispStt.uiIndex = 0;
        }
        else
        {
            if(gs_DispStt.ucMode != Const_DispMode_JX)
            {                                      
                gs_DispStt.ucMode = Const_DispMode_JX;
                gs_DispStt.uiIndex = 0;
            }
            gs_DispStt.uiIndex++;
            if(gs_DispStt.uiIndex >= gs_DispCont.ucJxCnt)
            {
                gs_DispStt.uiIndex = 0;
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}



/*=========================================================================================\n
* @function_name: KeyIrqCmd()
* @function_file: McuDrive.c
* @����: �����жϵĿ�����ر�
* 
* 
* @����: NewState: ENABLE,DISABLE
* @param:type 
* @����: 
* @����:   hjj (2013-08-01)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void KeyIrqCmd(FunctionalState NewState)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource0);
    /* Configure Button EXTI line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStructure.EXTI_LineCmd = NewState;
    EXTI_Init(&EXTI_InitStructure);
    /* Enable and set Button EXTI Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x07;
    NVIC_InitStructure.NVIC_IRQChannelCmd = NewState;
    NVIC_Init(&NVIC_InitStructure); 
    //EXTI_ClearITPendingBit(EXTI_Line0);
    //EXTI_ClearFlag(EXTI_Line0);

}

/*=========================================================================================\n
* @function_name: RtcIrqCmd()
* @function_file: McuDrive.c
* @����: RTC���жϵĿ�����ر�
* 
* 
* @����: NewState: ENABLE,DISABLE uint32 cnt:���ж����������32S
* @param:type 
* @����: 
* @����:   hjj (2013-08-05)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void RtcIrqCmd(FunctionalState NewState,uint32 cnt)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure Button EXTI line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line20;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStructure.EXTI_LineCmd = NewState;
    EXTI_Init(&EXTI_InitStructure);
    /* Enable and set EXTI Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x06;
    NVIC_InitStructure.NVIC_IRQChannelCmd = NewState;
    NVIC_Init(&NVIC_InitStructure); 
    IWDG_ReloadCounter();  
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
    RTC_SetWakeUpCounter(cnt);
    RTC_ITConfig(RTC_IT_WUT,NewState);
    RTC_WakeUpCmd(NewState);    
}

/*=========================================================================================\n
* @function_name: COMP_Configuration()
* @function_file: McuDrive.c
* @����: �����ϵ�Ƚϼ�������
* 
* 
* @����: 
* @param:type 
* @����: 
* @����:   hjj (2013-08-01)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void COMP_Configuration(void)
{
    /* COMP Periph clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_COMP , ENABLE);
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure and enable COMP interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = COMP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    EXTI_InitTypeDef EXTI_InitStructure;
    /* Configure and enable External interrupt */
    EXTI_InitStructure.EXTI_Line = EXTI_Line21;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable the switch control mode */
    SYSCFG_RISwitchControlModeCmd(ENABLE);
    /* Close the ADC analog switch of product VCOMP */
    SYSCFG_RIIOSwitchConfig(RI_IOSwitch_VCOMP, ENABLE);
    /* Close the I/O analog switch number 18 */
    SYSCFG_RIIOSwitchConfig(RI_IOSwitch_CH24, ENABLE);
    /* Enable the Comparator 1 */
    COMP_Cmd(ENABLE);
}

/*=========================================================================================\n
* @function_name: COMP_IRQHandler()
* @function_file: McuDrive.c
* @����: compare wakeup 
* 
* @����: 
* @param:type 
* @����: 
* @����:   hjj (2013-08-05)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void COMP_IRQHandler(void)
{ 
    if(EXTI_GetITStatus(EXTI_Line21)!= RESET)
    {
        RtcIrqCmd(DISABLE,0);
        guc_FromSleepToUpFlg = WakeUp_Power;
        /* Clear EXTI Line21 (COMP1) interrupt pending bit */
        EXTI_ClearITPendingBit(EXTI_Line21);
    }
}

 /*=========================================================================================\n
 * @function_name: RTC_WKUP_IRQHandler()
 * @function_file: McuDrive.c
 * @����: RTC wakeup 
 * 
 * @����: 
 * @param:type 
 * @����: 
 * @����:   hjj (2013-08-05)
 * @��ע: 
 *-------------------------------------------------------------------------------------------
 * @�޸���:  
 * @�޸�����: 
 ===========================================================================================*/
void RTC_WKUP_IRQHandler(void)
{
    if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
    {
        //RtcIrqCmd(DISABLE,0);
        RTC_WakeUpCmd(DISABLE);
        guc_FromSleepToUpFlg = WakeUp_Rtc;
        if(guc_RtcNorm == RESET)
        {
            guc_IntRtccnt ++;
        }       
        RTC_ClearITPendingBit(RTC_IT_WUT);
        EXTI_ClearITPendingBit(EXTI_Line20);
    }
}
//hjj add 2013.8.3 ���Ѻ������飬�����±�����guc_FromSleepToUpFlg
const  MCUForWakeUp  MCUWake[] =
{
    MCUForWakeUp_Key,
    MCUForWakeUp_Power,
    MCUForWakeUp_RTC,
};
//hjj add 2013.8.6 ���纯�����飬�����±�����guc_FromUpToSleepFlg
const  MCUForWakeUp  MCUPowerOff[] =
{
    MCUForPowerOff_Key,
    MCUForPowerOff_Adc,
    MCUForPowerOff_Rtc,
};


/*=========================================================================================\n
* @function_name: IWDGconfig(void)
* @function_file: McuDrive.c
* @����: IWDGconfig
* 
* @����: 
* @param:type 
* @����: 
* @����:   hjj (2013-09-22)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void IWDGconfig(void)
{
     
     // IWDG timeout equal to 300 ms (the timeout may varies due to LSI frequency
      //   dispersion) 
      //Enable write access to IWDG_PR and IWDG_RLR registers 
      IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
     
      // IWDG counter clock: 37KHz(LSI) / 256 = 1.15 KHz 
      IWDG_SetPrescaler(IWDG_Prescaler_256);
     
      // Set counter reload value to 375
      IWDG_SetReload(0x2BC);      // ���Ź�Լ2�� 32000/256 =125 (1/125)*375 = 3s
     
      // Reload IWDG counter 
      IWDG_ReloadCounter();
     
      //Enable IWDG (the LSI oscillator will be enabled by hardware) 
      IWDG_Enable();
}


/*=========================================================================================\n
* @function_name: DMA1_Channel2_IRQHandler
* @function_file: McuDrive.c
* @����: 
* 
* @����: 
* @����: 
* @����:   Vango Application Teams
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
 const uint8 Addr[6][6] = {{0,2,4,6,8,10},{10,0,2,4,6,8},{8,10,0,2,4,6}, //�������ݣ�����ͨ������λ��
                                          {6,8,10,0,2,4},{4,6,8,10,0,2},{2,4,6,8,10,0}};                                           
void DMA1_Channel2_IRQHandler(void)  // DMA1 Channel 2
{
    /* DMA2 channel1 used by SPI3 RX */
  //uint16 i=0;//j=0,k=0;  
  //uint16 a=0,b=0,c=0,d=0,e=0,f=0;
  //uint8 addr;
  //spi_wr_flash(0xC01A,0x0000,0x0000);	//DMA��UA
  //SPI_Cmd(SPI1, DISABLE);
   //WriteBronco(0x0,0xC01A);
  
  /* Test on DMA Channel3 Transfer Complete or Half Transfer interrupt */
  if(DMA_GetITStatus(DMA1_IT_TE2))
  {
        DMA_ClearITPendingBit(DMA1_IT_GL2);
  }
  
  if(DMA_GetITStatus(DMA1_IT_TC2))
  {
//      WriteBronco(0x0,0xC01A);
//      if(guc_FFT_Flg == true)
//      {
//          gui_RefreshEvent |= flgEtpara_CalcFFT;
//          guc_FFT_Flg = false;
//      }
//      else
//      {
          gui_RefreshEvent |= flgEtpara_CalcZB;       //����洢�ܲ��¼�
//      }
    /* Clear DMA Channel1 Half Transfer, Transfer Complete and Global interrupt pending bits */
    DMA_ClearITPendingBit(DMA1_IT_GL2);
    /* Stop SPI data transmision when half buffer is empty, waiting for
       half buffer full on Channel 1 - ADC */
    //SPI_Cmd(SPI3, ENABLE);	 
  }
//	DMA_ClearFlag(DMA2_FLAG_TC1);
}


/*=========================================================================================\n
* @function_name: EXTI9_5_IRQHandler
* @function_file: McuDrive.c
* @����: 
* 
* @����: 
* @����: 
* @����:   Vango Application Teams
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void EXTI9_5_IRQHandler(void)        // EXTI Line 9..5  CF1
{
    if(EXTI_GetITStatus(EXTI_Line7) != RESET)
    {    
        EnyB_IntCF(active);
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
 }

 /*=========================================================================================\n
 * @function_name: EXTI2_IRQHandler
 * @function_file: McuDrive.c
 * @����: 
 * 
 * @����: 
 * @����: 
 * @����:   Vango Application Teams
 * @��ע: 
 *-------------------------------------------------------------------------------------------
 * @�޸���:  
 * @�޸�����: 
 ===========================================================================================*/

 void EXTI2_IRQHandler(void)        // EXTI Line 2 CF2
 {
     if(EXTI_GetITStatus(EXTI_Line2) != RESET)
     {    
         EnyB_IntCF(reactive);
         EXTI_ClearITPendingBit(EXTI_Line2);
     }
  }

