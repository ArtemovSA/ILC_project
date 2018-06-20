
#define DLT_2007_645_EXT
#include "Includes.h"

/*=========================================================================================\n
* @function_name: ReadMeterAddr
* @function_file: DLT_645_2007.c
* @����: ��ͨѶ��ַ
* 
* 
* @����: 
* @param:s_FrmInfo  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-02-28)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 ReadMeterAddr(S_FRAMEINFO *s_FrmInfo)
{
    if(s_FrmInfo->ucAtb == Const_Addr_Any)
    {
        SysE2ReadData(EEP_COMADD, s_FrmInfo->ucbuf, 6);     //��ȡ���ַ
        s_FrmInfo->ucLen = 6;                               //�ظ��������򳤶�
    }
    else
    {
        s_FrmInfo->ucReply = 0;                             //����AA...AA֮���������ܶ�ȡ���ַ,����Ҫ�ظ�
    }
    return Ret_OK;
}

/*=========================================================================================\n
* @function_name: WriteMeterAddr
* @function_file: DLT_645_2007.c
* @����: д���ַ
* 
* 
* @����: 
* @param:s_FrmInfo  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-02-28)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 WriteMeterAddr(S_FRAMEINFO *s_FrmInfo)
{
    if(s_FrmInfo->ucAtb != Const_Addr_Any)              //ͨѶ��ַΪAA....AA
    {

        s_FrmInfo->ucReply = 0;                         //����AA...AA֮���������ܶ�ȡ���ַ,����Ҫ�ظ�
        return Ret_OK;
    }

    if((gui_SystemState & flgStSys_Program)
       || (guc_FactoryType == FactorMd))                      // ���ڳ�����Ҫ��̼����
    {
        if(JudgeIsNum(s_FrmInfo->ucbuf,6)==Const_Data_OK)
        {
            SysE2ParaSetManage(EEP_COMADD, s_FrmInfo->ucbuf,6); //дͨѶ��ַ
            memcpy(guc_MeteAddr,s_FrmInfo->ucbuf,6);            //��ֵ��ַ
            s_FrmInfo->ucLen = 0;                               //�ظ��������򳤶�
        }
        else
        {
            s_FrmInfo->ucErr.word |= ComErrBit_Other;            //��������
            s_FrmInfo->ucLen = 1;
        }
    }
    else
    {
        s_FrmInfo->ucErr.word |= ComErrBit_ErrPw;            //Ȩ�޲���
        s_FrmInfo->ucLen = 1;
    }
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: RWData
* @function_file: DLT_645_2007.c
* @����: ��д����
* 
* 
* @����: 
* @param:s_FrmInfo  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-03-01)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 RWData(S_FRAMEINFO *s_FrmInfo)
{
//    uint8 uclen;
    uint8 Index,i;
//    if(s_FrmInfo->ucLevel==0x99)
//    {
//        if( s_FrmInfo->uiID.lword==USERIDNUM        //�ͻ����
//           ||s_FrmInfo->uiID.lword==METERIDNUM)     //���            
//        {
//            ApiReverseData(s_FrmInfo->ucbuf,6);     //��ת��������
//            ApiReverseData(s_FrmInfo->ucbuf+6,4);   //��תMAC
//
//            if(s_FrmInfo->uiID.lword==METERIDNUM)   //��Ÿ��±��붼�ڹ�Կ��
//            {
//                s_FrmInfo->ucErr.word=ComCheckFormat(s_FrmInfo->ucbuf,MeterIdFm,6);
//                if(s_FrmInfo->ucErr.word!=0)
//                {    
//                    s_FrmInfo->ucLen = 1;
//                    return Ret_OK;
//                }
//
//                RemoteControl(s_FrmInfo->ucbuf,12,0);      
//            }else 
//            {
//                RemoteControl(s_FrmInfo->ucbuf,7,0);      
//            }
//            if(gun_EsamError.word.word==0)
//            {
//                ApiReverseData(s_FrmInfo->ucbuf,6);         //��ת����
//                if(s_FrmInfo->uiID.lword==METERIDNUM)       //���
//                {
//                    SysE2ParaSetManage(EEP_METERNO,s_FrmInfo->ucbuf,6);
//                }else                                       //�ͻ����
//                {
//                    SysE2ParaSetManage(EEP_KHBH,s_FrmInfo->ucbuf,6);
//                }
//                s_FrmInfo->ucErr.word=0;
//                s_FrmInfo->ucLen=0;
//            }
//            else 
//            {  
//                s_FrmInfo->ucErr.word=ComErrBit_ErrPw;
//            }
//        }else
//        {
//            s_FrmInfo->ucErr.word=ComErrBit_ErrPw;                  //�ܼ�����
//        }
//        
//        if(s_FrmInfo->ucErr.word!=0)
//        {    
//            s_FrmInfo->ucLen = 1;
//            return Ret_OK;
//        }
//    }
//    else   
//    {
//        if(s_FrmInfo->ucLevel==0x98)
//        {
//            uclen=s_FrmInfo->ucLen-12;;
//            ApiReverseData(s_FrmInfo->ucbuf,(uclen-4));
//            ApiReverseData(s_FrmInfo->ucbuf-4+uclen,4);
//            if(  guc_EsamAuthSt!=0x55
//               ||guc_EsamSuspend==true)         //�����֤���ɹ����߹��𣬷��������
//            {
//                s_FrmInfo->ucErr.word=ComErrBit_ErrPw;
//                gun_EsamError.ESAMERRORWord.EsamAuthError=true;
//            }else
//            {
//                uclen=StrDep(s_FrmInfo->uiID.byte[2]%5,s_FrmInfo->ucbuf,uclen);
//                ApiReverseData(s_FrmInfo->ucbuf,4); //���ݱ�ʶ
//                if(s_FrmInfo->uiID.lword!=Uint8_To_Uint32(s_FrmInfo->ucbuf))
//                {
//                    s_FrmInfo->ucErr.word = ComErrBit_Nodata;            //û���ҵ����ݱ�ʶ
//                    s_FrmInfo->ucLen = 1;
//                    return Ret_OK;
//                }
//                
//                s_FrmInfo->ucbuf+=4;            //ȥ�����ݱ�ʶ��ƫ����
//                s_FrmInfo->ucLen=uclen-4;       //��ȥ���ݱ�ʶ�ĳ���
//
//                if((s_FrmInfo->uiID.lword&0xffffff00)==0x04010000
//                   ||(s_FrmInfo->uiID.lword&0xffffff00)==0x04020000)
//                {                                                           //ʱ����ʱ�η�ת
//                    for(i=0;i<s_FrmInfo->ucLen/3;i++)
//                    {
//                        ApiReverseData(s_FrmInfo->ucbuf+i*3,3);             //ȡ������
//                    }
//                }else
//                {
//                    ApiReverseData(s_FrmInfo->ucbuf,s_FrmInfo->ucLen);      //ȡ������
//                }
//            }
//           
//            if(gun_EsamError.word.word!=0)
//            {
//                if(   gun_EsamError.ESAMERRORWord.EsamError
//                   || gun_EsamError.ESAMERRORWord.EsamAuthError)
//                {    
//                    s_FrmInfo->ucErr.word=ComErrBit_ErrPw;
//                }
//                else
//                {    
//                    s_FrmInfo->ucErr.word=ComErrBit_Other;
//                }
//                s_FrmInfo->ucLen = 1;
//                return Ret_OK;
//            }
//        } 

        //02 ����04��
        for(i = 0; i < NumofCoreID; i++)                    //���ȷ������
        {
            if(s_FrmInfo->uiID.byte[3] == gs_CoreID[i].uiID3)
            {
                break;
            }
        }
    
        if(i >= NumofCoreID)                                    //������û���ҵ�ID
        {
            s_FrmInfo->ucErr.word = ComErrBit_Nodata;            //û���ҵ����ݱ�ʶ
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
        }

    
        Index = MidFindID(s_FrmInfo->uiID,gs_CoreID[i].pTab, gs_CoreID[i].ucNum);
    
        if(Index == 0xFF)                                       //���û���ҵ�
        {
            s_FrmInfo->ucErr.word = ComErrBit_Nodata;           //û���ҵ����ݱ�ʶ
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
        }
    
        if(s_FrmInfo->uiID.byte[1] == 0xFF)                     //�����Ƿ�Ϊ����
        {
            s_FrmInfo->ucAtb |= Const_Atb_DataJC;
        }

        if(s_FrmInfo->ucCmd == 0x14)
        {
            if(((s_FrmInfo->uiID.lword&0xFFFFFF00)!=0x04010000)
               &&((s_FrmInfo->uiID.lword&0xFFFFFF00)!=0x04020000)
               )
            {
                if(s_FrmInfo->ucLen>(12+(gs_CoreID[i].pTab + Index)->ucLen))             
                {
                    s_FrmInfo->ucErr.word = ComErrBit_Other;           //û���ҵ����ݱ�ʶ
                    s_FrmInfo->ucLen = 1;
                    return Ret_OK;
                }
            }
        }
 
        //���ô�����
        if(Ret_Err == FunList[(gs_CoreID[i].pTab + Index)->ucFunNo](s_FrmInfo, gs_CoreID[i].pTab + Index))
        {
            return Ret_Err;
        }
//    }


    if(s_FrmInfo->ucCmd == 0x14&&
       s_FrmInfo->ucErr.word==0)                        //�����д����
    {
        SaveProgramLog(s_FrmInfo);                      //��̼�¼
    }
    return Ret_OK;
}

const Gs_E2RWCtr   gs_E2RwCtrTalbe[]=
{
    {0x20000002,    EEP_JBTOTAL,        sizeof(S_JBPM)  },      //0 У�����
    //{0x20000026,    EEP_RTCTEMPOFFSET,  1               },      //1 �¶�ƫ����
   // {0x20000027,    EEP_RTCBVAL,        20              },      //2 RTC Bֵ
    //{0x20000028,    EEP_RTCDDTEMP,      1               },      //3 RTC�����¶�
    //{0x20000029,    EEP_RTCFRQOFFSET,   4               },      //4 RTC����ƫ����
    //{0x4F520000,    EEP_AUTOCAL,        27              },      //5 ���ñ�׼������

//  {0x20000050,    EEP_PROTIME,        2               },      //6  �����Чʱ��(����)
//  {0x20000051,    EEP_BSTIME,         2               },      //7  ����ʱ��(����)
//  {0x20000052,    EEP_BSCOUNT,        1               },      //8  ��������(����)
 //   {0x20000053,    EEP_TZDLVAL,        4               },      //9  ��բ������ֵ��������
 //   {0x20000054,    EEP_TZDLDELAY,      2               },      //10 ��բ������ʱ��������
    {0x20000055,    EEP_POWUPJXTM,      1               },      //11 �е��¼�����ʾʱ��(����)
    {0x20000056,    EEP_POWDWJXTM,      1               },      //12 ͣ���¼�����ʾʱ��(����)
    {0x20000057,    EEP_POWOFFXSTM,     2               },      //13 ͣ����ʾʱ��(����)
    {0x20000058,    EEP_FULLSHOWTM,     1               },      //14 ȫ����ʾʱ��(����)
    {0x20000059,    EEP_POWUPBGTM,      1               },      //15 �ϵ米����ʱ��(����)
   // {0x2000005A,    EEP_IRBGTM,         1               },      //16 ���ⴥ��������ʱ��(����)
//    {0x2000005B,    EEP_KEYBGTM,        1               },      //17 ��������������ʱ��(����)
 //   {0x2000005C,    EEP_ESAMERRBGTM,    1               },      //18 ESAM���󱳹���ʱ��(����)
 //   {0x2000005D,    EEP_LEDSFCKTM,      1               },      //19 LED�Լ�ʱ��(����)
    {0x2000005E,    EEP_PRODUCE,        1               },      //20 ����ģʽ״̬(����)
    {0x2000005F,    EEP_PRODUCETM,      2               },      //21 ����ģʽ�˳�ʱ��(����)

    {0x20000060,    EEP_CFSAVECFG,      1               },      //22  CF���������������
//  {0x20000061,    EEP_CBRCFG,         1               },      //23  �����ղ��������������
//  {0x20000062,    EEP_DJCFG,          1               },      //24  ���Ჹ�������������
//  {0x20000063,    EEP_PWLCFG,         2               },      //25  ����ȼ���Ч�����������
 //   {0x20000064,    EEP_SQSDCFG,        1               },      //26 ʱ����ʱ���л�ʱ�������б������������
    {0x20000065,    EEP_ENSHOWCFG,      1               },      //27 ������ʾ��ʽ�����������
    {0x20000066,    EEP_TMSHOWCFG,      1               },      //28 ʱ����ʾ��ʽ�����������
    {0x20000067,    EEP_PWOFFSHOWMDCFG, 1               },      //29 ͣ����ʾģʽ�����������
//  {0x20000068,    EEP_JDQCKCFG,       1               },      //30 �̵�����ⷽʽ�����������
    {0x20000069,    EEP_METERCLRCNT,    1               },      //31 ������������������
    {0x2000006A,    EEP_METERCLFENGVAL, 4               },      //32 ������������ֵ��������
    {0x2000006B,    EEP_SHOWERRPOS,     1,              },      //33 ERR��ʾλ��
    {0x20000080,    EEP_PDIRTIME,       1,              },      //34 ���ʷ����¼��ж���ʱʱ��
    {0x20000081,    EEP_PDIRGATE,       3,              },      //35 ���ʷ����¼��й����ʴ�������
    
};
extern const uint8 SampTab[];
extern const uint16 SampE2SaveTab[];
/*=========================================================================================\n
* @function_name: FactoryRD
* @function_file: DLT_645_2007.c
* @����: ���ڶ�����
* 
* 
* @����: 
* @param:s_FrmInfo  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-03-01)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 FactoryRD(S_FRAMEINFO *s_FrmInfo)
{
    uint16 i;
    int16  temp;
    uint8  ucSampType;
    Word16 E2SaveBuf[1280];
    uint8  Send_Buffer[2];
    uint8  ucLen;
    uint8  offset;
    memcpy(s_FrmInfo->uiID.byte,s_FrmInfo->ucbuf,4);    //���ݱ�ʶ

    if(s_FrmInfo->uiID.lword==0x20000011)
    {
        temp=gi_Temprature/10;
        s_FrmInfo->ucbuf[4]=(uint8)(temp>>8);
        s_FrmInfo->ucbuf[5]=(uint8)(temp);
        s_FrmInfo->ucLen = 6;
        return Ret_OK;
    }else if(s_FrmInfo->uiID.lword==0x20000070)
    {
//      ulValue=EnyB_ReadMeterParaACK(CRPST); 
//      s_FrmInfo->ucbuf[4]=(uint8)ulValue;
        s_FrmInfo->ucLen = 5;
        return Ret_OK;
    }
//  else if((s_FrmInfo->uiID.byte[3]==0x20)&&
//          (s_FrmInfo->uiID.byte[2]==0xAA)&&
//          (s_FrmInfo->uiID.byte[1]==0x55))
    else if (s_FrmInfo->uiID.lword == 0x20000100)
    {
        BE_ReadP(EEP_ZBCNT_PT,(uint8*)&ucSampType,1);

        if (ucSampType>4)                //��������0-4����Ӧ8��16��32��64��128��Ĭ��32
        {
            ucSampType = 2;
        }
        ucLen = SampTab[ucSampType]; //ȡ��������һ����Ϊ��ȡ����
        //offset = s_FrmInfo->uiID.byte[0];   //
        offset = 0;
        for (i=0;i<20;i++)
        {
            BE_ReadP(SampE2SaveTab[offset]+ucLen*i,(uint8*)&E2SaveBuf[ucLen/2*i].word,ucLen);
        }

        for (i = 0; i < SampTab[ucSampType] * 10; i++) 
        {								   
            Send_Buffer[1] =  E2SaveBuf[i].word>>8;
            Send_Buffer[0] =  E2SaveBuf[i].word;
            
            for(uint8 j=0;j<2;j++)
            {
                USART_SendData(USART3,Send_Buffer[j]);
                while(USART_GetFlagStatus(USART3,USART_FLAG_TC) != 1);
            }
        }
        //WriteBronco(0x3f,0xC01A);
        s_FrmInfo->ucReply = 0;
        s_FrmInfo->ucbuf[4] = 0x0;
        s_FrmInfo->ucLen = 0;          
        return Ret_OK;
    }
    else if(s_FrmInfo->uiID.lword == 0x2000AA00)
    {
        //WriteBronco(0x3f,0xC01A);
        return Ret_OK;
    }
//    else if((s_FrmInfo->uiID.byte[3]==0x20)&&
//            (s_FrmInfo->uiID.byte[2]==0xAA)&&
//            (s_FrmInfo->uiID.byte[1]==0x01))
//    {
//        WriteBronco(0x0,0xC01A);
////        SysE2ReadData(EEP_ZBIA_PT,(uint8*)&gs_RawData.Raw_IA,160);
////        SysE2ReadData(EEP_ZBIA_PT+160,(uint8*)&gs_RawData.Raw_IA+160,160);
////        SysE2ReadData(EEP_ZBIA_PT+320,(uint8*)&gs_RawData.Raw_IA+320,160);
////        SysE2ReadData(EEP_ZBIA_PT+480,(uint8*)&gs_RawData.Raw_IA+480,160);
//        for(uint16 i=0;i<320;i++)
//        {
////            Send_Buffer[1] =  gs_RawData.Raw_IA[i]>>8;
////            Send_Buffer[0] =  gs_RawData.Raw_IA[i];
//            for(uint8 j=0;j<2;j++)
//            {
//                USART_SendData(USART3,Send_Buffer[j]);
//                while(USART_GetFlagStatus(USART3,USART_FLAG_TC) != 1);
//            }
//        }
//        WriteBronco(0x3f,0xC01A);
//        s_FrmInfo->ucbuf[4] = 0x1;
//        s_FrmInfo->ucLen = 5;
//        return Ret_OK;
//    }
    else if(s_FrmInfo->uiID.lword==0x20000026) 
    {
        //uint16 addr = 0;
        //SysE2ReadData(EEP_EPFH_PT,(uint8*)&addr,2);
        //if(addr >= (EEP_EPFH_PT +2 + 4*24*6) )
        //{
        //    addr = EEP_EPFH_PT +2;
        //}
        SysE2ReadData(EEP_EPFH_PT+2,s_FrmInfo->ucbuf + 4,36);
        s_FrmInfo->ucLen = 36+4;  
        return Ret_OK;      
    }
   /* else if(s_FrmInfo->uiID.lword==0x25000000)
    {
        uint8 len=s_FrmInfo->ucbuf[6];
        uint8 phy;
        //ѡ���ȡ��һ��
        switch(s_FrmInfo->ucbuf[7])
        {
            case 0x01:
                phy=APhy;
                break;
            case 0x02:
                phy=BPhy;
                break;
            case 0x03:
                phy=CPhy;
                break;
        }
        if(ReadRaccoon(Uint8_To_Uint16(s_FrmInfo->ucbuf + 4),len,phy))
        {
            memcpy(s_FrmInfo->ucbuf + 4,gs_RacCtrl.ucBuf+3,len*4);
            s_FrmInfo->ucLen = len*4+4;
        }else
        {
            s_FrmInfo->ucErr.word |= ComErrBit_Other;            //��������
        }
        return Ret_OK;
    }*/
    else                                                                               //У�����
    {
        for(i=0;i<sizeof(gs_E2RwCtrTalbe)/sizeof(Gs_E2RWCtr);i++)
        {
            if(s_FrmInfo->uiID.lword==gs_E2RwCtrTalbe[i].ulId)
            {
                BE_ReadP(gs_E2RwCtrTalbe[i].E2Addr,s_FrmInfo->ucbuf + 4,gs_E2RwCtrTalbe[i].ucLen);
                s_FrmInfo->ucLen = 4+gs_E2RwCtrTalbe[i].ucLen;

                return Ret_OK;
            }
        }
    }

    s_FrmInfo->ucReply = 0;                             //����Ҫ�ظ�*/
    return Ret_OK;
}

