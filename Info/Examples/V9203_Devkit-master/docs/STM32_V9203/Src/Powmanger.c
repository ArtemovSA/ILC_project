
#define POWMANGE_EXT

#include "Includes.h"

uint8 guc_ChkPowDn;                 // 下电检测寄存器
/*=========================================================================================\n
* @function_name: PowDownChk
* @function_file: Powmanger.c
* @描述:
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-23)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void PowDownChk(void)
{
    uint32 guc_adc;
    guc_ChkPowDn <<= 1;
    guc_adc = ADC_ConvertedValue;
    //guc_adc = 700;  //TEST
    if(guc_adc <= POWERDOWNVoltage)                      // 判断是否电压低
    {
        guc_ChkPowDn |=0x1;                              // 把当前电压状态存入寄存器，只有连续有低电平才可以判定是下电
    }
    
    if(0xff == guc_ChkPowDn)                             // 有连续的8次，也就是8ms低电平，设置下电事件
    {
        RtcIrqCmd(ENABLE,(0xfff));  //每隔2S唤醒喂狗
        guc_sleepflag = SET;
        guc_RtcNorm = SET;
        gui_SystemEvent |= flgEtGlb_PowDown;              // 设置下电事件标记
        guc_FromUpToSleepFlg = PowerOff_Adc;
        TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
        guc_ChkPowDn =0x0;
    }
}
/*=========================================================================================\n
* @function_name: PowerDnPro
* @function_file: System.c
* @描述: 掉电处理函数
*
* @参数:
* @返回:
* @作者:   lwb (2012-01-31)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人: hjj(2013-08-2)
* @修改内容:增加L152的唤醒
===========================================================================================*/
uint8 PowerDnPro(void)
{
    //Word32  ul_PowerDownRMU;
    //  uint16 uiPowOffShowTm;
    // uint16 CRC1;
    //__disable_interrupt();  //关闭总中断
    /*hjj add 2013.8.3*/
    uint8 i;
    KeyIrqCmd(ENABLE);
    IWDG_ReloadCounter();  
    COMP_Configuration();                                                   //掉电处理后无ADC,这样第一次断电后才能上电唤醒
    for(i = 0; i < 3; i++ )
    {
        //gs_UI.ul_I[i] =0;
        //gs_UI.ul_U[i] = 0;
        //gs_UI.ul_P[i] = 0;
        //gs_UI.ul_Hz[i] = 0;     
    }
   // for(i = 0; i < 4; i++ )
    //{
    //   gs_power.ul_power[i] = 0;
    //   gs_power.ul_Npower[i] = 0;
   // }
    MCUPowerOff[guc_FromUpToSleepFlg]();
    MCUWake[guc_FromSleepToUpFlg]();
    //ul_PowerDownRMU.lword = EnyB_ReadMeterParaACK(RMSIU);            //电源异常时读取电压值
    
    //保存电量处理
    
    //Eny_EnergyActiveSave();
    //  memcpy(XDATA_RAMZONE,   (uint8*)&gs_EnergyData.uiEPZ,   22);        //当前电量增量
    //  memcpy(XDATA_RAMZONE+22,(uint8*)&gs_FeeData.uc_FeeNo,   1);         //当前费率
    //  memcpy(XDATA_RAMZONE+23,(uint8*)&gs_DateTime.ucSecond,  6);         //当前时间
    //  memcpy(XDATA_RAMZONE+29,(uint8*)&gs_PowerCf.uc_Pz,  10);            //当前CF数
    //  memcpy(XDATA_RAMZONE+39,(uint8*)&gs_Energy.ucPz,  10);              //当前CF对于的能量
    //  memset(XDATA_RAMZONE+49,0x00,          2);                          //写入编程剩余时间
    // memcpy(XDATA_RAMZONE+51,(uint8*)&gui_DyProDmOut,        2);         //写入厂内剩余时间
    //  memcpy(XDATA_RAMZONE+53,(uint8*)&gs_PassWordCtr.uiDyPW2Out,4);      //写入闭锁剩余时间
    //  memcpy(XDATA_RAMZONE+57,(uint8*)&gul_PowerTm,4);                    //停电时间  
    //  CRC1=do_CRC(XDATA_RAMZONE, 61);
    //  memcpy(XDATA_RAMZONE+61,(uint8*)&CRC1,2);
    //  BE_WriteP(EEP_POWDWENERGY,(uint8*)XDATA_RAMZONE,63);
    
    // memcpy(XDATA_RAMZONE,   (uint8*)&gs_ReportState,   10);        //主动上报状态字1
    //  memcpy(XDATA_RAMZONE+10,   (uint8*)&gs_ReportStateFlg,10);        //当前电量增量
    //  gui_ReportCRC=do_CRC(XDATA_RAMZONE, 20);
    //  if(JudgePowerDnValue(ul_PowerDownRMU.lword))
    //  {
    //        if(POWERSTATE())
    //        {
    //            gui_SystemState |= flgStSys_PowErr;  
    //            gs_powerErr=0x5A5A;
    //        }
    //  }
    //gui_SystemState |= flgStSys_PowOff;                                 //掉电状态
    
    //guc_PowOffRuning=false;
    //LCD_Off();
    
    //    if(guc_CoverStatus==1)                      //掉电开表盖的话，分钟唤醒，否则小时唤醒。
    //    {
    //        RTCWakeUpTm(InitPara38[1]);             //1Min唤醒
    //        
    //    }else
    //    {
    //        RTCWakeUpTm(RTC_HOUR);                  //1hour唤醒
    //    }
    
    //处理完写E2操作后，在处理E2端口
    //BE_I2C_SDA_1();
    // BE_I2C_CLK_1();                     //I2C输出高
    
    
    //    if(POWERSTATE())
    //    {
    //        if(Sleep())
    //        {
    //        }
    //        else
    //        {
    //            return false;
    //        }
    //    }
    return false;
}
/*=========================================================================================\n
* @function_name: JudgePowerDnValue
* @function_file: Powmanger.c
* @描述: 判断下电时读取的电压瞬时值
* 
* 
* @参数: 
* @param:RMSUValue  
* 
* @返回: 
* @return: uint8 
* @作者:   xuqf (2013-1-30)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint8 JudgePowerDnValue(uint32 RMSUValue)
{
//    uint32 TempPara;
//    TempPara=gs_JbPm.ul_URmG/65536;   //ReadRMSPara(1);
//    if(TempPara==0)
//    {    
//      return 0;
//    }
//    else
//    {    
//       RMSUValue=RMSUValue/TempPara;
//    }
//    
//    if(RMSUValue>CRIULEVEL1&&RMSUValue<CRIULEVEL2)        //如果电压值大于0.8U=176V或者小于1.15U=253V
//    {   
//        return true;
//    }else
//    {
//        return 0;
//    }
    return 0;
}



