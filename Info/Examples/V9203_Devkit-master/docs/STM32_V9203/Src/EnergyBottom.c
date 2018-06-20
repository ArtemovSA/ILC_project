#define  ENERGYBOTTOMEXT
#include "Includes.h"
/*=========================================================================================\n
* @function_name: EnyB_Open_Cf
* @function_file: EnergyBottom.c
* @����: ����CF����
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void EnyB_Open_Cf(void)
{   
    if(guc_RtcConflict==0x55)
    {
        gui_RefreshEvent|=flgEtPara_RTC;    
        guc_RtcConflict=0x00;
    }

    if(guc_OpenCfTmCnt<1)
    {
        guc_OpenCfTmCnt++;
    }else
    {
        if(guc_CfOpenFlg&&(guc_OpenCfTmCnt>=1))
        {   
//            EnyB_SetMeterCfgACK(0xd8,PMCtrl4);  //�򿪼���,����Ǳ������
            EnyB_InitCFInt();                   //����CF�����ж�
             //TIM2_Int_Init(99,3199); 
            guc_CfOpenFlg=false;
        }
    }

//    if(guc_FrsCalRTCDelay>0)                    //��ʱ5s
//    {
//        guc_FrsCalRTCDelay--;
//        if(guc_FrsCalRTCDelay==0)
//        {
//            CalRTC();                           //�ϵ��һ��У��RTC��Ҫ��AD�ȶ�
//        }
//    }

}
/*========================================================
* @function_name: EnyB_InitCFInt
* @function_file: EnergyBottom.c
* @����: ��ʼ��CF�ж�
* 
* @����: lwb (2012/3/3)
* @��ע: 
*----------------------------------------------------------
* @�޸���: 
* @�޸�����:  
============================================================*/
void EnyB_InitCFInt(void)
{
//  EIE|=BIT0;
//  ExInt2IE|=BIT7;
//  P9OE&=(~BIT6);                          //��CF IO���
//  P9FC|=(BIT6);
}
/*=========================================================================================\n
* @function_name: EnyB_SetMeterRule
* @function_file: EnergyBottom.c
* @����: ���ü���ģ������
* 
* @����: 
* @����: 
* @����:  Vango Application Teams
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void EnyB_SetMeterRule(void)
{
    BroncoInit();
    EnyB_PurInit();         //����ģ���ϵ��ʼ��
}
/*=========================================================================================\n
* @function_name: EnyB_PurInit
* @function_file: EnergyBottom.c
* @����: �ϵ��ʼ��
* 
* @����: 
* @����: 
* @����:   Vango Application Teams
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void EnyB_PurInit(void)                                 
{
    Eny_RefVar();                                       //��������ˢ��
    IWDG_ReloadCounter();                         //ι��
    Eny_RefCalcType();                                  //������ʽ��ˢ��
    IWDG_ReloadCounter();                         //ι��
    Eny_PurCheckRAMEny();                               //���RAM����
    IWDG_ReloadCounter();                         //ι��
}
extern const uint8  InitPara5[sizeof(S_JBPM)-2];
/*=========================================================================================\n
* @function_name: EnyB_JbPm_Init
* @function_file: EnergyBottom.c
* @����: У������ĳ�ʼ��
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void EnyB_JbPm_Init(void)                                
{                                                        
  //  memcpy((uint8*)&gs_JbPm.ui_MeterC,InitPara5,sizeof(S_JBPM)-2);  //����Flash���ݵ�RAM��
   // gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);     // У�������CRC���
//  V9281_InitDefaultPara();
}

/*=========================================================================================\n
* @function_name: EnyB_IntCF
* @function_file: EnergyBottom.c
* @����: �����ж�
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-07)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void EnyB_IntCF(uint8 power)                       
{
    uint8 *pCfZ;
    uint8 *pEyZ;
    if(power == active)
    {
        if(gs_MeterState.ui_MeterState2.RunCode2Word.PADIR)      //�ж��Ƿ��Ƿ���
        {
            pEyZ=&gs_Energy.ucNz;
            pCfZ=&gs_PowerCf.uc_Nz;
        }else
        {
            pEyZ=&gs_Energy.ucPz;                                   //��������
            pCfZ=&gs_PowerCf.uc_Pz;                                 //����������
        }


        (*pCfZ)++;                                                  //�������ۼ�
        if(*pCfZ>=guc_GateCF)                                       //�б��Ƿ���Ҫ����
        {
            *pCfZ -=guc_GateCF;                                     //��CF������з�Ƶ������Ƶ������������ۼ�
            *pEyZ +=guc_UnitCF; 
            gui_SystemEvent |= flgEtGlb_EnergyCalc;                 //�������������¼�
        }
    }
    else
    {
         if(gs_MeterState.ui_MeterState2.RunCode2Word.PADIR)      //�ж��Ƿ��Ƿ���
        {
            pEyZ=&gs_NEnergy.ucNz;
            pCfZ=&gs_PowerCfN.uc_Nz;
        }else
        {
            pEyZ=&gs_NEnergy.ucPz;                                   //��������
            pCfZ=&gs_PowerCfN.uc_Pz;                                 //����������
        }


        (*pCfZ)++;                                                  //�������ۼ�
        if(*pCfZ>=guc_GateCF)                                       //�б��Ƿ���Ҫ����
        {
            *pCfZ -=guc_GateCF;                                     //��CF������з�Ƶ������Ƶ������������ۼ�
            *pEyZ +=guc_UnitCF; 
            gui_SystemEvent |= flgEtGlb_NEnergyCalc;                 //�������������¼�
        }
    }
}

/*=========================================================================================\n
* @function_name: ReadRMSPara
* @function_file: EnergyBottom.c
* @����: ��ȡ��ʾ����ϵ��k
* 
* 
* @����: 
* @param:ucpos  
* 
* @����: 
* @return: uint32 
* @����:   lwb (2012-05-29)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint32 ReadRMSPara(uint8 ucpos)
{
    uint32 TempValue;
    TempValue=0x1000;

    BE_ReadP(EEP_JBTOTAL+sizeof(S_JBPM)-19+ucpos*4,(uint8*)&TempValue,4);
    TempValue/=65536;
    return TempValue;
}
/*=========================================================================================\n
* @function_name: CalRMS
* @function_file: EnergyBottom.c
* @����: ����˲ʱ��
* 
* 
* @����: 
* @param:addr  �Ĵ�����ַ
* 
* @����: 
* @return: uint32 ������Чֵ
* @����:  Vango Application Teams
* @��ע: 
===========================================================================================*/
Word32 TempAA;
uint32 CalRMS(uint16 addr)
{
    Word32 TempValue;
    uint32 TempPara,TempValue1;

    TempValue.lword = ReadBronco(addr);

    switch (addr)
    {
      case DATAPA:
      case DATAPB:
      case DATAPC:
      case DATAPABC:  
          if(TempValue.byte[3]>0x7f)          //����Ǹ����������ȱ������
          {
              TempValue.lword=~TempValue.lword+1;
          }

          TempPara=gs_JbPm.ul_PG;        //��ȡ����ϵ��

          if(TempPara==0)
          {
              TempValue1=0;
          }
          else
          {
               TempValue.lword=((TempValue.lword)/TempPara);
          }
          if(TempValue.lword<(((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*2/100))
          {
               TempValue1=0;
          }
          else
          {
               TempValue1=Hex2BCD(TempValue.lword);
               if(TempValue1 >100)
               {
                  TempAA.lword = TempValue.lword;
               }
//               if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir==true)   //������Ӹ���
//               {
//                   TempValue1|=0x800000;
//               }
          }
          break;
        case RMSUA:
        case RMSUB:
        case RMSUC:
        case RegARTUA:
        case RegARTUB:
        case RegARTUC:         
          if(TempValue.byte[3]>0x7f)          //����Ǹ����������ȱ������
          {
              TempValue.lword=~TempValue.lword+1;
          }
           TempPara=gs_JbPm.ul_URmG;   //ReadRMSPara(1);
           if(TempPara==0)
           {
              return 0;
           }
           else
           {
               TempValue.lword=TempValue.lword/TempPara;
           }

           if(TempValue.lword<ULEVEL)
           {
               return 0;
           }else
           {
               TempValue1=Hex2BCD(TempValue.lword);
               if(TempValue1 >0x2300)
               {
                  TempAA.lword = TempValue.lword;
               }
           }
           break;
        case RMSI1A:
        case RMSI1B:
        case RMSI1C:
        case RegARTIA:
        case RegARTIB:
        case RegARTIC:
          //�����ļ���
          TempPara=gs_JbPm.ul_I1RmG;    //ReadRMSPara(2);
          if(TempPara==0)
          {
              return 0;
          }
          else
          {
              TempValue.lword=TempValue.lword/TempPara;
          }

          if(TempValue.lword<((gs_JbPm.ui_Ib/1000)*2))
          {    
              return 0;
          }else
          {
              TempValue1=Hex2BCD(TempValue.lword);
              //���ʵļ���
              /*if(ReadRaccoon(DATAP,1,phyadd))
              {
                  memcpy((uint8*)TempValue2.byte,gs_RacCtrl.ucBuf+3,4);
              }else
              {
                  TempValue2.lword=0;
              }

              if(TempValue2.byte[3]>0x7f)                         //����Ǹ����������ȱ������
              {
                  TempValue2.lword=~TempValue2.lword+1;
              }

              TempPara=gs_JbPm.ul_PG;        //��ȡ����ϵ��

              if(TempPara==0)
              {
                  TempValue2.lword=0;
              }
              else
              {
                  TempValue2.lword=((TempValue2.lword)/TempPara);
              }

              if(TempValue2.lword<(((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*2/100))
              {
                  //�������С���������ʣ����ǵ���С��0.2A,��������
                  if(TempValue1<0x200)
                  {
                      TempValue1=0;
                  }*/
//              if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir==true)   //������Ӹ���
//              {
//                  TempValue1|=0x800000;
//              }
          }
          break;
        case DATAFREQ:
            TempValue.lword=(327680000)/TempValue.lword;
            TempValue1=Hex2BCD(TempValue.lword);
           break;
      default:
          TempValue1=0;
          break;
    }
    return TempValue1;
}
/*=========================================================================================\n
* @function_name: CalRMS2
* @function_file: EnergyBottom.c
* @����: 
* 
* 
* @����: 
* @param:addr  
* @param:value  
* 
* @����: 
* @return: uint32 
* @����:   lwb (2013-09-27)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint32 CalRMS2(uint16 addr,uint32 value)
{
    uint32 TempPara,TempValue1;
    switch (addr)
    {
        case DATAP:
        case DATAQ:
          TempPara=gs_JbPm.ul_PG;        //��ȡ����ϵ��
          if(TempPara==0)
          {
              TempValue1=0;
          }
          else
          {
              value=((value)/TempPara);
          }

        //  if(value<(((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*2/100))
         // {
        //      TempValue1=0;
       //   }
        //  else
          {
              TempValue1=Hex2BCD(value*10);
              if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir==true)   //������Ӹ���
              {
                  TempValue1|=0x800000;
              }
          }
          break;
      case RMSU:
           TempPara=gs_JbPm.ul_URmG;   //ReadRMSPara(1);
           if(TempPara==0)
           {
              return 0;
           }
           else
           {
               value=value/TempPara;
           }

           if(value<ULEVEL)
           {
               return 0;
           }else
           {
               TempValue1=Hex2BCD(value);
           }
           break;
      case RMSI1:
          //�����ļ���
          TempPara=gs_JbPm.ul_I1RmG;    //ReadRMSPara(2);
          if(TempPara==0)
          {
              return 0;
          }
          else
          {
              value=value/TempPara;
          }

         // if(value<((gs_JbPm.ui_Ib/1000)*3))
        //  {
        //      return 0;
      //    }else
          {
              TempValue1=Hex2BCD(value);
              if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir==true)   //������Ӹ���
              {
                  TempValue1|=0x800000;
              }
          }
          break;
          default:
              break;
    }
    return TempValue1;
}
/*=========================================================================================\n
* @function_name: CalCos
* @function_file: EnergyBottom.c
* @����: ���㹦������
* 
* @����: 
* 
* @����: 
* @return: uint16 
* @����:  Vango Application Teams
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint32 CalCos(uint8 phy)
{
    Word32 TempValue;
    
    TempValue.lword= ReadBronco(RegMAFA+phy);
    if(TempValue.byte[3] > 0x80)
    {
        return 1000;
    }
    else
    {
        return TempValue.byte[3]*1000/0x80;
    }
    

    //return 0;
}
/*=========================================================================================\n
* @function_name: EnyB_Check_PowRev
* @function_file: EnergyBottom.c
* @����: ��⹦���Ƿ���
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-08)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void EnyB_Check_PowRev(void)
{
//    int32 l_pa;
//    uint32 TempPara;
//    uint32 ulPower;
////
////    l_pa = EnyB_ReadMeterParaACK(DATAP);
//    if(l_pa>0)
//    {                                       //���ʷ���
////        TempPara=gs_JbPm.ul_PG>>8;          //��ȡ����ϵ��
//        if(TempPara==0)
//        {                                   //У��������ʼ���ֵΪ0�Ļ���ֱ���жϹ���ԭʼֵ
//            if(l_pa>0x2000)
//            {
//                gs_PDirCnt.ucRevDirCnt++;
//                gs_PDirCnt.ucDirCnt=0;
//                if(gs_PDirCnt.ucRevDirCnt>=gs_PDirCtr.ucDelayTm)
//                {
//                    gs_MeterState.ui_MeterState1.RunCode1Word.PDir=true;
//                }
//            }
//            else
//            {
//                gs_PDirCnt.ucRevDirCnt=0;
//                gs_PDirCnt.ucDirCnt++;
//                if(gs_PDirCnt.ucDirCnt>=gs_PDirCtr.ucDelayTm)
//                {
//                    gs_MeterState.ui_MeterState1.RunCode1Word.PDir=false;
//                }
//            }
//        }else
//        {
//             ulPower=((l_pa*32)/TempPara)*8;
//             if(ulPower>gs_PDirCtr.ul_PDirGate
//                /*(((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*2/100)*/)
//             {
//                 gs_PDirCnt.ucRevDirCnt++;
//                 gs_PDirCnt.ucDirCnt=0;
//                 if(gs_PDirCnt.ucRevDirCnt>=gs_PDirCtr.ucDelayTm)
//                 {
//                     gs_MeterState.ui_MeterState1.RunCode1Word.PDir=true;
//                 }
//             }else
//             {
//                 gs_PDirCnt.ucRevDirCnt=0;
//                 gs_PDirCnt.ucDirCnt++;
//                 if(gs_PDirCnt.ucDirCnt>=gs_PDirCtr.ucDelayTm)
//                 {
//                     gs_MeterState.ui_MeterState1.RunCode1Word.PDir=false;
//                 }
//             }
//        }
//    }else
//    {
//        gs_PDirCnt.ucRevDirCnt=0;
//        gs_PDirCnt.ucDirCnt++;
//        if(gs_PDirCnt.ucDirCnt>=gs_PDirCtr.ucDelayTm)
//        {
//            gs_MeterState.ui_MeterState1.RunCode1Word.PDir=false;
//        }
//    }
//
//    if(gs_PDirCtr.ul_PDirGate == 0)
//    {
//        gs_MeterState.ui_MeterState1.RunCode1Word.PDir=false;
//    }
//
//    if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir)
//    {
////     gs_MeterState.ui_MeterState1.RunCode1Word.PDir = true;               //�ù��ʷ����־
//       gs_MeterState.ui_MeterState2.RunCode2Word.PADIR= true;
//       if(gs_ReportStateFlg.ui_ReportState3.ReportWord3.PDirRpt == false    // ״̬��־δ��
//          && gs_ReportWord.ui_ReportWord3.ReportWord3.PDirRpt == 1)         // ��Ҫ�����ϱ�
//       {
//           gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt = true;       // ״̬��λ
//       }
//       if(gs_ReportStateFlg.ui_ReportState2A.ReportWord2.PDirRpt == false   // ״̬��־δ��
//          && gs_ReportWord.ui_ReportWord2.ReportWord2.PDirRpt == 1)         // ��Ҫ�����ϱ�
//       {
//           gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt = true;      // ״̬��λ
//       }
////     gut_SysWarn.SysWarn.REVPOWER=true;
//       if(gs_SysRunSt.PDirFR==false
////        &&(gs_DispStt.ucMode!=Const_DispMode_Full)
//          )
//       {
//
////         gs_DispStt.ucMode=Const_DispMode_Err;
////         gui_SystemEvent |= flgEtGlb_RefreshLcd;
//
////         #ifdef FRCom
////              EVENTOUTHIGH();
////              gs_FREventOut.FREventOutStruct.PDirFR=true;
////         #endif
//           gs_SysRunSt.PDirFR=true;
//       }
//    }
//    else
//    {
////     gut_SysWarn.SysWarn.REVPOWER=false;
////     gs_MeterState.ui_MeterState1.RunCode1Word.PDir = false;      //�幦�ʷ����־
//       gs_MeterState.ui_MeterState2.RunCode2Word.PADIR= false;
//       gs_SysRunSt.PDirFR=false;
//       if(gs_ReportStateFlg.ui_ReportState3.ReportWord3.PDirRpt == true)     // �ж�״̬��־
//       {
//           gs_ReportStateFlg.ui_ReportState3.ReportWord3.PDirRpt = false;
//       }
//       if(gs_ReportStateFlg.ui_ReportState2A.ReportWord2.PDirRpt == true)     // �ж�״̬��־
//       {
//           gs_ReportStateFlg.ui_ReportState2A.ReportWord2.PDirRpt = false;
//       }
//    }
}
/*=========================================================================================\n
* @function_name: EnyB_Cal_Cos
* @function_file: EnergyBottom.c
* @����: ���㹦������
* 
* @����: 
* 
* @����: 
* @return: int16 
* @����:   lwb (2012-03-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
//int16 EnyB_Cal_Cos(void)
//{
//    uint32 ul_PS;
//    int16 CosA = 0;
//    if(gs_EnyInsData.ul_IARms < 40)
//    {
//        return 1000;
//    }
//    ul_PS = gs_EnyInsData.ul_URms*gs_EnyInsData.ul_IARms;
//    ul_PS /= 10000;
//    CosA = (gs_EnyInsData.l_PowerPA * 1000)/ (int32)ul_PS;
//    return CosA;
//}
 
/*=========================================================================================\n
* @function_name: EnyB_JbPm_Updata
* @function_file: EnergyBottom.c
* @����: ˢ��У�������SFR��
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void EnyB_JbPm_Updata(void)
{
        UpdateBroncoPara();
}

/*=========================================================================================\n
* @function_name: EnyB_JbPm_Updata
* @function_file: EnergyBottom.c
* @����: ˢ��У�������SFR��
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void RecoverPowDirConfig(void)
{
    gs_PDirCnt.ucDirCnt=0;
    gs_PDirCnt.ucRevDirCnt=0;
    SysE2ReadData(EEP_PDIRTIME,(uint8*)&gs_PDirCtr.ucDelayTm,4);    //���·������ж���ʱ�ͷ�ֵ 
    gs_PDirCtr.ucDelayTm=BCD2Hex(gs_PDirCtr.ucDelayTm);
    gs_PDirCtr.ul_PDirGate=BCD2Hex(gs_PDirCtr.ul_PDirGate&0x00ffffff);   
}

/*=============================================================================================\n
Function���Զ�У���㷨
 ================================================================================================*/

