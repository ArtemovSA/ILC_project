
#define POWMANGE_EXT

#include "Includes.h"

uint8 guc_ChkPowDn;                 // �µ���Ĵ���
/*=========================================================================================\n
* @function_name: PowDownChk
* @function_file: Powmanger.c
* @����:
*
* @����:
* @����:
* @����:   lwb (2012-03-23)
* @��ע:
*-------------------------------------------------------------------------------------------
* @�޸���:
* @�޸�����:
===========================================================================================*/
void PowDownChk(void)
{
    uint32 guc_adc;
    guc_ChkPowDn <<= 1;
    guc_adc = ADC_ConvertedValue;
    //guc_adc = 700;  //TEST
    if(guc_adc <= POWERDOWNVoltage)                      // �ж��Ƿ��ѹ��
    {
        guc_ChkPowDn |=0x1;                              // �ѵ�ǰ��ѹ״̬����Ĵ�����ֻ�������е͵�ƽ�ſ����ж����µ�
    }
    
    if(0xff == guc_ChkPowDn)                             // ��������8�Σ�Ҳ����8ms�͵�ƽ�������µ��¼�
    {
        RtcIrqCmd(ENABLE,(0xfff));  //ÿ��2S����ι��
        guc_sleepflag = SET;
        guc_RtcNorm = SET;
        gui_SystemEvent |= flgEtGlb_PowDown;              // �����µ��¼����
        guc_FromUpToSleepFlg = PowerOff_Adc;
        TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
        guc_ChkPowDn =0x0;
    }
}
/*=========================================================================================\n
* @function_name: PowerDnPro
* @function_file: System.c
* @����: ���紦����
*
* @����:
* @����:
* @����:   lwb (2012-01-31)
* @��ע:
*-------------------------------------------------------------------------------------------
* @�޸���: hjj(2013-08-2)
* @�޸�����:����L152�Ļ���
===========================================================================================*/
uint8 PowerDnPro(void)
{
    //Word32  ul_PowerDownRMU;
    //  uint16 uiPowOffShowTm;
    // uint16 CRC1;
    //__disable_interrupt();  //�ر����ж�
    /*hjj add 2013.8.3*/
    uint8 i;
    KeyIrqCmd(ENABLE);
    IWDG_ReloadCounter();  
    COMP_Configuration();                                                   //���紦�����ADC,������һ�ζϵ������ϵ绽��
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
    //ul_PowerDownRMU.lword = EnyB_ReadMeterParaACK(RMSIU);            //��Դ�쳣ʱ��ȡ��ѹֵ
    
    //�����������
    
    //Eny_EnergyActiveSave();
    //  memcpy(XDATA_RAMZONE,   (uint8*)&gs_EnergyData.uiEPZ,   22);        //��ǰ��������
    //  memcpy(XDATA_RAMZONE+22,(uint8*)&gs_FeeData.uc_FeeNo,   1);         //��ǰ����
    //  memcpy(XDATA_RAMZONE+23,(uint8*)&gs_DateTime.ucSecond,  6);         //��ǰʱ��
    //  memcpy(XDATA_RAMZONE+29,(uint8*)&gs_PowerCf.uc_Pz,  10);            //��ǰCF��
    //  memcpy(XDATA_RAMZONE+39,(uint8*)&gs_Energy.ucPz,  10);              //��ǰCF���ڵ�����
    //  memset(XDATA_RAMZONE+49,0x00,          2);                          //д����ʣ��ʱ��
    // memcpy(XDATA_RAMZONE+51,(uint8*)&gui_DyProDmOut,        2);         //д�볧��ʣ��ʱ��
    //  memcpy(XDATA_RAMZONE+53,(uint8*)&gs_PassWordCtr.uiDyPW2Out,4);      //д�����ʣ��ʱ��
    //  memcpy(XDATA_RAMZONE+57,(uint8*)&gul_PowerTm,4);                    //ͣ��ʱ��  
    //  CRC1=do_CRC(XDATA_RAMZONE, 61);
    //  memcpy(XDATA_RAMZONE+61,(uint8*)&CRC1,2);
    //  BE_WriteP(EEP_POWDWENERGY,(uint8*)XDATA_RAMZONE,63);
    
    // memcpy(XDATA_RAMZONE,   (uint8*)&gs_ReportState,   10);        //�����ϱ�״̬��1
    //  memcpy(XDATA_RAMZONE+10,   (uint8*)&gs_ReportStateFlg,10);        //��ǰ��������
    //  gui_ReportCRC=do_CRC(XDATA_RAMZONE, 20);
    //  if(JudgePowerDnValue(ul_PowerDownRMU.lword))
    //  {
    //        if(POWERSTATE())
    //        {
    //            gui_SystemState |= flgStSys_PowErr;  
    //            gs_powerErr=0x5A5A;
    //        }
    //  }
    //gui_SystemState |= flgStSys_PowOff;                                 //����״̬
    
    //guc_PowOffRuning=false;
    //LCD_Off();
    
    //    if(guc_CoverStatus==1)                      //���翪��ǵĻ������ӻ��ѣ�����Сʱ���ѡ�
    //    {
    //        RTCWakeUpTm(InitPara38[1]);             //1Min����
    //        
    //    }else
    //    {
    //        RTCWakeUpTm(RTC_HOUR);                  //1hour����
    //    }
    
    //������дE2�������ڴ���E2�˿�
    //BE_I2C_SDA_1();
    // BE_I2C_CLK_1();                     //I2C�����
    
    
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
* @����: �ж��µ�ʱ��ȡ�ĵ�ѹ˲ʱֵ
* 
* 
* @����: 
* @param:RMSUValue  
* 
* @����: 
* @return: uint8 
* @����:   xuqf (2013-1-30)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
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
//    if(RMSUValue>CRIULEVEL1&&RMSUValue<CRIULEVEL2)        //�����ѹֵ����0.8U=176V����С��1.15U=253V
//    {   
//        return true;
//    }else
//    {
//        return 0;
//    }
    return 0;
}