extern const uint8   InitPara5[sizeof(S_JBPM)-2];
/*=========================================================================================\n
* @function_name: FactoryWR
* @function_file: DLT_645_2007.c
* @����: ����д����
* 
* 
* @����: 
* @param:s_FrmInfo  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-03-01)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 FactoryWR(S_FRAMEINFO *s_FrmInfo)
{
   uint8 i;
//   uint8 ucbuf[4];
//   uint32 temp;
   uint16 uiCurrent;;
  guc_CoverStatus=1;
    memcpy(s_FrmInfo->uiID.byte,s_FrmInfo->ucbuf,4);   //���ݱ�ʶ

    if(s_FrmInfo->uiID.lword==0xFE005500)
    {
        if((guc_CoverStatus==1)
           ||(guc_FactoryType==FactorMd))              //�����״̬�����Խ��볧��״̬  
        {
            guc_FactoryType=FactorMd;
            SysE2ParaSetManage(EEP_PRODUCE,(uint8*)&guc_FactoryType,1);     //д�볧��״̬
            BE_ReadP(EEP_PRODUCETM,(uint8*)&gui_DyProDmOut,0x02);           //����ʱ��
            //BE_WriteP(EEP_PRODUCELEFTTM,(uint8*)&gui_DyProDmOut,0x02);      //����ʣ��ʱ��
            s_FrmInfo->ucLen = 0;
            return Ret_OK;
            
        }else
        {    
            s_FrmInfo->ucErr.word = ComErrBit_ErrPw;            //û���ҵ����ݱ�ʶ
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
        }
    }

    if(guc_FactoryType!=FactorMd)
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;                //û���ҵ����ݱ�ʶ
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }

    if(s_FrmInfo->uiID.lword==0xFE00AA00)
    {
        gui_DyProDmOut=0;
        guc_FactoryType=0;
        //BE_WriteP(EEP_PRODUCELEFTTM,(uint8*)&gui_DyProDmOut,0x02);      //����ʣ��ʱ��
        SysE2ParaSetManage(EEP_PRODUCE,(uint8*)&guc_FactoryType,1);     //д�볧��״̬
    }else if(s_FrmInfo->uiID.lword==0x4F520000)                         //�Զ�У�� 100%Ib
    {   
        //BE_WriteP(EEP_AUTOCAL, s_FrmInfo->ucbuf + 4, 27);
        EnyB_AutoCheck();
    }else if(s_FrmInfo->uiID.lword==0x4F520001)                         //�Զ�У��  100%Ib 0.5L Ĭ��PHCCtrl1��д0
    {   
       // BE_WriteP(EEP_AUTOCAL, s_FrmInfo->ucbuf + 4, 27);
        EnyB_AutoChkAngle();
    }
  else if(s_FrmInfo->uiID.lword==0x4F520002)     //�Զ�У��  1%Ib 1.0
  {
      //BE_WriteP(EEP_AUTOCAL, s_FrmInfo->ucbuf + 4, 27);
      EnyB_AutoChkPW2();
  }
    else if(s_FrmInfo->uiID.lword==0x4F520003)                          //���ߵ���У��
    {
        uiCurrent=Uint8_To_Uint16(s_FrmInfo->ucbuf + 4);
        EnyB_AutoChkInrms(uiCurrent);
    }
    else if(s_FrmInfo->uiID.lword==0x2055FF00)                          //��ʼ��У�����
    {
        SetCRCWord(guc_InitWd);
        guc_RTCSave = 0x55;
        gui_RefreshEvent|=flgEtPara_Init;
    }
    else if(s_FrmInfo->uiID.lword==0x2055FF55)     //��ʼ��У�����,����RTC
    {
        SetCRCWord(guc_InitWd);
        guc_RTCSave = 0x00;
        gui_RefreshEvent|=flgEtPara_Init;
    }
    else if(s_FrmInfo->uiID.lword==0x20000001)                          //���ȳ�ʼ��
    {
        //b0 04//����
        //���ѹ
        //�����
       
//        memset((uint8*)&gs_JbPm.ui_MeterC,0,sizeof(S_JBPM));                //��ȫ������
//        memcpy((uint8*)&gs_JbPm.ui_MeterC,s_FrmInfo->ucbuf+4,6);            //����,���ѹ������� 
//        memcpy((uint8*)&gs_JbPm.RacEGYTH,s_FrmInfo->ucbuf+10,28);           //�й������ơ�Ǳ��������
//        //ģ����ƼĴ���0��1��2����ͨ�˲���ϵ���Ĵ��� �������ƼĴ���0  
//        //      memcpy((uint8*)&gs_JbPm.RacANCtrl0,s_FrmInfo->ucbuf+26,20);         //ģ����ƼĴ���0��1��2����ͨ�˲���ϵ���Ĵ��� �������ƼĴ���0
//        memcpy((uint8*)&gs_JbPm.gs_JBA.RacMTPARA1,s_FrmInfo->ucbuf+38,4);   //A��������ƼĴ���1 
//        memcpy((uint8*)&gs_JbPm.gs_JBB.RacMTPARA1,s_FrmInfo->ucbuf+38,4);   //B��������ƼĴ���1 
//        memcpy((uint8*)&gs_JbPm.gs_JBC.RacMTPARA1,s_FrmInfo->ucbuf+38,4);   //C��������ƼĴ���1 
//        memcpy((uint8*)&gs_JbPm.ul_PG,s_FrmInfo->ucbuf+42,12);              //������ʾϵ������ѹ��ʾϵ����������ʾϵ��
//        gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);         // У�������CRC���
//        SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));         //д��E2��
       

//      memset((uint8*)&gs_GiantSCP.ul_Adjust,0,6);
//      SysE2ParaSetManage(EEP_DDLXZ,(uint8*)&gs_GiantSCP.ul_Adjust,6);

        gui_RefreshEvent |= flgEtPara_EnyBottom;
    }
    else if(s_FrmInfo->uiID.lword==0x20000003)                                  //���ʱȲ�ֵ
    {
//        gs_JbPm.gs_JBA.RacWAPT=Uint8_To_Uint32(s_FrmInfo->ucbuf+4);             //�й��Ȳ�ֵ
//        gs_JbPm.gs_JBB.RacWAPT=Uint8_To_Uint32(s_FrmInfo->ucbuf+8);             //�й��Ȳ�ֵ
//        gs_JbPm.gs_JBC.RacWAPT=Uint8_To_Uint32(s_FrmInfo->ucbuf+12);            //�й��Ȳ�ֵ
//        gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);             // У�������CRC���
//        SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));        //д��E2��

        gui_RefreshEvent |= flgEtPara_EnyBottom;
      
    }
    else if(s_FrmInfo->uiID.lword==0x20000004)                          //��ѹ�����Ȳ�ֵ
    {
//        gs_JbPm.gs_JBA.RacWARTU=Uint8_To_Uint32(s_FrmInfo->ucbuf+4);             //�й��Ȳ�ֵ
//        gs_JbPm.gs_JBA.RacWARTI=Uint8_To_Uint32(s_FrmInfo->ucbuf+8);            //�й��Ȳ�ֵ
//
//        gs_JbPm.gs_JBB.RacWARTU=Uint8_To_Uint32(s_FrmInfo->ucbuf+12);             //�й��Ȳ�ֵ
//        gs_JbPm.gs_JBB.RacWARTI=Uint8_To_Uint32(s_FrmInfo->ucbuf+16);            //�й��Ȳ�ֵ
//
//        gs_JbPm.gs_JBC.RacWARTU=Uint8_To_Uint32(s_FrmInfo->ucbuf+20);             //�й��Ȳ�ֵ
//        gs_JbPm.gs_JBC.RacWARTI=Uint8_To_Uint32(s_FrmInfo->ucbuf+24);            //�й��Ȳ�ֵ
//        
//        gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);     // У�������CRC���
//        SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));         //д��E2��

        gui_RefreshEvent |= flgEtPara_EnyBottom;

    }
    else if(s_FrmInfo->uiID.lword==0x20000005)                          //���ߵ����Ȳ�ֵ
    {
        //gs_JbPm.RacWARTM=Uint8_To_Uint32(s_FrmInfo->ucbuf+4);            //�й��Ȳ�ֵ
//        gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);     // У�������CRC���
//        SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));         //д��E2��
                                                                        //
        gui_RefreshEvent |= flgEtPara_EnyBottom;
    }
    else if(s_FrmInfo->uiID.lword==0x20000006)                          //A�ǲ�
    {
//        uint32 JbJc;
//        JbJc=Uint8_To_Uint16(s_FrmInfo->ucbuf+4);                       //�ǲ�
//        JbJc<<=8;
//        gs_JbPm.gs_JBA.RacMTPARA1&=0xfffB00ff;                          //�ǲ�λ����
//        gs_JbPm.gs_JBA.RacMTPARA1|=JbJc;
//        
//        JbJc=Uint8_To_Uint16(s_FrmInfo->ucbuf+6);                       //�ǲ�
//        JbJc<<=8;
//        gs_JbPm.gs_JBB.RacMTPARA1&=0xfffB00ff;                          //�ǲ�λ����
//        gs_JbPm.gs_JBB.RacMTPARA1|=JbJc;
//
//        JbJc=Uint8_To_Uint16(s_FrmInfo->ucbuf+8);                       //�ǲ�
//        JbJc<<=8;
//        gs_JbPm.gs_JBC.RacMTPARA1&=0xfffB00ff;                          //�ǲ�λ����
//        gs_JbPm.gs_JBC.RacMTPARA1|=JbJc;
//        
//        gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);     // У�������CRC���
//        SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));    //д��E2��
        gui_RefreshEvent |= flgEtPara_EnyBottom;

    }
    else if(s_FrmInfo->uiID.lword==0x20000007)                          //���β���ֵ
    {
//        gs_JbPm.gs_JBA.RacWWAPT=Uint8_To_Uint32(s_FrmInfo->ucbuf+4);           //���β���ֵ
//        gs_JbPm.gs_JBB.RacWWAPT=Uint8_To_Uint32(s_FrmInfo->ucbuf+8);           //���β���ֵ
//        gs_JbPm.gs_JBC.RacWWAPT=Uint8_To_Uint32(s_FrmInfo->ucbuf+12); 
//        gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);     // У�������CRC���
//        SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM)); //д��E2��
        gui_RefreshEvent |= flgEtPara_EnyBottom;
    }
    else if(s_FrmInfo->uiID.lword==0x20000008)                         //�����У������
    {
        //gs_GiantSCP.ui_CRC=do_CRC(s_FrmInfo->ucbuf+4,4);                //����CRC
        //gs_GiantSCP.ul_Adjust = Uint8_To_Uint32(s_FrmInfo->ucbuf+4); 
        //SysE2ParaSetManage(EEP_DDLXZ,(uint8*)&gs_GiantSCP.ul_Adjust,6);
        gui_RefreshEvent |= flgEtPara_EnyBottom;
    }
    else if(s_FrmInfo->uiID.lword==0x20000009)                          //�޹����β���ֵ
    {
//        gs_JbPm.gs_JBA.RacREWWAPT=Uint8_To_Uint32(s_FrmInfo->ucbuf+4);  //���β���ֵ
//        gs_JbPm.gs_JBB.RacREWWAPT=Uint8_To_Uint32(s_FrmInfo->ucbuf+8);  //���β���ֵ
//        gs_JbPm.gs_JBC.RacREWWAPT=Uint8_To_Uint32(s_FrmInfo->ucbuf+12); 
//        gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);     // У�������CRC���
//        SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM)); //д��E2��
        gui_RefreshEvent |= flgEtPara_EnyBottom;
    }
    else if(s_FrmInfo->uiID.lword==0x20000030)                         //RTC���ݴ�FLASH���µ�E2��
    {
        gui_RefreshEvent |= flgEtPara_RtcParaFsToE2;
    }
    else if(s_FrmInfo->uiID.lword==0x20AAAA00)//����
    {
        if(gs_DMACheck.ucNeedOpen == true)
        {
            WriteBronco(0x3f,0xC01A);  //����
            gs_DMACheck.ucNeedOpen = false;
        }
        else
        {
            return Ret_Err;
        }
    }
    else if(s_FrmInfo->uiID.lword==0x20AAAA01)
    {
        if(gs_DMACheck.ucNeedOpen == false)
        {
            WriteBronco(0x00,0xC01A);  //�ر�
            gs_DMACheck.ucNeedOpen = true;
        }
        else
        {
            return Ret_Err;
        }
    }
    else
    {
        for(i=0;i<sizeof(gs_E2RwCtrTalbe)/sizeof(Gs_E2RWCtr);i++)     //У�����
        {
            if(s_FrmInfo->uiID.lword==gs_E2RwCtrTalbe[i].ulId)
            {
                //SysE2ParaSetManage(gs_E2RwCtrTalbe[i].E2Addr, s_FrmInfo->ucbuf+4,gs_E2RwCtrTalbe[i].ucLen);
                BE_WriteP(EEP_JBTOTAL,(uint8*)s_FrmInfo->ucbuf+4,sizeof(S_JBPM));  
              break;
            }
        }

        if(i==0)
        {
            gui_RefreshEvent |= flgEtPara_EnyBottom;
        }
        else if(0<i&&i<=5)
        {
            gui_RefreshEvent |=  flgEtPara_RTC;     //ˢ��RTC����
        }
        else if(i>5)
        {

            gui_RefreshEvent |=  (flgEtPara_Config+flgEtPara_Show+flgEtPara_PowDir);  //ˢ�����ò���
        }


    }
    s_FrmInfo->ucLen = 0;
    return Ret_OK;
}

#define BYTE(num, n)        *((uint8  *)&(num) + (n))
/*=========================================================================================\n
* @function_name: MidFindID
* @function_file: DLT_645_2007.c
* @����: ��ֵ������ID
*
*
* @����:
* @param:id
* @param:tab
* @param:num
*
* @����:
* @return: uint8
* @����:   lwb (2012-03-01)
* @��ע:
*-------------------------------------------------------------------------------------------
* @�޸���:
* @�޸�����:
===========================================================================================*/
uint8 MidFindID(Word32 id, S_CORETAB  * tab, uint8 num)
{
    uint32  CmpID;
    uint8  ucMin = 0;
    uint8  ucMax = num - 1;
    uint8  ucMid;

    if(id.byte[3]==0x00)                                //����
    {
        if((id.lword&0xFF00ffff)==0x0000ffff)           //û���ҵ����ݱ�ʶ
        {
            return 0xFF;
        }

        if(id.byte[1]!=0xFF
           &id.byte[1]>=0x05)
        {
            return 0xFF;
        }
        
        if((id.byte[2]!=0x0c)
           &&(id.byte[2]!=0x0b))
        {
            id.byte[1]=0x00;
        }
    }else if(id.byte[1]==0xFF)                          //����Ǽ����Ļ�
    {
        id.byte[1]=0x00;
    }

    if(id.lword < tab->uiID)
    {                                                   //���������ݱ��е�һ��ID����0ʱ,��ôuiIDС�����ID���п��ܳ���
        return 0xFF;
    }

    while(1)
    {
        ucMid = (ucMin + ucMax) >> 1;                   //�൱�ڳ���2ȡ�м�ֵ

        CmpID = (tab + ucMid)->uiID;

        if (id.lword == CmpID)                          //�ҵ��������
        {
            break;
        }
        else if(id.lword > CmpID)                       //�������ݱ��ҵ������ݴ�
        {
            if (ucMid == ucMax)                         //�Ѿ�����
            {
                break;
            }
            ucMin = ucMid + 1;
        }
        else                                            //�������ݱ��ҵ�������С
        {
            if (ucMid == ucMin)                         //�Ѿ�����
            {
                if(ucMid > 0)
                {
                    ucMid--;
                }
                break;
            }
            ucMax = ucMid - 1;
        }
    }

    if((id.byte[0] <= (BYTE((tab + ucMid)->uiID,0)+(tab + ucMid)->ucTotal0))
       &&(id.byte[1] <= (BYTE((tab + ucMid)->uiID,1)+(tab + ucMid)->ucTotal1))
       &&(id.byte[2] <= (BYTE((tab + ucMid)->uiID,2))))
    {
        return ucMid;
    }

    return 0xFF;
}
/*=========================================================================================\n
* @function_name: ChangePassWord
* @function_file: DLT_645_2007.c
* @����: �޸�����
* 
* 
* @����: 
* @param:s_FrmInfo  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-05-15)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 ChangePassWord(S_FRAMEINFO *s_FrmInfo)
{
    //�ж����ַ�Ƿ���ȷ
    if(s_FrmInfo->ucAtb != Const_Addr_Normal)                //������ַ
    {
        s_FrmInfo->ucReply = 0;                  //����Ҫ�ظ���
        return Ret_OK;
    }
    memcpy(s_FrmInfo->uiID.byte,s_FrmInfo->ucbuf,4);        //�������ݱ�ʶ
    if(guc_FactoryType!=FactorMd)                           //���ڳ���ģʽ
    {
       if((s_FrmInfo->ucbuf[8]!=0x02)
          &&(s_FrmInfo->ucbuf[8]!=0x04)
            )
       {
            s_FrmInfo->ucErr.word = ComErrBit_Other;             //δ��Ȩ
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
       }else  if((s_FrmInfo->ucbuf[8]==0x02) &&(s_FrmInfo->uiID.lword!=0x04000c03)
           ||(s_FrmInfo->ucbuf[8]==0x04)&&(s_FrmInfo->uiID.lword!=0x04000c05))
        {
            s_FrmInfo->ucErr.word = ComErrBit_Other;             //δ��Ȩ
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
        }
 
        if((!(gui_SystemState & flgStSys_Program)) 
           ||(s_FrmInfo->ucbuf[4] > s_FrmInfo->ucbuf[8]))   //������ڱ��״̬����������Ⱦ�����ȼ���
        {
            s_FrmInfo->ucErr.word = ComErrBit_ErrPw;             //δ��Ȩ
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
        }

        s_FrmInfo->ucLevel = JudgePassword(s_FrmInfo->ucbuf + 4);
        if(s_FrmInfo->ucLevel == 0xFF)                           //�ж�Ȩ��
        {
            s_FrmInfo->ucAtb = Const_Atb_Pswd_Err;
            s_FrmInfo->ucErr.word = ComErrBit_ErrPw;             //�������
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
        }
    } 
    SysE2ParaSetManage(EEP_645PASSWD1 + (s_FrmInfo->ucbuf[8]) * 3, s_FrmInfo->ucbuf + 9, 3);//д��������
    memcpy(s_FrmInfo->ucbuf,s_FrmInfo->ucbuf+8,4);
    s_FrmInfo->ucLen = 4;
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: ChangeBode
* @function_file: DLT_645_2007.c
* @����: ���Ĳ�����
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
uint8 ChangeBode(S_FRAMEINFO *s_FrmInfo)
{

    uint8 i;
    uint8 mode;
    if(s_FrmInfo->ucAtb!=Const_Addr_Normal)      //������ò�����ͨ��ַ���򷵻ش���
    {
        s_FrmInfo->ucReply = 0;                  //����Ҫ�ظ���
        return Ret_OK;
    }


    mode = s_FrmInfo->ucbuf[0];             //���ĵ�ģʽ,��һ���ֽ��ǳ���

    for(i = 2; i < 6; i++)                  //�ж��������Ƿ���ȷ
    {
        if(mode == BIT8MAP[i])
        {
            break;
        }
    }

    if(i >= 6 || s_FrmInfo->ucPort==Port_Uart2)
    {   
        s_FrmInfo->ucErr.word = ComErrBit_ChBod;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }
 
    //�ж�ͨ����485���Ǻ���
    if(s_FrmInfo->ucPort==Port_Uart3)               //485ͨ��
    {                                               //����485������ 
        SysE2ParaSetManage(EEP_UARTBDM,&mode,1);    //ͨ�ſ�1ͨ������������

    }
//    else if(s_FrmInfo->ucPort==Port_Uart2)         //����ͨ���Ļ������ú��Ⲩ����
//    {
//        SysE2ParaSetManage(EEP_IARBDM,&mode,1);    //�����ͺ�����ͨ������������
//    }
    
    guc_BodeDely=40;
    s_FrmInfo->ucLen = 1;
    return Ret_OK;
} 
/*=========================================================================================\n
* @function_name: ClearMeter
* @function_file: DLT_645_2007.c
* @����: ����
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
uint8 ClearMeter(S_FRAMEINFO *s_FrmInfo)
{
    if(s_FrmInfo->ucAtb != Const_Addr_Normal)               //������ַ
    {
        s_FrmInfo->ucReply = 0;                             //����Ҫ�ظ���
        return Ret_OK;
    }
 
    if(guc_FactoryType!=FactorMd)                           //���ڳ���ģʽ
    {
        s_FrmInfo->ucLevel = JudgePassword(s_FrmInfo->ucbuf);

        if((!_IsSecuProduct) 
           || (s_FrmInfo->ucLevel != 0x02))                 //û�б�̼������벻��
        {
            s_FrmInfo->ucErr.word = ComErrBit_ErrPw;        //Ȩ�޲��㷵��
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
        }
    }

    gui_SystemEvent|=flgEtGlb_ClearMeter;                   //����������Ϣ
    s_FrmInfo->ucLen = 0;
    memcpy(guw_UseCode.byte,s_FrmInfo->ucbuf+4,4);    //�����ߴ���
    //���Ʋ����ߴ���
    SetCRCWord(guc_ClearWd);
    return Ret_OK;
    
}
 
/*=========================================================================================\n
* @function_name: ClearEvent
* @function_file: DLT_645_2007.c
* @����: ���¼�
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
uint8 ClearEvent(S_FRAMEINFO *s_FrmInfo)
{
    Word32 ID;
    uint8 i;

    if(s_FrmInfo->ucAtb != Const_Addr_Normal)
    {
        s_FrmInfo->ucReply = 0;                  //����Ҫ�ظ���
        return Ret_OK;
    }

    memcpy(guw_UseCode.byte,s_FrmInfo->ucbuf+4,4);    //�����ߴ��� 
    memcpy(ID.byte,s_FrmInfo->ucbuf+8,4);


    if(guc_FactoryType!=FactorMd)                           //���ڳ���ģʽ
    {
        s_FrmInfo->ucLevel = JudgePassword(s_FrmInfo->ucbuf);

        if((!_IsSecuProduct)
           ||(0x02 != s_FrmInfo->ucLevel))                  //û�б�̼������벻��
        {
            s_FrmInfo->ucErr.word = ComErrBit_ErrPw;          //Ȩ�޲��㷵��
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
        }
    }

    if(ID.byte[0] != 0xFF)                              //�¼��������ݱ�ʶID0�϶���FF
    {
        s_FrmInfo->ucErr.word |= ComErrBit_ErrPw;            //Ȩ�޲���
        s_FrmInfo->ucLen = 1;                           //������Ϣ��һ���ֽ�
    }
    else
    {
        for(i = 0; i < CntgClrE; i++)                   //Ѱ�����¼��ı�־
        {
            if(ID.lword == gs_ClearEvent[i].ID)         //�����Ƿ���ȣ���Ⱦ����ҵ���
            {
                break;
            }
        }

        if(i >= CntgClrE)                               //������û���ҵ�
        {
            s_FrmInfo->ucErr.word |= ComErrBit_Nodata;
            s_FrmInfo->ucLen = 1;
        }
        else
        {
            guc_ClearType = gs_ClearEvent[i].type;          //���¼�������

            gui_SystemEvent|=flgEtGlb_ClearEv;              //���͵�����¼���Ϣ
            s_FrmInfo->ucLen = 0;
        }
    }
    return Ret_OK;
}
 
/*=========================================================================================\n
* @function_name: BroadcastTime
* @function_file: DLT_645_2007.c
* @����: �㲥Уʱ
* 
* 
* @����: 
* @param:s_FrmInfo  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-05-10)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 BroadcastTime(S_FRAMEINFO *s_FrmInfo)
{
    Word16 W16_JSR[3];
    uint8 i;
    if(s_FrmInfo->ucAtb != Const_Addr_Broadcast)        //����ǹ㲥��ַ�Ļ�
    {
        return Ret_Err;
    }
    //���ȶ�ȡ3��������

    BE_ReadP(EEP_CBR1,W16_JSR[0].byte, 6);//��ȡ3��������


    if(gs_DateTime.ucHour==0x23) //��ҹ0��ǰ��5MIN����У׼ʱ��
    {
        if(gs_DateTime.ucMinute>=0x55)
        {
            return Ret_Err;
        }
    }

    if(gs_DateTime.ucHour==0)   //��ҹ0��ǰ��5MIN����У׼ʱ��
    {
        if(gs_DateTime.ucMinute<=0x04)
        {
            return Ret_Err;
        }
    }

    //������ǰ��5min������Уʱ
    for(i=0;i<3;i++)                            //����������
    {
        //�жϵ�ǰ
        if(W16_JSR[i].byte[1]==gs_DateTime.ucDay)   //����Ҫ��Ҫһ�£�С��ģʽ
        {
            if(W16_JSR[i].byte[0]==gs_DateTime.ucHour)
            {
                if(gs_DateTime.ucMinute<=0x04)
                {
                    return Ret_Err;
                }
            }else if(((APIByteBCD2HEX(gs_DateTime.ucHour))+1)==APIByteBCD2HEX(W16_JSR[i].byte[0]))        
            {   //23�������Ѿ��ж�
                if(gs_DateTime.ucMinute>=0x55)
                {
                    return Ret_Err;
                }
            }else
            {
                continue;
            }
        }else
        {
            continue;
        }
    }

    S_TIMEDATA time;
 
    memcpy(&time.ucSecond,s_FrmInfo->ucbuf,6);


    if(Ret_OK != ValidTime(s_FrmInfo->ucbuf, 2))    //����ʱ���Ƿ�Ϸ���S mm H D MM YY
    {
        return Ret_Err;
    }
    //�����ж��Ƿ���5min�ڣ���һ�ַ����ǣ������ж��������Ƿ���ȣ���Ⱥ󣬼���ʱ�����Ƿ����5min
    //��2�з����ǣ���������ʱ��������Ƿ����5min��Ŀǰ������õ�2�з���
//  if(ApiBufCmp(gs_DateTime.ucYear,s_FrmInfo->ucbuf,3)!=CMP_EQU)
//  {
//
//
//  }

    if(CalcTimeGap(&time, &gs_DateTime) > 300)          //�����ǲ�����5��������
    {
        return Ret_Err;                                 //����������������
    }
    
    if(gui_SystemState &flgStSys_IsTodayJS)             //��������Ѿ�У�������Ͳ�����У��
    {
        return Ret_Err;                                 //����������������
    }else
    {
        //�㲥Уʱ�������,�������ڲ����,�������ﲻ��Ҫ����
        SetSysRTCClock(s_FrmInfo->ucbuf, Const_YYMMDDhhmmss);
        gui_SystemState |= flgStSys_IsTodayJS;          //��ʾ�����Ѿ����й��㲥Уʱ�ˣ�һ��ֻ����Уʱһ��
        gui_RefreshEvent|= flgEtPara_Fee;               //ˢ�·��ʲ���
        //guc_Timechange = 0x55;
    }
    s_FrmInfo->ucReply = 0;                             //����Ҫ�ظ�*/
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: Freeze
* @function_file: DLT_645_2007.c
* @����: ���ᣬ���ݹ�Լ��������Ӧ�Ķ���
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
uint8 Freeze(S_FRAMEINFO *s_FrmInfo)
{

    if((s_FrmInfo->ucAtb != Const_Addr_Broadcast)
       &&(s_FrmInfo->ucAtb != Const_Addr_Normal))           //�㲥����Ҫ�ظ�
    {
        s_FrmInfo->ucReply = 0;                             //����Ҫ�ظ���
        return Ret_OK;
    }


 
    if((s_FrmInfo->ucbuf[0] == 0x99) && (s_FrmInfo->ucbuf[1] == 0x99) &&
       (s_FrmInfo->ucbuf[2] == 0x99) && (s_FrmInfo->ucbuf[3] == 0x99))
    {        
        //����˲ʱ����
        guc_DjCheckFlg|=flgInsDjEvent;
    }
    else
    {

        if(JudgeIsNum(s_FrmInfo->ucbuf,4)==Const_Data_OK)
        {
            if(s_FrmInfo->ucbuf[3]!=0x99)        //��
            {
                if(s_FrmInfo->ucbuf[3]<=0x12     //��       
                   &&s_FrmInfo->ucbuf[3]>=0x01)
                {

                }else
                {
                    s_FrmInfo->ucErr.word |= ComErrBit_Other;            //����֡����
                    s_FrmInfo->ucLen = 1;
                    return Ret_OK;
                }
            }
            
            //2 ��   1 ʱ 0 ��
            if(s_FrmInfo->ucbuf[2]!=0x99)
            {
                if(s_FrmInfo->ucbuf[2]<=0x28     //��       
                   &&s_FrmInfo->ucbuf[2]>=0x01)
                {

                }else
                {
                    s_FrmInfo->ucErr.word |= ComErrBit_Other;            //����֡����
                    s_FrmInfo->ucLen = 1;
                    return Ret_OK;
                }
            }
    
            if(s_FrmInfo->ucbuf[1]!=0x99)
            {
                if(s_FrmInfo->ucbuf[1]<=0x23                        //ʱ      
                   )
                {

                }else
                {
                    s_FrmInfo->ucErr.word |= ComErrBit_Other;       //����֡����
                    s_FrmInfo->ucLen = 1;
                    return Ret_OK;
                }
            }

            if(s_FrmInfo->ucbuf[0]!=0x99)
            {
                if(s_FrmInfo->ucbuf[0]<=0x59                        //��
                   )
                {

                }else
                {
                    s_FrmInfo->ucErr.word |= ComErrBit_Other;       //����֡����
                    s_FrmInfo->ucLen = 1;
                    return Ret_OK;
                }
            }

        }else
        {
            s_FrmInfo->ucErr.word |= ComErrBit_Other;            //����֡����
            s_FrmInfo->ucLen = 1;
            return Ret_OK;
        }
        //�ж�һ�¸�ʽ�Ƿ���ȷ
        SysE2ParaSetManage(EEP_DSFRZ,s_FrmInfo->ucbuf,4);    //д�붨ʱ����ʱ��
    }

    if(s_FrmInfo->ucAtb == Const_Addr_Broadcast)        //�㲥����Ҫ�ظ�
    {
        s_FrmInfo->ucReply = 0;                         //����Ҫ�ظ�
    }
    else if((s_FrmInfo->ucAtb == Const_Addr_Normal) ||
        (s_FrmInfo->ucAtb == Const_Addr_Any))
    {
        s_FrmInfo->ucLen = 0;
    }
    else
    {
        s_FrmInfo->ucErr.word|= ComErrBit_Other;            //��ַ����
        s_FrmInfo->ucLen = 1;
    }
    return Ret_OK;
}