/*=========================================================================================\n
* @function_name: CalculateSC
* @function_file: EnergyBottom.c
* @����: ���㹫ʽ
* 
* 
* @����: 
* @param:CurValue  
* @param:TarValue  
* @param:LastSC  
* 
* @����: 
* @return: uint32 
* @����:   lwb (2012-03-08)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
/*uint32 CalculateSC(int32 CurValue,int32 TarValue,int32 LastSC)
{
    //���㹫ʽ����,���ı�2011��7��14��
    int16 Error;                    
    uint16 Stemp0remd,Stemp2remd;  
    int32 Stemp0,Stemp1,Stemp2,Stemp3;//
    uint32 Result;

    //��ʽ˵�� ������ʽ S=2^31(1/(1+e)-1)+S1(1/(1+e)),���e����1000����
    //���⹫ʽ��:  S=  2^31(-e/(1000+e))+S1(1000/(1000+e)=S1(1000/(1000+e)-2^31(e/(1000+e))
    //�������  2^31(e/(1000+e))
    Error=(CurValue-TarValue)*1000/TarValue;            //�������ֵ��������1000
    Stemp0=0x80000000/(1000+Error);                     //�ȱ����������� 2^31/(1000+e)=0x80000000/(1000+e)
    Stemp0remd=0x80000000%(1000+Error);                 //��������
    Stemp0remd=Stemp0remd*1000/(1000+Error);            //����С�����֣�С����������1000
    Stemp1=Stemp0*Error+(Stemp0remd*Error)/1000;        //�Ѽ����������С�����ֶ�����ERROR����2^31(e/(1000+e))

    //�������S1*(1000/(1000+e)
    Stemp2=LastSC/(1000+Error);                         //������������
    Stemp2remd=LastSC%(1000+Error);                     //������������
    Stemp2remd=Stemp2remd*1000/(1000+Error);            //С������
    Stemp3=Stemp2*1000+Stemp2remd;                      //�������ּ�С������  

    Result=Stemp3-Stemp1;                               //S=S1(1000/(1000+e)-2^31(e/(1000+e))
    return Result;
 
}*/


