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
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);   //使能HSE,外部高频晶振

    
    //等待外部晶振稳定，超时退出
    do
    {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;            //外部晶振是否稳定
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
  
    //如果外部晶振起振
    if (HSEStatus == (uint32_t)0x01)
    {
      //64位FLASH访问失效，32位
      FLASH->ACR &= ~FLASH_ACR_PRFTEN;

      //预取缓存失效，只有在64位访问FLASH有效
      FLASH->ACR &= ~FLASH_ACR_PRFTEN;

      //读取FLASH等待0个周期
      FLASH->ACR &= ~FLASH_ACR_LATENCY;

      //使能PWR APB1时钟
      RCC->APB1ENR |= RCC_APB1ENR_PWREN;

      //选择电压范围为1.5V
      PWR->CR = PWR_CR_VOS_1;

      //等待电源稳定
      while((PWR->CSR & PWR_CSR_VOSF) != RESET)
      {
      }

      //HCLK = SYSCLK
      RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

      //PCLK2 = HCLK 
      RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

      //PCLK1 = HCLK
      RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;
      
      //PLL 选择HSE作为时钟源 PLLCLK = HSE = 8MHz
      RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
      RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSE;
    
      //等待外部HSE稳定作为系统时钟
      while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
      {
      }
    }
    else
    {
      //时钟内部高频RC作为系统时钟,待写

    }   
}

