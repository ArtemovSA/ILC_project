 
#define AP_DATAPROC_EXT
#include "Includes.h"

//void RecoverRunPara(void)
//{
//    //�ָ����״̬
//    //�̵���״̬
//    //����״̬
//    //  DataRamZone.RunCode3.RunCode3Word.LoadSwitchCmd=gs_LoadSwichStatus.SwitchAction;
//    //  DataRamZone.RunCode3.RunCode3Word.LOWChargeCmd=gs_LoadSwichStatus.ControlWarning;
//    uint32 ul_Cnt;
//    ul_Cnt=0;
//    //�ָ�����������ϱ�
//    BE_ReadP(EEP_KBGETNO, (uint8*)&ul_Cnt, 3);     //�����Ǵ���
//    gul_CoverUpCnt &= 0x00ffffff;
//    if(gul_CoverUpCnt != ul_Cnt)
//    {
//        if(gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt == false      //״̬��־δ��
//               && gs_ReportWord.ui_ReportWord1.ReportWord1.CoverRpt == 1)           //��Ҫ�����ϱ�
//        {
//          gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt = true;           //״̬��λ
//        }
//        gul_CoverUpCnt = ul_Cnt;
//    }
//
//    SysE2ReadData(EEP_DDLXZ, (uint8*)&gs_GiantSCP.ul_Adjust, 6);                //��ȡ�����У��
//    if(gs_GiantSCP.ui_CRC != do_CRC((uint8*)&gs_GiantSCP.ul_Adjust,4))
//    {
//        memset((uint8*)&gs_GiantSCP.ul_Adjust,0,6);
//    }
//
//    //�ָ���Կ״̬�ֺ�������
////  RecoverEsamKeySta();
//}

