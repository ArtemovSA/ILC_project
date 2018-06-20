#define KEY_EXT
#include "Includes.h"

/*=========================================================================================\n
* @function_name: KeyInit
* @function_file: Key.c
* @����: ������ʼ��
* 
* @����: 
* @����: 
* @����:   lwb (2012-05-14)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void KeyInit(void)
{
    uint8 i;
    KeyPortIn();                                        // �����˿����ó�����ģʽ
    for(i=0;i<KeyNums;i++)
    {
        gs_KeyCtr[i].KeyThd=1;
        gs_KeyCtr[i].LgKeyThd=200;
    }
}

/*=========================================================================================\n
* @function_name: KeyScan
* @function_file: Key.c
* @����: ����ɨ��
* 
* @����: 
* @����: 
* @����:   lwb (2012-02-27)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void KeyScan(void)
{
    if(KeyStUp())                                   //�Ϸ�����ʱ����
    {
        if((gs_KeyCtr[UpKey].DlyCnt>=gs_KeyCtr[UpKey].KeyThd)&&
           gs_KeyCtr[UpKey].Status==KeyRls)
        {
           // OpenBeiGuang();
            guc_DyBgLed = gs_DispCont.ucKeyBgTm;     // ���ñ�����ʱʱ��
            guc_KeyEvent|=flgEtKey_Up;              // ���Ϸ������
            gs_KeyCtr[UpKey].Status=KeyDw;          //�Ϸ���Ϊ����״̬
        }
        if( gs_KeyCtr[UpKey].Status !=KeyDWLong)
        {
            gs_KeyCtr[UpKey].DlyCnt++;              //��ʱ��������1
        }
        if((gs_KeyCtr[UpKey].DlyCnt>=gs_KeyCtr[UpKey].LgKeyThd)&&
                 (gs_KeyCtr[UpKey].Status==KeyDw))
        {                                           //�����״̬Ϊ���£�������ʱ��������2sΪ����
            gs_KeyCtr[UpKey].Status=KeyDWLong;      //����״̬Ϊ����
            LongKeyProc();                          //����������
        }
    }
    else
    {
        gs_KeyCtr[UpKey].DlyCnt=0;                  //��ʱ��������
        gs_KeyCtr[UpKey].Status=KeyRls;             //����Ϊ�ͷ�
    }
    
    
    if(KeyStPr())                                   //�Ϸ�����ʱ����
    {
        if((gs_KeyCtr[PrKey].DlyCnt>=gs_KeyCtr[PrKey].KeyThd)&&
           gs_KeyCtr[PrKey].Status==KeyRls)
        {
            guc_KeyEvent|=flgEtKey_Pr;              // ���Ϸ������
            gs_KeyCtr[PrKey].Status=KeyDw;          //�Ϸ���Ϊ����״̬
        }

        if( gs_KeyCtr[PrKey].Status !=KeyDw)
        {
            gs_KeyCtr[PrKey].DlyCnt++;              //��ʱ��������1
        }

//      if((gs_KeyCtr[PrKey].DlyCnt>=gs_KeyCtr[PrKey].LgKeyThd)&&
//               (gs_KeyCtr[PrKey].Status==KeyDw))
//      {                                           //�����״̬Ϊ���£�������ʱ��������2sΪ����
//          gs_KeyCtr[PrKey].Status=KeyDWLong;      //����״̬Ϊ����
//          //LongKeyProc();                          //����������
//      }
    }
    else
    {
        gs_KeyCtr[PrKey].DlyCnt=0;                  //��ʱ��������
        gs_KeyCtr[PrKey].Status=KeyRls;             //����Ϊ�ͷ�
    }
}

/*=========================================================================================\n
* @function_name: LongKeyProc
* @function_file: Key.c
* @����: 
* 
* @����: 
* @����: 
* @����:   lwb (2012-02-27)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void LongKeyProc(void)
{
//    if(gs_LoadSwichStatus.SwitchActionFlag)
//    {
//        if(gs_LoadSwichStatus.SwitchAction==0)
//        { 
//            guc_LongKeyFlg=0x55;
//        }
//    }
}
 
/*=========================================================================================\n
* @function_name: CoverKeyScan
* @function_file: Key.c
* @����: ���Ǽ����
* 
* @����: 
* @����: 
* @����:   lwb (2012-06-03)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void CoverKeyScan(void)
{
//    if(KeyStBg())
//    {
//        guc_CoverKeyUpCnt=0;
//        guc_CoverKeyDwCnt++;
//        if(guc_CoverKeyDwCnt>2)
//        {
//            if(guc_CoverStatus==0)
//            {
//                CoverOpenLog();
//                gul_CoverUpCnt++;
//            }
//            gs_MeterState.ui_MeterState7.RunCode7Word.OpenCover=true;
//            guc_CoverStatus=1;
//            guc_PoweroffCoverJl=0x50;                                               //�е��¿����
////            if(gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt == false      //״̬��־δ��
////               && gs_ReportWord.ui_ReportWord1.ReportWord1.CoverRpt == 1)           //��Ҫ�����ϱ�
////            {
////                gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt = true;         //״̬��λ
////            }
//        
//        #ifdef FRCom
//            if(gs_SysRunSt.OpenGapFRFlag==false)                        //���¼���������û�г���
//            {
//                gs_SysRunSt.OpenGapFRFlag=true;
//            }
//        #endif    
//        }
//    }
//    else
//    {
//        guc_CoverKeyUpCnt++;
//        guc_CoverKeyDwCnt=0;
//        if(guc_CoverKeyUpCnt>2)
//        {
//            if(guc_CoverStatus==1)
//            {
//                CoverCloseLog();
//            }
//    
//            gs_MeterState.ui_MeterState7.RunCode7Word.OpenCover=false;
//            guc_PoweroffCoverJl=0x55;           //�е��¹ر��
//    
//            guc_CoverStatus=0;
////            if(gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt == true)     //�ж�״̬��־
////            {
////                gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt = false;
////            }
//            gs_SysRunSt.OpenGapFRFlag=false;
//        }
//    }
}
/*=========================================================================================\n
* @function_name: CoverScanSleep
* @function_file: Key.c
* @����: �����⿪���
* 
* @����: 
* @����: 
* @����:   lwb (2012-06-03)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void CoverScanSleep(void)
{
    if(KeyStBg())
    {
        if(guc_PoweroffCoverJl==0x55)   //�е����ǹر�ǵ����¼
        {
//            CoverCheck();               //�������⴦��,IO��ת
            if(guc_CoverStatus==0)      //�е����ǹر�ǵ���ֻҪ��¼�����
            {
                CoverOpenLog();
            }
 

            gs_MeterState.ui_MeterState7.RunCode7Word.OpenCover=true;
            guc_PoweroffCoverJl=0x50;
            guc_CoverStatus=1;
//            if(gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt == false      //״̬��־δ��
//               && gs_ReportWord.ui_ReportWord1.ReportWord1.CoverRpt == 1)           //��Ҫ�����ϱ�
//            {
//                gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt = true;         //״̬��λ
//            }
        }
    }
    else    
    {
        if(guc_PoweroffCoverJl==0x50)   //�е����ǹر�ǣ�����ͣ���¿��˱��
        {
            if(guc_CoverStatus==1)
            {    
                CoverCloseLog();
            }

//            if(!guc_PowOffRuning)
//            {
//                RTCWakeUpTm(RTC_HOUR);      //�л���Сʱ����
//            }

            guc_CoverStatus=0;
            guc_PoweroffCoverJl=0;      //���ڼ�¼������¼�

            gs_MeterState.ui_MeterState7.RunCode7Word.OpenCover=false;
//            if(gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt == true)     //�ж�״̬��־
//            {
//                gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt = false;
//            }
        }
    } 
}
/*=========================================================================================\n
* @function_name: KeyDispSleepDown
* @function_file: Key.c
* @����: ����ʱ��ʾ����ɨ��
* 
* @����: 
* @����: 
* @����:   lwb (2012-06-03)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void KeyDispSleepDown(void)
{  
    if(gs_KeyCtr[UpKey].Status==KeyRls)
    {
        if(KeyStUp())
        {
            gs_KeyCtr[UpKey].Status=KeyDw;
            ShowKeyManage(Const_Disp_KeyUp);            // �Ϸ��İ���
        }
    }
    else    
    {
        if(!KeyStUp())
        {
            gs_KeyCtr[UpKey].Status=KeyRls;
            gs_KeyCtr[UpKey].DlyCnt=0;
        }
    }
}
/*=========================================================================================\n
* @function_name: KeyProc
* @function_file: Key.c
* @����: ����������
* 
* @����: 
* @����: 
* @����:   lwb (2012-02-27)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void KeyProc(void)
{
    if(guc_KeyEvent & flgEtKey_Up)
    {
        ShowKeyManage(Const_Disp_KeyUp);            // �Ϸ��İ���
        guc_KeyEvent &=~ flgEtKey_Up;               // �����־
    }
    else if(guc_KeyEvent & flgEtKey_Pr)                  // ���״̬����
    {                                               //
        if(gui_SystemState & flgStSys_Program)      // �����б��Ƿ��ڱ��״̬
        {
            guc_PrKeyNewDown=0;
            gui_SystemState &= ~flgStSys_Program;   // �Ѿ����ڱ��״̬���������־
            gui_DyPgmOut = 0x00;                     // ��̳�ʱ�˳�ʱ������
            gs_MeterState.ui_MeterState3.RunCode3Word.Program=false;
        }                                           //
        else                                        //
        {                                           //
            gui_SystemState |= flgStSys_Program;    // ���ñ��״̬��־
            guc_PrKeyNewDown=0x55;
            BE_ReadP(EEP_PROTIME,(uint8*)&gui_DyPgmOut,0x02);       // ��̳�ʱ�˳�ʱ������
            if(gui_DyPgmOut==0xffff)                                // ���E2û�г�ʼ���Ļ���Ĭ��Ϊ240����
            {
                gui_DyPgmOut=240;
            }
            gs_MeterState.ui_MeterState3.RunCode3Word.Program=true;
        }                                           //
        guc_KeyEvent &= ~flgEtKey_Pr;               //


        //BE_WriteP(EEP_PRGLEFTTM,(uint8*)&gui_DyPgmOut,0x02);    //д����ʣ��ʱ��
    }                                               //
    else                                            //
    {                                               //
        guc_KeyEvent =0x0;                          //  ��ֹ����λ����������Ƶ��������¼�
    }  
                                                    //
}
/*=========================================================================================\n
* @function_name: KeyPgm_DyOut
* @function_file: Key.c
* @����: ���ʱ���˳�����
* 
* @����: 
* @����: 
* @����:   lwb (2012-06-06)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void KeyPgm_DyOut(void)
{
    if(gui_DyProDmOut>0)
    {
        gui_DyProDmOut--;
        if(gui_DyProDmOut==0)
        {
            guc_FactoryType=0;                                          // �˳�����״̬
            //BE_WriteP(EEP_PRODUCELEFTTM,(uint8*)&gui_DyProDmOut,0x02);  //����ʣ��ʱ��
            SysE2ParaSetManage(EEP_PRODUCE,(uint8*)&guc_FactoryType,1);  //д�볧��״̬           
        }
    }

}