const uint16   SqSdTab[2]=
{
    EEP_SQCNT,EEP_FSDCNT,

};

/*=========================================================================================\n
* @function_name: RwSqSd
* @function_file: DLT_645_2007.c
* @����: ��д��ʱ��ʱ��
*
*
* @����:
* @param:s_FrmInfo
* @param:CTable
*
* @����:
* @return: uint8
* @����:   lwb (2012-03-20)
* @��ע:
*-------------------------------------------------------------------------------------------
* @�޸���:
* @�޸�����:
===========================================================================================*/
/*uint8 RwSqSd(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{
    uint16 addr;
    uint8 ucNum;
    uint8 ucType;
#ifdef SQSDPRLOG
    uint8 DataID;
#endif

    if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    //��������ҳΪ��ַ����Ȼʱ�α�ֻҪ42���ֽڣ�����ÿ��ʱ�α�ռ��һҳ
    addr = CTable->pAddr + (s_FrmInfo->uiID.byte[0]) *64;
    
    BE_ReadP(SqSdTab[CTable->uiOther-1],&ucNum,1);
    if(ucNum>14)
    {
        ucNum=14;
    }
    if(s_FrmInfo->ucCmd == 0x11)                //����Ƕ����ݵĻ�
    {
        //�������ʱ�α�Ļ�������Ҫ��ȡÿ���л���

        BE_ReadP(addr,s_FrmInfo->ucbuf, ucNum*3);
        s_FrmInfo->ucLen = ucNum*3;             //���ݳ���

    }else
    {
        if(CTable->uiOther==0x01)       //ʱ��
        {
            ucType=SqDtFm;
        }else                           //ʱ��
        {
            ucType=SdDtFm;
        }

        if(s_FrmInfo->ucLevel!=0x98)
        {
            s_FrmInfo->ucLen-=12;
        }
        //�����ж����ݳ����Ƿ���3��������
        if((s_FrmInfo->ucLen)%3==0)
        {
            if((s_FrmInfo->ucLen)<=42)         //��Ҫ���ʱ�α����ͷ����л������ж�
            {
                //�ж��Ƿ���BCD,�ж��Ƿ�����������ж��Ƿ��Ƿ񳬹�15min
                s_FrmInfo->ucErr.word=ComCheckFormat(s_FrmInfo->ucbuf,ucType,s_FrmInfo->ucLen);
                if(s_FrmInfo->ucErr.word==0)
                {  
                #ifdef SQSDPRLOG
                    DataID=SeekDataID(s_FrmInfo->uiID);               //��ȡID
                #endif    
                    memcpy(s_FrmInfo->ucbuf+s_FrmInfo->ucLen,s_FrmInfo->ucbuf+s_FrmInfo->ucLen-3,42-s_FrmInfo->ucLen);//��ȫ
                    //BE_WriteP(addr,s_FrmInfo->ucbuf, 42);
                    SysE2ParaSetManage(addr,s_FrmInfo->ucbuf, 42);
                    s_FrmInfo->ucLen =0;                //���ݳ���
                #ifdef SQSDPRLOG
                    RSDJLlog(s_FrmInfo->uiUserCode.lword,DataID);     //��ʱ�κ�ʱ����̼�¼
                #endif
                    gui_RefreshEvent |= flgEtPara_Fee;  //����ˢ�·��ʲ���
                    return Ret_OK;
                }else
                {
                    s_FrmInfo->ucLen = 1;
                    return Ret_OK;
                }
            }else
            {
                if(s_FrmInfo->uiID.byte[0]==0x00)
                {
                    s_FrmInfo->ucErr.word = ComErrBit_YearQ;
                }else
                {
                    s_FrmInfo->ucErr.word = ComErrBit_TimeD;
                }

                s_FrmInfo->ucLen = 1;
                return Ret_OK;
            }
        }
        
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }
    return Ret_OK;
}*/
/*=========================================================================================\n
* @function_name: ComReadRunCode
* @function_file: DLT_645_2007.c
* @����: ��ȡ�������״̬��
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-06-13)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
/*uint8 ComReadRunCode(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{
    uint8 ucOffSet;
    uint8 *pData;

    if(s_FrmInfo->ucCmd==0x14)     
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }else if(s_FrmInfo->ucCmd==0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }


    if(s_FrmInfo->uiID.byte[0]==0xFF)
    {
        memcpy(s_FrmInfo->ucbuf,(uint8*)&gs_MeterState.ui_MeterState1.Word,CTable->ucLen);
        memset(s_FrmInfo->ucbuf+CTable->ucLen,0xFF,4);
        s_FrmInfo->ucLen = CTable->ucLen+4;                           //���ݳ���
        return Ret_OK;
    }else
    {
        ucOffSet=s_FrmInfo->uiID.lword-CTable->uiID;
        pData= (uint8*)(&gs_MeterState.ui_MeterState1.Word+ucOffSet);
        memcpy(s_FrmInfo->ucbuf,pData,CTable->ucLen);
    }
    s_FrmInfo->ucLen = CTable->ucLen;                           //���ݳ���
    return Ret_OK;
}*/

 
/*=========================================================================================\n
* @function_name: SecPulsCtrl
* @function_file: DLT_645_2007.c
* @����: �๦�ܶ��ӵĿ���
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
/*uint8 SecPulsCtrl(S_FRAMEINFO *s_FrmInfo)
{
    if((s_FrmInfo->ucAtb!=Const_Addr_Normal)
       &&(s_FrmInfo->ucAtb!=Const_Addr_Broadcast))      //������ò�����ͨ��ַ���򷵻ش���
    {
        s_FrmInfo->ucReply = 0;                  //����Ҫ�ظ���
        return Ret_OK;
    }

    if(s_FrmInfo->ucbuf[0] == 0x00)              //����ź�Ϊ������
    {
        SwichPluseOutType(0);
        gs_SysRunSt.PluseType=false;
    }
    else if(s_FrmInfo->ucbuf[0] == 0x02)        //����ź�Ϊʱ��Ͷ��
    {
        SwichPluseOutType(2);
        gs_SysRunSt.PluseType=true;
    } 
    else                                        //������Ч
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;     //������
    }

    s_FrmInfo->ucLen = 1;

    if(s_FrmInfo->ucAtb == Const_Addr_Broadcast)//�㲥��ַ
    {
        s_FrmInfo->ucReply = 0;                 //����Ҫ�ظ���
    }
    return Ret_OK;
}*/
/*=========================================================================================\n
* @function_name: ReadAccuEgy
* @function_file: DLT_645_2007.c
* @����: ��ȡ�¡����ۼƵ���   other  �� 0x00    ��ǰ�¶�
*                                       0x01    ��1�¶�
*                                       0x10    ��ǰ���
*                                       0x11    ��1���
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-05-17)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
/*uint8 ReadAccuEgy(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{
    Word32 Curdt[3],Lstdt[3],data1;
    int16 iDelta[2];
    uint16 ui_Other=CTable->uiOther;
    if(s_FrmInfo->ucCmd == 0x14)//д
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    if(ui_Other&0x01)                                           //��ʷ�ۼƵ�������1�¶�
    {
        DataProcRead2Slice(LSDLID,1,20,4,Lstdt[1].byte);        //��ȡ��2�¶��������
        DataProcRead2Slice(LSDLID,1,40,4,Lstdt[2].byte);        //��ȡ��2�¶ȷ������
        Lstdt[0].lword = Lstdt[1].lword + Lstdt[2].lword;       //��2����+��
        DataProcRead2Slice(LSDLID,0,20,4,Curdt[1].byte);        //��ȡ��1�¶��������
        DataProcRead2Slice(LSDLID,0,40,4,Curdt[2].byte);        //��ȡ��1�¶ȷ������
        Curdt[0].lword = Curdt[1].lword + Curdt[2].lword;       //��1����+��
    }else                       //��ǰ�¶ȣ���ȵ���
    {
        DataProcRead2Slice(LSDLID,0,20,4,Lstdt[1].byte);        //��ȡ��1�¶��������
        DataProcRead2Slice(LSDLID,0,40,4,Lstdt[2].byte);        //��ȡ��1�¶ȷ������
        Lstdt[0].lword = Lstdt[1].lword + Lstdt[2].lword;       //��1����+��
        iDelta[0] = Eny_GetEp1(1, 0,ABCPhy,active);                           //��ǰ�����������
        iDelta[1] = Eny_GetEp1(2, 0,ABCPhy,active);                           //��ǰ�����������
        Curdt[0].lword = gs_EnergyA.ulP[0] + gs_EnergyA.ulN[0] + iDelta[0] + iDelta[1];//��+��
    }
    
    data1.lword=Curdt[0].lword - Lstdt[0].lword;
    if(data1.lword > 0x80000000)                    //�������С���㣬����0��
    {
        data1.lword = 0;
    }else
    {
        data1.lword/=10;
        data1.lword = Hex2BCD(data1.lword);         //bcdת��
        //data1.lword >>=4;                           //С������
    }

    memcpy(s_FrmInfo->ucbuf,data1.byte,4);
    s_FrmInfo->ucLen=CTable->ucLen;
    return Ret_OK;

}*/
/*=========================================================================================\n
* @function_name: ReadZHLYEgy
* @function_file: DLT_645_2007.c
* @����: 
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2013-03-01)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
/*uint8 ReadZHLYEgy(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{
    int16 iDelta[2];
    Word32 Curdt,data1,data2;
    uint8  ucPt;
    if(s_FrmInfo->ucCmd == 0x14)//д
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    BE_ReadB(EEP_SYZHLYDL_PT, (uint8*)&ucPt);                      //ָ��
    BE_ReadP(EEP_SYZHLYDLADDR+ucPt*4,data1.byte,4);                //��ȡ��һ���µ��ܵ���

    if(CTable->uiOther&0x01)                                        //��1�¶�����й������õ���
    {
        ucPt++;
        ucPt%=2;
        BE_ReadP(EEP_SYZHLYDLADDR+ucPt*4,data2.byte,4);           //��ȡ��2�µ��ܵ���
        Curdt.lword=(data1.lword-data2.lword);

    }else                                                           //��ǰ�¶��ܵ���
    {
        iDelta[0] = Eny_GetEp1(1, 0,ABCPhy,active);                               //��ǰ�����������
        iDelta[1] = Eny_GetEp1(2, 0,ABCPhy,active);                               //��ǰ�����������
        data2.lword = (gs_EnergyA.ulP[0] + gs_EnergyA.ulN[0] + iDelta[0] + iDelta[1]);//��+��
        Curdt.lword = data2.lword-data1.lword;
    }
    
    if(Curdt.lword > 0x80000000)                                    //�������С���㣬����0��
    {
        Curdt.lword = 0;
    }else
    {   
        Curdt.lword/=10;
        Curdt.lword = Hex2BCD(Curdt.lword);                         //bcdת��
//      Curdt.lword >>=4;                                           //С������
    }

    memcpy(s_FrmInfo->ucbuf,Curdt.byte,4);
    s_FrmInfo->ucLen=CTable->ucLen;
    return Ret_OK;
}*/
/*=========================================================================================\n
* @function_name: ReadCurEgy
* @function_file: DLT_645_2007.c
* @����: ��ȡ��ǰ��������
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-05-17)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 ReadCurEgy(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{
    int16 Eadd;
    uint32 *pEnyData,*pEnyData1;
    //Word32 data1;
    int32  data1;
    uint8 flag=0;

    if(s_FrmInfo->ucCmd == 0x14)
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

   // pEnyData=(uint32*)gs_EnergyA.lCP;                    //��ȡ�������ݽṹ�׵�ַ


    if(s_FrmInfo->ucAtb & Const_Atb_DataJC)     //����Ǽ����Ļ�
    {
        for(uint8 i = 0; i <= 4; i++)
        {
            flag=0;
            data1=*(pEnyData+s_FrmInfo->uiID.byte[2]*5+i);
             data1=*(pEnyData);
            Eadd = Eny_GetEp1(s_FrmInfo->uiID.byte[2], i,ABCPhy,active);      //ȡ��ĳ�����ʵĵ�������
            data1 += Eadd;
             data1 = gs_EnergyA.lCP + Eadd;
            if(s_FrmInfo->uiID.byte[2]==0)                      //���
            {
                if(data1>0x80000000)
                {
                    data1=-data1;
                    flag=1;
                }
            }
            data1/=10;
            data1= Hex2BCD(data1);                              //ת��ΪBCD��
            //data1 >>=4;                                         //С��λ
            if(flag)
            {
                data1|=0x80000000;
            }
            else if(s_FrmInfo->uiID.byte[2]==0)                 //��ϴ���799999.99,��0��ʼ
            {
                data1&=~0x80000000;
            }
            memcpy((uint8*)&s_FrmInfo->ucbuf[i * 4],(uint8*)&data1,4);
        }

        s_FrmInfo->ucLen = 20;
    }
    else                                        //���Ǽ���
    {                                           //ȡ��ĳ�����ʵĵ�������
       if(s_FrmInfo->uiID.byte[2] <20)
        {
            flag=0;
            guc_ShowEnergyFlag=0;  
            //data1=*(pEnyData+s_FrmInfo->uiID.byte[2]*5+s_FrmInfo->uiID.byte[1]);
            //data1=*pEnyData;
            Eadd = Eny_GetEp1(s_FrmInfo->uiID.byte[2], s_FrmInfo->uiID.byte[1],ABCPhy,active);
           // data1 += Eadd;
            data1 = gs_EnergyA.lCP + Eadd;
            //data1 = gs_UIP.ul_power;
        }
       else
        {
            //pEnyData1=(pEnyData+s_FrmInfo->uiID.byte[2]/20 +14 +s_FrmInfo->uiID.byte[1]);
            data1 = *pEnyData1;
            Eadd = Eny_GetEp1(1, 0,ABCPhy,active);
            data1 += Eadd;
             data1 = gs_EnergyA.lCP + Eadd;
            //data1 = gs_UIP.ul_power;
        }  
        
        if(s_FrmInfo->uiID.byte[2]==0)                      //���
        {
            if(data1>0x80000000)
            {
                guc_ShowEnergyFlag=0x55;
                data1=-data1;
                flag=1;
            }
        }else if(s_FrmInfo->uiID.byte[2]==0x02)     //�������
        {
            guc_ShowEnergyFlag=0x55;
        }
        data1/=10;
        data1 = Hex2BCD(data1);             //ת��ΪBCD��
        //data1 >>= 4;                        //С��λ
        if(flag)
        {
            data1|=0x80000000;
        }
        else if(s_FrmInfo->uiID.byte[2]==0)                 //��ϴ���799999.99,��0��ʼ
        {
            data1&=~0x80000000;
        }

        memcpy(s_FrmInfo->ucbuf,(uint8*)&data1,4);
        s_FrmInfo->ucLen = 4;
    }
    return Ret_OK;
}


uint8 ReadCurNEgy(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{
    int16 Eadd;
    uint32 *pEnyData,*pEnyData1;
    //Word32 data1;
    int32  data1;
    uint8 flag;

    if(s_FrmInfo->ucCmd == 0x14)
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    pEnyData=(uint32*)gs_NEnergyA.lCP;                    //��ȡ�������ݽṹ�׵�ַ


    if(s_FrmInfo->ucAtb & Const_Atb_DataJC)     //����Ǽ����Ļ�
    {
        for(uint8 i = 0; i <= 4; i++)
        {
            flag=0;
            data1=*(pEnyData+s_FrmInfo->uiID.byte[2]*5+i);
            Eadd = Eny_GetEp1(s_FrmInfo->uiID.byte[2], i,ABCPhy,reactive);      //ȡ��ĳ�����ʵĵ�������
            data1 += Eadd;
            if(s_FrmInfo->uiID.byte[2]==0)                      //���
            {
                if(data1>0x80000000)
                {
                    data1=-data1;
                    flag=1;
                }
            }
            data1/=10;
            data1= Hex2BCD(data1);                              //ת��ΪBCD��
            //data1 >>=4;                                         //С��λ
            if(flag)
            {
                data1|=0x80000000;
            }
            else if(s_FrmInfo->uiID.byte[2]==0)                 //��ϴ���799999.99,��0��ʼ
            {
                data1&=~0x80000000;
            }
            memcpy((uint8*)&s_FrmInfo->ucbuf[i * 4],(uint8*)&data1,4);
        }

        s_FrmInfo->ucLen = 20;
    }
    else                                        //���Ǽ���
    {                                           //ȡ��ĳ�����ʵĵ�������
       if(s_FrmInfo->uiID.byte[2] <20)
        {
            flag=0;
            guc_ShowEnergyFlag=0;  
            data1=*(pEnyData+s_FrmInfo->uiID.byte[2]*5+s_FrmInfo->uiID.byte[1]);
            Eadd = Eny_GetEp1(s_FrmInfo->uiID.byte[2], s_FrmInfo->uiID.byte[1],CTable->uiOther,reactive);
            data1 += Eadd;
        }
       else
        {
            pEnyData1=(pEnyData+s_FrmInfo->uiID.byte[2]/20 +14 +s_FrmInfo->uiID.byte[1]);
            data1 = *pEnyData1;
            Eadd = Eny_GetEp1(1, 0,CTable->uiOther,reactive);
            data1 += Eadd;
        }  
        
        if(s_FrmInfo->uiID.byte[2]==0)                      //���
        {
            if(data1>0x80000000)
            {
                guc_ShowEnergyFlag=0x55;
                data1=-data1;
                flag=1;
            }
        }else if(s_FrmInfo->uiID.byte[2]==0x02)     //�������
        {
            guc_ShowEnergyFlag=0x55;
        }
        data1/=10;
        data1 = Hex2BCD(data1);             //ת��ΪBCD��
        //data1 >>= 4;                        //С��λ
        if(flag)
        {
            data1|=0x80000000;
        }
        else if(s_FrmInfo->uiID.byte[2]==0)                 //��ϴ���799999.99,��0��ʼ
        {
            data1&=~0x80000000;
        }

        memcpy(s_FrmInfo->ucbuf,(uint8*)&data1,4);
        s_FrmInfo->ucLen = 4;
    }
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: ReadHisEgy
* @function_file: DLT_645_2007.c
* @����: ��ȡ��ʷ����
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-05-17)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
/*uint8 ReadHisEgy(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{
    uint32 data1;
    uint8 i;
    uint8 ucMonth;
    uint8 ucoffset;
    uint8 flag;

    if(s_FrmInfo->ucCmd == 0x14)
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    ucMonth=(s_FrmInfo->uiID.byte[0]-1);

    if(s_FrmInfo->ucAtb & Const_Atb_DataJC)     //����Ǽ����Ļ�
    {
        ucoffset=(s_FrmInfo->uiID.byte[2] *20);
        DataProcRead2Slice(LSDLID,ucMonth,ucoffset,20,XDATA_RAMZONE);
        for(i = 0; i <= 4; i++)
        {         
            flag=0;                                      //��ȡ����
            data1=*((uint32*)(XDATA_RAMZONE+i*4));
            if(s_FrmInfo->uiID.byte[2]==0)                      //���
            {
                if(data1>0x80000000)
                {
                    data1=~data1+1;
                    flag=1;
                }
            }
            data1/=10;
            data1 = Hex2BCD(data1);             //ת��ΪBCD��
            //data1 >>=4;                         //�൱�ڳ���10
            if(flag)
            {
                data1|=0x80000000;
            }
            else if(s_FrmInfo->uiID.byte[2]==0)
            {
                data1&=~0x80000000;
            }

            memcpy(s_FrmInfo->ucbuf + (i << 2), (uint8*)&data1, 4);
        }
        s_FrmInfo->ucLen = 20;
    }
    else                                        //���Ǽ���
    {
        flag=0;
        guc_ShowEnergyFlag=0;
        ucoffset=(s_FrmInfo->uiID.byte[2] * 20) + (s_FrmInfo->uiID.byte[1] << 2);

        DataProcRead2Slice(LSDLID,ucMonth,ucoffset,4,(uint8*)&data1);
        if(s_FrmInfo->uiID.byte[2]==0)                      //���
        {
            if(data1>0x80000000)
            {
                guc_ShowEnergyFlag=0x55;
                data1=~data1+1;
                flag=1;
            }
        }
        else if(s_FrmInfo->uiID.byte[2]==0x02)     //�������
        {
            guc_ShowEnergyFlag=0x55;
        }
        
        data1/=10;
        data1 = Hex2BCD(data1);             //ת��ΪBCD��
        //data1>>=4;                          //E2�������3ΪС����ͨѶҪ��2ΪС��

        if(flag)
        {
            data1|=0x80000000;
        }
        else if(s_FrmInfo->uiID.byte[2]==0)
        {
            data1&=~0x80000000;
        }

        s_FrmInfo->ucLen = 4;
                                                //������ͨѶ������ȥ
        memcpy(s_FrmInfo->ucbuf, (uint8*)&data1, 4);
    }

    return Ret_OK;   
}*/
/*=========================================================================================\n
* @function_name: ReadEnyD0Block
* @function_file: DLT_645_2007.c
* @����: ��ȡ�ַ������ݿ�
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-06-26)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
/*uint8 ReadEnyD0Block(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{
    int16 Eadd;
    uint32 *pEnyData;
    int32  data1;
    uint8 flag;
    uint8 i;
    

    if(s_FrmInfo->ucCmd == 0x14)
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    flag=0;
    pEnyData=(uint32*)gs_EnergyA.lCP;                    //��ȡ�������ݽṹ�׵�ַ
    data1=*(pEnyData+s_FrmInfo->uiID.byte[2]*5+s_FrmInfo->uiID.byte[1]);
    Eadd = Eny_GetEp1(s_FrmInfo->uiID.byte[2], s_FrmInfo->uiID.byte[1],ABCPhy,active);
    data1 += Eadd;
    if(s_FrmInfo->uiID.byte[2]==0)                      //���
    {
        if(data1>0x80000000)
        {
            data1=-data1;
            flag=1;
        }
    }

    data1/=10;
    data1 = Hex2BCD(data1);             //ת��ΪBCD��
    //data1 >>= 4;                        //С��λ

    if(flag)
    {
        data1|=0x80000000;
    }
    else if(s_FrmInfo->uiID.byte[2]==0)
    {
        data1&=~0x80000000;
    }

    memcpy(s_FrmInfo->ucbuf,(uint8*)&data1,4);

    for(i=0;i<12;i++)
    {
        flag=0;
        guc_ShowEnergyFlag=0x00;
        DataProcRead2Slice(LSDLID,i,CTable->pAddr+s_FrmInfo->uiID.byte[1]*4,4,(uint8*)&data1);
        if(s_FrmInfo->uiID.byte[2]==0)                      //���
        {
            if(data1>0x80000000)
            {
                guc_ShowEnergyFlag=0x55;
                data1=~data1+1;
                flag=1;
            }
        }
        data1/=10;
        data1 = Hex2BCD(data1);             //ת��ΪBCD��
        //data1>>=4;                          //E2�������3ΪС����ͨѶҪ��2ΪС��

        if(flag)
        {
            data1|=0x80000000;
        }
        else if(s_FrmInfo->uiID.byte[2]==0)
        {
            data1&=~0x80000000;
        }

        memcpy(s_FrmInfo->ucbuf+4+i*4,(uint8*)&data1,4);
    }

    s_FrmInfo->ucLen=52;
    return Ret_OK;
}*/



