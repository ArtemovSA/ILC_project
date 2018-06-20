#define __MAIN__
#define VAREXT
#include "Includes.h"


/*=========================================================================================\n
* @function_name: Init_Var
* @function_file: main.c
* @����: ��ʼ��ϵͳȫ�ֱ����Ͳ���
*
* @����:
* @����:
* @����:
* @��ע:
*-------------------------------------------------------------------------------------------
* @�޸���:  Lwb (2011-8-9)
* @�޸�����:
===========================================================================================*/
void Init_Var(void)
{
 //    uint16 i;
//    guc_1msCnt=0;
    gui_SystemEvent=0;
    guc_TimeEvent=0;
    gui_RefreshEvent=0;
    guc_KeyEvent=0;
    guc_DjCheckFlg=0;
//    guc_CheckBatFlg=false;
    guc_RtcConflict=0;
    guc_BodeDely=0;
//    guc_SwichPluse=0;
//    guc_LongKeyFlg=0;
//    guc_PluseCount=0;
//    guc_LoopChk1Cnt=0;
//    guc_LoopChk2Cnt=0;
   // guc_SleepFlg=true;
    guc_CfOpenFlg=true;
//    guc_OpenCfTmCnt=0;
    guc_CoverKeyDwCnt=0;
//    guc_ShowStCom=0;
    gs_SysRunSt.PluseType=0;
    gut_SysWarn.byte=0;
    gs_MeterState.ui_MeterState1.Word.word=0;
    gs_MeterState.ui_MeterState2.Word.word=0;
    gs_MeterState.ui_MeterState3.Word.word&=0x0021;
    gs_MeterState.ui_MeterState4.Word.word=0;
    gs_MeterState.ui_MeterState5.Word.word=0;
    gs_MeterState.ui_MeterState6.Word.word=0;
    gs_MeterState.ui_MeterState7.Word.word=0;
    guc_ComSendDy=0;
//    guc_FrsCalRTCDelay=5;
//    gui_SystemSleepState=0;
    guc_SecCnt= 0;
    guc_PrOutTm= 0;
//    guc_Timechange=0x55;
//    memcpy(XDATA_RAMZONE,   (uint8*)&gs_ReportState,   10);        //�����ϱ�״̬��1
//    memcpy(XDATA_RAMZONE+10,   (uint8*)&gs_ReportStateFlg,10);        //��ǰ��������
//    if(gui_ReportCRC != do_CRC(XDATA_RAMZONE, 20))
//    {
//        memset((uint8*)&gs_ReportState,0x00,sizeof(REPORTSTATE));                   //�����ϱ�״̬����
//        memset((uint8*)&gs_ReportStateFlg,0x00,sizeof(REPORTSTATE));                //�����ϱ�״̬��־����
//    }
//    
    guc_SleepFlg=false;
    guc_Intkeycnt = 0;
    guc_WakeUpCnt_2second = false;
    guc_sleepflag = RESET;
    guc_PwOnCnt = 0;
    guc_DMACnt = 0;
//    guc_FFT_Flg = false;
    guc_DODMAFlg = 0;
    //bronco_test = 6;
    //gs_UIP.ul_Npower = 0;
    //gs_UIP.ul_power = 0;
    Flg_FirstPowerUp = true;
    memset((uint8*)gs_DMACheck.ucOffset,0x00,sizeof(S_DMACHK));
}
/*=========================================================================================\n
* @function_name: SysEventProc
* @function_file: Main.c
* @����: 
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-12)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void SysEventProc(void)
{
    IWDG_ReloadCounter();  

    if(gui_SystemEvent & flgEtGlb_PowDown)              
    {                                                   
        //PowerDnPro();                  //���紦��
     gui_SystemEvent &=~ flgEtGlb_PowDown;           
     //gui_SystemSleepState=0x55aa;
    }                                                   
    else if(gui_SystemEvent & flgEtGlb_EnergyCalc)      
    {                                                   
        {
             Eny_EnergyProc(active);                               // �����¼�
        }     
        gui_SystemEvent &=~ flgEtGlb_EnergyCalc;        
    }     
    else if(gui_SystemEvent & flgEtGlb_NEnergyCalc)      
    {                                                   
        {
             Eny_EnergyProc(reactive);                               // �����¼�
        }
       
        gui_SystemEvent &=~ flgEtGlb_NEnergyCalc;        
    }     
    else if(gui_SystemEvent & flgEtGlb_RefreshLcd)      
    {                                                   
        ShowRefresh();                                  // ��ʾˢ��
        gui_SystemEvent &=~ flgEtGlb_RefreshLcd;        
    }                                                   
    else if(gui_SystemEvent & flgEtGlb_NeedPrtclChk)    
    {                                                   
        {
              ComApp_Proc();                                  // ͨѶ�¼�
        }
      
        gui_SystemEvent &=~flgEtGlb_NeedPrtclChk;       
    }
     else if( gui_SystemEvent&flgEtGlb_ClearEv)         //�����¼��¼�
    {

        {
             Data_ClearEvent();
        }         
        gui_SystemEvent&=~flgEtGlb_ClearEv;
    }
    else if(gui_SystemEvent&flgEtGlb_ClearMeter)
    {
  
        {
            Data_ClearMeter();
        }       
        gui_SystemEvent&=~flgEtGlb_ClearMeter;
    }
    else if(gui_SystemEvent&flgEtGlb_ReadBronco)
    {
        //Bronco_massdata();
        gui_SystemEvent&=~flgEtGlb_ReadBronco;
    }
    else
    {
        gui_SystemEvent = 0;                            //��ֹ����ʱ,����ѭ������Ĵ�����Զ������
    }
}
/*=========================================================================================\n
* @function_name: TimeEventProc
* @function_file: main.c
* @����: ϵͳʱ���¼�����
* 
* @����: 
* @����: 
* @����:   lwb (2012-05-09)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
//Word16 gw_Data[50];
void TimeEventProc(void)
{

    if(guc_TimeEvent & flgEtTim_Second)                 // �봦��
    {        
        //BE_ReadP(EEP_ZBUA_PT,(uint8*)gw_Data,100);
        TimerProc();                                    // ��ͣ����е��¾�����  
        EnyB_Open_Cf();
        ShowSecondProc();
        DMA_Channel_Enable();
        gi_Temprature = (((T_Get_Temp()*3250/4096)*100)/175)-273;

         //gi_Temprature = (((T_Get_Temp()*3250/4096)*100)/175)-273;
        //EnyB_Check_PowRev();                            //���ʷ����ж�
        guc_TimeEvent |= flgEtTim_SecSoft;
        guc_TimeEvent &= ~flgEtTim_Second;              //
    }
    else if(guc_TimeEvent&flgEtTim_Minute)
    {
         Data_MinProc();
         guc_TimeEvent&=~flgEtTim_Minute;
    }                                                   //
    else if(guc_TimeEvent & flgEtTim_Hour)              // Сʱ�¼�
    {                                                   //
        Data_HourProc();
        guc_TimeEvent &= ~flgEtTim_Hour;                //
    }else if(guc_TimeEvent & flgEtTim_Day)
    {
        Data_DayProc();
        guc_TimeEvent &=~flgEtTim_Day;
    }
    else if(guc_TimeEvent & flgEtTim_SecSoft)
    {
        BgCtrFun();
//      MChannelCal();                                  // ����M
        guc_TimeEvent &= ~flgEtTim_SecSoft;
    }
    else                                                //
    {                                                   //
        guc_TimeEvent = 0;                              //  
    }
}
/*=========================================================================================\n
* @function_name: main
* @function_file: Main.c
* @����: ��ں���main
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����:  
===========================================================================================*/
void main(void)
{
    __disable_interrupt();  //�ر����ж�
    SystemInit();           //����ϵͳ��ʱ�� ,�������ļ��Ѿ������˸ú���
    SetSysOscClock();       //����ϵͳOSCʱ��  
    RCC_HSICmd(ENABLE);     //�ڲ�����ʱ��
    //��ʼ��NVIC�ж�ϵͳ
    //�����ж����������ʼ��ַΪ0x08000000
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    Init_Var();
    DataSecurityInit();
    SysIO_Init();               //��ʼ��ϵͳIO
    ShowInitial();
    Sys_InitExit();
    //KeyIrqCmd(DISABLE);
    SysUart_Init();
    SysLcd_Init();
    //TIM3_Int_Init(99,799);      //10Khz�ļ���Ƶ�ʣ�������100Ϊ10ms
    TIM3_Int_Init(99,3199);  // 32Mhz�µ�����
    //TIM2_Int_Init(99,3199); 
    //TIM2_Int_Init(99,79);       //100Khz�ļ���Ƶ�ʣ�������100Ϊ1ms 
    //TIM2_Int_Init(99,159);    // 16Mhz�µ�����
    ADC_Configuration();        //hjj add 
    RTC_Configuration();        //hjj add 
    //RtcIrqCmd(DISABLE,0);
    Disp_Full();  
    Initspi2();  //����SPI
    Initspi1();   //DMA SPI
   // IWDGconfig();
    //__enable_interrupt();
    Bronco_PMCtrl(Work_normal);  //9203PM����
    JbpmInit();
    EnyB_SetMeterRule();
    __enable_interrupt();
    DMA_Configuration();   
    //WriteBronco(0x3f,0xC01A);
    SPI_Cmd(SPI1, ENABLE); 
//    for (uint8 i=0;i<3;i++)
//    {
//        BE_ReadP(SampE2SaveTab[ucTemp]+200*i,(uint8*)&E2SaveBuf[i*100].word,200);
//    }
//    for(uint16 j=0;j<300;j++)
//    {
//        E2SaveBuf1[j]=(int16)E2SaveBuf[j].word;
//    }
//    FFT_Task(E2SaveBuf1);
//    for(uint8 i=0;i<15;i++)
//    {
//        uiFFTArr[i] = fftx[(i+1)*8].real;
//    }
    while(1)
    {
        IWDG_ReloadCounter();                         //ι��
        if(0x00 != gui_SystemEvent)                     //
        {                                               //
                SysEventProc();                             // ϵͳ�¼�     
        }
        else if(0x00!= guc_TimeEvent)                   //
        {                                               //
                TimeEventProc();                            // ʱ�䴦������     
        }                                               //
        else if(0x00 != guc_KeyEvent)                   //
        {                                              
                KeyProc();                                  // ��������         
        }        
        else if(0x00 != gui_RefreshEvent)               //
        {                                               
                 RefreshPara();                              // ����ˢ��       
        }              
    }  
}