#elif defined SYSCLK_FREQ_16MHz
/*=========================================================================================\n
* @function_name: SetSysClockTo16
* @function_file: Main.c
* @描述: 设置系统时钟为32M,配置 HCLK, PCLK2、PCLK1
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
static void SetSysClockTo16(void)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);   //使能HSE,外部高频晶振

    //等待外部晶振稳定，超时退出
    do
    {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;            //外部晶振是否稳定
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

    //如果外部晶振起振
    if (HSEStatus == (uint32_t)0x01)
    {
      //使能64位FLASH访问
      FLASH->ACR |= FLASH_ACR_ACC64;

      //使能预取缓存，只有在64位访问FLASH有效
      FLASH->ACR |= FLASH_ACR_PRFTEN;

      //读取FLASH等待1个周期
      FLASH->ACR |= FLASH_ACR_LATENCY;

      //使能PWR APB1时钟
      RCC->APB1ENR |= RCC_APB1ENR_PWREN;

      //选择电压范围为1.5V
      PWR->CR = PWR_CR_VOS_1;

      //等待电源稳定
      while((PWR->CSR & PWR_CSR_VOSF) != RESET)
      {
      }

      //HCLK = SYSCLK
      RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV2;

      //PCLK2 = HCLK 
      RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

      //PCLK1 = HCLK
      RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;

      //PLL 选择HSE作为时钟源 PLLCLK = (HSE * 12) / 3 = 32MHz
      RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL |
                                        RCC_CFGR_PLLDIV));
      RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMUL12 | RCC_CFGR_PLLDIV3);

      //使能PLL
      RCC->CR |= RCC_CR_PLLON;

      //等待PLL稳定
      while((RCC->CR & RCC_CR_PLLRDY) == 0)
      {
      }

      //选择PLL作为系统时钟
      RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
      RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

      //等待系统时钟切换到PLL稳定
      while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x0C)
      {
      }
    }
    else
    {
      //时钟内部高频RC作为系统时钟,待写

    }
}

#elif defined SYSCLK_FREQ_32MHz
/*=========================================================================================\n
* @function_name: SetSysClockTo32
* @function_file: Main.c
* @描述: 设置系统时钟为32M,配置 HCLK, PCLK2、PCLK1
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
static void SetSysClockTo32(void)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);   //使能HSE,外部高频晶振

    //等待外部晶振稳定，超时退出
    do
    {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;            //外部晶振是否稳定
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

    //如果外部晶振起振
    if (HSEStatus == (uint32_t)0x01)
    {
      //使能64位FLASH访问
      FLASH->ACR |= FLASH_ACR_ACC64;

      //使能预取缓存，只有在64位访问FLASH有效
      FLASH->ACR |= FLASH_ACR_PRFTEN;

      //读取FLASH等待1个周期
      FLASH->ACR |= FLASH_ACR_LATENCY;

      //使能PWR APB1时钟
      RCC->APB1ENR |= RCC_APB1ENR_PWREN;

      //选择电压范围为1.8V
      PWR->CR = PWR_CR_VOS_0;

      //等待电源稳定
      while((PWR->CSR & PWR_CSR_VOSF) != RESET)
      {
      }

      //HCLK = SYSCLK
      RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

      //PCLK2 = HCLK 
      RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

      //PCLK1 = HCLK
      RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;

      //PLL 选择HSE作为时钟源 PLLCLK = (HSE * 12) / 3 = 32MHz
      RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL |
                                        RCC_CFGR_PLLDIV));
      RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMUL12 | RCC_CFGR_PLLDIV3);

      //使能PLL
      RCC->CR |= RCC_CR_PLLON;

      //等待PLL稳定
      while((RCC->CR & RCC_CR_PLLRDY) == 0)
      {
      }

      //选择PLL作为系统时钟
      RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
      RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

      //等待系统时钟切换到PLL稳定
      while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x0C)
      {
      }
    }
    else
    {
      //时钟内部高频RC作为系统时钟,待写

    }
}
#endif

/*=========================================================================================\n
* @function_name: SetSysClock
* @function_file: Main.c
* @描述: 设置系统时钟
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
static void SetSysClock(void)
{
#if defined SYSCLK_FREQ_8MHz
    SetSysClockTo8();
#elif defined SYSCLK_FREQ_16MHz
    SetSysClockTo16();            //设置系统时钟为16M
#elif defined SYSCLK_FREQ_32MHz
    SetSysClockTo32();            //设置系统时钟为32M
#endif
}
/*=========================================================================================\n
* @function_name: SystemInit
* @function_file: Main.c
* @描述: 系统初始化
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void SystemInit (void)
{
    //设置MSI ON
    RCC->CR |= (uint32_t)0x00000100;
    
    //复位SW[1:0], HPRE[3:0], PPRE2[2:0], MCOSEL[2:0] and MCOPRE[2:0] bits
    RCC->CFGR &= (uint32_t)0x88FFC00C;
    
    //复位HSION, HSEON, CSSON and PLLON bits
    RCC->CR &= (uint32_t)0xEEFEFFFE;
    
    //复位HSEBYP bit 
    RCC->CR &= (uint32_t)0xFFFBFFFF;
    
    //复位PLLSRC, PLLMUL[3:0] and PLLDIV[1:0] bits
    RCC->CFGR &= (uint32_t)0xFF02FFFF;
    
    //关闭所有中断
    RCC->CIR = 0x00000000;
    
    //配置 System clock frequency, HCLK, PCLK2 and PCLK1 prescalers 
    //配置FLASH访问位数、等待周期和使能预取缓存
    SetSysClock();
}
/*=========================================================================================\n
* @function_name: SystemInit
* @function_file: Main.c
* @描述: 初始化系统OSC时钟
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void SetSysOscClock(void)
{
    //Reset Backup Domain 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    /* Allow access to RTC */
    PWR_RTCAccessCmd(ENABLE);
    RCC_RTCResetCmd(ENABLE);
    RCC_RTCResetCmd(DISABLE);
    RCC_LSEConfig(RCC_LSE_ON);      //开启外部低频时钟
    //等待LSE稳定
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
    RCC_LSICmd(ENABLE);
}