typedef struct
{
    uint8 ucFormat;
    uint16 ucParaRefFlg; 
}GS_PARAFORMAT;

const GS_PARAFORMAT   gs_ParaFormatTab[]=
{
    {InValid,       0,                  },//0
    {QhTmFm,        flgEtPara_Fee,      },//1����ʱ�����л�ʱ��//����ˢ��
    {QhCnFm,        flgEtPara_Fee,      },//2��ʱ����p��14
    {SdBCnFm,       flgEtPara_Fee,      },//3��ʱ�α���q��8
    {SqCntFm,       flgEtPara_Fee,      },//4��ʱ����(ÿ���л���) m��14
    {FeeCnFm,       flgEtPara_Fee,      },//5������k��63
    {HldCnFm,       flgEtPara_Fee,      },//6����������n��254
    {AtSwCnFm,      flgEtPara_Show,     },//7�Զ�ѭ����ʾ����
    {AtSwTmFm,      flgEtPara_Show,     },//8ÿ����ʾʱ��
    {EngDocFm,      flgEtPara_Show,     },//9��ʾ����С��λ��
    {PowDocFm,      flgEtPara_Show,     },//10��ʾ����(�������)С��λ��
    {KeySwCnFm,     flgEtPara_Show,     },//11����ѭ����ʾ����
    {MeterIdFm,     0,                  },//12���
    {EPCalcFm,      flgEtPara_Config,   },//13�й���Ϸ�ʽ��
    {IrBdFm,        0,                  },//14�����ͺ�����ͨ������������
    {RS485Fm,       0,                  },//15ͨ�ſ�1ͨ������������
    {ZxrWdFm,       flgEtPara_Fee,      },//16������������
    {ZxrSdFm,       flgEtPara_Fee,      },//17�����ղ��õ���ʱ�α��
    {JsrFm,         0,                  },//18ÿ�µ�1������
    {ZdDjTmFm,      0,                  },//19���㶳��ʱ����
    {RDjTmFm,       0,                  },//20�ն���ʱ��
    {TzTmFm,        0,                  },//21��բ��ʱʱ���ʽ
    {GgJRFm,        0,                  },//22�������ո�ʽ
    {DjMdFm,        0,                  },//23�����ʽ�����㶳����
    {ZdDjMdFm,      0,                  },//24���㶳���ʽ
    {ZdDjStTmFm,    0,                  },//25���㶳����ʼʱ��
    {MeterAdsFm,    0,                  },//26ͨ�ŵ�ַ
    {MeterRptFm,    flgEtPara_Config    },//27������������֣������ϱ�ģʽ��
    {PowerFullTm,   0,                  },//28�ϵ�ȫ��ʱ��
    {LoadCurveFm,   flgEtPara_LoadCurve },//29���ɼ�¼ģʽ��
    {PdirFm,        flgEtPara_PowDir    },//30���ʷ����¼�
    {MeterModeFm,   flgEtPara_Config    },//31�������״̬������
//    {EsamStatus,    flgEtpara_EsamStatus},//32��Կ������
};