/*=========================================================================================\n
* @function_name: RecoverPara
* @function_file: Dataproc.c
* @����: �ָ�����
* 
* @����: 
* @����: 
* @����:   lwb (2012-05-30)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
//void RecoverPara(void)
//{        
//    CLRWDT();               //ι��
//    RecoverCfg();           //�ָ�������
////#ifdef LOADCURVE
////    RecoverLoadCurve();     //�ָ�������������
////#endif
//
//    CLRWDT();               //ι��
//    RecoverEnegyData();     //�ָ���������
//
//    CLRWDT();               //ι��
//    RecoverRtcPara();       //�ָ�RTC����
//
//    CLRWDT();               //ι��
//    RecoverRunPara();       //�ָ�����״̬��
//
//    CLRWDT();               //ι��
//    if(guc_ClockErr!=CLOCKERR)
//    {
//        RecoverJSRData();   //�ϵ粹������
//        CLRWDT();           //ι��
//        RecoverDayDjData(); //�ϵ粹�ն���
//    }
//    guc_ClockErr=0;
//    CLRWDT();
//}
/*=========================================================================================\n
* @function_name: CheckCRCWord
* @function_file: Dataproc.c
* @����: �������״̬���Ƿ���ȷ
* 
* @����: 
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-06-06)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 CheckCRCWord(uint8 *WrodSt)
{
    Word16 TempCrcCode;
    uint8 i;
    TempCrcCode.byte[0]=WrodSt[8];
    TempCrcCode.byte[1]=WrodSt[9];
    if(TempCrcCode.word==do_CRC(WrodSt,8))
    {
        for(i=0;i<8;i++)
        {
            if(WrodSt[i]==i)
            {
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;  
    }
} 
/*=========================================================================================\n
* @function_name: ClRCRCWord
* @function_file: Dataproc.c
* @����: ����������״̬��
* 
* @����: 
* @����: 
* @����:   lwb (2012-06-06)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void ClRCRCWord(uint8 *WrodSt)
{
    uint8 i;
    for(i=0;i<10;i++)
    {
        WrodSt[i]=55;
    }
}
/*=========================================================================================\n
* @function_name: SetCRCWord
* @function_file: Dataproc.c
* @����: ���õ������״̬��
* 
* @����: 
* @����: 
* @����:   lwb (2012-06-06)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void SetCRCWord(uint8 *WrodSt)
{
    uint8 i;
    Word16 TempCrcCode;
    for(i=0;i<8;i++)
    {
        WrodSt[i]=i;
    }
    TempCrcCode.word=do_CRC(WrodSt,8);
    WrodSt[8]=TempCrcCode.byte[0];
    WrodSt[9]=TempCrcCode.byte[1];
}
/*=========================================================================================\n
* @function_name: Data_ClearMeter
* @function_file: Dataproc.c
* @����: ����
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-07)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Data_ClearMeter(void)
{
    //uint8 i,j;
   // if(CheckCRCWord(guc_ClearWd))
    {
        ClRCRCWord(guc_ClearWd);
        Dsip_Clr();
        //��������¼
        if(guc_FactoryType!=FactorMd)           //���ڳ���ģʽ
        {
            Com_ClearMeterJL();                 //�����¼
        }
        
        memset(XDATA_RAMZONE,0x00,64);          //�������ⲿRAM 

 
        if(guc_FactoryType!=FactorMd)                           //���ڳ���ģʽ
        {
 
             CLRWDT();               //ι��
        }
        else
        {

            //����������������
//            guc_MacErrorCnt=0;                  //��֤ʧЧ��MAC��������
//          guc_OneMacErrorCnt=0;
            gut_SysWarn.byte=0;                     //�澯������
            gs_MeterState.ui_MeterState3.RunCode3Word.LoadSwitch=false;     //��
//            gs_LoadSwichStatus.BAODIAN =false;
//            gs_LoadSwichStatus.SwitchActionFlag=false;
//            gs_LoadSwichStatus.SwitchAction=false;
//            gs_LoadSwichStatus.SwitchSys=false;
//            guc_LongKeyFlg=0;
            gs_FREventOut.byte=0;

//            gs_EsamKeyUpDateCtrl.W32Status.lword=0;
//            SysE2ParaSetManage(EEP_MYZTZ,gs_EsamKeyUpDateCtrl.W32Status.byte,4);  //д����Կ״̬
//            ClrChargeWarning();
//            guc_EsamSuspend=0;
            gul_PowerTm=0;      //ͣ����ʾʱ������

            memset((uint8*)&gs_MeterState.ui_MeterState1.Word,0x00,sizeof(METERSTATE));
            if(gui_SystemState&flgStSys_Program)
            {
                gs_MeterState.ui_MeterState3.RunCode3Word.Program=true;
            }

//            if(gs_FeeData.uc_FeeRSDN==0x02)
//            {
//                gs_MeterState.ui_MeterState3.RunCode3Word.RateTable=true;       //�޸ĵ��״̬�֣�ʹ�õڶ���ʱ��
//            }
//            if(gs_FeeData.uc_FeeSQN==0x02)
//            {
//                gs_MeterState.ui_MeterState3.RunCode3Word.TimeZoneTable=true;  //�޸ĵ��״̬�֣�ʹ�õ�һ��ʱ��  
//            }
//
//            gui_RefreshEvent|=flgEtPara_Fee;
        }
        guc_CoverStatus=0;
        guc_PoweroffCoverJl=0x55;
        guc_PrKeyNewDown=0x55;

    
        Eny_EnergyClrCur(ENERGY_CLR,active);                               //�����ǰ����
        memset((uint8*)&gs_PowerCf,0,sizeof(S_POWERCF));            //������
        memset((uint8*)&gs_Energy,0,sizeof(S_ENERGY));              //��������
        SleepTimeProc();                                            //��RTC��ȡʱ�ӵ�RAM��
    }
}
/*=========================================================================================\n
* @function_name: Data_HourProc
* @function_file: Dataproc.c
* @����: Сʱ����
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-07)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Data_HourProc(void)
{
    //�������մ���
    EnergyCBRManage();
}
/*=========================================================================================\n
* @function_name: Data_MinProc
* @function_file: Dataproc.c
* @����: ���ݴ���ķ��Ӵ���
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-07)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Data_MinProc(void)
{
static uint8 tmp = 0;
   tmp++;
   if(tmp >=15)
   {
        Recording();
        tmp = 0;
    }
      // Eny_RefVar();
    CfgCheck();     //��������
    E2ParaCheck();  //E2���
    
    //���ö������־λ
    guc_DjCheckFlg|=flgDsDjCheck+flgZdDjCheck+flgDayDjCheck;   
//    if(gs_DMACheck.ucNeedOpen == true)
//    {
//        gs_DMACheck.ucNeedOpen = false;
//        WriteBronco(0x3f,0xC01A);  //����
//    }
}
/*=========================================================================================\n
* @function_name: DjProc
* @function_file: Dataproc.c
* @����: ���ᴦ��
* 
* @����: 
* @����: 
* @����:   lwb (2012-05-30)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void DjProc(void)
{
    if(guc_DjCheckFlg&flgDsDjCheck)             //��ʱ���ᴦ��
    {
        DSFrez();
        guc_DjCheckFlg&=~flgDsDjCheck;
    }else if(guc_DjCheckFlg&flgZdDjCheck)       //���㶳���ж�
    {
        ZDFrez();
        guc_DjCheckFlg&=~flgZdDjCheck;  

    }else if(guc_DjCheckFlg&flgDayDjCheck)      //�ն����ж�
    {
        DayFrez();
        guc_DjCheckFlg&=~flgDayDjCheck;
    }
    else if(guc_DjCheckFlg&flgInsDjEvent)  //˲ʱ����
    {
        HurryFrez();                                                            
        guc_DjCheckFlg&=~flgInsDjEvent;
    }
    else 
    {
        guc_DjCheckFlg=0;
    }
 
}
/*=========================================================================================\n
* @function_name: DataProcEvCountAdd
* @function_file: Dataproc.c
* @����: �¼��ۼƴ�������
* 
* 
* @����: 
* @param:ucNo  
* @����: 
* @����:   lwb (2012-05-22)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void DataProcEvCountAdd(uint8 ucNo)
{//�¼��ۼƴ����ӵĶ������ݽӿ�,�ṩ����������ģ��,ʹģ�鲻ֱ�Ӵ����ⲿ������,���ǽ������ݴ������
/*    uint8   ucLen;
    uint16  uiAddr;
    S_EVCNTADD code* pECA;
    uint32  ulCnt;

    if(ucNo < sizeof(gCs_EvCntAddTab)/sizeof(S_EVCNTADD))
    {//�����Ϸ����ж�
        pECA = (S_EVCNTADD code*)&gCs_EvCntAddTab + ucNo;
        ulCnt = 0;
        ucLen = pECA->ucLen;
        uiAddr = pECA->uiAddr;
        BE_ReadP(uiAddr, (uint8*)&ulCnt, ucLen);    //�ۼ�ֵ��ȡ
        ulCnt++;                                    //�ۼ�ֵ��1
        BE_WriteP(uiAddr, (uint8*)&ulCnt, ucLen);   //�ۼ�ֵ��1���д
    }*/
}
/*=========================================================================================\n
* @function_name: DataProcWriteRound2
* @function_file: Dataproc.c
* @����:
*
*
* @����:
* @param:ucRound    0��ʾָ�벻��Ҫ�ƶ�,�����ʾָ����Ҫ�����ƶ�
* @param:ucPos      ��������,���Ʊ�2�е�λ��,��Ӧһ����������
* @param:pucBuf     ���ݻ���,����Ч���ݳ����ɾ��Ʊ�2�е��������ָ��,�������Ʊ�ʱȷ������ȷ
* @����:
* @����:   lwb (2012-04-06)
* @��ע:
*-------------------------------------------------------------------------------------------
* @�޸���:
* @�޸�����:
===========================================================================================*/
void DataProcWriteRound2(uint8 ucRound, uint8 ucPos, uint8* pucBuf)
{
 /*   uint16  uiPt;                       //ָ��
    uint16  uiAddr;                     //E2��ַ
    S_ROUND2 code* psR2;                     //ָ����Ʊ�1ĳһ����Ԫ��ָ��
       
    if(ucPos >= sizeof(gCs_Round2Tab)/sizeof(S_ROUND2))
    {
        return;
    }
    uiPt=0;
    
    psR2 = (S_ROUND2 code*)&gCs_Round2Tab +ucPos;
    if(ucPos>=ZFHJLID && ucPos<=FH4JLID)
    {        
        BE_ReadP(psR2->uiPtAddr, (uint8*)&uiPt, 2);                //��ȡƫ��ָ��
    }
    else
    {
        BE_ReadB(psR2->uiPtAddr, (uint8*)&uiPt);    //ָ��
    }

    if(ucRound != R2_NO_MOVE)
    {//��Ҫ����ƶ�һλ
        if(uiPt == 0)
        {
            uiPt = psR2->ucMod - 1;
        }
        else
        {
            uiPt--;
        }
    }
    uiPt %= psR2->ucMod;                //�õ���Ҫ���ҵ��¼�ָ��,ʵ��Ϊ���ݱ���

    uiAddr = psR2->uiDataAddr + uiPt * psR2->ucLen;

    BE_WriteP(uiAddr, pucBuf, psR2->ucLen);
    if(ucRound != R2_NO_MOVE)
    {
        if(ucPos>=ZFHJLID && ucPos<=FH4JLID)
        {
            BE_WriteP(psR2->uiPtAddr, (uint8*)&uiPt,2);//д��ָ������            
        }
        else
        {
            BE_WriteP(psR2->uiPtAddr, (uint8*)&uiPt,1);//д��ָ������
        }
    }*/
}
/*=========================================================================================\n
* @function_name: DataProcRead2Slice
* @function_file: Dataproc.c
* @����:
*
*
* @����:
* @param:ucR2Pos  ��������,���Ʊ�2�е�λ��,��Ӧһ����������
* @param:ucCnt    ǰ�ڼ�������  0���һ��/1���ϴ�/...
* @param:ucOffset  �ڶ�Ӧ���ݿ��е�ƫ��,��λ���ֽ�
* @param:ucLen      ��Ҫ��ȡ�����ݳ���
* @param:pucBuf  ���ݻ���,����Ч���ݳ����ɾ��Ʊ�2�е��������ָ��,�������Ʊ�ʱȷ������ȷ
* @����:
* @����:   lwb (2012-04-06)
* @��ע:
*-------------------------------------------------------------------------------------------
* @�޸���:
* @�޸�����:
===========================================================================================*/
void DataProcRead2Slice(uint8 ucR2Pos, uint8 ucCnt, uint8 ucOffset, uint8 ucLen, uint8* pucBuf)
{
 /*   uint16  uiPt;                       //ָ��
    uint16  uiAddr;                     //E2��ַ
    S_ROUND2 code* psR2;                     //ָ����Ʊ�1ĳһ����Ԫ��ָ��

    if(ucR2Pos >= sizeof(gCs_Round2Tab)/sizeof(S_ROUND2))
    {
        return;
    }
    
    uiPt=0;
    
    psR2 = (S_ROUND2 code*)&gCs_Round2Tab + ucR2Pos;
    if(ucR2Pos>=ZFHJLID && ucR2Pos<=FH4JLID)
    {
        BE_ReadP( psR2->uiPtAddr, (uint8*)&uiPt,2);//��ȡָ��        
    }
    else
    {
        BE_ReadP( psR2->uiPtAddr, (uint8*)&uiPt,1);//��ȡָ��
    }
    uiPt += ucCnt;
    uiPt %= psR2->ucMod;                //�õ���Ҫ���ҵ��¼�ָ��,ʵ��Ϊ���ݱ���

    uiAddr = psR2->uiDataAddr + uiPt * psR2->ucLen + ucOffset;

    BE_ReadP(uiAddr, pucBuf, ucLen);*/
}