/*=========================================================================================\n
* @function_name: SysIO_Init
* @function_file: McuDrive.c
* @描述: 初始GPIO
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void SysIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //开启GPIOA和GPIOH时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA
                          |RCC_AHBPeriph_GPIOD
                          |RCC_AHBPeriph_GPIOH
                          |RCC_AHBPeriph_GPIOE
                          |RCC_AHBPeriph_GPIOC
                           , ENABLE); //使能GPIOA、GPIOH、GPIOE的时钟
 //配置P口,输出、上拉、端口速度40M
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

    /*   //跳闸灯

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

    //背光
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOE, GPIO_Pin_12);

  //悬空输0，EMC测试
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
    //按键 PA.12   UP PA.11 DW
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
    //GPIO_ResetBits(GPIOA, GPIO_Pin_4);   //spi1 dma通道
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

    
     
    //485控制
    Uart3_RevEn();               //默认接收态
    //GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;    //485 控制管脚
    //GPIO_Init(GPIOD, &GPIO_InitStructure);
         
    
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
     GPIO_Init(GPIOE, &GPIO_InitStructure);
     BE_I2C_Init();
     //GPIO_ResetBits(GPIOB, GPIO_Pin_11);
     //Stop_Delay();
    // GPIO_SetBits(GPIOB, GPIO_Pin_11);
     Bronco_PMCtrl(Work_normal);  //9203PM控制
}
/*=========================================================================================\n
* @function_name: Sys_InitExit
* @function_file: McuDrive.c
* @描述: 初始化外部中断
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-12)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Sys_InitExit(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG , ENABLE);
    RCC_APB2PeriphClockLPModeCmd(RCC_APB2Periph_SYSCFG , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE); //使能GPIOE的时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); //使能GPIOB的时钟
    //PE7 设置为输入模式
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
   //PE7 CF1输入
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

        //PB2 设置为输入模式
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
   //PB2 CF2输入
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //允许EXTI7中断    
    //中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;        //EXTI7中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);                             //初始化NVIC寄存器

       //允许EXTI2中断    
    //中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;        //EXTI2中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;          //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);                             //初始化NVIC寄存器

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
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void TIM3_IRQHandler(void)   //TIM3中断
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
    {

        ComBom_10msDy();
        Uart2_Dy10ms();
        Uart3_Dy10ms();
        KeyScan();              //按键
        
        //产生秒事件
        guc_SecCnt++;
        if(guc_SecCnt>=100)
        {
            guc_SecCnt=0;
            guc_TimeEvent |= flgEtTim_Second;
        }       
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
    }
}

/*=========================================================================================\n
* @function_name: TIM2_IRQHandler
* @function_file: McuDrive.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   hjj (2013-08-01)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void TIM2_IRQHandler(void)   //TIM2中断
{
//    static uint8 i = 0;
//    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
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
//        TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
//    }
}


/*=========================================================================================\n
* @function_name: TIM3_IRQHandler
* @function_file: McuDrive.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   hjj (2013-09-11)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void TIM4_IRQHandler(void)   //TIM4中断
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx更新中断标志 
    }
}

/*=========================================================================================\n
* @function_name: TIM3_Int_Init
* @function_file: McuDrive.c
* @描述: 定时器3初始化
* 
* 
* @参数: 
* @param:arr  
* @param:psc  
* @返回: 
* @作者:   lwb (2013-07-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void TIM3_Int_Init(uint16 arr,uint16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);        //时钟使能
    
    //定时器TIM3初始化
    TIM_TimeBaseStructure.TIM_Period = arr;                     //设置在下一个更新事件装入活动的自动重装载寄存器周期的值    
    TIM_TimeBaseStructure.TIM_Prescaler =psc;                   //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);             //根据指定的参数初始化TIMx的时间基数单位
 
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );                   //使能指定的TIM3中断,允许更新中断

    //中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;             //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);                             //初始化NVIC寄存器

    TIM_Cmd(TIM3, ENABLE);                                      //使能TIMx                   
}


/*=========================================================================================\n
* @function_name: TIM2_Int_Init
* @function_file: McuDrive.c
* @描述: 定时器2初始化
* @参数: 
* @param:arr  更新值
* @param:psc  时钟值
//---------------------------------------------------------------
        TIM2 Configuration: Output Compare Timing Mode:
        TIM2 counter clock at 32 MHz
        hz = (TIM2 counter clock/psc) / arr
//---------------------------------------------------------------   
* @返回: 
* @作者:   hjj (2013-07-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void TIM2_Int_Init(uint16 arr,uint16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);        //时钟使能
    
    //定时器TIM2初始化
    TIM_TimeBaseStructure.TIM_Period = arr;                     //设置在下一个更新事件装入活动的自动重装载寄存器周期的值    
    TIM_TimeBaseStructure.TIM_Prescaler =psc;                   //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);             //根据指定的参数初始化TIMx的时间基数单位
 
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE );                  //使能指定的TIM2中断,允许更新中断

    //中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             //TIM2中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;          //从优先级4级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);                             //初始化NVIC寄存器

    TIM_Cmd(TIM2, ENABLE);                                     //使能TIMx                   
}

/*=========================================================================================\n
* @function_name: TIM4_Int_Init
* @function_file: McuDrive.c
* @描述: 定时器4初始化
* @参数: 
* @param:arr  更新值
* @param:psc  时钟值
//---------------------------------------------------------------
        TIM4 Configuration: Output Compare Timing Mode:
        TIM4 counter clock at 32 MHz
        hz = (TIM2 counter clock/psc) / arr
//---------------------------------------------------------------   
* @返回: 
* @作者:   hjj (2013-09-11)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void TIM4_Int_Init(uint16 arr,uint16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);        //时钟使能
    
    //定时器TIM2初始化
    TIM_TimeBaseStructure.TIM_Period = arr;                     //设置在下一个更新事件装入活动的自动重装载寄存器周期的值    
    TIM_TimeBaseStructure.TIM_Prescaler =psc;                   //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);             //根据指定的参数初始化TIMx的时间基数单位
 
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE );                  //使能指定的TIM4中断,允许更新中断

    //中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;             //TIM4中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //从优先级5级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);                             //初始化NVIC寄存器

    TIM_Cmd(TIM4, ENABLE);                                     //使能TIMx                   
}

/*=========================================================================================\n
* @function_name: TIM6_Int_Init
* @function_file: McuDrive.c
* @描述: 定时器6初始化
* @参数: 
* @param:arr  更新值
* @param:psc  时钟值
//---------------------------------------------------------------
        TIM4 Configuration: Output Compare Timing Mode:
        TIM4 counter clock at 32 MHz
        hz = (TIM2 counter clock/psc) / arr
//---------------------------------------------------------------   
* @返回: 
* @作者:   hjj (2013-09-11)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
/*void TIM6_Int_Init(uint16 arr,uint16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);        //时钟使能
    
    //定时器TIM6初始化
    TIM_TimeBaseStructure.TIM_Period = arr;                     //设置在下一个更新事件装入活动的自动重装载寄存器周期的值    
    TIM_TimeBaseStructure.TIM_Prescaler =psc;                   //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);             //根据指定的参数初始化TIMx的时间基数单位
 
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE );                  //使能指定的TIM4中断,允许更新中断

    //中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;             //TIM4中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;          //从优先级5级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);                             //初始化NVIC寄存器

    TIM_Cmd(TIM6, ENABLE);                                     //使能TIMx                   
}
*/