/*=========================================================================================\n
* @function_name: ComRWE2Para
* @function_file: DLT_645_2007.c
* @����: ��дE2����
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-05-10)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 ComRWE2Para(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{
    
    Word32 temp;

    Word16 W16_JSR;

    //����ǳ���ģʽ��д�Ļ�������ʱ���ڳ��ڣ��򷵻ش���
    if((CTable->ucAtb&Data_Factor)
       &&(guc_FactoryType != FactorMd)
       &&(s_FrmInfo->ucCmd == 0x14)
       ||((CTable->ucAtb & Data_WO) && (s_FrmInfo->ucCmd == 0x11)))//�ж�Ȩ��
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;  
    }

    if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }else if(((CTable->ucAtb & Data_RO) && (s_FrmInfo->ucCmd == 0x14)))
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;                                                  //���ش������
    }

    temp.lword=0;

    if(s_FrmInfo->ucCmd == 0x11)                //����Ƕ����ݵĻ�
    {
        SysE2ReadData(CTable->pAddr,s_FrmInfo->ucbuf,CTable->ucLen);
        if(CTable->ucAtb & Data_BCD)            //�����Ҫת��ΪBCD�Ļ�
        {
            if(CTable->ucLen==1)
            {
                s_FrmInfo->ucbuf[0] = APIByteHEX2BCD(s_FrmInfo->ucbuf[0]);
            }else if(CTable->ucLen==2)
            {
                temp.word[0]=Uint8_To_Uint16(s_FrmInfo->ucbuf);
                temp.word[0]=IntHex2BCD(temp.word[0]);
                memcpy(s_FrmInfo->ucbuf,temp.byte,2);
            }
            else if(CTable->ucLen==4)
            {
                temp.lword=Uint8_To_Uint32(s_FrmInfo->ucbuf);
                temp.lword=Hex2BCD(temp.lword);
                memcpy(s_FrmInfo->ucbuf,temp.byte,4);
            }
        }
        s_FrmInfo->ucLen = CTable->ucLen;       //�ظ������ݳ���
    }else
    {
        //�������ĸ�ʽ�Ƿ���ȷ
        s_FrmInfo->ucErr.word=ComCheckFormat(s_FrmInfo->ucbuf,gs_ParaFormatTab[CTable->uiOther].ucFormat,CTable->ucLen);
        if(s_FrmInfo->ucErr.word==0)
        {
            if(CTable->ucAtb & Data_BCD)            //�����Ҫת��ΪBCD�Ļ�
            {
                if(CTable->ucLen==1)
                {
                    s_FrmInfo->ucbuf[0] = APIByteBCD2HEX(s_FrmInfo->ucbuf[0]);
                }else if(CTable->ucLen==2)
                {
                    temp.word[0]=Uint8_To_Uint16(s_FrmInfo->ucbuf);
                    temp.word[0]=IntBCD2Hex(temp.word[0]);
                    memcpy(s_FrmInfo->ucbuf,temp.byte,2);
                }
                else if(CTable->ucLen==4)
                {
                    temp.lword=Uint8_To_Uint32(s_FrmInfo->ucbuf);
                    temp.lword=BCD2Hex(temp.lword);
                    memcpy(s_FrmInfo->ucbuf,temp.byte,4);
                }
            }

//          if((s_FrmInfo->uiID.lword&0xffffff00)==0x04000C00)      //�޸�����
//          {
//              if(guc_FactoryType!=FactorMd)                           //���ڳ���ģʽ
//              {
//                  if(s_FrmInfo->uiID.byte[0]==0x03)                   //02��
//                  {
//                      if(s_FrmInfo->ucLevel!=0x02)                    //02��������Ҫ02��Ȩ��ȥ�޸�
//                      {
//                          s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
//                          s_FrmInfo->ucLen = 1;
//                          return Ret_OK;
//                      }
//                  }
//              }
//              SysE2ParaSetManage(CTable->pAddr+3*(s_FrmInfo->uiID.byte[0]-1),s_FrmInfo->ucbuf+1, CTable->ucLen);
//          }else
            {
            #ifdef JSRPRLOG
                if((s_FrmInfo->uiID.lword&0xffffff00)==0x04000B00)          //�����ձ�̼�¼
                {
                    ProJL(s_FrmInfo->uiUserCode.lword,FLG_JSRJL);           //��¼���ǰ������ʱ��
                    if(s_FrmInfo->uiID.byte[0]==0x01)
                    {
                        BE_ReadP(EEP_CBR1,W16_JSR.byte,2); 
                        if(Uint8_To_Uint16(s_FrmInfo->ucbuf)!=W16_JSR.word)
                        {
                            gui_RefreshEvent|=flgEtPara_FrsJsr;                 //��һ�����ղ���ˢ��
                        }
                    }
                }
                else 
            #endif

            #ifdef ZXRPRLOG
                if(s_FrmInfo->uiID.lword==0x04000802)                  //�����ձ�̼�¼
                {
                    ProJL(s_FrmInfo->uiUserCode.lword,FLG_ZXRJL);           //��¼���ǰ������ʱ�α��
                }
                else 
            #endif

            #ifdef YGZHZPRLOG
                if(s_FrmInfo->uiID.lword==0x04000601)                  //�й���Ϸ�ʽ��̼�¼
                {
                    ProJL(s_FrmInfo->uiUserCode.lword,FLG_YGZHJL);          //��¼���ǰ
                }
            #else
                {
                }
            #endif
                //����Ҫ�ж��ò�����E2��ҳ�׵�ַ��Ȼ���ȡ��ҳ���ݣ������ҳ����CRC��д��E2��
                SysE2ParaSetManage(CTable->pAddr,s_FrmInfo->ucbuf, CTable->ucLen);
                if(s_FrmInfo->uiID.lword==0x04000A01)                  //д���ɼ�¼��ʼʱ��
                {
                    temp.lword=0;                                      //�ж��Ƿ�ȫ0
//                    if(ApiBufCmp(s_FrmInfo->ucbuf, (uint8*)&temp.lword, 4) == CMP_EQU)
//                    {
//                        temp.byte[0] = 0xAA;
//                    }
                    temp.byte[0] = gs_DateTime.ucYear;
//                    SysE2ParaSetManage(EEP_FHJLFLAG, &temp.byte[0], 1);
                }
            }
            //��Ҫˢ�²���������ˢ��
            if(CTable->uiOther==15)           //�������޸�
            {
                guc_BodeDely=40;
            }else if(CTable->ucAtb & Data_RF)           
            {
                gui_RefreshEvent|=gs_ParaFormatTab[CTable->uiOther].ucParaRefFlg;
            } 
            s_FrmInfo->ucLen = 0;                   //д�ɹ�
        }else
        {
            s_FrmInfo->ucLen = 1;                   //д�ɹ�
        }

    }

    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: WrPassWord
* @function_file: DLT_645_2007.c
* @����: ��ͨ����д
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-06-27)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
/*uint8 WrPassWord(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{
    if(s_FrmInfo->ucCmd == 0x11)                    //�ж�Ȩ��
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;                              //���ش������
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }

    //�������ĸ�ʽ�Ƿ���ȷ
    //s_FrmInfo->ucErr.word=ComCheckFormat(s_FrmInfo->ucbuf,gs_ParaFormatTab[CTable->uiOther].ucFormat,CTable->ucLen);
    if(JudgeIsNum(s_FrmInfo->ucbuf+1,3)==Const_Data_OK)
    {
        if(s_FrmInfo->ucLevel == 0x02
           && ((s_FrmInfo->uiID.byte[0] == 0x03 && s_FrmInfo->ucbuf[0]==0x02)
           || (s_FrmInfo->uiID.byte[0] == 0x05 && s_FrmInfo->ucbuf[0]==0x04)))
        {
            SysE2ParaSetManage(CTable->pAddr+(s_FrmInfo->uiID.byte[0]-1)*3,s_FrmInfo->ucbuf+1, 3);
            s_FrmInfo->ucLen = 0;                   //д�ɹ�
        }
        else if(s_FrmInfo->ucLevel == 0x04
                && s_FrmInfo->uiID.byte[0] == 0x05
                && s_FrmInfo->ucbuf[0]==0x04)
        {
            SysE2ParaSetManage(CTable->pAddr+(s_FrmInfo->uiID.byte[0]-1)*3,s_FrmInfo->ucbuf+1, 3);
            s_FrmInfo->ucLen = 0;                   //д�ɹ�
        }
        else
        {
            s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
            s_FrmInfo->ucLen = 1;                   //д�ɹ�
        }
    }
    else
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;                   //д�ɹ�
    }
    return Ret_OK;
}*/
/*=========================================================================================\n
* @function_name: ComMeterConst
* @function_file: DLT_645_2007.c
* @����: ��ȡ�����
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-05-18)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 ComMeterConst(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{

    Word32 temp;

    if(s_FrmInfo->ucCmd == 0x14)
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    temp.lword=0;
    SysE2ReadData(CTable->pAddr,s_FrmInfo->ucbuf,2);
    //2���ֽڱ��3���ֽڵ�BCD
    temp.word[0]=Uint8_To_Uint16(s_FrmInfo->ucbuf);
    temp.lword=Hex2BCD(temp.lword);
    //temp.word[0]=IntHex2BCD(temp.word[0]);
    memcpy(s_FrmInfo->ucbuf,temp.byte,3);   //���3���ֽڵ�BCD
    s_FrmInfo->ucLen = CTable->ucLen;       //�ظ������ݳ���
    return Ret_OK;
}
//extern const uint16 FFTE2SaveTab[];
/*=========================================================================================\n
* @function_name: ReadHarMCont
* @function_file: DLT_645_2007.c
* @����: ��ȡг��������ÿ��FFT�������256���ֽ�
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-05-31)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 ReadHarMCont(S_FRAMEINFO *s_FrmInfo, S_CORETAB * CTable)
{
    float uiFFTArr[15];
    uint8  ucTemp;
    uint8  i;
    Word16 E2SaveBuf[300];
    uint16 uiHarmCont[15];
    
    ucTemp = (s_FrmInfo->uiID.byte[2]-0x0A)+(s_FrmInfo->uiID.byte[1]-1)*2;            //������һ�࣬����г��
    for (i=0;i<3;i++)
    {
        BE_ReadP(SampE2SaveTab[ucTemp]+200*i,(uint8*)&E2SaveBuf[i*100].word,200);   //��ȡE2���ݣ���200�ֽڶ�ȡ
    }
    FFT_Task((int16*)&E2SaveBuf[0].word);
    for(i=0;i<15;i++)
    {
        uiFFTArr[i] = fftx[(i+1)*8].real;
    }
//             bcd = 0x22;          
//            byte[0]: n��г������
//            byte[1]: ��λ
//            byte[3]: UI
//            Ƶ�ʷֱ���6.25������ܼ��㵽15��г��
//
    if (s_FrmInfo->uiID.byte[0] != 0xFF)        //�������ݵ�
    {
        uiHarmCont[s_FrmInfo->uiID.byte[0] - 1] = (uint16)(uiFFTArr[s_FrmInfo->uiID.byte[0] - 1] * 100 / uiFFTArr[0]); 
        if (CTable->ucAtb & Data_BCD)
        {
            uiHarmCont[s_FrmInfo->uiID.byte[0] - 1] = Hex2BCD(uiHarmCont[s_FrmInfo->uiID.byte[0] - 1]);
        }
        memcpy(s_FrmInfo->ucbuf, (uint8 *)&uiHarmCont[s_FrmInfo->uiID.byte[0]-1], CTable->ucLen);  
        
        s_FrmInfo->ucLen = CTable->ucLen;           //�ظ������ݳ���
    }
    else                                        //����
    {
        for (i=0;i<15;i++)
        {
            uiHarmCont[i] = (uint16)(uiFFTArr[i] * 100 / uiFFTArr[0]); 
            if (CTable->ucAtb & Data_BCD)
            {
                uiHarmCont[i] = Hex2BCD(uiHarmCont[i]);
            }
            memcpy(s_FrmInfo->ucbuf+i*2, (uint8 *)&uiHarmCont[i], 2); 
        }
        s_FrmInfo->ucLen = CTable->ucLen; 
    }
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: RwGGJR
* @function_file: DLT_645_2007.c
* @����: ��д��������
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-06-16)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
//uint8 FFTCalc(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
//{
//    guc_FFT_Flg = true;
//    WriteBronco(0x3f,0xC01A);       //����DMAͨ��
//    s_FrmInfo->ucLen = 0;
//    return Ret_OK;
//}

/*=========================================================================================\n
* @function_name: FunToBeDef
* @function_file: DLT_645_2007.c
* @����: �պ���
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-03-01)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 FunToBeDef(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{
    return Ret_OK;
}

/*=========================================================================================\n
* @function_name: ReadJL
* @function_file: DLT_645_2007.c
* @����: ��ȡ�¼���¼
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-05-31)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 ReadCycWave(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{
    uint8  ucCnt;
    uint16 uiLen;
    uint32 ulE2Addr;

    if(s_FrmInfo->ucCmd == 0x14)                            //�����д���ݵĻ�
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;                                      //��֧��д�����ش������
    }
    else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }
    BE_ReadP(EEP_ZBCNT_PT,(uint8*)&ucCnt,1);
    if (ucCnt>4)                //��������0-4����Ӧ8��16��32��64��128��Ĭ��32//��ʱû��128
    {
        ucCnt = 2;
    }
    uiLen = SampTab[ucCnt]*20;
    if ((s_FrmInfo->uiID.byte[0] - 1)>(uiLen/200))
    {
        s_FrmInfo->ucReply = 0;
        return Ret_OK;
    }
    
    ulE2Addr = SampE2SaveTab[s_FrmInfo->uiID.byte[1]] + (s_FrmInfo->uiID.byte[0] - 1) * 200; 
//    for (i=0;i<4;i++)
//    {
        BE_ReadP(ulE2Addr,XDATA_RAMZONE,200);
        memcpy(s_FrmInfo->ucbuf, XDATA_RAMZONE, 200); 
//    }

    uiLen = uiLen-(s_FrmInfo->uiID.byte[0] - 1)*200;
    if (uiLen < 200)
    {
        s_FrmInfo->ucLen = uiLen;                       //�ظ������ݳ���
    }
    else
    {
        s_FrmInfo->ucLen = CTable->ucLen;
    }
    

    return Ret_OK;
}

/*typedef struct
{
    uint16  uiDjModeAdrr;      //����ģʽ�ֵ�ַ
    uint16  uiDjCntAdrr;   //���������ַ
}S_RDDjDATA;


const S_RDDjDATA   DjDATATab[]=
{
    {EEP_DSFREEZ,EEP_DSDJHISNO},// 0 ��ʱ����
    {EEP_YDFREEZ,EEP_SSDJHISNO},// 1 ˲ʱ����
    {EEP_SSFREEZ,EEP_SQDJHISNO},// 2 ����ʱ�����л�����

    {EEP_SSFREEZ,EEP_RSDJHISNO},// 3 ������ʱ�α��л�����
    {EEP_ZDFREEZ,EEP_ZDDJHISNO},// 4 ���㶳��
    {EEP_RFREEZ,EEP_RDJHISNO},//  5 �ն���
};*/
/*=========================================================================================\n
* @function_name: RwDjData
* @function_file: DLT_645_2007.c
* @����: ��������
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-05-30)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
/*uint8 RwDjData(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{
    uint8 ucDjMode;
    Word32 data1;
    uint8 ucpos;
    uint32 ulDjCnt;

    if(s_FrmInfo->ucCmd == 0x14)                            //�����д���ݵĻ�
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;                                      //��֧��д�����ش������
    }else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    ulDjCnt=0;

    //��ȡ���¼�����
    BE_ReadP(DjDATATab[CTable->uiOther].uiDjCntAdrr,(uint8*)&ulDjCnt,3);

    BE_ReadP(DjDATATab[CTable->uiOther].uiDjModeAdrr,&ucDjMode,1);              //��ȡ����ģʽ��
    //��ȡ��������
    DataProcRead2Slice(CTable->pAddr,s_FrmInfo->uiID.byte[0] - 1,0,CTable->ucLen,XDATA_RAMZONE);
 

    if(s_FrmInfo->uiID.byte[2] == 0x04)         //��ȡ���㶳�����ݱȽ�����
    {
        if(s_FrmInfo->uiID.byte[1] == 0x00)     //����Ƕ�ȡ����ʱ��Ļ�
        {
            memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE,5);
            s_FrmInfo->ucLen = 5;
        }
        else if(s_FrmInfo->uiID.byte[1] == 0x01)        //�����ܵ���
        {
            if(ucDjMode&BIT0)
            {
                memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE+5,4);
                s_FrmInfo->ucLen = 4;
            }else
            {
                s_FrmInfo->ucErr.word = BIT1;
                s_FrmInfo->ucLen = 1;
            }
        }
        else if(s_FrmInfo->uiID.byte[1] == 0x02)        //�����ܵ���
        {
            if(ucDjMode&BIT1)
            {
                memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE+9,4);
                s_FrmInfo->ucLen = 4;
            }else
            {
                s_FrmInfo->ucErr.word = BIT1;
                s_FrmInfo->ucLen = 1;
            }
        }
        else if(s_FrmInfo->uiID.byte[1] == 0xFF)//����
        {
            memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE,5);       //ʱ��
            memset(s_FrmInfo->ucbuf+5,0xAA,1);              //AA
            ucpos=6;

            if(ucDjMode&BIT0)
            {
                memcpy(s_FrmInfo->ucbuf+ucpos,XDATA_RAMZONE+5,4);    //�������
                memset(s_FrmInfo->ucbuf+ucpos+4,0xAA,1);
                ucpos+=5;
            }else
            {
                memset(s_FrmInfo->ucbuf+ucpos,0xAA,1);
                ucpos+=1;
            }
  

            if(ucDjMode&BIT1)                                       //�������
            {
                memcpy(s_FrmInfo->ucbuf+ucpos,XDATA_RAMZONE+9,20);
                memset(s_FrmInfo->ucbuf+4+ucpos,0xAA,1);
                ucpos+=5;
            }else
            {
                memset(s_FrmInfo->ucbuf+ucpos,0xAA,1);
                ucpos+=1;
            }
            s_FrmInfo->ucLen = ucpos;
        }
    }
    else                                        //������Ƕ�ȡ���㶳�����ݵĻ�
    {
        if(s_FrmInfo->uiID.byte[1] == 0x00)     //����Ƕ�ȡ����ʱ��Ļ�
        {                                       //��ȡ����
            memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE,5);
            s_FrmInfo->ucLen = 5;
        }else if(s_FrmInfo->uiID.byte[1] == 0x01)   //�������
        {
            if(ucDjMode&BIT0)
            {
                memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE+5,20);
                s_FrmInfo->ucLen = 20;
            }else
            {
                s_FrmInfo->ucErr.word = BIT1;
                s_FrmInfo->ucLen = 1;
            }

        }
        else if(s_FrmInfo->uiID.byte[1] == 0x02)    //�������
        {
            if(ucDjMode&BIT1)
            {
                memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE+25,20);
                s_FrmInfo->ucLen = 20;
            }else
            {
                s_FrmInfo->ucErr.word = BIT1;
                s_FrmInfo->ucLen = 1;
            }
        }
        else if(s_FrmInfo->uiID.byte[1] == 0x10)                //����
        {
            if(ucDjMode&BIT7)
            {
                data1.lword = Uint8_To_Uint32(XDATA_RAMZONE+45);
                memcpy(s_FrmInfo->ucbuf,data1.byte,3);          //�ܹ���
                memcpy(s_FrmInfo->ucbuf+3,data1.byte,3);        //A���

                if(ulDjCnt>=s_FrmInfo->uiID.byte[0])            //��������������ڶ�ȡ�����ݵĴ�����Ĭ��ֵ��0XFF,��Ȼ��0x00
                {
                    //������ݴ����ж��Ƿ���Ҫ��д0xff������0x00
                    memset(s_FrmInfo->ucbuf+6,0xff,18);
                }else
                {
                    memset(s_FrmInfo->ucbuf+6,0x00,18);
                }
                s_FrmInfo->ucLen = 24;
            }else
            {
                s_FrmInfo->ucErr.word = BIT1;
                s_FrmInfo->ucLen = 1;
            }
        }
        else if(s_FrmInfo->uiID.byte[1] == 0xFF)
        {
            memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE,5);       //ʱ��
            memset(s_FrmInfo->ucbuf+5,0xAA,1);              //AA
            ucpos=6;

            if(ucDjMode&BIT0)
            {
                memcpy(s_FrmInfo->ucbuf+ucpos,XDATA_RAMZONE+5,20);    //�������
                memset(s_FrmInfo->ucbuf+ucpos+20,0xAA,1);
                ucpos+=21;
            }else
            {
                memset(s_FrmInfo->ucbuf+ucpos,0xAA,1);
                ucpos+=1;
            }
  

            if(ucDjMode&BIT1)                                       //�������
            {
                memcpy(s_FrmInfo->ucbuf+ucpos,XDATA_RAMZONE+25,20);
                memset(s_FrmInfo->ucbuf+20+ucpos,0xAA,9);
                ucpos+=29;
            }else
            {
                memset(s_FrmInfo->ucbuf+ucpos,0xAA,9);
                ucpos+=9;
            }


            if(ucDjMode&BIT7)
            {
                data1.lword = Uint8_To_Uint32(XDATA_RAMZONE+45);
                memcpy(s_FrmInfo->ucbuf+ucpos,data1.byte,3);          //�ܹ���
                memcpy(s_FrmInfo->ucbuf+ucpos+3,data1.byte,3);        //A���

                if(ulDjCnt>=s_FrmInfo->uiID.byte[0])            //��������������ڶ�ȡ�����ݵĴ�����Ĭ��ֵ��0XFF,��Ȼ��0x00
                {
                    //������ݴ����ж��Ƿ���Ҫ��д0xff������0x00
                    memset(s_FrmInfo->ucbuf+ucpos+6,0xff,18);
                }else
                {
                    memset(s_FrmInfo->ucbuf+ucpos+6,0x00,18);
                }

                memset(s_FrmInfo->ucbuf+ucpos+24,0xAA,1);
                ucpos+=25;
            }else
            {
                memset(s_FrmInfo->ucbuf+ucpos,0xAA,1);
                ucpos+=1;
            }

            s_FrmInfo->ucLen = ucpos;
        }
    }
    return Ret_OK;
}*/

 
/*=========================================================================================\n
* @function_name: RwLcdData
* @function_file: DLT_645_2007.c
* @����: ������ʾ��
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-05-17)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 RwLcdData(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{
    uint16 addr;
    Word32 ulLcdid;
    uint8 uctemp;

    if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    if((s_FrmInfo->uiID.byte[1]==0x01 && s_FrmInfo->uiID.byte[0]>gs_DispCont.ucLxCnt)    //ѭ��  
      || (s_FrmInfo->uiID.byte[1]==0x02 && s_FrmInfo->uiID.byte[0]>gs_DispCont.ucJxCnt))   //����
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }

    addr = CTable->pAddr + (s_FrmInfo->uiID.byte[0] - 1) * 5;   //��ȡ���ŵĵ�ַ
    if(s_FrmInfo->ucCmd == 0x11)                                //����Ƕ����ݵĻ�
    {
        BE_ReadP(addr, s_FrmInfo->ucbuf, 5);                    //��ȡ����
        s_FrmInfo->ucLen =5;
    }
    else                                                        //�������д����
    {
        if(s_FrmInfo->ucLevel==0x98)
        {
            uctemp=s_FrmInfo->ucbuf[0];
            memcpy(s_FrmInfo->ucbuf,s_FrmInfo->ucbuf+1,4);
            s_FrmInfo->ucbuf[4]=uctemp;
        }
        
        ulLcdid.lword=Uint8_To_Uint32(s_FrmInfo->ucbuf);
        if(SearchDispNO(ulLcdid)!=0xff)
        {
            BE_WriteP(addr, s_FrmInfo->ucbuf, 5);                   //д�����ݱ�ʶ
            s_FrmInfo->ucLen = 0;
        }else
        {
            s_FrmInfo->ucErr.word = ComErrBit_Other;
            s_FrmInfo->ucLen = 1;
        }
    }
    return Ret_OK;
}
 
/*=========================================================================================\n
* @function_name: RwSysTime
* @function_file: DLT_645_2007.c
* @����: ��дϵͳʱ��
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-03-01)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 RwSysTime(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{

    if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    if(s_FrmInfo->ucCmd == 0x11)                        //������
    {
        if(s_FrmInfo->uiID.lword == 0x04000101)         //���ں�����
        {
            GetSysClock(s_FrmInfo->ucbuf, Const_YYMMDDWW);
            s_FrmInfo->ucLen = CTable->ucLen;
        }
        else if(s_FrmInfo->uiID.lword == 0x04000102)        //ʱ��
        {
            GetSysClock(s_FrmInfo->ucbuf, Const_hhmmss);
            s_FrmInfo->ucLen = CTable->ucLen;
        }
        else if(s_FrmInfo->uiID.lword == 0x0400010C)         //����+ʱ��
        {
            GetSysClock(s_FrmInfo->ucbuf, Const_hhmmssYYMMDDWW);
            s_FrmInfo->ucLen = CTable->ucLen;
        }
        else
        {
            s_FrmInfo->ucLen = 1;
            s_FrmInfo->ucErr.word = ComErrBit_Nodata;
        }
//  #ifdef FRCom
//      if(s_FrmInfo->ucPort==ComIndex_Uart1)
//      {
//          if(gs_FREventOut.FREventOutStruct.PDirFR==0)
//          {
//              if(gs_FREventOut.FREventOutStruct.OpenGapFRFlag==0)
//              {
//                      EVENTOUTLOW();
//              }
//          }
//          gs_FREventOut.FREventOutStruct.TimeFR=0;
//      }
//  #endif
    }
    else                                                        //д����
    {
        S_TIMEDATA time;
        if(s_FrmInfo->uiID.lword == 0x04000101)                 //���ں�����
        {
            if(Ret_OK == ValidTime(s_FrmInfo->ucbuf+1, 0)       //�ж�ʱ��ĺϷ���
               && s_FrmInfo->ucbuf[0]<=6)    //���������ж�  
            {
                memcpy(&time.ucSecond,&gs_DateTime.ucSecond,7);     //����Уʱǰ��ʱ��
                s_FrmInfo->ucLen = 0;
                SetSysRTCClock(s_FrmInfo->ucbuf, Const_YYMMDDWW);  //У��ʱ��
                Com_JSJL(s_FrmInfo->uiUserCode.lword, &time);   //����Уʱǰ��ʱ��
                gui_RefreshEvent|= flgEtPara_Fee;               //ˢ�·��ʲ���
           
//         #ifdef FRCom
//              if(s_FrmInfo->ucPort==ComIndex_Uart1)
//              {
//                  if(gs_FREventOut.FREventOutStruct.PDirFR==0)
//                  {
//                      if(gs_FREventOut.FREventOutStruct.OpenGapFRFlag==0)
//                      {
//                              EVENTOUTLOW();
//                      }
//                  }
//                  gs_FREventOut.FREventOutStruct.TimeFR=0;
//              }
//          #endif
                     
            
            
            }
            else
            {
                s_FrmInfo->ucErr.word = ComErrBit_Other;
                s_FrmInfo->ucLen = 1;
            }
            
        }
        else if(s_FrmInfo->uiID.lword == 0x04000102)            //ʱ��
        {
            if(Ret_OK == ValidTime(s_FrmInfo->ucbuf, 1))        //�ж�ʱ��ĺϷ���
            {

                memcpy(&time.ucSecond,&gs_DateTime.ucSecond,7);     //����Уʱǰ��ʱ��
                s_FrmInfo->ucLen = 0;
                SetSysRTCClock(s_FrmInfo->ucbuf, Const_hhmmss);
                Com_JSJL(s_FrmInfo->uiUserCode.lword, &time);
                gui_RefreshEvent|= flgEtPara_Fee;               //ˢ�·��ʲ���
               
//         #ifdef FRCom
//              if(s_FrmInfo->ucPort==ComIndex_Uart1)
//              {
//                  if(gs_FREventOut.FREventOutStruct.PDirFR==0)
//                  {
//                      if(gs_FREventOut.FREventOutStruct.OpenGapFRFlag==0)
//                      {
//                              EVENTOUTLOW();
//                      }
//                  }
//                  gs_FREventOut.FREventOutStruct.TimeFR=0;
//              }
//          #endif
                
                
            }
            else
            {
                s_FrmInfo->ucErr.word = ComErrBit_Other;
                s_FrmInfo->ucLen = 1;
            }
        }
        else if(s_FrmInfo->uiID.lword == 0x0400010C)            //����+ʱ��
        {
//            if(s_FrmInfo->ucLen!=19)
//            {
//                s_FrmInfo->ucErr.word = ComErrBit_Other;
//                s_FrmInfo->ucLen = 1;
//                return Ret_OK;
//            }
            
            if(Ret_OK == ValidTime(s_FrmInfo->ucbuf, 3) 
               && s_FrmInfo->ucbuf[3]<=6)      //�ж�ʱ��ĺϷ���
            {
                memcpy(&time.ucSecond,&gs_DateTime.ucSecond,7);     //����Уʱǰ��ʱ��
                SetSysRTCClock(s_FrmInfo->ucbuf, Const_hhmmssYYMMDDWW);
                Com_JSJL(s_FrmInfo->uiUserCode.lword, &time);
                gui_RefreshEvent|= flgEtPara_Fee;               //ˢ�·��ʲ���
                s_FrmInfo->ucLen = 0;
            
//          #ifdef FRCom
//              if(s_FrmInfo->ucPort==ComIndex_Uart1)
//              {
//                  if(gs_FREventOut.FREventOutStruct.PDirFR==0)
//                  {
//                      if(gs_FREventOut.FREventOutStruct.OpenGapFRFlag==0)
//                      {
//                              EVENTOUTLOW();
//                      }
//                  }
//                  gs_FREventOut.FREventOutStruct.TimeFR=0;
//              }
//          #endif
            }else
            {
                s_FrmInfo->ucErr.word = ComErrBit_Other;
                s_FrmInfo->ucLen = 1;
            }
        }
        else
        {
            s_FrmInfo->ucLen = 1;
            s_FrmInfo->ucErr.word = ComErrBit_Nodata;
        }
    }
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: RwRamE2
* @function_file: DLT_645_2007.c
* @����: ֱ�Ӷ�дE2
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-05-15)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 RwRamE2(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{
    uint8 len;
    Word32 addr;

    if(s_FrmInfo->ucCmd == 0x11)                //������
    {
        addr.byte[0] = s_FrmInfo->ucbuf[0];     //��д�ĵ�ַ
        addr.byte[1] = s_FrmInfo->ucbuf[1];
        addr.byte[2] = 0;     //��д�ĵ�ַ
        addr.byte[3] = 0;
        len = s_FrmInfo->ucbuf[3];              //��д�ĳ���
        if(len > 0x50)                          //���ȴ���0x40�Ǵ����
        {
            return Ret_Err;
        }
        BE_ReadP(addr.lword, s_FrmInfo->ucbuf, len);
        s_FrmInfo->ucLen = len;
    }
    else                                        //�������д����
    {
        addr.byte[0] = s_FrmInfo->ucbuf[0];     //��д�ĵ�ַ
        addr.byte[1] = s_FrmInfo->ucbuf[1];
        addr.byte[2] = 0;     //��д�ĵ�ַ
        addr.byte[3] = 0;
        len = s_FrmInfo->ucbuf[3];              //��д�ĳ���
        if(len > 0x50)                          //���ȴ���0x40�Ǵ����
        {
            return Ret_Err;
        }
 
        BE_WriteP(addr.lword, s_FrmInfo->ucbuf + 4, len);
        s_FrmInfo->ucLen = 0;
    }
    return Ret_OK;
}

typedef struct
{
    uint32 ulAdrr;
    uint8  phy;
    uint8  Atb;
    uint8  ucLen;
}Gs_VarType;


const Gs_VarType   Gs_RdVarTab[]=
{
    {RMSU,      0x04,   0x01,       0x02,   },      //0 ��ѹ���ݿ�
    {RMSU,      0x00,   0x01,       0x02,   },      //1 A��ѹ
    {RMSU,      0x01,   0x01,       0x02,   },      //2 B��ѹ
    {RMSU,      0x02,   0x01,       0x02,   },      //3 C��ѹ
     
    {RMSI1,     0x04,   0x01,       0x03,   },      //4 �������ݿ�
    {RMSI1,     0x00,   0x01,       0x03,   },      //5 A����
    {RMSI1,     0x01,   0x01,       0x03,   },      //6 B����
    {RMSI1,     0x02,   0x01,       0x03,   },      //7 A����

    {DATAP,     0x04,   0x01,       0x03,   },      //8  �й��������ݿ�
    {DATAP,     0x00,   0x01,       0x03,   },      //9 �й�A����
    {DATAP,     0x01,   0x01,       0x03,   },      //10 �й�B����
    {DATAP,     0x02,   0x01,       0x03,   },      //11 �й�C����


    {DATAFREQ,  APhy,   0x02,       0x02,   },      //12 Ƶ��
    {0,         0x04,   0x00,       0x02,   },      //13 ����������
    {0,         0x00,   0x00,       0x02,   },      //14 ��������A 
    {0,         0x01,   0x00,       0x02,   },      //15 ��������B
    {0,         0x02,   0x00,       0x02,   },      //16 ��������C

    {DATAQ,     0x04,   0x01,       0x03,   },      //17 �޹��������ݿ�
    {DATAQ,     0x00,   0x01,       0x03,   },      //18 �޹�A����
    {DATAQ,     0x01,   0x01,       0x03,   },      //19 �޹�B����
    {DATAQ,     0x02,   0x01,       0x03,   },      //20 �޹�C����

};
/*=========================================================================================\n
* @function_name: ReadVar
* @function_file: DLT_645_2007.c
* @����: ��ȡ˲ʱ����ֵ
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-06-01)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 ReadVar(S_FRAMEINFO *s_FrmInfo, S_CORETAB   * CTable)
{
    uint32 ulVarValue;
    uint32 uiCosVal;
    uint32 bcd;
    uint8  ucpos;


    Gs_VarType   * pVar;
    if(s_FrmInfo->ucCmd == 0x14)                //�����д�Ļ�
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }
    else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    ucpos=0;
    if(s_FrmInfo->ucAtb & Const_Atb_DataJC)     //����Ǽ����Ļ�
    {
        ucpos=0;    
    }

    pVar = (Gs_VarType  *)&Gs_RdVarTab + CTable->uiOther+ucpos;

    if(pVar->Atb==0x01)
    {
        if(s_FrmInfo->ucAtb & Const_Atb_DataJC)     //����Ǽ����Ļ�
        {
            if(pVar->ulAdrr == RMSU)
            {
                for(uint8 i=0;i<3;i++)
                {
                    //ulVarValue = CalRMS2(RMSU,gs_UI.ul_U[i]);
                    ulVarValue = CalRMS(RMSUA+i);
                    memcpy(s_FrmInfo->ucbuf+i*pVar->ucLen,(uint8*)&ulVarValue,pVar->ucLen);
                }
                s_FrmInfo->ucLen=pVar->ucLen*3;
                return Ret_OK;
            }
            else if(pVar->ulAdrr == RMSI1)
            {
                for(uint8 i=0;i<3;i++)
                {
                    //ulVarValue = CalRMS2(RMSI1,gs_UI.ul_I[i]);
                    ulVarValue = CalRMS(RMSI1A+i);
                    memcpy(s_FrmInfo->ucbuf+i*pVar->ucLen,(uint8*)&ulVarValue,pVar->ucLen);
                }
                s_FrmInfo->ucLen=pVar->ucLen*3;
                return Ret_OK;
            }
            else if (pVar->ulAdrr == DATAP)
            {
                //ulVarValue = CalRMS2(DATAP,(gs_power.ul_power));
                ulVarValue = CalRMS(DATAPABC);
                memcpy(s_FrmInfo->ucbuf,(uint8*)&ulVarValue,pVar->ucLen);
                for(uint8 i=0;i<3;i++)
                {
                    //ulVarValue = CalRMS2(DATAP,gs_power.ul_power);
                     ulVarValue = CalRMS(DATAPA+i);
                    memcpy(s_FrmInfo->ucbuf+pVar->ucLen*(i+1),(uint8*)&ulVarValue,pVar->ucLen);
                }
                s_FrmInfo->ucLen=pVar->ucLen*4;
                return Ret_OK;
            }
            else if(pVar->ulAdrr == DATAQ)
            {
                //ulVarValue = CalRMS2(DATAQ,((gs_power.ul_Npower)*127/100));
                ulVarValue = CalRMS(DATAQABC);
                memcpy(s_FrmInfo->ucbuf,(uint8*)&ulVarValue,pVar->ucLen);
                for(uint8 i=0;i<3;i++)
                {
                    //ulVarValue = CalRMS2(DATAQ,((gs_power.ul_Npower*127)/100));
                    ulVarValue = CalRMS(DATAQ+i);
                    memcpy(s_FrmInfo->ucbuf+pVar->ucLen*(i+1),(uint8*)&ulVarValue,pVar->ucLen);
                }
                s_FrmInfo->ucLen=pVar->ucLen*4;
                return Ret_OK;
            }
        }
        else
        {
            if(pVar->ulAdrr == RMSU)
            {
                 //ulVarValue = CalRMS2(RMSU,gs_UI.ul_U[pVar->phy]);
                 ulVarValue = CalRMS(RMSUA+pVar->phy);
            }
            else if(pVar->ulAdrr == RMSI1)
            {
                 //ulVarValue = CalRMS2(RMSI1,gs_UI.ul_I[pVar->phy]);
                 ulVarValue = CalRMS(RMSI1A+pVar->phy);
            }
            else if (pVar->ulAdrr == DATAP)
            {
                if(pVar->phy == 0x04)
                {
                    ulVarValue = CalRMS(DATAPABC);
                }
                else
                {
                    ulVarValue = CalRMS(DATAPA+pVar->phy);
                }
              /*  switch(pVar->phy)
                {
                    case 0x00:
                    case 0x01:
                    case 0x02:
                        ulVarValue = CalRMS2(DATAP,gs_power.ul_power);
                        break;
                    case 0x04:
                        ulVarValue = CalRMS2(DATAP,(gs_power.ul_power));
                    default:
                        break;
                }*/
            }
            else if(pVar->ulAdrr == DATAQ)
            {

                if(pVar->phy == 0x04)
               {
                   ulVarValue = CalRMS(DATAQABC);
               }
               else
               {
                   ulVarValue = CalRMS(DATAQA+pVar->phy);
               }
            }
           else if(pVar->ulAdrr == DATAFREQ)
            {
                   ulVarValue = CalRMS(DATAFREQ);
            }
            memcpy(s_FrmInfo->ucbuf,(uint8*)&ulVarValue,pVar->ucLen);
        }
    }else 
    {
        if(CTable->uiOther>=13
           &&CTable->uiOther<=16)                      //��������
        {
            if(s_FrmInfo->ucAtb & Const_Atb_DataJC)    //����
            {
                for(uint8 i=0;i<4;i++)
                {
                    uiCosVal=CalCos(i);  
                    memcpy(s_FrmInfo->ucbuf+i*pVar->ucLen,(uint8*)&uiCosVal,pVar->ucLen);
                }
                s_FrmInfo->ucLen=pVar->ucLen*4;
                return Ret_OK;
            }else                                       //����
            {
                uiCosVal=CalCos(pVar->phy); 
                uiCosVal = Hex2BCD(uiCosVal);
                memcpy(s_FrmInfo->ucbuf,(uint8*)&uiCosVal,pVar->ucLen);
            }
        }
        else if(CTable->uiOther==21)
        {
            //gi_Temprature = (((T_Get_Temp()*3250/4096)*100)/175)-273;
            gi_Temprature = gi_Temprature*10;
            if(gi_Temprature<0)
            {
                bcd=~gi_Temprature+1;
                bcd=Hex2BCD(bcd);
                bcd|=0x8000;
            }else
            {
                bcd=gi_Temprature;
                bcd=Hex2BCD(bcd);
            }

            memcpy(s_FrmInfo->ucbuf,(uint8*)&bcd,CTable->ucLen);      //�����¶�
        }
        else if(CTable->uiOther==11)
        {
            //bcd=Hex2BCD(gul_CellVoltage);
            bcd>>=4;
            memcpy(s_FrmInfo->ucbuf,(uint8*)&bcd,CTable->ucLen);     //��ص�ѹ 
        }else if(CTable->uiOther==12)
        {
            //Ƶ��
            bcd = CalRMS(DATAFREQ);
            memcpy(s_FrmInfo->ucbuf,(uint8*)&bcd,CTable->ucLen);     //Ƶ��
        }
//        else if(CTable->uiOther>=22&&CTable->uiOther<=27)
//        {
//            ucTemp = CTable->uiOther-22;
//            for (i=0;i<3;i++)
//            {
//                BE_ReadP(SampE2SaveTab[ucTemp]+200*i,(uint8*)&E2SaveBuf[i*100].word,200);
//            }
//            FFT_Task((int16*)&E2SaveBuf[0].word);
//            for(i=0;i<15;i++)
//            {
//                uiFFTArr[i] = fftx[(i+1)*8].real;
//            }
////             bcd = 0x22;          
////            byte[0]: n��г������
////            byte[1]: ��λ
////            byte[3]: UI
////            Ƶ�ʷֱ���6.25������ܼ��㵽15��г��
////
//            memcpy(s_FrmInfo->ucbuf,(uint8*)uiFFTArr,CTable->ucLen);    
//        }
    }
    s_FrmInfo->ucLen=CTable->ucLen;
    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: RTHZjl