/*=========================================================================================\n
* @function_name: CalculateAC
* @function_file: EnergyBottom.c
* @����: ���㹫ʽ
* 
* 
* @����: 
* @param:CurValue  
* @param:TarValue  
* @param:LastSC  
* 
* @����: 
* @return: uint32 
* @����:   lwb (2012-06-18)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
/*uint8 CalculateAC(int32 CurValue,int32 TarValue)
{
    //���㹫ʽ����,���ı�2011��7��14��
    int16 Error;                    
    uint8 Result;

    Error=(CurValue-TarValue)*1000/TarValue;            //�������ֵ��������1000
    Result = (int32)3011*Error/2000;

    return Result; 
}*/
uint8 CalculateAC( int16 Error)
{
    //���㹫ʽ����            
    uint8 Result;
    Result = (int32)3011*Error*4/2000;
    return Result; 
}

 
/*=========================================================================================\n
* @function_name: EnyB_AutoChkPW
* @function_file: EnergyBottom.c
* @����: У�����ʺ����
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-08)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void EnyB_AutoChkPW(void)
{
//    uint8 ucData[12];                        //�����׼����ֵ
//    uint8 i;
//    int32 l_SdPower[3];                        //��׼����ֵ
//    int16  err[3];
//    int32 pa[3];
//   BE_ReadP(EEP_AUTOCAL,ucData,12);         //��ȡ��׼����ֵ 
//   for(i=0;i<3;i++)
//   {
//       l_SdPower[i] = Uint8_To_Uint32(ucData + 4*i)/10;   //��ñ�׼����ֵ
//       pa[i] = CalRMS2(DATAP,gs_power.ul_power[i]);      //ʵ�ʹ���ֵ
//       pa[i] =BCD2Hex(pa[i]);
//       err[i] = (pa[i] -  l_SdPower[i])*1000/ l_SdPower[i] ;   //�������
//       *ul_pRacReg[i] = CalculateSC(err[i],*ul_pRacReg[i]); //�����µıȲ�ֵ������У����
//   }
//   
////  pa = EnyB_ReadMeterParaACK(DATAP);        //��ȡ����ֵ�������ж�
////
////  if(pa>0)           //���Ȱѱ�ƶ�ȡ�Ĺ���ֵת����ֵ
////  {
////     pa= -BCD2Hex(CalRMS(DATAP));//gs_EnyInsData.l_PowerPA;
////  }else
////  {
////     pa= BCD2Hex(CalRMS(DATAP));//-gs_EnyInsData.l_PowerPA;
////  }
//
//    pa=CalRMS(DATAP);               //��ȡ����
//    if(pa&0x800000)                 //�жϷ���
//    {
//        pa&=~0x800000;               
//    } 
//
//    pa =BCD2Hex(pa);
//    gs_JbPm.ul_SCP = CalculateSC(pa, l_SdPower, ul_Scp);   //����Ȳ�ֵ
// 
//    EnyB_SetMeterCfgACK(gs_JbPm.ul_SCP, SCP);                                 //�����±Ȳ�ֵ
}

/*=========================================================================================\n
* @function_name: EnyB_AutoChkPW2
* @function_file: EnergyBottom.c
* @����: С�ź�У�����ʺ����
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-08)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
//uint32 * const ul_pRacSReg[3] ={(uint32*)&gs_JbPm.gs_JBA.RacWWAPT,(uint32*)&gs_JbPm.gs_JBB.RacWWAPT,
 //                               (uint32*)&gs_JbPm.gs_JBC.RacWWAPT};
void EnyB_AutoChkPW2(void)
{
//    uint8 ucData[12],ucData1[4];                        //�����׼����ֵ
//    uint8 i;
//    int32 l_SdPower[3];                        //��׼����ֵ
//    int32 l_SdPreg;
//    int32  err[3];
//    int32 pa[3];
//    BE_ReadP(EEP_AUTOCAL,ucData,12);         //��ȡ��׼����ֵ 
//    BE_ReadP(EEP_AUTOCAL+24,ucData1,4);         //��ȡ��׼����ֵ 
//    l_SdPreg = Uint8_To_Uint32(ucData1);
//    for(i=0;i<3;i++)
//    {
//       l_SdPower[i] = Uint8_To_Uint32(ucData + 4*i)/10;   //��ñ�׼����ֵ
//       
//       pa[i] = CalRMS2(DATAP,gs_power.ul_power[i]);      //ʵ�ʹ���ֵ
//       pa[i] =BCD2Hex(pa[i]);
//       err[i] = (pa[i] -  l_SdPower[i])*1000/ l_SdPower[i] ;   //�������
//       *ul_pRacSReg[i] = (0-err[i])*l_SdPreg/1000 ;
//    }
//    gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);         // У�������CRC���
//    SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));         //д��E2��
//    EnyB_JbPm_Updata();   
//    uint8 ucData[4];                        //�����׼����ֵ
//    uint8 i;
//    int32 l_SdPower;                        //��׼����ֵ   
//    Word32 TempValue;
//    uint32 TempSum;
//    uint32 pa_sum,TempPara;
//    
//    pa_sum = 0;
//    TempSum = 0;
//    BE_ReadP(EEP_AUTOCAL,ucData,4);         //��ȡ��׼����ֵ  
//
//    l_SdPower = Uint8_To_Uint32(ucData);   //��ñ�׼����ֵ
//
//    for(i=0;i<16;i++)
//    {
//        TempValue.lword=EnyB_ReadMeterParaACK(DATAIP); 
//        DelayXms(20);
//        CLRWDT();                           //ι��
//        //DelayXms(255);
//        if(TempValue.byte[3]>0x7f)          //����Ǹ����������ȱ������
//        {
//            TempValue.lword=~TempValue.lword+1;
//        }
//        TempSum += TempValue.lword;
//    }   
//    
//    TempSum = TempSum>>4;              //��ȡ���ʼĴ���ֵ
//    TempPara = gs_JbPm.ul_PG/256;        //��ȡ����ϵ��
//    pa_sum = (TempSum*2560)/TempPara;  //����ֵ
//      
//    TempValue.lword = l_SdPower*TempPara/2560;    //���ۼĴ���ֵ
//    
//    if(l_SdPower > pa_sum)
//      gs_JbPm.ul_PARAPC = (TempValue.lword*(l_SdPower-pa_sum)/l_SdPower);       //������β���ֵ
//    else
//      gs_JbPm.ul_PARAPC = -(TempValue.lword*(pa_sum-l_SdPower)/l_SdPower);   //������β���ֵ
//    EnyB_SetMeterCfgACK(gs_JbPm.ul_PARAPC, PARAPC);                                 //�����±Ȳ�ֵ
}
 
/*=========================================================================================\n
* @function_name: EnyB_AutoChkIrms
* @function_file: EnergyBottom.c
* @����: 
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-08)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
//uint32 * const ul_pRacIReg[3] ={(uint32*)&gs_JbPm.gs_JBA.RacWARTI,(uint32*)&gs_JbPm.gs_JBB.RacWARTI,
//                               (uint32*)&gs_JbPm.gs_JBC.RacWARTI};
void EnyB_AutoChkIrms(void)
{
//    uint8 ucData[2];
//    uint32  ul_SdCurrent;
//    uint32  ul_Irmg;
//    int32  l_IARms;
//
//    BE_ReadP(EEP_AUTOCAL+12,ucData,2);      //��ȡ��׼����ֵ
//    ul_SdCurrent=Uint8_To_Uint16(ucData);    //��ñ�׼����ֵ
//    ul_Irmg = EnyB_ReadMeterParaACK(SCI1);                    //��ȡ�Ȳ�ֵ
//
//    l_IARms= BCD2Hex(CalRMS(RMSI1));//gs_EnyInsData.ul_IARms;
//
//    gs_JbPm.ul_SCI1 = CalculateSC(l_IARms, ul_SdCurrent, ul_Irmg);   //����Ȳ�ֵ
//
//    EnyB_SetMeterCfgACK(gs_JbPm.ul_SCI1, SCI1);                                 //�����±Ȳ�ֵ
//    uint8 ucData[6];                        //�����׼����ֵ
//    uint8 i;
//    int32 ul_SdCurrent[3];                        //��׼����ֵ
//    int16  err[3];
//    int32 pa[3];
//    BE_ReadP(EEP_AUTOCAL+12,ucData,6);         //��ȡ��׼����ֵ
//    for(i=0;i<3;i++)
//    {
//        ul_SdCurrent[i] = Uint8_To_Uint16(ucData + 2*i);   //��ñ�׼����ֵ
//        pa[i] = CalRMS2(RMSI1,gs_UI.ul_I[i]);      //ʵ�ʵ���
//        pa[i] =BCD2Hex(pa[i]);
//        err[i] = (pa[i] -  ul_SdCurrent[i])*1000/ ul_SdCurrent[i] ;   //�������
//        *ul_pRacIReg[i] = CalculateSC(err[i],*ul_pRacIReg[i]); //�����µıȲ�ֵ������У����
//    }

}

 
/*=========================================================================================\n
* @function_name: EnyB_AutoChkUrms
* @function_file: EnergyBottom.c
* @����: 
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-08)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
//uint32 * const ul_pRacUReg[3] ={(uint32*)&gs_JbPm.gs_JBA.RacWARTU,(uint32*)&gs_JbPm.gs_JBB.RacWARTU,
//                                (uint32*)&gs_JbPm.gs_JBC.RacWARTU};
void EnyB_AutoChkUrms(void)
{
//    uint8 ucData[2];
//    uint32  ul_SdVoltage;
//    uint32  ul_Urmg;
//    int32  l_URms;
//
//    BE_ReadP(EEP_AUTOCAL+18,ucData,2);      //��ȡ��׼��ѹֵ
//    ul_SdVoltage=Uint8_To_Uint16(ucData);    //��ñ�׼��ѹֵ
//    ul_Urmg = EnyB_ReadMeterParaACK(SCU);                    //��ȡ�Ȳ�ֵ
//
//    l_URms= BCD2Hex(CalRMS(RMSU))*10;//gs_EnyInsData.ul_URms;
//
//    gs_JbPm.ul_SCU = CalculateSC(l_URms, ul_SdVoltage, ul_Urmg);   //����Ȳ�ֵ
//
//    EnyB_SetMeterCfgACK(gs_JbPm.ul_SCU, SCU);                                 //�����±Ȳ�ֵ
//    uint8 ucData[6];                        //�����׼����ֵ
//    uint8 i;
//    int32 ul_SdVoltage[3];                        //��׼����ֵ
//    int16  err[3];
//    int32 pa[3];
//    BE_ReadP(EEP_AUTOCAL+18,ucData,6);         //��ȡ��׼����ֵ
//    for(i=0;i<3;i++)
//    {
//        ul_SdVoltage[i] = Uint8_To_Uint16(ucData + 2*i);   //��ñ�׼����ֵ
//        pa[i] = CalRMS2(RMSU,gs_UI.ul_U[i]);      //ʵ�ʵ���
//        pa[i] =BCD2Hex(pa[i])*10;
//        err[i] = (pa[i] -  ul_SdVoltage[i])*1000/ ul_SdVoltage[i] ;   //�������
//        *ul_pRacUReg[i] = CalculateSC(err[i],*ul_pRacUReg[i]); //�����µıȲ�ֵ������У����
//    }

}

/*=========================================================================================\n
* @function_name: EnyB_AutoChkAngle
* @function_file: EnergyBottom.c
* @����: 
* 
* @����: 
* @����: 
* @����:   lwb (2012-06-18)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
//uint32 * const ul_pRacACReg[3] ={(uint32*)&gs_JbPm.gs_JBA.RacMTPARA1,(uint32*)&gs_JbPm.gs_JBB.RacMTPARA1,
//                                (uint32*)&gs_JbPm.gs_JBC.RacMTPARA1};

void EnyB_AutoChkAngle(void)
{
//    uint8 ucData[4];                        //�����׼����ֵ
//    int32 l_SdPower;                        //��׼����ֵ
//    int32 pa;
//    
//    //EnyB_SetMeterCfgACK(0, PHCCtrl1);       //�ǲ�ֵд0
//    //��Ҫ��ʱ�ȴ����ʷŵ�ͨ��д��E2ǰ
//    BE_ReadP(EEP_AUTOCAL,ucData,4);         //��ȡ��׼����ֵ  
//   
//    l_SdPower = Uint8_To_Uint32(ucData)/10;   //��ñ�׼����ֵ
////  pa = EnyB_ReadMeterParaACK(DATAP);        //��ȡ����ֵ�������ж�
////
////  if(pa>0)           //���Ȱѱ�ƶ�ȡ�Ĺ���ֵת����ֵ
////  {
////     pa= -BCD2Hex(CalRMS(DATAP));//gs_EnyInsData.l_PowerPA;
////  }else
////  {
////     pa= BCD2Hex(CalRMS(DATAP));//-gs_EnyInsData.l_PowerPA;
////  }
//
//    pa=CalRMS(DATAP);               //��ȡ����
//    if(pa&0x800000)                 //�жϷ���
//    {
//        pa&=~0x800000;               
//        pa =BCD2Hex(pa);
//    }else
//    {
//        pa =BCD2Hex(pa);
//    }
//
/*    uint8 ucData[12];                        //�����׼����ֵ
    uint8 i;
    int32 l_SdPower[3];                        //��׼����ֵ
    int16  err[3];
    int32 pa[3];
     uint32 JbJc;
    BE_ReadP(EEP_AUTOCAL,ucData,12);         //��ȡ��׼����ֵ 
    for(i=0;i<3;i++)
    {
        l_SdPower[i] = Uint8_To_Uint32(ucData + 4*i)/10;   //��ñ�׼����ֵ
        pa[i] = CalRMS2(DATAP,gs_power.ul_power[i]);      //ʵ�ʹ���ֵ
        pa[i] =BCD2Hex(pa[i]);
        err[i] = (pa[i] -  l_SdPower[i])*1000/ l_SdPower[i] ;   //�������    
        JbJc = CalculateAC(err[i]); //�����µĽǲ�ֵ������У����
        JbJc<<=8;
        (*ul_pRacACReg[i])&=0xfffB00ff;                          //�ǲ�λ����
        (*ul_pRacACReg[i])|=JbJc;
    }
    gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);         // У�������CRC���
    SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));         //д��E2��
    EnyB_JbPm_Updata();     
*/
//    
//    gs_JbPm.ui_JbCRC = do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2);  //����CRCУ��ֵ
//
//    SysE2ParaSetManage(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));
//
//    EnyB_JbPm_Updata();                                       //����У�����������оƬ
}
/*=========================================================================================\n
* @function_name: EnyB_AutoChkInrms
* @function_file: EnergyBottom.c
* @����: У�����ߵ���
* 
* 
* @����: 
* @param:ui_SdCurrent  
* @����: 
* @����:   lwb (2012-10-16)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void EnyB_AutoChkInrms(uint16 ui_SdCurrent)
{
//    uint32  ul_Irmg;
//    int32   l_IARms;
//    int32   ul_SdCurrent;
//    
//    ul_SdCurrent=ui_SdCurrent;
//
//    ul_Irmg = EnyB_ReadMeterParaACK(SCI2);                    //��ȡ�Ȳ�ֵ
//
//    l_IARms= BCD2Hex(CalRMS(RMSI2));//gs_EnyInsData.ul_IARms;
//
//    gs_JbPm.ul_SCI2 = CalculateSC(l_IARms, ul_SdCurrent, ul_Irmg);   //����Ȳ�ֵ
//
//    EnyB_SetMeterCfgACK(gs_JbPm.ul_SCI2, SCI2);                     //�����±Ȳ�ֵ
//
//
//    gs_JbPm.ui_JbCRC = do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2);  //����CRCУ��ֵ
//    BE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));        //д��У�����
//        
//    #if (_SW_SE_Type > 0)
//        SE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));    
//    #else
//        BE_WriteP(EEP_JBTOTAL+EEP_BAK_LEN,(uint8*)&gs_JbPm, sizeof(S_JBPM));
//    #endif     

} 
/*=========================================================================================\n
* @function_name: EnyB_AutoCheck
* @function_file: EnergyBottom.c
* @����: �Զ�У��
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-08)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void EnyB_AutoCheck(void)
{
//    EnyB_AutoChkPW();                                                   // У�����ʺ����
//    EnyB_AutoChkIrms();                                                 // У��������Чֵ
//    EnyB_AutoChkUrms();                                                 // У����ѹ��Чֵ
//    gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);         // У�������CRC���
 //   SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));         //д��E2��
 //   EnyB_JbPm_Updata();      
//    //if((0x00 == gs_JbPm.uc_JbOkFlag)&&(guc_FactoryType))                    // û�н�ֹд���־��ͬʱ��������ģʽ������дУ�����
//    //{//д������
//        gs_JbPm.ui_JbCRC = do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2);  //����CRCУ��ֵ
//
//        SysE2ParaSetManage(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));            //д��У�����
//
////      BE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));            //д��У�����
////
////  #if (_SW_SE_Type > 0)
////      SE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));
////  #else
////      BE_WriteP(EEP_JBTOTAL+EEP_BAK_LEN,(uint8*)&gs_JbPm, sizeof(S_JBPM));
////  #endif
//             
//        EnyB_JbPm_Updata();                                       //����У�����������оƬ
//    //}
}
 
/*=========================================================================================\n
* @function_name: EnyB_CloseAutoCheck
* @function_file: EnergyBottom.c
* @����: �ر��Զ�У��һ��У����ɺ���Ҫ�ر�
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-08)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void EnyB_CloseAutoCheck(void)
{
//    //��Ҫ�ӱ������ƣ�Ҫ��������ģʽ
//    if(guc_FactoryType==FactorMd)                                                    // ֻ��������ģʽ�£�������д������
//    {
//        gs_JbPm.ui_JbCRC = do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2);      //����CRCУ��ֵ
//        BE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));
//        
//    #if (_SW_SE_Type > 0)
//        SE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));    
//    #else
//        BE_WriteP(EEP_JBTOTAL+EEP_BAK_LEN,(uint8*)&gs_JbPm, sizeof(S_JBPM));
//    #endif 
//    }
}
 
/*=========================================================================================\n
* @function_name: EnyB_OpenAutoCheck
* @function_file: EnergyBottom.c
* @����: ���Զ�У��
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-08)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void EnyB_OpenAutoCheck(void)
{
//    if(guc_FactoryType==FactorMd)// ֻ��������ģʽ�£�������д������
//    {
//        
//        gs_JbPm.ui_JbCRC = do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2);  //����CRCУ��ֵ
//        BE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));        //д������
//        
//    #if (_SW_SE_Type > 0)
//        SE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));    
//    #else
//        BE_WriteP(EEP_JBTOTAL+EEP_BAK_LEN,(uint8*)&gs_JbPm, sizeof(S_JBPM));
//    #endif 
//    }
}
/*=========================================================================================\n
* @function_name: EnyB_Check_RMSI
* @function_file: EnergyBottom.c
* @����: ����Ƿ�����
* 
* @����: 
* @����: 
* @����:   lm (2013-03-21)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void EnyB_Check_RMSI(void)
{
//    uint32 ul_rmsi;
//
//    if(gs_GiantSCP.ui_CRC == do_CRC((uint8*)&gs_GiantSCP.ul_Adjust,4)                  
//       && gs_GiantSCP.ul_Adjust != 0)                                        //������в���ֵ��Ҫ����
//    {
//        ul_rmsi = CalRMS(RMSI1)&0x7fffffff;                                  //��ȡ������Чֵ
//        
//        if((ul_rmsi >= 0x048000 && guc_GiantSCPflag==0x55)                    //����48A & 
//           || ul_rmsi >= 0x052000)                                           //����52A 
//        {
//            if(guc_GiantSCPcnt>3)
//            {
//                if(gs_GiantSCP.ul_Adjust<0x014AFD6A && gs_GiantSCP.ul_Adjust>-0x014AFD6A) //����
//                {
//                    ul_rmsi = gs_JbPm.ul_SCP + gs_GiantSCP.ul_Adjust;
//                }
//                else
//                {
//                    ul_rmsi = gs_JbPm.ul_SCP;
//                }
//                EnyB_SetMeterCfgACK(ul_rmsi, SCP);
//                guc_GiantSCPflag = 0x55;
//                guc_GiantSCPcnt = 3;
//            }
//            else
//            {
//                EnyB_SetMeterCfgACK(gs_JbPm.ul_SCP, SCP);
//                guc_GiantSCPflag = 0xAA;
//            }
//            guc_GiantSCPcnt++;
//        }
//        else
//        {
//            EnyB_SetMeterCfgACK(gs_JbPm.ul_SCP, SCP);
//            guc_GiantSCPflag = 0xAA;
//            guc_GiantSCPcnt = 0;
//        }
//    }
//    else
//    {
//        EnyB_SetMeterCfgACK(gs_JbPm.ul_SCP, SCP);
//        guc_GiantSCPflag = 0x00;
//    }
}



/*=========================================================================================\n
* @function_name: CalculateSC
* @function_file: EnergyBottom.c
* @����: ���㹫ʽ
* 
* 
* @����: 
* @param:CurValue  
* @param:TarValue  
* @param:LastSC  
* 
* @����: 
* @return: uint32 
* @����:   lwb (2012-03-08)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint32 CalculateSC(int16 error,int32 LastSC)
{
    //���㹫ʽ����,���ı�2011��7��14��
    int16 Error;                    
    uint16 Stemp0remd,Stemp2remd;  
    int32 Stemp0,Stemp1,Stemp2,Stemp3;//
    uint32 Result;
    //��ʽ˵�� ������ʽ S=2^31(1/(1+e)-1)+S1(1/(1+e)),���e����1000����
    //���⹫ʽ��:  S=  2^31(-e/(1000+e))+S1(1000/(1000+e)=S1(1000/(1000+e)-2^31(e/(1000+e))
    //�������  2^31(e/(1000+e))
    Error=error;                                                               //�������ֵ��������1000
    Stemp0=0x80000000/(1000+Error);                     //�ȱ����������� 2^31/(1000+e)=0x80000000/(1000+e)
    Stemp0remd=0x80000000%(1000+Error);                 //��������
    Stemp0remd=Stemp0remd*1000/(1000+Error);            //����С�����֣�С����������1000
    Stemp1=Stemp0*Error+(Stemp0remd*Error)/1000;        //�Ѽ����������С�����ֶ�����ERROR����2^31(e/(1000+e))
    //�������S1*(1000/(1000+e)
    Stemp2=LastSC/(1000+Error);                         //������������
    Stemp2remd=LastSC%(1000+Error);                     //������������
    Stemp2remd=Stemp2remd*1000/(1000+Error);            //С������
    Stemp3=Stemp2*1000+Stemp2remd;                      //�������ּ�С������  

    Result=Stemp3-Stemp1;                               //S=S1(1000/(1000+e)-2^31(e/(1000+e))
    return Result;
 
}