/*=========================================================================================\n
* @function_name: ChangeBodeProc
* @function_file: McuDrive.c
* @描述: 修改波特率处理
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-31)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
    ComBom_Init(ComIndex_Uart3);    //将uart4 端口对应的 COM初始化
    ComBom_Init(ComIndex_Uart2);    //将uart4 端口对应的 COM初始化
}
/*=========================================================================================\n
* @function_name: SysUart_Init
* @function_file: McuDrive.c
* @描述: 初始化串口
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-04)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void SysUart_Init(void)
{
    ChangeBodeProc();
}
/*=========================================================================================\n
* @function_name: TZ_GPIOCtrl
* @function_file: McuDrive.c
* @描述: 
* 
* 
* @参数: 
* @param:State  
* @返回: 
* @作者:   lwb (2013-07-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
* @描述: 背光控制
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-07)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
            if(gut_SysWarn.byte==0                      //没有告警字
//               &&gs_LoadSwichStatus.ControlWarning==0   //没有报警命令
               &&flag==0)            
            {
                CloseBeiGuang();//关闭背光
            }
        }
        else
        {
            OpenBeiGuang();
        }
    }
    else
    {
        if(gut_SysWarn.byte==0                      //没有告警字
//           &&gs_LoadSwichStatus.ControlWarning==0   //没有报警命令
           &&flag==0
           )  
        {
            CloseBeiGuang();                        //关闭背光
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

        if(gut_SysWarn.byte>0)              //有错误字，开启背光和告警等
        {
            OpenBeiGuang();
        }

    }
}

/*=========================================================================================\n
* @function_name: SwichPluseOutType
* @function_file: McuDrive.c
* @描述: 切换端子功能
* 
* 
* @参数: 
* @param:type  0-RTC  2-时段投切
* @返回: 
* @作者:   lwb (2012-06-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void SwichPluseOutType(uint8 type)
{
    if(type==0)
    {
//        P9FC|=BIT3;                                                     //输出RTC
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
* @描述: RTC配置
* 
* @参数: 
* @返回: 
* @作者:  hjj (2013-08-05)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
* @描述: RTC硬时钟获取
* 
* @参数: 
* @返回: 
* @作者:  hjj (2013-08-05)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
* @描述: RTC硬时钟设置
* 
* @参数: 
* @返回: 
* @作者:  hjj (2013-08-05)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void SetExtRTC(void)           //设置硬时钟
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
* @描述: ADC配置
* 
* @参数: 
* @返回: 
* @作者:  hjj (2013-08-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
        ADC_SoftwareStartConv(ADC1);      //使能指定的ADC1的软件转换启动功能
        while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
        return ADC_GetConversionValue(ADC1);    //返回最近一次ADC1规则组的转换结果
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
* @描述: 进入低功耗前IO口，外设处理，最后进入STOP模式
*        通过ADC进入
* 
* @参数: 
* @param:type 
* @返回: 
* @作者:   hjj (2013-07-31)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
* @描述: 进入低功耗前IO口，外设处理，最后进入STOP模式
*        通过Key唤醒定时
* 
* @参数: 
* @param:type 
* @返回: 
* @作者:   hjj (2013-07-31)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
* @描述: key唤醒，通过定时在进入睡眠，不需要TIM2定时检测ADC，同时开比较器，用以上电唤醒
* 
* @参数: 
* @param:type 
* @返回: 
* @作者:   hjj (2013-07-31)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void MCUForWakeUp_Key(void)
{
    __disable_interrupt();  //关闭总中断
    //SystemInit();           //配置系统主时钟
    //SetSysOscClock();       //配置系统OSC时钟 
    // RCC_HSICmd(ENABLE);   //内部高速时钟
    //初始化NVIC中断系统
    //设置中断向量表的起始地址为0x08000000
    //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    // Init_Var();
    // DataSecurityInit();
    //SysIO_Init();               //初始化系统IO
    BE_I2C_Init();
    ShowInitial();
    //Init_RacCtrl();
    //Sys_InitExit();
    //RxReset_V9281();            //复位V9281
    //SysUart_Init();
    //SysLcd_Init();
    //TIM3_Int_Init(99,799);      //10Khz的计数频率，计数到100为10ms  
    //TIM2_Int_Init(99,79);      //100Khz的计数频率，计数到100为1ms  
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
    gui_SystemEvent |= flgEtGlb_PowDown;// 设置下电事件标记
    guc_FromUpToSleepFlg = PowerOff_Rtc;
    IWDG_ReloadCounter();                         //喂狗
    //Disp_Full();
    __enable_interrupt();    
}

/*=========================================================================================\n
* @function_name: MCUForWakeUp_Power
* @function_file: McuDrive.c
* @描述: 上电唤醒 
* 
* 
* @参数: 
* @param:type 
* @返回: 
* @作者:   hjj (2013-07-31)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void MCUForWakeUp_Power(void)
{
//    __disable_interrupt();  //关闭总中断
//    SystemInit();           //配置系统主时钟
//    SetSysOscClock();       //配置系统OSC时钟 
//    RCC_HSICmd(ENABLE);   //内部高速时钟
//    //初始化NVIC中断系统
//    //设置中断向量表的起始地址为0x08000000
//    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//    Init_Var();
//    DataSecurityInit();
//    SysIO_Init();               //初始化系统IO
//    ShowInitial();
//    IWDG_ReloadCounter();                         //喂狗
//    Init_RacCtrl();
//    Sys_InitExit();
//    KeyIrqCmd(DISABLE);
//    RxReset_V9281();            //复位V9281
//    SysUart_Init();
//    SysLcd_Init();
//    TIM3_Int_Init(99,1599);      //10Khz的计数频率，计数到100为10ms  
//    TIM2_Int_Init(99,159);       //100Khz的计数频率，计数到100为1ms  
//    ADC_Configuration();
//    //RTC_Configuration();        //hjj add 
//     RCC_RTCCLKCmd(ENABLE);
//    // Wait for RTC APB registers synchronisation 
//    RTC_WaitForSynchro();  
//    RtcIrqCmd(DISABLE,0);
//    Disp_Full();
//    __enable_interrupt();  
//    IWDG_ReloadCounter();                         //喂狗
//     EnyB_SetMeterRule();
//    TIM4_Int_Init(199,15);      //1Mhz的计数频率，计数到120为120us 
}

/*=========================================================================================\n
* @function_name: MCUForWakeUp_Power
* @function_file: McuDrive.c
* @描述:  实时时钟唤醒
* 
* 
* @参数: 
* @param:type 
* @返回: 
* @作者:   hjj (2013-08-05)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void MCUForWakeUp_RTC(void)
{
    __disable_interrupt();      //关闭总中断
    //SystemInit();             //配置系统主时钟
    //SetSysOscClock();         //配置系统OSC时钟 
    //RCC_HSICmd(ENABLE);       //内部高速时钟
    //初始化NVIC中断系统
    //设置中断向量表的起始地址为0x08000000
    //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    //Init_Var();
    //DataSecurityInit();
    //SysIO_Init();               //初始化系统IO
    
    //Sys_InitExit();
    //SysLcd_Init();
    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    //PWR_RTCAccessCmd(ENABLE);
    //LCD时钟源选择 LSE  （LCD和RTC同一个时钟源）
    //RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    //TIM3_Int_Init(99,799);      //10Khz的计数频率，计数到100为10ms  
    // COMP_Configuration();
    if(guc_RtcNorm == SET)
    {
        IWDG_ReloadCounter(); 

        RtcIrqCmd(ENABLE,(0xfff));  //每隔2S唤醒喂狗
        IWDG_ReloadCounter(); 
        guc_RtcNorm = SET;
        gui_SystemEvent |= flgEtGlb_PowDown;              // 设置下电事件标记
        guc_FromUpToSleepFlg = PowerOff_Adc;
    }
    else
    {
        //SystemInit(); 
        IWDG_ReloadCounter();                         //喂狗
        BE_I2C_Init();
        IWDG_ReloadCounter();                         //喂狗
        ShowInitial();
        if(guc_WakeUpCnt_2second == true)                      //键显
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
                IWDG_ReloadCounter();                         //喂狗
                RtcIrqCmd(ENABLE,(0x7ff));  
                gui_SystemEvent |= flgEtGlb_PowDown;// 设置下电事件标记
                guc_FromUpToSleepFlg = PowerOff_Rtc;
            }
            else
            {
                guc_SleepFlg = false;
                guc_Intkeycnt = 0;
                guc_IntRtccnt = 0;
                guc_WakeUpCnt_2second = false;
                IWDG_ReloadCounter();                         //喂狗
                RtcIrqCmd(ENABLE,(0xfff));
                guc_RtcNorm = SET;
                gui_SystemEvent |= flgEtGlb_PowDown;// 设置下电事件标记
                guc_FromUpToSleepFlg = PowerOff_Adc;
            }
        }
        else                                                                         //轮显
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
                IWDG_ReloadCounter();                         //喂狗
                RtcIrqCmd(ENABLE,(0x7ff));
                gui_SystemEvent |= flgEtGlb_PowDown;// 设置下电事件标记
                guc_FromUpToSleepFlg = PowerOff_Rtc;
            }
            else
            {
                guc_Intkeycnt = 0;
                guc_IntRtccnt = 0;
                guc_SleepFlg = false;
                IWDG_ReloadCounter();                         //喂狗
                RtcIrqCmd(ENABLE,(0xfff));
                guc_RtcNorm = SET;
                gui_SystemEvent |= flgEtGlb_PowDown;// 设置下电事件标记
                guc_FromUpToSleepFlg = PowerOff_Adc;
            }
        }
    } 
     IWDG_ReloadCounter();                         //喂狗
    __enable_interrupt();
}


/*=========================================================================================\n
* @function_name: EXTI0_IRQHandler()
* @function_file: McuDrive.c
* @描述: 按键中断
*        
* 
* @参数: NewState: ENABLE,DISABLE
* @param:type 
* @返回: 
* @作者:   hjj (2013-08-01)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
* @描述: 按键中断的开启与关闭
* 
* 
* @参数: NewState: ENABLE,DISABLE
* @param:type 
* @返回: 
* @作者:   hjj (2013-08-01)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
* @描述: RTC秒中断的开启与关闭
* 
* 
* @参数: NewState: ENABLE,DISABLE uint32 cnt:秒中断秒数，最高32S
* @param:type 
* @返回: 
* @作者:   hjj (2013-08-05)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
* @描述: 唤醒上电比较检测口配置
* 
* 
* @参数: 
* @param:type 
* @返回: 
* @作者:   hjj (2013-08-01)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
* @描述: compare wakeup 
* 
* @参数: 
* @param:type 
* @返回: 
* @作者:   hjj (2013-08-05)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
 * @描述: RTC wakeup 
 * 
 * @参数: 
 * @param:type 
 * @返回: 
 * @作者:   hjj (2013-08-05)
 * @备注: 
 *-------------------------------------------------------------------------------------------
 * @修改人:  
 * @修改内容: 
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
//hjj add 2013.8.3 唤醒函数数组，关联下标项是guc_FromSleepToUpFlg
const  MCUForWakeUp  MCUWake[] =
{
    MCUForWakeUp_Key,
    MCUForWakeUp_Power,
    MCUForWakeUp_RTC,
};
//hjj add 2013.8.6 掉电函数数组，关联下标项是guc_FromUpToSleepFlg
const  MCUForWakeUp  MCUPowerOff[] =
{
    MCUForPowerOff_Key,
    MCUForPowerOff_Adc,
    MCUForPowerOff_Rtc,
};


/*=========================================================================================\n
* @function_name: IWDGconfig(void)
* @function_file: McuDrive.c
* @描述: IWDGconfig
* 
* @参数: 
* @param:type 
* @返回: 
* @作者:   hjj (2013-09-22)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
      IWDG_SetReload(0x2BC);      // 看门狗约2秒 32000/256 =125 (1/125)*375 = 3s
     
      // Reload IWDG counter 
      IWDG_ReloadCounter();
     
      //Enable IWDG (the LSI oscillator will be enabled by hardware) 
      IWDG_Enable();
}


/*=========================================================================================\n
* @function_name: DMA1_Channel2_IRQHandler
* @function_file: McuDrive.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   Vango Application Teams
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
 const uint8 Addr[6][6] = {{0,2,4,6,8,10},{10,0,2,4,6,8},{8,10,0,2,4,6}, //单点数据，各个通道所在位置
                                          {6,8,10,0,2,4},{4,6,8,10,0,2},{2,4,6,8,10,0}};                                           
void DMA1_Channel2_IRQHandler(void)  // DMA1 Channel 2
{
    /* DMA2 channel1 used by SPI3 RX */
  //uint16 i=0;//j=0,k=0;  
  //uint16 a=0,b=0,c=0,d=0,e=0,f=0;
  //uint8 addr;
  //spi_wr_flash(0xC01A,0x0000,0x0000);	//DMA关UA
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
          gui_RefreshEvent |= flgEtpara_CalcZB;       //计算存储周波事件
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
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   Vango Application Teams
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
 * @描述: 
 * 
 * @参数: 
 * @返回: 
 * @作者:   Vango Application Teams
 * @备注: 
 *-------------------------------------------------------------------------------------------
 * @修改人:  
 * @修改内容: 
 ===========================================================================================*/

 void EXTI2_IRQHandler(void)        // EXTI Line 2 CF2
 {
     if(EXTI_GetITStatus(EXTI_Line2) != RESET)
     {    
         EnyB_IntCF(reactive);
         EXTI_ClearITPendingBit(EXTI_Line2);
     }
  }

