
#define AP_EVENT_EXT
#include "Includes.h"


/******************************************************************************************************* 
�¼���¼
    a�� ���ü�¼���ܱ������¼��ķ���ʱ�̼�����ʱ�ĵ��������ݡ�
    b�� Ӧ��¼����ܴ��������10�α�̵�ʱ�̡������ߴ��롢���������ݱ�ʶ��
    c�� Ӧ��¼Уʱ�ܴ������������㲥Уʱ�������10��Уʱ��ʱ�̡������ߴ��롣
    d�� Ӧ��¼������ܴ��������10�ε��緢����������ʱ�̡�
    e�� Ӧ��¼���10��Զ�̿�����բ�����10��Զ�̿��ƺ�բ�¼�����¼������բ�¼�����ʱ�̺͵����������ݡ�
    f�� Ӧ��¼������ܴ��������10�ο�����¼��ķ���������ʱ�̡�
 
���Ṧ��
��ʱ���᣺����Լ����ʱ�估���������������ݣ�ÿ������������Ӧ����12�Ρ�
˲ʱ���᣺�ڷ���������£����ᵱǰ��������ʱ�䡢���е���������Ҫ�����������ݣ�˲ʱ������Ӧ�������3�ε����ݡ�
Լ�����᣺���������׷���/ʱ��ת�������ݵ��ת���������˾��Ϊ��������Ҫʱ������ת��ʱ�̵ĵ������Լ����� 
          ��Ҫ���ݣ��������2�ζ������ݡ�
�ն��᣺  �洢ÿ�����ʱ�̵ĵ�������Ӧ�ɴ洢�����µ����ݡ�
���㶳�᣺�洢����ʱ�̻���ʱ�̵��й��ܵ��ܣ�Ӧ�ɴ洢96�����ݡ�
********************************************************************************************************/


/*******************************************************************************
**���¼���Ϣ
*******************************************************************************/
const S_CLEAREVENT  gs_ClearEvent[] =
{
    {0x031100FF, ClrType_Power},
    {0x033000FF, ClrType_BC},
    {0x033004FF, ClrType_JS},
    {0x03300DFF, ClrType_BG},
    {0x033700FF, ClrType_POWERR},
    {0x1D0000FF, ClrType_LZ},
    {0x1D0001FF, ClrType_LZ},
    {0x1D0002FF, ClrType_LZ},
    {0x1D0003FF, ClrType_LZ},
    {0x1D0004FF, ClrType_LZ},
    {0x1E0000FF, ClrType_HZ},
    {0x1E0001FF, ClrType_HZ},
    {0x1E0002FF, ClrType_HZ},
    {0x1E0003FF, ClrType_HZ},
    {0x1E0004FF, ClrType_HZ},
    {0x033012FF, ClrType_MYGX},
    {0x033600FF, ClrType_FHKG},

#ifdef SQSDPRLOG
    {0x033005FF, ClrType_RSDBC},
    {0x033006FF, ClrType_SQBC},
#endif

#ifdef ZXRPRLOG
    {0x033007FF, ClrType_ZXRBC},
#endif

#ifdef YGZHZPRLOG
    {0x033009FF, ClrType_YGZHBC},
#endif

#ifdef JSRPRLOG
    {0x03300CFF, ClrType_JSRBC},
#endif

    {0xFFFFFFFF, ClrType_All},
      
};

                                                        //����
const uint8  CntgClrE = sizeof(gs_ClearEvent) / sizeof(S_CLEAREVENT);

/*=========================================================================================\n
* @function_name: Data_ClearEvent
* @function_file: Event.c
* @����: ���¼�
* 
* @����: 
* @����: 
* @����:   lwb (2012-05-17)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Data_ClearEvent(void)
{
   // uint8 i,j;
    //���Ƚ����жϣ��Ƿ�������

    memset(XDATA_RAMZONE,0x00,64);          //�������ⲿRAM 
    switch(guc_ClearType)
    {
        case ClrType_All:                                       //�����м�¼

           /* for(i=2;i<dim(gCs_MeterClearAddrTab);i++)
            {
                for(j=0;j<gCs_MeterClearAddrTab[i].ucPageCnt;j++)
                {
                    CLRWDT();               //ι��
                    BE_WriteP(gCs_MeterClearAddrTab[i].uiE2Addr+64*j,XDATA_RAMZONE,64);    //��������¼
                }
            }*/
            guc_PrKeyNewDown=0x55;
            guc_CoverStatus=0;
            guc_PoweroffCoverJl=0x55;
            //memset((uint8*)&gs_ReportState,0x00,sizeof(REPORTSTATE));                   //�����ϱ�״̬����
            //memset((uint8*)&gs_ReportStateFlg,0x00,sizeof(REPORTSTATE));                //�����ϱ�״̬��־����
            break;
        case ClrType_Power:                                     //�����µ��¼
           /* for(i=0;i<2;i++)
            {
                CLRWDT();               //ι��
                BE_WriteP(EEP_DDETNO+64*i,XDATA_RAMZONE,64);    //�����¼
            }*/
            break;

        case ClrType_BC:                                        //���̼�¼
