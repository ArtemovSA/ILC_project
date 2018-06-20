#ifndef __MCUDRIVE_H__
    #define __MCUDRIVE_H__

//#define SYSCLK_FREQ_32MHz  32000000
#define NVIC_VectTab_FLASH           ((uint32_t)0x08000000)
#define ADC1_DR_ADDRESS    ((uint32_t)0x40012458)

typedef void (*MCUForWakeUp)(void); //hjj add 2013.8.3
extern const  MCUForWakeUp  MCUWake[];//hjj add 2013.8.3
extern const  MCUForWakeUp  MCUPowerOff[]; //hjj add 2013.8.5

enum                                                              //hjj add 2013.8.3
{
    WakeUp_Key =0,
    WakeUp_Power ,
    WakeUp_Rtc,
};
enum                                                              //hjj add 2013.8.5
{
    PowerOff_Key =0,
    PowerOff_Adc,
    PowerOff_Rtc,
};

//按键
#define KeyPortIn()    {   GPIO_InitTypeDef GPIO_InitStructure;\
                            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN; \
                            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;\
                            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;\
                            GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;\
                            GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12; \
                            GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13; \
                            GPIO_Init(GPIOA, &GPIO_InitStructure);\
                            }        //


#define KeyStUp()       (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12))                // 上翻按键

#define KeyPrEnable()   {}//{P9IE|=BIT1;P9OE|=BIT1;P9FC&=(~BIT1);}
#define KeyPrDisable()  {}//{P9IE&=(~BIT1);P9OE|=BIT1;}
#define KeyStPr()       (!(GPIOA->IDR  & GPIO_Pin_11))               // 编程按键

#define KeyBgEnable()    {}
#define KeyBgDisable()    {}
#define KeyStBg()       (1)                           //表盖检测按键


//背光控制
#define OpenBeiGuang()      //{GPIO_SetBits(GPIOH, GPIO_Pin_2);}
#define CloseBeiGuang()    // {GPIO_ResetBits(GPIOH, GPIO_Pin_2);}


//跳闸灯
#define OpenSwitchLight() //{GPIO_ResetBits(GPIOA, GPIO_Pin_12);}
#define CloseSwitchLight()// {GPIO_SetBits(GPIOA, GPIO_Pin_12);}

//继电器检测
#define LOOPOpen()  (1)           //回路打开
#define LOOPOff()   (0)          //回路关闭



//#define EXTERNAL
#ifdef EXTERNAL

//合闸
#define SwitchOFF()  {;}
//拉闸
#define SwitchOn()   {;}
//拉合闸IDLE
#define SwitchIdle()  

#else

//合闸
#define SwitchOFF()  {}
//拉闸
#define SwitchOn()   {;}
//拉合闸IDLE
#define SwitchIdle()  {;}

#endif


void GetExtRTC(void);           //获取硬时钟
void SetExtRTC(void);           //设置硬时钟
void SystemInit(void);
void SetSysOscClock(void);
void SysIO_Init(void);
void TZ_GPIOCtrl(bool State);
void BgCtrFun(void);
void Lcd_Init(void);
void SysUart_Init(void);
void TIM3_Int_Init(uint16 arr,uint16 psc);
void TIM2_Int_Init(uint16 arr,uint16 psc);
void TIM4_Int_Init(uint16 arr,uint16 psc);
void TIM6_Int_Init(uint16 arr,uint16 psc);
void SwichPluseOutType(uint8 type);
void ChangeBodeProc(void);
void Sys_InitExit(void);
void ADC_Configuration(void);//hjj add 2013.7.26
void RTC_Configuration(void);//hjj add 2013.8.5
void COMP_Configuration(void); //hjj add 2013.8.2


void MCUForPowerOff_Adc(void);//hjj add 2013.8.5
void MCUForPowerOff_Key(void);//hjj add 2013.8.6
void MCUForPowerOff_Rtc(void);//hjj add 2013.8.6

void MCUForWakeUp_Key(void);  //hjj add 2013.8.2
void MCUForWakeUp_Power(void); //hjj add 2013.8.2
void MCUForWakeUp_RTC(void); //hjj add 2013.8.3

void KeyIrqCmd(FunctionalState NewState);//hjj add 
void RtcIrqCmd(FunctionalState NewState,uint32 cnt);//hjj add
void IWDGconfig(void);
uint16 T_Get_Adc(void) ;
uint16 T_Get_Temp(void);






#endif
