#define LCDBOTTOMEXT
#include "Includes.h"
/*=========================================================================================\n
* @function_name: LCD_GPIOConfig
* @function_file: LcdBottom.c
* @描述: 配置LCD segment 和COM
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
static void LCD_GPIOConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //使能 GPIOA, GPIOB, GPIOC,GPIOE和 GPIOD AHB 时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC
                        | RCC_AHBPeriph_GPIOD|RCC_AHBPeriph_GPIOE, ENABLE);

    // Connect PA.08 to LCD COM0 
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_LCD);
    
    // Connect PA.09 to LCD COM1 
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_LCD);
    
    // Connect PA.10 to LCD COM2 
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_LCD);
    
    // Connect PB.09 to LCD COM3 
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_LCD);
/********************************NEXT SEG***************************************/
    // Connect PB.00 to LCD SEG0
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_LCD);

    // Connect PB.01 to LCD SEG1 
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_LCD);

    // Connect PB.12 to LCD SEG2 
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_LCD);

    //Connect PB.13 to LCD SEG3 
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_LCD);
    
    //Connect PB.14 to LCD SEG4
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_LCD);
    
    //Connect PB.15 to LCD SEG5
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_LCD);
    
    //Connect PD.08 to LCD SEG6
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_LCD);
    
    //Connect PD.09 to LCD SEG7
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_LCD);
    
    //Connect PD.10 to LCD SEG8
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_LCD);
    
    //Connect PD.11 to LCD SEG9
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_LCD);
    
    //Connect PD.12 to LCD SEG10
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_LCD);
    
    //Connect PD.13 to LCD SEG11
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_LCD);
    
    //Connect PD.14 to LCD SEG12
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_LCD);
    
    //Connect PD.15 to LCD SEG13
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_LCD);
    
    //Connect PC.6 to LCD SEG14
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_LCD);
    
    //Connect PC.7 to LCD SEG15
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_LCD);
    
    //Connect PC.8 to LCD SEG16
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_LCD);
    
    //Connect PC.9 to LCD SEG17
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_LCD);

   //Connect PB.3 to LCD SEG18
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_LCD);
   
   //Connect PB.4 to LCD SEG19
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_LCD);
   
   //Connect PB.5 to LCD SEG20
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_LCD);

   //Connect PB.8 to LCD SEG21
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_LCD);

   //Connect PE.0 to LCD SEG22
   GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_LCD);

    //Connect PE.1 to LCD SEG23
   GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_LCD);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3| GPIO_Pin_4| \
                                GPIO_Pin_5| GPIO_Pin_8 |GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |\
                                GPIO_Pin_15 |GPIO_Pin_9;
    
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | \
                                GPIO_Pin_8 | GPIO_Pin_9 ;
                                
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| \
                                GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14 |GPIO_Pin_15;
    
    GPIO_Init(GPIOD, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
                                                            
    GPIO_Init(GPIOE, &GPIO_InitStructure);
        

}
/*=========================================================================================\n
* @function_name: Lcd_Init
* @function_file: McuDrive.c
* @描述: 液晶初始化
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void SysLcd_Init(void)
{    
    LCD_InitTypeDef LCD_InitStructure;
    //RCC 配置
   // RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    //允许RTC访问
    //PWR_RTCAccessCmd(ENABLE);
    
    //配置LCD接口
    LCD_DeInit();
    // LCD时钟源选择 LSE  （LCD和RTC同一个时钟源）
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_LCD, ENABLE);     //使能LCD APB1 时钟
    LCD_GPIOConfig();                                       //配置LCD的GPIO端口
    LCD_InitStructure.LCD_Prescaler = LCD_Prescaler_2;
    LCD_InitStructure.LCD_Divider = LCD_Divider_20;
    LCD_InitStructure.LCD_Duty = LCD_Duty_1_4;              //8Com
    //LCD_InitStructure.LCD_Bias = LCD_Bias_1_3;
    LCD_InitStructure.LCD_Bias = LCD_Bias_1_4;
    LCD_InitStructure.LCD_VoltageSource = LCD_VoltageSource_Internal;
    LCD_Init(&LCD_InitStructure);

    LCD_MuxSegmentCmd(DISABLE);

    //High drive enable 
    LCD_HighDriveCmd(ENABLE);
  
    //Configure the Pulse On Duration
    LCD_PulseOnDurationConfig(LCD_PulseOnDuration_3);
    
    //Configure the LCD Contrast 
    LCD_ContrastConfig(LCD_Contrast_2V9);
    
    //Wait Until the LCD FCR register is synchronized
    LCD_WaitForSynchro();
    
    // Enable LCD peripheral
    LCD_Cmd(ENABLE);
    
    // Wait Until the LCD is enabled 
    while(LCD_GetFlagStatus(LCD_FLAG_ENS) == RESET)
    {
    }
    //Wait Until the LCD Booster is ready 
    while(LCD_GetFlagStatus(LCD_FLAG_RDY) == RESET)
    {
    }
}
/*=========================================================================================\n
* @function_name: Disp_Clear
* @function_file: Lcd.c
* @描述:
*
* @参数:
* @返回:
* @作者:   lwb (2012-02-27)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Disp_Clear(void)
{
    memset(guc_LCDRAM, 0,Const_LCDRAMLen);
    LCD_RAMUpdata();
}


/*=========================================================================================\n
* @function_name: Disp_Full
* @function_file: Lcd.c
* @描述: 全屏显示
*
* @参数:
* @返回:
* @作者:   lwb (2012-02-27)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Disp_Full(void)
{
    memset(guc_LCDRAM, 0xff,Const_LCDRAMLen);
    LCD_RAMUpdata();
}
/*=========================================================================================\n
* @function_name: Dsip_Clr
* @function_file: LcdBottom.c
* @描述: 显示CLR-00
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-06)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Dsip_Clr(void)
{
    memset(guc_LCDRAM, 0,Const_LCDRAMLen);
    Disp_Numb(0x00000000,0xAA723100);
    LCD_RAMUpdata();
}
/*=========================================================================================\n
* @function_name: LCD_RAMUpdata
* @function_file: LcdBottom.c
* @描述:
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-15)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void LCD_RAMUpdata(void)
{
    RefreshLCDRAM(guc_LCDRAM);
}

/*=========================================================================================\n
* @function_name: RefreshLCDRAM
* @function_file: DispPlay.c
* @描述: 把缓存显示数据刷新到LCD RAM中
*
*
* @参数：
* @param: p  缓存首地址，该缓存对应的是SEG2到SEG39，每个SEG对应1个字节
*
* @返回：
* @作者: lim (2011-11-02)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void RefreshLCDRAM(uint8 *p)
{

  // Wait Until the last LCD RAM update finish 
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
      //超时返回错误
  }

  for(uint8 i=0;i<16;i++)
  {
      LCD->RAM[i]=0;
  }

  uint32 ulTemp;
  for(uint8 i=0;i<32;i++)
  {
      ulTemp=p[i];
      if(i<24)
      {
          ulTemp<<=i;
          for(uint8 j=0;j<8;j++)
          {
              LCD->RAM[2*j]|=((ulTemp>>j)&BIT32MAP[i]);
          }
      }else
      {
          for(uint8 j=0;j<8;j++)
          {
              LCD->RAM[2*j]|=(((ulTemp>>j)&BIT0)<<(i));
          } 
      }
  }

  for(uint8 i=0;i<8;i++)
  {
      ulTemp=p[32+i];
      ulTemp<<=i;
      for(uint8 j=0;j<8;j++)
      {
          LCD->RAM[2*j+1]|=((ulTemp>>j)&BIT32MAP[i]);
      }
  }

  //  刷新LCD RAM    
  LCD_UpdateDisplayRequest();
}