//            for(i=0;i<8;i++)
//            {
//                CLRWDT();               //ι��
//                BE_WriteP(EEP_BCETNO+64*i,XDATA_RAMZONE,64);    //��̼�¼
//            }
//            guc_PrKeyNewDown=0x55;                              //�����̼�¼��Ҫ�ָ���ȥ���
            break;
        case ClrType_JS:                                        //��Уʱ��¼
//            for(i=0;i<3;i++)
//            {
//                CLRWDT();               //ι��
//                BE_WriteP(EEP_JSETNO+64*i,XDATA_RAMZONE,64);    //Уʱ��¼
//            }
            break;

        case ClrType_BG:                                        //���Ǽ�¼
//            for(i=0;i<5;i++)
//            {
//                CLRWDT();               //ι��
//                BE_WriteP(EEP_KBGETNO+64*i,XDATA_RAMZONE,64);    //����Ǽ�¼
//            }
//            guc_CoverStatus=0;
//            guc_PoweroffCoverJl=0x55;
            //gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt=0;
            //gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt=0;  
            break;

        case ClrType_LZ:                                        //����բ��¼
//            for(i=0;i<3;i++)
//            {
//                CLRWDT();               //ι��
//                BE_WriteP(EEP_LZETNO+64*i,XDATA_RAMZONE,64);    //����բ��¼
//            }
            break;

        case ClrType_HZ:                                        //���բ��¼
//            for(i=0;i<3;i++)
//            {
//                CLRWDT();               //ι��
//                BE_WriteP(EEP_HZETNO+64*i,XDATA_RAMZONE,64);    //����բ��¼
//            }
            break;
        case ClrType_MYGX:                                      //����Կ���¼�¼
//            for(i=0;i<3;i++)
//            {
//                CLRWDT();               //ι��
//                BE_WriteP(EEP_MYGXNO+64*i,XDATA_RAMZONE,64);   
//            }
            break;
        case ClrType_FHKG:                                      //�帺�ɿ���������¼
//            for(i=0;i<5;i++)
//            {
//                CLRWDT();               //ι��
//                BE_WriteP(EEP_FHKGNO+64*i,XDATA_RAMZONE,64);   
//            }
            break;
#ifdef SQSDPRLOG
        case ClrType_RSDBC:                                     //����ʱ�α�̼�¼
            for(i=0;i<9;i++)
            {
                CLRWDT();               //ι��
                BE_WriteP(EEP_RSDXGNO+64*i,XDATA_RAMZONE,64);    //��ʱ�α�̼�¼
            }
            break;
        case ClrType_SQBC:                                       //��ʱ����̼�¼
            for(i=0;i<9;i++)
            {
                CLRWDT();               //ι��
                BE_WriteP(EEP_SQXGNO+64*i,XDATA_RAMZONE,64);    //ʱ����̼�¼
            }
            break;
#endif

#ifdef ZXRPRLOG
        case ClrType_ZXRBC:                                //�������ձ�̼�¼
            for(i=0;i<2;i++)
            {
                CLRWDT();               //ι��
                BE_WriteP(EEP_ZXRXGNO+64*i,XDATA_RAMZONE,64);    //�����ձ�̼�¼
            }
            break;
#endif

#ifdef YGZHZPRLOG
        case ClrType_YGZHBC:                                //���й���Ϸ�ʽ��̼�¼
//            for(i=0;i<2;i++)
//            {
//                CLRWDT();               //ι��
//                BE_WriteP(EEP_YGZHETNO+64*i,XDATA_RAMZONE,64);    //�й���Ϸ�ʽ��̼�¼
//            }
            break;
#endif
#ifdef JSRPRLOG
        case ClrType_JSRBC:                                         //������ձ�̼�¼
//            for(i=0;i<3;i++)
//            {
//                CLRWDT();               //ι��
//                BE_WriteP(EEP_JSRETNO+64*i,XDATA_RAMZONE,64);    //�����ձ�̼�¼
//            }
            break;