/*=========================================================================================\n
* @function_name: Data_DayProc
* @function_file: Dataproc.c
* @����: �촦��
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-07)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Data_DayProc(void)
{
    gui_SystemState &= ~flgStSys_IsTodayJS;         //ȥ���Ѿ����й��㲥Уʱ���
 
//    gul_ShowState&=~flgShowSt_Lock;                 //ȡ�������־
//    guc_MacErrorCnt=0;                              //���죬ȡ��ESAM����
//    guc_EsamSuspend=false;
 
}
/*=========================================================================================\n
* @function_name: CalPowDwTm
* @function_file: Dataproc.c
* @����: �������ʱ��
* 
* @����: 
* @����: 
* @����:   lwb (2012-06-07)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void CalPowDwTm(void)
{
    uint32 ul_SecTm;
    uint32 ul_MinTm;
    CLRWDT();               //ι��
    DataProcRead2Slice(DDJLID,0,0,12,XDATA_RAMZONE);                //��ȡ���һ�ε����¼
    //BE_ReadP(EEP_PRODUCELEFTTM,(uint8*)&gui_DyProDmOut,0x02);       //����ʣ��ʱ��
//  BE_ReadP(EEP_PRGLEFTTM,(uint8*)&gui_DyPgmOut,0x02);             //��ȡ���ʣ��ʱ��
    //BE_ReadP(EEP_PW2BSTM,(uint8*)&gs_PassWordCtr.uiDyPW2Out,0x04);  //��ȡ����ʱ��
    //BE_ReadP(EEP_POWERDWTM,(uint8*)&gul_PowerTm,4);                 //ͣ��ʱ��                 
    if(XDATA_RAMZONE[3+6]!=XDATA_RAMZONE[3])                         //�ж��Ƿ����
    {
        gui_SystemState &= ~flgStSys_IsTodayJS;                     //ȥ���Ѿ����й��㲥Уʱ���
//        if(guc_MacErrorCnt>=MACERRORNUM)                            //���죬ȡ��ESAM����
//        {
//            guc_MacErrorCnt=0;
//            guc_EsamSuspend=false;                                  //ESAM ����ȡ��
//            gul_ShowState&=~flgShowSt_Lock;                         //ȡ�������־
//        }
    }

    ul_SecTm=CalcTimeGap((S_TIMEDATA*)(XDATA_RAMZONE+6),(S_TIMEDATA*)XDATA_RAMZONE);
    ul_MinTm=ul_SecTm/60;
    gul_PowerTm+=ul_MinTm;
    if(ul_MinTm==0)                                                 //������µ�С�ڷ��ӣ��ж�ʵ����ʱ�Ƿ���֣��������1��
    {
        if(XDATA_RAMZONE[7]!=XDATA_RAMZONE[1])
        {
            ul_MinTm=1;
        }
    }

//    if((gs_FunCfg.uc_JDQCKCfg&0xC0)==0x80)      //�����ж���բʱ���ж���ʱʱ���Ƿ񵽴�
//    {
//        if(ul_SecTm>=gul_SwitchDelay)
//        {
//            gul_SwitchDelay=0;
//        }else
//        {
//            gul_SwitchDelay-=ul_SecTm;
//        }
//    }

//  if(ul_MinTm>=gui_DyPgmOut)                                      //���ʱ��
//  {
//      gui_DyPgmOut=0;
//      gui_SystemState &= ~flgStSys_Program;                       // ���ñ��״̬��־
//      gs_MeterState.ui_MeterState3.RunCode3Word.Program=false;
//
//  }else
//  {
//      gui_SystemState|=flgStSys_Program;
//      gs_MeterState.ui_MeterState3.RunCode3Word.Program=true;
//      gui_DyPgmOut-=ul_MinTm;
//  }

//  BE_WriteP(EEP_PRGLEFTTM,(uint8*)&gui_DyPgmOut,0x02);    //д����ʣ��ʱ��

    BE_ReadP(EEP_PRODUCE,(uint8*)&guc_FactoryType,1);        //��ȡ�볧��״̬
    if(guc_FactoryType==FactorMd)                           //��ȡ����״̬
    {
        if(ul_MinTm>=gui_DyProDmOut)                        //����ʱ��
        {
            gui_DyProDmOut=0;
            guc_FactoryType=0;                              // �˳�����״̬
        }else
        {
            guc_FactoryType=FactorMd;
            gui_DyProDmOut-=ul_MinTm;
        }
        //BE_WriteP(EEP_PRODUCELEFTTM,(uint8*)&gui_DyProDmOut,0x02);  //����ʣ��ʱ��
        SysE2ParaSetManage(EEP_PRODUCE,(uint8*)&guc_FactoryType,1);  //д�볧��״̬
    }

//  if(gs_PassWordCtr.ucSt02==BSST && ul_MinTm>=gs_PassWordCtr.uiDyPW2Out)
//  {
//      gs_PassWordCtr.uiDyPW2Out=0;            //����ʱ������
//      gs_PassWordCtr.ucSt02=0;
//      gs_PassWordCtr.ucPW2ErrCnt=0;
//
//  }else
//  {
//      gs_PassWordCtr.uiDyPW2Out-=ul_MinTm;
//  }
//
//  if(gs_PassWordCtr.ucSt04==BSST && ul_MinTm>=gs_PassWordCtr.uiDyPW4Out)
//  {
//      gs_PassWordCtr.uiDyPW4Out=0;            //����ʱ������
//      gs_PassWordCtr.ucSt04=0;
//      gs_PassWordCtr.ucPW4ErrCnt=0;
//
//  }else
//  {
//      gs_PassWordCtr.uiDyPW4Out-=ul_MinTm;
//  }
//  //����ʱ��
//  BE_WriteP(EEP_PW2BSTM,(uint8*)&gs_PassWordCtr.uiDyPW2Out,0x04);    //д�����ʣ��ʱ��
}

/*=========================================================================================\n
* @function_name: RecoverJSRData
* @function_file: Dataproc.c
* @����: �ϵ粹������
* 
* @����: 
* @����: 
* @����:   lwb (2012-06-07)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void RecoverJSRData(void)
{
    uint8 CbrCnt;
    uint8 ucDay;
    uint8 ucHour;
    uint8 i,j;
    uint16 temp;
    S_TIMEDATA LastCbrTm;
    Word32 CurTime;
    Word16 W16_JSR[3];
    uint8 ucYdCnt;
    uint8 JSRflg;
    uint8 ucData;
    uint8 CurJSRcnt;

    DataProcRead2Slice(DDJLID,0,0,12,XDATA_RAMZONE);    //��ȡ���һ�ε����¼

    memcpy(&LastCbrTm.ucSecond,XDATA_RAMZONE,6);        //��ȡ����ʱ��

    BE_ReadP(EEP_CBR1,W16_JSR[0].byte, 6);              //��ȡ3��������
                                                        //ȥ����ͬ�ĳ�����
    for(i=0;i<2;i++)
    {
        for(j=i+1;j<3;j++)
        {
            if(W16_JSR[i].word==W16_JSR[j].word)        
            {
                W16_JSR[j].word=0x9999;
            }else                                       //�Ƚϴ�С��С������ǰ
            {
                if(W16_JSR[i].word > W16_JSR[j].word)
                {
                    temp=W16_JSR[i].word;
                    W16_JSR[i].word=W16_JSR[j].word;
                    W16_JSR[j].word=temp;
                }
            }
        }
    }
    JSRflg=0;
    for(i=0;i<3;i++)                          //�ж���Ч�����ո���
    {
        if(W16_JSR[i].word==0x9999)
        {
            break;
        }else
        {
            JSRflg++;
        }
    }

    CbrCnt=0;
    //��ȡ������
    for(i=0;i<JSRflg;i++)
    {
       if(W16_JSR[i].word==0x9999)
       {
           continue;
       }

       ucDay=W16_JSR[i].byte[1];
       ucHour=W16_JSR[i].byte[0];
       //�жϹ�ȥ����������
       CbrCnt+=CalCBRCount(&LastCbrTm.ucMinute,&gs_DateTime.ucMinute,ucDay,ucHour);
       if(i==0)             //��һ�������ղ�������
       {
           ucYdCnt = CbrCnt;
       }
    }

    if(CbrCnt>12)
    {
        CbrCnt = 12;
    }

    memcpy((uint8*)CurTime.byte,(uint8*)&gs_DateTime.ucHour,4); //������ǰʱ��

    CurJSRcnt=JudgeJSR(CurTime.byte,W16_JSR[0].byte,JSRflg);       //�ж���Ҫ���ĸ������տ�ʼ
    
    for(i=0;i<CbrCnt;i++)
    {
      if(CurJSRcnt == 0)
      {                                                       //�������1����
            CurJSRcnt=JSRflg;
            if(CurTime.byte[2]>0x01)
            {
                ucData=APIByteBCD2HEX(CurTime.byte[2]);
                ucData--;                                           
                CurTime.byte[2] = APIByteHEX2BCD(ucData);
            }else
            {
                CurTime.byte[2] = 0x12;
                ucData = APIByteBCD2HEX(CurTime.byte[3]);
                ucData--;                                                    
                CurTime.byte[3] = APIByteHEX2BCD(ucData);
            }
        }
      
        CurTime.byte[0] = W16_JSR[CurJSRcnt-1].byte[0];
        CurTime.byte[1] = W16_JSR[CurJSRcnt-1].byte[1];
        memcpy(XDATA_RAMZONE+i*4,CurTime.byte,4);         //�������ʱ��
        CurJSRcnt--;
    }    
    
    
    //ת����ٴι�����������
    while(CbrCnt)
    {
        CbrCnt--;
        WriteHisEP();
        DataProcWriteRound2(R2_MOVE,JSSJJLID,XDATA_RAMZONE+CbrCnt*4);   //д�����ݣ�����
    }

    if(ucYdCnt>2)
    {
        ucYdCnt=2;
    }

    while(ucYdCnt)
    {
        ucYdCnt--;
        MonthLYDL(0);
    }
}
/*=========================================================================================\n
* @function_name: RecoverDayDjData
* @function_file: Dataproc.c
* @����: �ϵ粹�ն���
* 
* @����: 
* @����: 
* @����:   lm (2012-12-19)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void RecoverDayDjData(void)
{
//    uint8 CbrCnt;
//    uint8 ucMin;
//    uint8 ucHour;
//    S_TIMEDATA LastCbrTm;
//    S_CURTIME CurTime;
//    Word16 RdjTm;
//    int16 Eadd;
//    Word32 data1;
//    uint8 i;
//
//    DataProcRead2Slice(DDJLID,0,0,12,XDATA_RAMZONE);                //��ȡ���һ�ε����¼
//
//    memcpy(&LastCbrTm.ucSecond,XDATA_RAMZONE,6);            //��ȡ����ʱ��
//
//    BE_ReadP(EEP_RFREEZTM,RdjTm.byte,2);                    //��ȡ�ն���ʱ��
//    
//    ucHour=RdjTm.byte[1];
//    ucMin=RdjTm.byte[0];
//    CurTime.ucDay=gs_DateTime.ucDay;
//    CurTime.ucMonth=gs_DateTime.ucMonth;
//    CurTime.ucYear=gs_DateTime.ucYear;
//    
//           //�жϹ�ȥ�����ն���
//    CbrCnt=CalRDJCount(&LastCbrTm.ucMinute,&gs_DateTime.ucMinute,ucHour,ucMin);
//    CalDayBack(&CurTime,JudgeCurTime(&gs_DateTime.ucMinute,RdjTm.byte));
//    
//    for(uint8 i=0;i<CbrCnt;i++)
//    {
//        WriLastDJTime(&CurTime,RdjTm.byte,XDATA_RAMZONE1+i*5);
//        CalDayBack(&CurTime,1);
//    }
//
//    for(i = 0; i <= 4; i++)
//    {
//        Eadd = Eny_GetEp1(1, i,ABCPhy,active);                            //ȡ��ĳ�����ʵĵ�������
//        //data1.lword= gs_EnergyA.ulP[i]+Eadd;
//        data1.lword/=10;
//        data1.lword=Hex2BCD(data1.lword);
//        memcpy(XDATA_RAMZONE+5+i*4,(void*)data1.byte,4);    //���� �ܼ��ƽ�ȵ���
//    }
//
//    for(i = 0; i <= 4; i++)
//    {
//        Eadd = Eny_GetEp1(2, i,ABCPhy,active);                            //ȡ��ĳ�����ʵĵ�������
//        //data1.lword= gs_EnergyA.ulN[i]+Eadd;
//        data1.lword/=10;
//        data1.lword=Hex2BCD(data1.lword);
//        memcpy(XDATA_RAMZONE+25+i*4,(void*)data1.byte,4);   //���� �ܼ��ƽ�ȵ���
//    }
//   
//    memset(XDATA_RAMZONE+45,0,4);                           //����
//    
//    //ת����ٴι��ն�������
//    while(CbrCnt)
//    {
//        CbrCnt--;
//        DataProcEvCountAdd(RDJNUMID);                       //�����ն������
////      StructFreezeDate();
//        memcpy(XDATA_RAMZONE,(XDATA_RAMZONE1+CbrCnt*5),5);
//        DataProcWriteRound2(R2_MOVE,RDJID,XDATA_RAMZONE);   //д������  
//    }
}
/*=========================================================================================\n
* @function_name: RecoverEnegyData
* @function_file: Dataproc.c
* @����: 
* 
* @����: 
* @����: 
* @����:   lwb (2012-09-27)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void RecoverEnegyData(void)
{
//    uint16 CRC1;
//    int16 iDelta;
//    uint32 ul_EnyData[2];
//    BE_ReadP(EEP_POWDWENERGY,(uint8*)XDATA_RAMZONE,63);
//    CRC1=Uint8_To_Uint16(XDATA_RAMZONE+61);
//    if(CRC1==do_CRC(XDATA_RAMZONE, 61))      //���籣��������ȷ
//    {
//        memcpy((uint8*)&gs_EnergyData.uiEPZ,XDATA_RAMZONE,22);        //�ָ���������
//        //gs_FeeData.uc_FeeNo=XDATA_RAMZONE[22];                        //�ָ�����ǰ�ķ��ʣ�
//        Eny_EnergyActiveSave(active);                                       //�������
//
//        memcpy((uint8*)&gs_PowerCf.uc_Pz,XDATA_RAMZONE+29,10);      //��ǰCF��
//        memcpy((uint8*)&gs_Energy.ucPz, XDATA_RAMZONE+39, 10);      //��ǰCF���ڵ�����
//        DataProcEvCountAdd(DDNUMID);                                //���ӵ������
//        memcpy(XDATA_RAMZONE+29,&gs_DateTime.ucSecond,6);           //�ϵ�ʱ������
//        DataProcWriteRound2(R2_MOVE,DDJLID,XDATA_RAMZONE+23);       //д������¼
//        CRC1=0;
//        BE_WriteP(EEP_POWDWENERGY+61,(uint8*)&CRC1,2);               //ʹ����������Ч
//        if((gui_SystemState & flgStSys_PowErr)
//           &&(gs_powerErr==0x5A5A))                                 //��Դ�쳣�¼�����
//        {   //����
//            iDelta = Eny_GetEp1(1, 0,ABCPhy,active);
//            ul_EnyData[0]=gs_EnergyA.ulP[0]+iDelta;
//            ul_EnyData[0]/=10;
//            ul_EnyData[0]=Hex2BCD(ul_EnyData[0]);
//            //�����й�
//            iDelta = Eny_GetEp1(2, 0,ABCPhy,active);
//            ul_EnyData[1]=gs_EnergyA.ulN[0]+iDelta;
//            ul_EnyData[1]/=10;
//            ul_EnyData[1]=Hex2BCD(ul_EnyData[1]);
//            memcpy(XDATA_RAMZONE+35,(uint8*)&ul_EnyData,8);         //д�����������
//            DataProcEvCountAdd(DYGZNUMID);                              //���ӵ�Դ�쳣����
//            DataProcWriteRound2(R2_MOVE,DYGZJLID,XDATA_RAMZONE+23);     //д������¼
//            gui_SystemState &= ~flgStSys_PowErr;        //ȡ����Դ�쳣���
//            gs_powerErr=0;
//        }            
////        if(guc_LoadSwitchFlg==0x00)
////        {                        
////            LoadSwitchEndLog(1);
////            guc_LoadSwitchFlg=0xAA;
////        }
//    }else
//    {
//        memset((uint8*)&gs_PowerCf.uc_Pz,0,10);        //��ǰCF��
//        memset((uint8*)&gs_Energy.ucPz, 0, 10);      //��ǰCF���ڵ�����
//    }
//   
//    gui_SystemState &= ~flgStSys_PowOff;
}
/*=========================================================================================\n
* @function_name: JudgeCurTime
* @function_file: Dataproc.c
* @����: �жϵ�ǰʱ�����޹��ն���ʱ�䡣��û������ʱ�䣬����true��
* 
* 
* @����: 
* @param:CurTime  
* @param:RdjTime  
* 
* @����: 
* @return: uint8 
* @����:   xuqf (2013-3-2)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 JudgeCurTime(uint8* CurTime,uint8* RdjTime)
{
    if(*(CurTime+1) < *(RdjTime+1))             //�ж�ʱ�������ǰhourС���ն���hour
    {
        return true;
    }else                                       
    {
        if(*(CurTime+1) == *(RdjTime+1))        //��hour���
        {
            if(*CurTime < *RdjTime)             //�ж�Minute
            {
                return true;
            }
        }
        return 0;
    }
}
/*=========================================================================================\n
* @function_name: CalLastDJTime
* @function_file: Dataproc.c
* @����: д������
* 
* 
* @����: 
* @param:CurTime  
* @param:time  
* @param:pucBuf  
* @����: 
* @����:   xuqf (2013-3-2)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void WriLastDJTime(S_CURTIME *CurTime,uint8* time,uint8* pucBuf)
{
    CurTime->ucMinute = *time;                    //�ն���ķ�
    CurTime->ucHour = *(time+1);                  //�ն����ʱ
    memcpy(pucBuf,&CurTime->ucMinute,5);
}
/*=========================================================================================\n
* @function_name: CalDayBack
* @function_file: Dataproc.c
* @����: ��ǰ��һ���жϣ��������꣬����
* 
* 
* @����: 
* @param:CurTime  
* @param:offset  
* @����: 
* @����:   xuqf (2013-3-2)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void CalDayBack(S_CURTIME *CurTime,uint8 offset)
{
    uint8 ucData;
    if(offset)                                //��������Ч����Ҫ��1��
    {
        if(CurTime->ucDay>0x01)                  //�ж�day�ǲ��Ǵ���1�ա�����ǣ���1��
        {
            ucData=APIByteBCD2HEX(CurTime->ucDay);
            ucData--;                                                            //����1��
            CurTime->ucDay = APIByteHEX2BCD(ucData);
        }else                                 //�������
        {
            if(CurTime->ucMonth > 0x01)          //�ж��·��ǲ���1�¡��������
            {
                ucData=APIByteBCD2HEX(CurTime->ucMonth);
                ucData--;                                                            //����1��
                CurTime->ucMonth = APIByteHEX2BCD(ucData);

                if(((APIByteBCD2HEX(CurTime->ucYear) & 0x03) == 0)&& CurTime->ucMonth == 0x02)  
                {//�жϵ�ǰ���ǲ��������2�·�
                    CurTime->ucDay = MonthDays[CurTime->ucMonth] + 1; //�����1��
                }else
                {
                    CurTime->ucDay = MonthDays[CurTime->ucMonth];
                }
                CurTime->ucDay=APIByteHEX2BCD(CurTime->ucDay);
            }else                             //�����1�£���1��1�գ�
            {

                ucData=APIByteBCD2HEX(CurTime->ucYear);
                ucData--;                                                            //����1��
                CurTime->ucYear = APIByteHEX2BCD(ucData);
                CurTime->ucMonth = 0x12;
                CurTime->ucDay = 0x31;
            }
        }
    }
}
/*=========================================================================================\n
* @function_name: JudgeJSR
* @function_file: Dataproc.c
* @����: �жϵ�ǰʱ��������ձȽ�
* 
* 
* @����: 
* @param:CurTime  
* @param:JSRTime  
* @param:flg  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2013-03-06)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 JudgeJSR(uint8* CurTime,uint8* JSRTime,uint8 flg)
{
    uint8 i;
    for(i=0;i<flg;i++)
    {
        if(*(CurTime+1) < *(JSRTime+1+i*2))         //�ж���
        {
            return i;                               
        }else                                       
        {
            if(*(CurTime+1) == *(JSRTime+1+i*2))    //��Day���
            {
                if(*CurTime < *(JSRTime+i*2))       //�ж�Hour
                {
                    return i;                       //
                }
            }
        }
    }

    return flg;
}
/*=========================================================================================\n
* @function_name: DMA_Channel_Enable
* @function_file: Dataproc.c
* @����: 
* 
* @����: 
* @����: 
* @����:   xuqf (2015/12/1)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void DMA_Channel_Enable(void)
{
    if(guc_DMACnt < 4)
    {
        guc_DMACnt++;
    }
    
    if(guc_DMACnt == 4)
    {
        //TIM2_Int_Init(99,3199);
        guc_DMACnt++;
        WriteBronco(0x3f,0xC01A);
    }
}