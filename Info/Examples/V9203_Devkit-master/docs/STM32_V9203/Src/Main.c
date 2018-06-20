#define __MAIN__
#define VAREXT
#include "Includes.h"


/*=========================================================================================\n
* @function_name: Init_Var
* @function_file: main.c
* @描述: 初始化系统全局变量和参数
*
* @参数:
* @返回:
* @作者:
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:  Lwb (2011-8-9)
* @修改内容:
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
//    memcpy(XDATA_RAMZONE,   (uint8*)&gs_ReportState,   10);        //主动上报状态字1
//    memcpy(XDATA_RAMZONE+10,   (uint8*)&gs_ReportStateFlg,10);        //当前电量增量
//    if(gui_ReportCRC != do_CRC(XDATA_RAMZONE, 20))
//    {
//        memset((uint8*)&gs_ReportState,0x00,sizeof(REPORTSTATE));                   //主动上报状态清零
//        memset((uint8*)&gs_ReportStateFlg,0x00,sizeof(REPORTSTATE));                //主动上报状态标志清零
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
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-12)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void SysEventProc(void)
{
    IWDG_ReloadCounter();  

    if(gui_SystemEvent & flgEtGlb_PowDown)              
    {                                                   
        //PowerDnPro();                  //掉电处理
     gui_SystemEvent &=~ flgEtGlb_PowDown;           
     //gui_SystemSleepState=0x55aa;
    }                                                   
    else if(gui_SystemEvent & flgEtGlb_EnergyCalc)      
    {                                                   
        {
             Eny_EnergyProc(active);                               // 能量事件
        }     
        gui_SystemEvent &=~ flgEtGlb_EnergyCalc;        
    }     
    else if(gui_SystemEvent & flgEtGlb_NEnergyCalc)      
    {                                                   
        {
             Eny_EnergyProc(reactive);                               // 能量事件
        }
       
        gui_SystemEvent &=~ flgEtGlb_NEnergyCalc;        
    }     
    else if(gui_SystemEvent & flgEtGlb_RefreshLcd)      
    {                                                   
        ShowRefresh();                                  // 显示刷新
        gui_SystemEvent &=~ flgEtGlb_RefreshLcd;        
    }                                                   
    else if(gui_SystemEvent & flgEtGlb_NeedPrtclChk)    
    {                                                   
        {
              ComApp_Proc();                                  // 通讯事件
        }
      
        gui_SystemEvent &=~flgEtGlb_NeedPrtclChk;       
    }
     else if( gui_SystemEvent&flgEtGlb_ClearEv)         //清零事件事件
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
        gui_SystemEvent = 0;                            //防止出错时,整个循环下面的处理永远被挂起
    }
}
/*=========================================================================================\n
* @function_name: TimeEventProc
* @function_file: main.c
* @描述: 系统时间事件处理
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-05-09)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
//Word16 gw_Data[50];
void TimeEventProc(void)
{

    if(guc_TimeEvent & flgEtTim_Second)                 // 秒处理
    {        
        //BE_ReadP(EEP_ZBUA_PT,(uint8*)gw_Data,100);
        TimerProc();                                    // 在停电和有电下均处理  
        EnyB_Open_Cf();
        ShowSecondProc();
        DMA_Channel_Enable();
        gi_Temprature = (((T_Get_Temp()*3250/4096)*100)/175)-273;

         //gi_Temprature = (((T_Get_Temp()*3250/4096)*100)/175)-273;
        //EnyB_Check_PowRev();                            //功率方向判断
        guc_TimeEvent |= flgEtTim_SecSoft;
        guc_TimeEvent &= ~flgEtTim_Second;              //
    }
    else if(guc_TimeEvent&flgEtTim_Minute)
    {
         Data_MinProc();
         guc_TimeEvent&=~flgEtTim_Minute;
    }                                                   //
    else if(guc_TimeEvent & flgEtTim_Hour)              // 小时事件
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
//      MChannelCal();                                  // 测试M
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
* @描述: 入口函数main
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容:  
===========================================================================================*/
void main(void)
{
    __disable_interrupt();  //关闭总中断
    SystemInit();           //配置系统主时钟 ,在启动文件已经调用了该函数
    SetSysOscClock();       //配置系统OSC时钟  
    RCC_HSICmd(ENABLE);     //内部高速时钟
    //初始化NVIC中断系统
    //设置中断向量表的起始地址为0x08000000
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    Init_Var();
    DataSecurityInit();
    SysIO_Init();               //初始化系统IO
    ShowInitial();
    Sys_InitExit();
    //KeyIrqCmd(DISABLE);
    SysUart_Init();
    SysLcd_Init();
    //TIM3_Int_Init(99,799);      //10Khz的计数频率，计数到100为10ms
    TIM3_Int_Init(99,3199);  // 32Mhz下的配置
    //TIM2_Int_Init(99,3199); 
    //TIM2_Int_Init(99,79);       //100Khz的计数频率，计数到100为1ms 
    //TIM2_Int_Init(99,159);    // 16Mhz下的配置
    ADC_Configuration();        //hjj add 
    RTC_Configuration();        //hjj add 
    //RtcIrqCmd(DISABLE,0);
    Disp_Full();  
    Initspi2();  //配置SPI
    Initspi1();   //DMA SPI
   // IWDGconfig();
    //__enable_interrupt();
    Bronco_PMCtrl(Work_normal);  //9203PM控制
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
        IWDG_ReloadCounter();                         //喂狗
        if(0x00 != gui_SystemEvent)                     //
        {                                               //
                SysEventProc();                             // 系统事件     
        }
        else if(0x00!= guc_TimeEvent)                   //
        {                                               //
                TimeEventProc();                            // 时间处理流程     
        }                                               //
        else if(0x00 != guc_KeyEvent)                   //
        {                                              
                KeyProc();                                  // 按键处理         
        }        
        else if(0x00 != gui_RefreshEvent)               //
        {                                               
                 RefreshPara();                              // 参数刷新       
        }              
    }  
}