#endif
        case ClrType_POWERR:             
//            for(i=0;i<4;i++)
//            {
//                CLRWDT();               //ι��
//                BE_WriteP(EEP_DYYCNO+64*i,XDATA_RAMZONE,64);    //��Դ�쳣��¼
//            }
            break;   
    }

    //�¼������¼
    Com_ClearEventJL(guc_ClearType);
}
 
/*=========================================================================================\n
* @function_name: PowerJL
* @function_file: Event.c
* @����: ͣ���¼
* 
* 
* @����: 
* @param:type  1: ͣ��  2���ϵ�
* @����: 
* @����:   lwb (2012-05-17)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void PowerJL(uint8 type)
{
    if(type == 1)                                               //�����ͣ��
    {
        DataProcEvCountAdd(DDNUMID);                            //���ӵ������
        memcpy(XDATA_RAMZONE,&gs_DateTime.ucSecond,6);
        memset(XDATA_RAMZONE+6,0x00,6);                         //�ϵ�ʱ������
        DataProcWriteRound2(R2_MOVE,DDJLID,XDATA_RAMZONE);      //д������¼
    }
    else if(type == 2)
    {
        DataProcRead2Slice(DDJLID,0,0,6,XDATA_RAMZONE);         //��ȡ���һ�ε����¼
        memcpy(XDATA_RAMZONE+6,&gs_DateTime.ucSecond,6);         //д���ϵ�ʱ��
        DataProcWriteRound2(R2_NO_MOVE,DDJLID,XDATA_RAMZONE);   //д������¼
    } 
}
 
/*=========================================================================================\n
* @function_name: Com_JSJL
* @function_file: Event.c
* @����: Уʱ��¼
* 
* 
* @����: 
* @param:code  
* @param:time  
* @����: 
* @����:   lwb (2012-05-17)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Com_JSJL(uint32 usecode, S_TIMEDATA* time)
{
    DataProcEvCountAdd(JSNUMID);                        //����Уʱ����
    memcpy(XDATA_RAMZONE,(uint8*)&usecode,4);           //��������
    memcpy(XDATA_RAMZONE+4,&time->ucSecond,6);          //У��ǰ��ʱ��
    memcpy(XDATA_RAMZONE+10,&gs_DateTime.ucSecond,6);   //У�����ʱ��
    DataProcWriteRound2(R2_MOVE,JSJLID,XDATA_RAMZONE);  //д��Уʱ��¼
    if( gs_DateTime.ucDay != time->ucDay 
       || gs_DateTime.ucMonth != time->ucMonth
       || gs_DateTime.ucYear != time->ucYear )
    {
        gui_SystemState &= ~flgStSys_IsTodayJS;         //ȥ���Ѿ����й��㲥Уʱ���
    }
    //guc_Timechange = 0x55;
}

 
/*=========================================================================================\n
* @function_name: PowerUpProc
* @function_file: Dataproc.c
* @����: �ϵ��ʼ��
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-07)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void PowerUpProc(void)
{
    PowerJL(2);                                         //ͣ���¼
    gui_SystemState &= ~flgStSys_PowOff;
}
 
/*=========================================================================================\n
* @function_name: PowerDnProc
* @function_file: Dataproc.c
* @����: �µ��ʼ��
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-07)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void PowerDnProc(void)
{
    PowerJL(1);                                         //ͣ���¼
    gui_SystemState |= flgStSys_PowOff;
} 
/*=========================================================================================\n
* @function_name: Com_ClearMeterJL
* @function_file: Event.c
* @����: ��������¼
* 
* 
* @����: 
* @param:code  
* @����: 
* @����:   lwb (2012-05-17)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Com_ClearMeterJL(void)
{
//    int16 Eadd;
//    Word32 data1;
//
//    //����������
//    DataProcEvCountAdd(DBQLNUMID);
//
//    memcpy(XDATA_RAMZONE,&gs_DateTime.ucSecond,6);          //д��˲ʱ����ʱ��
//    memcpy(XDATA_RAMZONE+6,(uint8*)&guw_UseCode.byte,4);    //�����ߴ���
//
//    Eadd = Eny_GetEp1(1, 0);                                //ȡ��ĳ�����ʵĵ�������
//    data1.lword= gs_EnergyA.ulP[0]+Eadd;
//    data1.lword/=10;
//    data1.lword=Hex2BCD(data1.lword);
//    memcpy(XDATA_RAMZONE+10,(void*)data1.byte,4);           //���� �ܵ���
// 
//    Eadd = Eny_GetEp1(2, 0);                                //ȡ��ĳ�����ʵĵ�������
//    data1.lword= gs_EnergyA.ulN[0]+Eadd;
//    data1.lword/=10;
//    data1.lword=Hex2BCD(data1.lword);
//    memcpy(XDATA_RAMZONE+14,(void*)data1.byte,4);           //���� �ܵ���
//
//    DataProcWriteRound2(R2_MOVE,DBQLJLID,XDATA_RAMZONE);    //д������  
     
}

/*=========================================================================================\n
* @function_name: Com_ClearEventJL
* @function_file: Event.c
* @����: 
* 
* 
* @����: 
* @param:usecode  
* @����: 
* @����:   lwb (2012-06-06)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Com_ClearEventJL(uint8 type)
{
//    uint8 i;
//
//    for(i = 0; i < CntgClrE; i++)                                       //Ѱ�����¼��ı�־
//    {
//        if(type == gs_ClearEvent[i].type)                               //�����Ƿ���ȣ���Ⱦ����ҵ���
//        {
//            break;
//        }
//    }
//
//    if(i >= CntgClrE)                                                   //������û���ҵ�
//    {
//        return;
//    }
//    //����������
//    DataProcEvCountAdd(SJQLNUMID);
//    memcpy(XDATA_RAMZONE,&gs_DateTime.ucSecond,6);                      //д��˲ʱ����ʱ��
//    memcpy(XDATA_RAMZONE+6,(uint8*)&guw_UseCode.byte,4);                //�����ߴ���
//
//    FCpyTMem(XDATA_RAMZONE+10,(uint8 code*)&gs_ClearEvent[i].ID ,4);    //�¼���ʶ
//    
// 
//    DataProcWriteRound2(R2_MOVE,SJQLJLID,XDATA_RAMZONE);                //д������  

}
/*=========================================================================================\n
* @function_name: SaveProgramLog
* @function_file: Event.c
* @����: д��̼�¼��Ҫд�İ����ܱ�̴����������ߴ��롢
* 
* 
* @����: 
* @param:s_FrmInfo  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-05-17)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 SaveProgramLog(S_FRAMEINFO *s_FrmInfo)
{
    //��̼����¸հ��±�׼��1�����±�̼����������֤û��ͨ��������£��ñ�־
    //2.�¼����壨���߱���¼����㣩���������󣬱�̼����µ�״̬��
    //3�������֤�꣬�øհ��±�־
//    if((guc_EsamAuthSt==0x55)/*||(gul_ShowState&flgShowSt_Pgm)*/)                                      //�����жϱ�̼��Ƿ��»��������֤�Ƿ�ͨ��,
//    {
//        if(guc_PrKeyNewDown==0x55)                                  //���ݱ�̼��Ƿ�հ���,û�����ù�����,�����Ļ�����Ҫ���ӱ�̴��������ұ�̼�¼��Ҫ����һ����
//        {
//            memcpy(XDATA_RAMZONE,&gs_DateTime.ucSecond,6);          //��ȡʱ�� 
//            memcpy(XDATA_RAMZONE+6,s_FrmInfo->uiUserCode.byte,4);   //�����ߴ���
//            guc_PrKeyNewDown=0;                                     //��̼��հ��±�־����
//            DataProcEvCountAdd(BCNUMID);                            //��ȡ��̴������ۼ�1��д���̴���
//            memcpy(XDATA_RAMZONE+10,s_FrmInfo->uiID.byte,4);        //����µ����ݱ�ʶ
//            memset(XDATA_RAMZONE+14,0xff,36);                       //��һ�����ݱ�ʶ���������ݱ�ʶ��дΪ0XFF
//            DataProcWriteRound2(R2_MOVE,BCJLID,XDATA_RAMZONE);      //д������  
//        }
//        else
//        {
//            DataProcRead2Slice(BCJLID,0,0,50,XDATA_RAMZONE);        //��ȡ�����̼�¼
//            MoveBuffer(XDATA_RAMZONE+10,XDATA_RAMZONE+14,36);       //�ӵ�10�����ݿ�ʼ����ƶ�4������,�ƶ���ȡ�ı�̽ڵ������
//            memcpy(XDATA_RAMZONE+10,s_FrmInfo->uiID.byte,4);        //����µ����ݱ�ʶ
//            DataProcWriteRound2(R2_NO_MOVE,BCJLID,XDATA_RAMZONE);   //д������  
//        }
//    }
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: StructFreezeDate
* @function_file: Event.c
* @����: ������������
* 
* @����: 
* @����: 
* @����:   lwb (2012-05-22)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void StructFreezeDate(void)
{
//    int16 Eadd;
//    Word32 data1;
//    uint8 i;
//
//    memcpy(XDATA_RAMZONE,&gs_DateTime.ucMinute,5);          //д��˲ʱ����ʱ��
//
//    for(i = 0; i <= 4; i++)
//    {
//        Eadd = Eny_GetEp1(1, i);                            //ȡ��ĳ�����ʵĵ�������
//        data1.lword= gs_EnergyA.ulP[i]+Eadd;
//        data1.lword/=10;
//        data1.lword=Hex2BCD(data1.lword);
//        memcpy(XDATA_RAMZONE+5+i*4,(void*)data1.byte,4);    //���� �ܼ��ƽ�ȵ���
//    }
//
//    for(i = 0; i <= 4; i++)
//    {
//        Eadd = Eny_GetEp1(2, i);                            //ȡ��ĳ�����ʵĵ�������
//        data1.lword= gs_EnergyA.ulN[i]+Eadd;
//        data1.lword/=10;
//        data1.lword=Hex2BCD(data1.lword);
//        memcpy(XDATA_RAMZONE+25+i*4,(void*)data1.byte,4);   //���� �ܼ��ƽ�ȵ���
//    }
//   
//    data1.lword=CalRMS(DATAP);
//    memcpy(XDATA_RAMZONE+45,data1.byte,4);                  //����
}
/*=========================================================================================\n
* @function_name: DSFrez
* @function_file: Event.c
* @����: ��ʱ����
* 
* @����: 
* @����: 
* @����:   lwb (2012-05-17)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void DSFrez(void)
{
    uint8 time[4];
    uint8 i;
    uint8 * pTm;

    pTm=(uint8*)&gs_DateTime.ucMinute;

    //�ж϶�ʱ�����Ƿ���

    BE_ReadP(EEP_DSFRZ,time,4);          //��ʱ����

    for(i=0;i<4;i++)
    {
        if(time[i]==0x99)
        {
            continue;
        }
  
        if(time[i]!=*(pTm+i))                   //ʱ�䲻�ȣ�ֱ�ӷ���
        {
            return;
        }
    }

    DataProcEvCountAdd(DSDJNUMID);                      //�����ն������
    StructFreezeDate();
    DataProcWriteRound2(R2_MOVE,DSDJID,XDATA_RAMZONE);  //д������  
}
/*=========================================================================================\n
* @function_name: HurryFrez
* @function_file: Event.c
* @����: ��������
* 
* @����: 
* @����: 
* @����:   lwb (2012-05-17)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void HurryFrez(void)
{
    DataProcEvCountAdd(SSDJNUMID);                       //����˲ʱ�������
    StructFreezeDate();
    DataProcWriteRound2(R2_MOVE,SSDJID,XDATA_RAMZONE);   //д������  
}
/*=========================================================================================\n
* @function_name: ZDFreStartTm
* @function_file: Event.c
* @����: ���㶳����ʼʱ���Ƿ񵽴�
* 
* @����: 
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-10-23)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 ZDFreStartTm(void) 
{
    uint8 time[5];              //������ʱ��
    uint8 Curtime[5];
    BE_ReadP(EEP_ZDFREEZTM, time,5);  //���㶳����ʼʱ��,�� ʱ �� �� ��
    ApiReverseData(time,5);            //�������� �� �� �� ʱ ��
    memcpy(Curtime,(uint8*)&gs_DateTime.ucMinute,5);
    ApiReverseData(Curtime,5);
    if(ApiBufCmp(time,Curtime,5)!=CMP_LG)               //�����ǰʱ��Ƚϴ�һ��Ļ��Ż�ȥ�����㶳��
    {
        return Ret_OK;
    }else
    {
        return Ret_Err;
    }
}
/*=========================================================================================\n
* @function_name: ZDFrez
* @function_file: Event.c
* @����: ���㶳��
* 
* @����: 
* @����: 
* @����:   lwb (2012-05-17)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void ZDFrez(void)
{
//    int16 Eadd;
//    Word32 data1;
//    uint8 IntTm;                //���ʱ��
//    uint8 temp2,temp3;
//
//    if(ZDFreStartTm()==Ret_OK)
//    {
//        BE_ReadP(EEP_ZDTMJG, &IntTm, 1);                //����ʱ����,�Ѿ���HEX����
//        temp2=APIByteBCD2HEX(gs_DateTime.ucMinute);
//        temp3=temp2%IntTm;
//        if(temp3==0)                                            //���㶳��ʱ�䵽
//        {
//
//            DataProcEvCountAdd(ZDDJNUMID);                      //����ʱ���������
//                                                                //ִ�����㶳��
//            memcpy(XDATA_RAMZONE,&gs_DateTime.ucMinute,5);        //д��˲ʱ����ʱ��
//            
//            Eadd = Eny_GetEp1(1, 0);                            //ȡ��ĳ�����ʵĵ�������
//            data1.lword= gs_EnergyA.ulP[0]+Eadd;
//            data1.lword/=10;
//            data1.lword=Hex2BCD(data1.lword);
//            memcpy(XDATA_RAMZONE+5,(void*)data1.byte,4);        //�����ܵ���
//            
//            Eadd = Eny_GetEp1(2, 0);                            //ȡ��ĳ�����ʵĵ�������
//            data1.lword= gs_EnergyA.ulN[0]+Eadd;
//            data1.lword/=10;
//            data1.lword=Hex2BCD(data1.lword);
//            memcpy(XDATA_RAMZONE+9,(void*)data1.byte,4);        //�����ܵ���
//            DataProcWriteRound2(R2_MOVE,ZDDJID,XDATA_RAMZONE);   //д������  
//        }
//    }
}
 
/*=========================================================================================\n
* @function_name: DayFrez
* @function_file: Event.c
* @����: �ն���
* 
* @����: 
* @����: 
* @����:   lwb (2012-05-17)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void DayFrez(void)
{
    Word16 RdjTm;
    //�ж��ն����Ƿ񵽴�
    //���ȶ�ȡ�ն���ʱ��
    BE_ReadP(EEP_RFREEZTM,RdjTm.byte,2);                    //��ȡ�ն���ʱ��
    if((gs_DateTime.ucHour==RdjTm.byte[1])
       &&(gs_DateTime.ucMinute==RdjTm.byte[0]))             //�ն���ʱ�䵽,д������
    {

        DataProcEvCountAdd(RDJNUMID);                       //�����ն������
        StructFreezeDate();
        DataProcWriteRound2(R2_MOVE,RDJID,XDATA_RAMZONE);   //д������  
    }
}
 
/*=========================================================================================\n
* @function_name: RSDFrez
* @function_file: Event.c
* @����: ��ʱ�α��л�����
* 
* @����: 
* @����: 
* @����:   lwb (2012-05-17)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void RSDFrez(void)
{
    DataProcEvCountAdd(RSDQHDJNUMID);                       //����ʱ�α����
    StructFreezeDate();
    DataProcWriteRound2(R2_MOVE,RSDQHDJID,XDATA_RAMZONE);   //д������  
}
/*=========================================================================================\n
* @function_name: SQFrez
* @function_file: Event.c
* @����: ʱ�����л�����
* 
* @����: 
* @����: 
* @����:   lwb (2012-05-17)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void SQFrez(void)
{
    DataProcEvCountAdd(SQQHDJNUMID);                        //����ʱ���������
    StructFreezeDate();
    DataProcWriteRound2(R2_MOVE,SQQHDJID,XDATA_RAMZONE);   //д������  
} 
/*=========================================================================================\n
* @function_name: SwitchOnLog
* @function_file: Event.c
* @����: 
* 
* 
* @����: 
* @param:usercode  
* @����: 
* @����:   lwb (2012-05-22)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void SwitchOnLog(uint32 usercode)
{
//    uint32 ul_EnyData[2];
//    int16 iDelta;
//
//    DataProcEvCountAdd(LZNUMID);     //������բ����
//
//    memcpy(XDATA_RAMZONE,&gs_DateTime.ucSecond,6);    //д����բʱ��
//    memcpy(XDATA_RAMZONE+6,(uint8*)&usercode,4);    //д������ߴ���
//  
//     //�����й�
//    iDelta = Eny_GetEp1(1, 0);
//    ul_EnyData[0]=gs_EnergyA.ulP[0]+iDelta;
//    ul_EnyData[0]/=10;
//    ul_EnyData[0]=Hex2BCD(ul_EnyData[0]);
//    //ul_EnyData[0]>>=4;
//  
//    //�����й�
//    iDelta = Eny_GetEp1(2, 0);
//    ul_EnyData[1]=gs_EnergyA.ulN[0]+iDelta;
//    ul_EnyData[1]/=10;
//    ul_EnyData[1]=Hex2BCD(ul_EnyData[1]);
//    //ul_EnyData[1]>>=4;
//    memcpy(XDATA_RAMZONE+10,(uint8*)&ul_EnyData,8);             //д�����������
// 
//    DataProcWriteRound2(R2_MOVE,LZJLID,XDATA_RAMZONE);  //д����բ��¼
}
/*=========================================================================================\n
* @function_name: SwitchOffLog
* @function_file: Event.c
* @����: 
* 
* 
* @����: 
* @param:usercode  
* @����: 
* @����:   lwb (2012-05-22)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void SwitchOffLog(uint32 usercode)
{
//    uint32 ul_EnyData[2];
//    int16 iDelta;
//
//    DataProcEvCountAdd(HZNUMID);                 //���Ӻ�բ����
//
//    memcpy(XDATA_RAMZONE,&gs_DateTime.ucSecond,6);    //д���բʱ��
//    memcpy(XDATA_RAMZONE+6,(uint8*)&usercode,4);    //д������ߴ���
//
//     //�����й�
//    iDelta = Eny_GetEp1(1, 0);
//    ul_EnyData[0]=gs_EnergyA.ulP[0]+iDelta;
//    ul_EnyData[0]/=10;
//    ul_EnyData[0]=Hex2BCD(ul_EnyData[0]);
//    //ul_EnyData[0]>>=4;
//    //�����й�
//    iDelta = Eny_GetEp1(2, 0);
//    ul_EnyData[1]=gs_EnergyA.ulN[0]+iDelta;
//    ul_EnyData[1]/=10;
//    ul_EnyData[1]=Hex2BCD(ul_EnyData[1]);
//    //ul_EnyData[1]>>=4;
//    memcpy(XDATA_RAMZONE+10,(uint8*)&ul_EnyData,8);     //д�����������
//
//    DataProcWriteRound2(R2_MOVE,HZJLID,XDATA_RAMZONE);  //д���բ��¼
}
/*=========================================================================================\n
* @function_name: CoverOpenLog
* @function_file: Event.c
* @����: 
* 
* @����: 
* @����: 
* @����:   lwb (2012-05-22)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void CoverOpenLog(void)
{
//    uint32 ul_EnyData[2];
//    int16 iDelta;
//
//    DataProcEvCountAdd(KBGNUMID);                   //���ӿ���Ǵ���
//
//    if(_SYS_IS_PWR_OFF)
//    {
//        SleepTimeProc();                                            //��ȡRTCʱ�ӵ�RAM��,
//    }
//    memcpy(XDATA_RAMZONE,&gs_DateTime.ucSecond,6);    //д�뿪���ʱ��
//    memset(XDATA_RAMZONE+6,0x00,6);                 //�ر��ʱ������
//    //�����й�
//    iDelta = Eny_GetEp1(1, 0);
//    ul_EnyData[0]=gs_EnergyA.ulP[0]+iDelta;
//    ul_EnyData[0]/=10;
//    ul_EnyData[0]=Hex2BCD(ul_EnyData[0]);
//    //�����й�
//    iDelta = Eny_GetEp1(2, 0);
//    ul_EnyData[1]=gs_EnergyA.ulN[0]+iDelta;
//    ul_EnyData[1]/=10;
//    ul_EnyData[1]=Hex2BCD(ul_EnyData[1]);
//    memcpy(XDATA_RAMZONE+12,(uint8*)&ul_EnyData,8);     //д�����������
//
//
//    memset(XDATA_RAMZONE+20,0x00,8);
//    //�޹�����Ҫ
//    DataProcWriteRound2(R2_MOVE,KBGJLID,XDATA_RAMZONE); //д�뿪��Ǽ�¼
 
}
/*=========================================================================================\n
* @function_name: CoverCloseLog
* @function_file: Event.c
* @����: �رձ�ǵĴ���
* 
* @����: 
* @����: 
* @����:   lwb (2012-05-22)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void CoverCloseLog(void)
{
//    uint32 ul_EnyData[2];
//    int16 iDelta;
//
//    DataProcRead2Slice(KBGJLID,0,0,20,XDATA_RAMZONE);               //��ȡ���һ�ο���Ǽ�¼����ȡ20���ֽڣ����8���ر�ǵ�������Ҫ��ȡ
//
//    if(_SYS_IS_PWR_OFF)
//    {
//        SleepTimeProc();                                            //��ȡRTCʱ�ӵ�RAM��,
//    }
//
//    memcpy(XDATA_RAMZONE+6,&gs_DateTime.ucSecond,6);                //д��ر��ʱ��
//
//    //�����й�
//    iDelta = Eny_GetEp1(1, 0);
//    ul_EnyData[0]=gs_EnergyA.ulP[0]+iDelta;
//    ul_EnyData[0]/=10;
//    ul_EnyData[0]=Hex2BCD(ul_EnyData[0]);
// 
//    //�����й�
//    iDelta = Eny_GetEp1(2, 0);
//    ul_EnyData[1]=gs_EnergyA.ulN[0]+iDelta;
//    ul_EnyData[1]/=10;
//    ul_EnyData[1]=Hex2BCD(ul_EnyData[1]);
//
//    memcpy(XDATA_RAMZONE+20,(uint8*)&ul_EnyData,8);         //д�����������
//    DataProcWriteRound2(R2_NO_MOVE,KBGJLID,XDATA_RAMZONE);  //д�뿪��Ǽ�¼
}
/*=========================================================================================\n
* @function_name: LoadSwitchOccurLog
* @function_file: Event.c
* @����: ���ɿ�������������¼
* 
* @����: 
* @����: 
* @����:   xuqf (2013-4-1)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void LoadSwitchOccurLog(void)
{
//    uint32 ul_EnyData[2];
//    int16 iDelta;
//    uint8 LoadSwichSt;
//
//    DataProcEvCountAdd(FUKGNUMID);                   //���Ӹ��ɿ�����������
//    LoadSwichSt=gs_LoadSwichStatus.SwitchSys&0x01;
//    memcpy(XDATA_RAMZONE,&gs_DateTime.ucSecond,6);    //д�뷢��ʱ��
//    memset(XDATA_RAMZONE+6,0x00,6);                   //����
//    memcpy(XDATA_RAMZONE+12,&LoadSwichSt,1);//д��������ǰ״̬
//    //�����й�
//    iDelta = Eny_GetEp1(1, 0);
//    ul_EnyData[0]=gs_EnergyA.ulP[0]+iDelta;
//    ul_EnyData[0]/=10;
//    ul_EnyData[0]=Hex2BCD(ul_EnyData[0]);
//    //�����й�
//    iDelta = Eny_GetEp1(2, 0);
//    ul_EnyData[1]=gs_EnergyA.ulN[0]+iDelta;
//    ul_EnyData[1]/=10;
//    ul_EnyData[1]=Hex2BCD(ul_EnyData[1]);
//    memcpy(XDATA_RAMZONE+13,(uint8*)&ul_EnyData,8);     //д�����������
//
//
//    memset(XDATA_RAMZONE+21,0x00,8);
//    //�޹�����Ҫ
//    DataProcWriteRound2(R2_MOVE,FHKGJLID,XDATA_RAMZONE); //д�븺�ɿ�����������ʱ�̼�¼
}
/*=========================================================================================\n
* @function_name: LoadSwitchEndLog
* @function_file: Event.c
* @����: ���ɿ�������������¼
* 
* @����: 
* @����: 
* @����:   xuqf (2013-4-1)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void LoadSwitchEndLog(uint8 uc_Save)
{
//    uint32 ul_EnyData[2];
//    int16 iDelta;
//
//    DataProcRead2Slice(FHKGJLID,0,0,21,XDATA_RAMZONE1);   //��ȡ���һ�θ��ɿ���������¼����ȡ21���ֽڣ����8���ر�ǵ�������Ҫ��ȡ
//    if(uc_Save == 0)
//    {
//        memcpy(XDATA_RAMZONE1+6,&gs_DateTime.ucSecond,6);     //д�����ʱ��
//    }
//    else
//    {
//        memcpy(XDATA_RAMZONE1+6,XDATA_RAMZONE+23,6);     //д�����ʱ��
//    }
//    //�����й�
//    iDelta = Eny_GetEp1(1, 0);
//    ul_EnyData[0]=gs_EnergyA.ulP[0]+iDelta;
//    ul_EnyData[0]/=10;
//    ul_EnyData[0]=Hex2BCD(ul_EnyData[0]);
// 
//    //�����й�
//    iDelta = Eny_GetEp1(2, 0);
//    ul_EnyData[1]=gs_EnergyA.ulN[0]+iDelta;
//    ul_EnyData[1]/=10;
//    ul_EnyData[1]=Hex2BCD(ul_EnyData[1]);
//
//    memcpy(XDATA_RAMZONE1+21,(uint8*)&ul_EnyData,8);         //д�����������
//
//    DataProcWriteRound2(R2_NO_MOVE,FHKGJLID,XDATA_RAMZONE1); //д�븺�ɿ���������¼
}