#ifndef  __INCLUDES_H__
    #define __INCLUDES_H__

#ifndef VAREXT
    #define VAREXT extern
#endif


/* Includes ------------------------------------------------------------------*/
/* Uncomment the line below to enable peripheral header file inclusion */
#include "TypeDef.h"
#include "string.h"
#include <math.h>
#include  <stdlib.h>
#include "stm32l1xx.h"
#include "stm32l1xx_it.h"
#include "stm32l1xx_adc.h"
//#include "stm32l1xx_crc.h"
#include "stm32l1xx_comp.h"
#include "stm32l1xx_dac.h"
//#include "stm32l1xx_dbgmcu.h"
#include "stm32l1xx_dma.h"
#include "stm32l1xx_exti.h"
#include "stm32l1xx_flash.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_syscfg.h"
//#include "stm32l1xx_i2c.h"
#include "stm32l1xx_iwdg.h"
#include "stm32l1xx_lcd.h"
#include "stm32l1xx_pwr.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_rtc.h"
 #include "stm32l1xx_spi.h"
#include "stm32l1xx_tim.h"
#include "stm32l1xx_usart.h"
//#include "stm32l1xx_wwdg.h"
#include "misc.h"
#include "McuDrive.h"
#include "LcdBottom.h"
#include "BEBottom.h"
#include "ComConfig.h"
#include "ComBottom.h"
#include "Uart2.h"
#include "Uart3.h"
#include "Bronco.h"
#include "Spi.h"
#include "ComApplication.h"
#include "Prcfrm.h"
#include "ComCoretab.h"
#include "PrologFee.h"
#include "E2AddTab.h"
#include "Api.h"
#include "DataProtect.h"
#include "Timeproc.h"
#include "Dataproc.h"
#include "DLT_645_2007.h"
#include "Key.h"
#include "LcdMiddle.h"
#include "Lcdshow.h"
#include "FunCfg.h"
//#include "Multariff.h"
#include "Event.h"
//#include "LoadCurve.h"
#include "Version.h"
#include "EnergyBottom.h"
#include "Energy.h"
#include "InitPara.h"
#include "RtcCpt.h"
#include "Powmanger.h"



#define  CLRWDT()   {IWDG_ReloadCounter(); }                        //喂狗

VAREXT uint16 gui_SystemEvent;
#define flgEtGlb_PowDown        0x01
#define flgEtGlb_NEnergyCalc   0x02
#define flgEtGlb_EnergyCalc     0x04
#define flgEtGlb_RefreshLcd     0x08
#define flgEtGlb_NeedPrtclChk   0x10
#define flgEtGlb_ClearEv        0x20
#define flgEtGlb_ClearMeter     0x40
#define flgEtGlb_ReadBronco     0x80  //读电量时间


VAREXT uint8 guc_TimeEvent;
#define flgEtTim_Second   0x01
#define flgEtTim_Minute   0x02
#define flgEtTim_Hour     0x04
#define flgEtTim_SecSoft  0x08
#define flgEtTim_Day      0x10


VAREXT uint8 guc_KeyEvent;
#define flgEtKey_Up             0x01
#define flgEtKey_Pr             0x02


VAREXT uint16 gui_RefreshEvent;
#define flgEtPara_Fee           0x0001
#define flgEtPara_EnyBottom     0x0002
#define flgEtPara_Config        0x0004
#define flgEtPara_RTC           0x0008
#define flgEtPara_Bode          0x0010
#define flgEtPara_Show          0x0020
#define flgEtPara_Init          0x0040
#define flgEtPara_RtcParaFsToE2 0x0080
#define flgEtPara_LoadCurve     0x0100
#define flgEtPara_PowDir        0x0200
#define flgEtPara_FrsJsr        0x0400
#define flgEtpara_CalcZB        0x0800
//#define flgEtpara_CalcFFT       0x1000
//#define flgEtpara_EsamKeyUpdata 0x1000

VAREXT uint8 guc_DjCheckFlg;
#define flgDsDjCheck        0x01
#define flgZdDjCheck        0x02
#define flgDayDjCheck       0x04
#define flgInsDjEvent       0x08

VAREXT uint8  guc_SleepFlg;
VAREXT uint8  guc_PowOffRuning;
VAREXT uint16 gui_WakeUpCnt;
VAREXT uint8  guc_WakeUpCnt_2second;    //hjj add
VAREXT uint8  guc_SecCnt;
VAREXT uint8  guc_PwOnCnt;
VAREXT uint8  guc_DMACnt;
VAREXT uint8  guc_FromSleepToUpFlg;     //hjj add
VAREXT uint8  guc_WakeUpRtcCnt;         //hjj add
VAREXT uint8  guc_FromUpToSleepFlg;     //hjj add
VAREXT uint16  guc_Intkeycnt;            //hjj add
VAREXT uint16  guc_IntRtccnt;            //hjj add
VAREXT uint8  guc_RtcNorm;              //RTC唤醒标志，1；正常唤醒，0: 按键唤醒后的RTC唤醒
VAREXT uint8  guc_sleepflag;
VAREXT uint32 bronco_test;

//VAREXT uint8 Send_Buffer[4];

typedef struct
{
    uint32  ul_Energy;        //有功总电量 
    uint32  ul_NEnergy;     //无功总电量
    uint16  ul_U[3];         //电压有效值
    uint16  ul_I[3];         //电流有效值
    uint16  ul_Power[3];       //有功功率
    uint16  ul_Npower[3];       //无功功率
    uint8   ulSecond;       //秒
    uint8   ulMinute;       //分
    uint8   ulHour;         //时
    uint8   ulDay;          //日
}S_UIP;

VAREXT S_UIP gs_UIP; 

//typedef struct
//{
//    int16 Raw_UA[640];         
//    int16 Raw_IA[640];             
//    int16 Raw_UB[640];        
//    int16 Raw_IB[640];                         
//    int16 Raw_UC[640]; 
//    int16 Raw_IC[640];    
//}S_RAW;
//VAREXT S_RAW  gs_RawData;//gs_RawDatadisp;    


VAREXT uint16 gui_SystemState;
#define flgStSys_Program  0x01
#define flgStSys_IsTodayJS 0x02
#define flgStSys_PowOffing 0x04
#define flgStSys_PowOff 0x08
#define flgStSys_PowErr 0x10


#define POWERDOWNVoltage 800 //this value depend on ADC (3.3*(1100/4096))


#define  _IsSecuProduct  (gui_SystemState&flgStSys_Program)
#define _SYS_IS_PWR_OFF  (gui_SystemState&flgStSys_PowOff)



void Init_Var(void);



//Exported macro
#ifdef  USE_FULL_ASSERT
//*
//* @brief  The assert_param macro is used for function's parameters check.
//* @param  expr: If expr is false, it calls assert_failed function which reports
//*         the name of the source file and the source line number of the call
//*         that failed. If expr is true, it returns no value.
//* @retval None
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
//Exported functions
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif //USE_FULL_ASSERT

#endif /* __STM32L1xx_CONF_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