* @function_file: DLT_645_2007.c
* @����: ��ȡ����բ��¼���ݿ�
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-06-06)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
/*uint8 RTHZjl(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{
    uint32 ulEvCnt;
    Word32 W32Count;

    if(s_FrmInfo->ucCmd == 0x14)                //�����д�Ļ�
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }

    ulEvCnt=0;
    //��ȡ���¼�����
    BE_ReadP(CTable->uiOther,(uint8*)&ulEvCnt,3);

    DataProcRead2Slice(CTable->pAddr,s_FrmInfo->uiID.byte[0] - 1,0,18,XDATA_RAMZONE);
    if(s_FrmInfo->ucAtb & Const_Atb_DataJC)                 //����Ǽ����Ļ�
    {
        //��ȡ�¼�����
        memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE,18);          //�����¼��������ߴ��룬�����ܵ���

        if(ulEvCnt>=s_FrmInfo->uiID.byte[0])                //���ʹ������ڶ�ȡ���������0xff,��Ȼ���0x00
        {
            memset(s_FrmInfo->ucbuf+18,0xff,16);
        }else
        {
            memset(s_FrmInfo->ucbuf+18,0x00,16);
        }
        s_FrmInfo->ucLen = 34;
    }
    else
    {
        if(s_FrmInfo->uiID.byte[1]==0x00)               //��ȡ����
        {
            W32Count.lword=Hex2BCD(ulEvCnt);            //HEXתBCD   
            //�¼���¼����3���ֽ�
            memcpy(s_FrmInfo->ucbuf,W32Count.byte,3);   //���3���ֽڵ�BCD
        }else if(s_FrmInfo->uiID.byte[1]==0x01)         //ʱ��
        {
            memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE,6);   //����ʱ��

        }else if(s_FrmInfo->uiID.byte[1]==0x02)         //�����ߴ���
        {
            memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE+6,4); //�����ߴ���

        }else if(s_FrmInfo->uiID.byte[1]==0x03)         //�������
        {
            memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE+10,4);//�������
        }else if(s_FrmInfo->uiID.byte[1]==0x04)         //�������
        {
            memcpy(s_FrmInfo->ucbuf,XDATA_RAMZONE+14,4);//�������
        }
        s_FrmInfo->ucLen = CTable->ucLen;           //�ظ������ݳ���
    }
    return Ret_OK;
}*/
/*=========================================================================================\n
* @function_name: IrShowLcd
* @function_file: DLT_645_2007.c
* @����: ����ָ��LCD��ʾ
* 
* 
* @����: 
* @param:s_FrmInfo  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-06-22)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 IrShowLcd(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{
    Word32 ulLcdid;
    uint8 ucbuff[5];

    if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    if(s_FrmInfo->ucCmd != 0x11)                                //����Ƕ����ݵĻ�
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }
    else                                                        //�������д����
    { 
        memset(ucbuff,0xff,5);
        if(ApiBufCmp(s_FrmInfo->ucbuf,ucbuff,5) == CMP_EQU)
        {
            gs_DispStt.ucMode = Const_DispMode_Full;
            gs_DispStt.ucTmr = 10;
        }//����ˢ��
        else
        {
            ulLcdid.lword=Uint8_To_Uint32(s_FrmInfo->ucbuf);
            if(SearchDispNO(ulLcdid)!=0xff && s_FrmInfo->ucbuf[4] < 2) 
            {
                if(ulLcdid.lword != 0x04000401 && ulLcdid.lword != 0x04000402     //��ռ2����NN��Ϊ0�����
                    && ulLcdid.lword != 0x0400040E && s_FrmInfo->ucbuf[4] != 0)
                {
                    s_FrmInfo->ucErr.word = ComErrBit_Other;
                    s_FrmInfo->ucLen = 1;
                    return Ret_OK; 
                }
                memcpy(guc_IrShowLcdId,s_FrmInfo->ucbuf,5);       //��������
                gs_DispStt.ucMode=Const_DispMode_IRDA;
                gs_DispStt.ucTmr = 10;
            }else
            {
                s_FrmInfo->ucErr.word = ComErrBit_Other;
                s_FrmInfo->ucLen = 1;
                return Ret_OK; 
            }
        }
    }
                                                    //��Ҫ��������,����ʱ�����
    OpenBeiGuang();
    guc_DyBgLed = 10;                               // ���ñ�����ʱʱ��
    gs_DispStt.ucPwrOffRefresh |= Const_RefreshAll;
    ShowRefresh(); 
    s_FrmInfo->ucLen = 5;
    return Ret_OK;
}

/*=========================================================================================\n
* @function_name: ReadSwVersion
* @function_file: DLT_645_2007.c
* @����: ��ȡ����汾��
* 
* 
* @����: 
* @param:s_FrmInfo  
* @����: 
* @����:   lwb (2012-06-24)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 ReadSwVersion(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{

    if(s_FrmInfo->ucCmd == 0x14)                //�����д�Ļ�
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }
    else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }

    if(s_FrmInfo->uiID.byte[1]==0x0A)
    {
        memcpy(s_FrmInfo->ucbuf,gCuc_VerSoft,12);                  //����汾��
        s_FrmInfo->ucErr.word =0;
        s_FrmInfo->ucLen=12;
    }
    else
    {
        s_FrmInfo->ucErr.word=ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
    }

    return Ret_OK;
}
/*=========================================================================================\n
* @function_name: ReadHisJSTime
* @function_file: DLT_645_2007.c
* @����: ��ȡ��ʷ����ʱ��
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   xuqf (2013-1-5)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
/*uint8 ReadHisJSTime(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
{   
    uint8  uiPt;                       //ָ��
    if(s_FrmInfo->ucCmd == 0x14)        //ָ��д
    {
        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;
    }
    else if(s_FrmInfo->ucCmd == 0x12)
    {
        s_FrmInfo->ucErr.word = ComErrBit_Other;
        s_FrmInfo->ucLen = 1;
        return Ret_OK;   
    }
    
    BE_ReadP(EEP_JSTIME_PT, &uiPt,1);                                     //��ȡָ��
    
    if(s_FrmInfo->uiID.byte[0] == 0xFF)                                   //����
    {
        BE_ReadP(EEP_JSTIME,XDATA_RAMZONE,48);
        
        for(uint8 i = 0;i < 12;i++)
        {
            memcpy(s_FrmInfo->ucbuf+i*4 , XDATA_RAMZONE+uiPt*4, 4);       //����
            uiPt++;
            uiPt%=12;
        }
        s_FrmInfo->ucLen = 48;
    }else
    {
        DataProcRead2Slice(JSSJJLID,s_FrmInfo->uiID.byte[0]-1,0,4,XDATA_RAMZONE);
        memcpy(s_FrmInfo->ucbuf , XDATA_RAMZONE, 4);
        s_FrmInfo->ucLen = 4;
    }
    
    return Ret_OK;
}*/
/*=========================================================================================\n
* @function_name: CmpCode
* @function_file: DLT_645_2007.c
* @����: ���ݱȶ�
* 
* 
* @����: 
* @param:s_FrmInfo  
* @����: 
* @����:   lm (2012-12-24)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 CmpCode(S_FRAMEINFO *s_FrmInfo)
{
//    uint8 ucChip;
//    uint8 ucKeyindex;
//    Word32 W32Factoraddr,W32Dataaddr;
//
////    if(s_FrmInfo->ucCmd == 0x14)
////    {
////        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
////        s_FrmInfo->ucLen = 1;
////        return Ret_OK;   
////    }
////    else if(s_FrmInfo->ucCmd == 0x12)
////    {
////        s_FrmInfo->ucErr.word = ComErrBit_Other;
////        s_FrmInfo->ucLen = 1;
////        return Ret_OK;   
////    }
//       
////    memcpy(s_FrmInfo->uiID.byte,s_FrmInfo->ucbuf,4);   //���ݱ�ʶ
////    memcpy(s_FrmInfo->uiUserCode.byte,s_FrmInfo->ucbuf+4,4);   //�����ߴ���
//    
////    if((s_FrmInfo->uiID.lword&0xffffff00)==0x07800200)
////    {
////        if(s_FrmInfo->uiID.byte[0]==0x01 || s_FrmInfo->uiID.byte[0]==0x02)
////        {
////            s_FrmInfo->ucbuf+=8;
////            s_FrmInfo->ucbuf[0] = 0;
////            s_FrmInfo->ucLen = 1;
////        }
////        else if(s_FrmInfo->uiID.byte[0]==0x03 || s_FrmInfo->uiID.byte[0]==0x04)
////        {
////            s_FrmInfo->ucbuf+=8;
////            memset(s_FrmInfo->ucbuf, 0, 4);
////            s_FrmInfo->ucLen = 4;
////        }
////        else if(s_FrmInfo->uiID.byte[0]==0xff)
////        {
////            ucChip = s_FrmInfo->ucbuf[8];
////            ucKeyindex = s_FrmInfo->ucbuf[9];
//            memcpy(W32Factoraddr.byte,s_FrmInfo->ucbuf+10,4);
//            memcpy(W32Dataaddr.byte,s_FrmInfo->ucbuf+14,4);
//            if(W32Factoraddr.lword>0x20000 || W32Dataaddr.lword>0x20000)
//            {
//                s_FrmInfo->ucErr.word=ComErrBit_Address;
//                s_FrmInfo->ucLen = 2; 
//                return Ret_OK;
//            }
////            else if(ucChip==0 && ucKeyindex==0)    //MCUΪ0������Ϊ0������128�ڣ� ������ʼ��ַ20000��
////            {
//                ReadFlash(W32Factoraddr.lword,  XDATA_RAMZONE1, 16);
////                GetFactor(guc_CmpFactor, XDATA_RAMZONE1);
//                //ReadFlash(W32Factoraddr.lword+128,  XDATA_RAMZONE1+128, 128);
//                if(GetCmpData(guc_CmpFactor, XDATA_RAMZONE1,0)==Ret_OK)
//                {
//                    ReadFlash(W32Dataaddr.lword,  XDATA_RAMZONE1, 128);
//                    ReadFlash(W32Dataaddr.lword+128,  XDATA_RAMZONE1+128, 128);
//                    //s_FrmInfo->ucbuf+=8;
//                    GetCmpData(XDATA_RAMZONE, XDATA_RAMZONE1,1);                                        
//                    //s_FrmInfo->ucLen = 64;
//                }
//                else
//                {
//                    s_FrmInfo->ucErr.word=ComErrBit_Address;
//                    s_FrmInfo->ucLen = 2; 
//                }
//                
////            }
////            else
////            {
////                s_FrmInfo->ucErr.word=ComErrBit_Other;
////                s_FrmInfo->ucLen = 2; 
////            }            
////        }
////        else
////        {
////            s_FrmInfo->ucErr.word=ComErrBit_Other;
////            s_FrmInfo->ucLen = 2; 
////        }
////    }
////    else
////    {
////        s_FrmInfo->ucErr.word=ComErrBit_Other;
////        s_FrmInfo->ucLen = 2; 
////    }    
    return Ret_OK;
}

/*=========================================================================================\n
* @function_name: Report
* @function_file: DLT_645_2007.c
* @����: ��ȡ�����ϱ�
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:    (2013-2-2)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
//uint8 Report(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
//{
    
//    if(s_FrmInfo->ucCmd == 0x11 || s_FrmInfo->ucCmd == 0x12)        //ָ���
//    {
//        if(gs_MeterMode.ReportMode == 0 && s_FrmInfo->ucCmd == 0x12)
//        {
//            s_FrmInfo->ucErr.word=ComErrBit_Other;
//            s_FrmInfo->ucLen = 1;  
//            return Ret_OK;
//        }
//        else if(s_FrmInfo->ucCmd == 0x12)
//        {
//            s_FrmInfo->uiID.lword = 0x04001501;
//            s_FrmInfo->ucbuf[10]= s_FrmInfo->ucbuf[0];            
//            //s_FrmInfo->ucLen = 11;  
//        }
//        
//        memcpy(s_FrmInfo->ucbuf, (uint8*)&gs_ReportState, sizeof(REPORTSTATE));       //
//        
//        if(gs_ReportState.ui_ReportState1.ReportWord1.CutBatRpt)
//        {
//            gs_ReportState.ui_ReportState1.ReportWord1.CutBatRpt = false;
//            gs_ReportStateFlg.ui_ReportState1.ReportWord1.CutBatRpt = true;
//        }
//        if(gs_ReportState.ui_ReportState1.ReportWord1.RtcBatRpt)
//        {
//            gs_ReportState.ui_ReportState1.ReportWord1.RtcBatRpt = false;
//            gs_ReportStateFlg.ui_ReportState1.ReportWord1.RtcBatRpt = true;
//        }
//        if(gs_ReportState.ui_ReportState1.ReportWord1.EepErrRpt)
//        {
//            gs_ReportState.ui_ReportState1.ReportWord1.EepErrRpt = false;
//            gs_ReportStateFlg.ui_ReportState1.ReportWord1.EepErrRpt = true;
//        }
//        if(gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt)
//        {
//            gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt = false;
//            gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt = true;            
//        }
//        if(gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt)
//        {
//            gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt = false;
//            gs_ReportStateFlg.ui_ReportState3.ReportWord3.PDirRpt = true;
//        } 
//        if(gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt)
//        {
//            gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt = false;
//            gs_ReportStateFlg.ui_ReportState2A.ReportWord2.PDirRpt = true;
//        }  
//        
//        s_FrmInfo->ucLen = 10 + (s_FrmInfo->ucCmd - 0x11); 
//
//    }else
//    {
//        s_FrmInfo->ucErr.word=ComErrBit_Other;
//        s_FrmInfo->ucLen = 1;
//    }
    
//    return Ret_OK;
//}
#ifdef LOADCURVE
/*=========================================================================================\n
* @function_name: ReadCurveJl
* @function_file: DLT_645_2007.c
* @����: 
* 
* 
* @����: 
* @param:s_FrmInfo  
* @param:CTable  
* 
* @����: 
* @return: uint8 
* @����:   lwb (2013-02-15)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
//uint8 ReadCurveJl(S_FRAMEINFO *s_FrmInfo, S_CORETAB  * CTable)
//{
//    S_CURVECNT s_CurveCnt; 
//    uint32 *ulCntPt;
//    uint8 i;
//    uint8 ucFrameNum;
//    uint8 ucLen;
//
//    if(s_FrmInfo->ucCmd == 0x14)        //ָ��д
//    {
//        s_FrmInfo->ucErr.word = ComErrBit_ErrPw;
//        s_FrmInfo->ucLen = 1;
//        return Ret_OK;
//    }
//
//
//    if(s_FrmInfo->ucCmd == 0x12)        //����֡
//    {
//        ucFrameNum=s_FrmInfo->ucbuf[0]; //����֡���
//    }else
//    {
//        if(CTable->uiOther>=0x05)       //��5��6�ฺ�ɼ�¼��û�з������������ݡ�
//        {
//            s_FrmInfo->ucLen = 0;
//            return Ret_OK;   
//        }
//    }
//
//    memset((uint8*)&s_CurveCnt,0x00,sizeof(S_CURVECNT));    //����
//    ulCntPt=&s_CurveCnt.ulTotalCnt;
//
//    for(i=0;i<5;i++)
//    {
//        BE_ReadP(gs_CurveStoreTab[i].uiCntAddr,(uint8*)&ulCntPt[i],3);       //��ȡ�ܴ���
//    }
//
//    if(s_FrmInfo->ucCmd==0x11)                                               //��֡
//    {
//        if(s_FrmInfo->ucLen<0x05
//           || JudgeIsNum(s_FrmInfo->ucbuf,1)==Const_Data_Err)
//        {
//            s_FrmInfo->ucErr.word = ComErrBit_Other;
//            s_FrmInfo->ucLen = 1;
//            return Ret_OK;        
//        }
//
//        gs_CurveFollowFrame.CurCnt=0;
//        gs_CurveFollowFrame.SeqFrameEnd=false;
//        if(APIByteBCD2HEX(s_FrmInfo->ucbuf[0])>ulCntPt[s_FrmInfo->uiID.byte[2]])
//        {
//            gs_CurveFollowFrame.TotalCnt=ulCntPt[s_FrmInfo->uiID.byte[2]];
//        }else
//        {
//            gs_CurveFollowFrame.TotalCnt=APIByteBCD2HEX(s_FrmInfo->ucbuf[0]);
//        }
//        if(gs_CurveFollowFrame.TotalCnt == 0)
//        {
//            gs_CurveFollowFrame.SeqFrameEnd=true;
//            s_FrmInfo->ucLen=0;
//            return Ret_OK;
//        }
//        
//    }else if(gs_CurveFollowFrame.SeqFrameEnd==true)
//    {
//            if(gs_MeterMode.ReportMode == 0)
//            {
//                s_FrmInfo->ucErr.word=ComErrBit_Other;
//                s_FrmInfo->ucLen = 1;  
//                return Ret_OK;
//            }
//            else
//            {
//                s_FrmInfo->uiID.lword = 0x04001501;
//                memcpy(s_FrmInfo->ucbuf, (uint8*)&gs_ReportState, sizeof(REPORTSTATE));       //
//                
//                if(gs_ReportState.ui_ReportState1.ReportWord1.CutBatRpt)
//                {
//                    gs_ReportState.ui_ReportState1.ReportWord1.CutBatRpt = false;
//                    gs_ReportStateFlg.ui_ReportState1.ReportWord1.CutBatRpt = true;
//                }
//                if(gs_ReportState.ui_ReportState1.ReportWord1.RtcBatRpt)
//                {
//                    gs_ReportState.ui_ReportState1.ReportWord1.RtcBatRpt = false;
//                    gs_ReportStateFlg.ui_ReportState1.ReportWord1.RtcBatRpt = true;
//                }
//                if(gs_ReportState.ui_ReportState1.ReportWord1.EepErrRpt)
//                {
//                    gs_ReportState.ui_ReportState1.ReportWord1.EepErrRpt = false;
//                    gs_ReportStateFlg.ui_ReportState1.ReportWord1.EepErrRpt = true;
//                }
//                if(gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt)
//                {
//                    gs_ReportState.ui_ReportState1.ReportWord1.CoverRpt = false;
//                    gs_ReportStateFlg.ui_ReportState1.ReportWord1.CoverRpt = true;            
//                }
//                if(gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt)
//                {
//                    gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt = false;
//                    gs_ReportStateFlg.ui_ReportState3.ReportWord3.PDirRpt = true;
//                }
//                if(gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt)
//                {
//                    gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt = false;
//                    gs_ReportStateFlg.ui_ReportState2A.ReportWord2.PDirRpt = true;
//                }  
//                s_FrmInfo->ucbuf[10]= ucFrameNum;            
//                s_FrmInfo->ucLen = 11;           
//            }
//            return Ret_OK;
//    }
//    
//    if(s_FrmInfo->uiID.byte[2]==0x00)                                         //��ȡ�ܸ��ɼ�¼
//    {
//        if((guc_LoadCurveMode&0x0F)!=0x00)
//        {
//            if(s_CurveCnt.ulTotalCnt==0)
//            {
//                s_FrmInfo->ucLen=0;
//                return Ret_OK;
//            }
//        }
//        else
//        {
//            s_FrmInfo->ucLen=0;
//            return Ret_OK;
//        }
//    }
//    s_FrmInfo->ucLen=0;
//    if(s_FrmInfo->uiID.byte[0]==0x00)           //����ļ�¼��
//    {
//        if(s_FrmInfo->uiID.byte[2]==0x00)       //��ȡ�ܸ��ɼ�¼
//        {
//            for(;gs_CurveFollowFrame.CurCnt<gs_CurveFollowFrame.TotalCnt;)
//            {
//                ucLen=GetTotalOldestRecord(gs_CurveFollowFrame.CurCnt,s_FrmInfo->ucbuf+s_FrmInfo->ucLen);
//                if((s_FrmInfo->ucLen+ucLen)>200)
//                {
//                    gs_CurveFollowFrame.SeqFrameEnd=false;
//                    break;
//                }
//                s_FrmInfo->ucLen+=ucLen;
//                gs_CurveFollowFrame.CurCnt++;
//            }
//        }else                                   //��ȡ�ָ��ɼ�¼
//        {
//            if(guc_LoadCurveMode&BIT8MAP[(s_FrmInfo->uiID.byte[2]-1)])
//            {
//                for(;gs_CurveFollowFrame.CurCnt<gs_CurveFollowFrame.TotalCnt;)
//                {
//                    ucLen=GetOneOldestRecord(s_FrmInfo->uiID.byte[2],gs_CurveFollowFrame.CurCnt,s_FrmInfo->ucbuf+s_FrmInfo->ucLen);
//                    if((s_FrmInfo->ucLen+ucLen)>200)
//                    {
//                        gs_CurveFollowFrame.SeqFrameEnd=false;
//                        break;
//                    }
//                    s_FrmInfo->ucLen+=ucLen;
//                    gs_CurveFollowFrame.CurCnt++;
//                }
//            }else
//            {
//                s_FrmInfo->ucLen=0;
//                return Ret_OK;
//            }
//        }
//    }
//    else if(s_FrmInfo->uiID.byte[0]==0x01)     //����ʱ��ļ�¼��
//    {
//        if(s_FrmInfo->uiID.byte[2]==0x00)       //��ȡ�ܸ��ɼ�¼
//        { 
//            if(s_FrmInfo->ucCmd==0x11)
//            {
//                s_FrmInfo->ucLen=GetTotalTmRecord(s_FrmInfo->ucbuf+1,s_FrmInfo->ucbuf);
//                if(s_FrmInfo->ucLen==0)
//                {
//                    s_FrmInfo->ucLen = 0;
//                    gs_CurveFollowFrame.SeqFrameEnd=true;
//                    return Ret_OK;
//                }else if(gs_CurveFollowFrame.SeqFrameEnd==true)
//                {
//                    return Ret_OK;
//                }
//                gs_CurveFollowFrame.CurCnt++;
//            }
//
//            for(;gs_CurveFollowFrame.CurCnt<gs_CurveFollowFrame.TotalCnt;)
//            {
//                ucLen=GetTotalTmSeqRecord(gs_CurveFollowFrame.CurCnt,s_FrmInfo->ucbuf+s_FrmInfo->ucLen);
//                if(ucLen==0)
//                {
//                    if(gs_CurveFollowFrame.TotalCnt < s_CurveCnt.ulTotalCnt)
//                    {
//                        gs_CurveFollowFrame.TotalCnt++;
//                    }
//                }
//                if((s_FrmInfo->ucLen+ucLen)>200)
//                {
//                    gs_CurveFollowFrame.SeqFrameEnd=false;
//                    break;
//                }
//                s_FrmInfo->ucLen+=ucLen;
//                if(gs_CurveFollowFrame.SeqFrameEnd==true)
//                {
//                    break;
//                }
//                gs_CurveFollowFrame.CurCnt++;
//            }
//        }else                                   //��ȡ�ָ��ɼ�¼
//        {
//            if(guc_LoadCurveMode&BIT8MAP[(s_FrmInfo->uiID.byte[2]-1)])
//            {
//                if(s_FrmInfo->ucCmd==0x11)
//                {    
//                    s_FrmInfo->ucLen=GetOneTmRecord(s_FrmInfo->uiID.byte[2],s_FrmInfo->ucbuf+1,s_FrmInfo->ucbuf);
//                    if(s_FrmInfo->ucLen==0)
//                    {
//                        s_FrmInfo->ucLen = 0;
//                        gs_CurveFollowFrame.SeqFrameEnd=true;
//                        return Ret_OK;
//                    }
//                    else if(gs_CurveFollowFrame.SeqFrameEnd==true)
//                    {
//                        return Ret_OK;
//                    }
//                    gs_CurveFollowFrame.CurCnt++;
//                }
//
//                for(;gs_CurveFollowFrame.CurCnt<gs_CurveFollowFrame.TotalCnt;)
//                {
//                    ucLen=GetOneTmSeqRecord(s_FrmInfo->uiID.byte[2],gs_CurveFollowFrame.CurCnt,s_FrmInfo->ucbuf+s_FrmInfo->ucLen);
//                    if((s_FrmInfo->ucLen+ucLen)>200)
//                    {
//                        gs_CurveFollowFrame.SeqFrameEnd=false;
//                        break;
//                    }
//                    s_FrmInfo->ucLen+=ucLen;
//                    if(gs_CurveFollowFrame.SeqFrameEnd==true)
//                    {
//                        break;
//                    }
//                    gs_CurveFollowFrame.CurCnt++;
//                }
//
//            }else
//            {
//                s_FrmInfo->ucLen=0;
//                return Ret_OK;
//            }
//        }
//    }
//    else if(s_FrmInfo->uiID.byte[0]==0x02)     //���һ����¼��
//    {
//        if(s_FrmInfo->ucbuf[0]!=0x01)
//        {
//            s_FrmInfo->ucErr.word = ComErrBit_Nodata;
//            s_FrmInfo->ucLen = 1;
//            return Ret_OK;
//        }else
//        {
//    
//            if(s_FrmInfo->uiID.byte[2]==0x00)       //��ȡ�ܸ��ɼ�¼
//            {  
//                s_FrmInfo->ucLen=GetTotalItemRecord(s_FrmInfo->ucbuf);
//            }else                                   //��ȡ�ָ��ɼ�¼
//            {   
//                if(guc_LoadCurveMode&BIT8MAP[(s_FrmInfo->uiID.byte[2]-1)])
//                {
//                    s_FrmInfo->ucLen=GetOneItemRecord(s_FrmInfo->uiID.byte[2],s_FrmInfo->ucbuf);
//                }else
//                {
//                    s_FrmInfo->ucLen=0;
//                    return Ret_OK;
//                }
//            }
//        }
//        gs_CurveFollowFrame.SeqFrameEnd=true;
//    }
//
////  gs_CurveFollowFrame.CurCnt++;
//    if(gs_CurveFollowFrame.CurCnt>=gs_CurveFollowFrame.TotalCnt)
//    {
//        if(s_FrmInfo->ucCmd==0x12)
//        {
//            s_FrmInfo->ucbuf[s_FrmInfo->ucLen++]=ucFrameNum;            
//        }
//        gs_CurveFollowFrame.SeqFrameEnd=true;
//        return Ret_OK;
//    }else
//    {
//        if(s_FrmInfo->ucCmd==0x12)
//        {
//            s_FrmInfo->ucbuf[s_FrmInfo->ucLen++]=ucFrameNum;
//        }
//
//        if(s_FrmInfo->ucLen!=0
//           &&(gs_CurveFollowFrame.SeqFrameEnd==false))
//        {
//            s_FrmInfo->ucCmd|=0x20;
//        }
//        else if(gs_MeterMode.ReportMode == 1)
//        {
//            //���һ֡cmd=0x92;          
//            if((gs_ReportState.ui_ReportState1.Word.word&0x0528) != 0 
//               || (gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt != 0)  
//               || (gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt != 0))//��ֹʽ������ж�
//            {
//                s_FrmInfo->ucCmd|=0x20;
//            }
//        }
//    }

//    return Ret_OK;
//}

#endif
