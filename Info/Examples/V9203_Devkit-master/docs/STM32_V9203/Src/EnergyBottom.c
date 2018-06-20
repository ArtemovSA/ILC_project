#define  ENERGYBOTTOMEXT
#include "Includes.h"
/*=========================================================================================\n
* @function_name: EnyB_Open_Cf
* @function_file: EnergyBottom.c
* @描述: 开启CF脉冲
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
//            EnyB_SetMeterCfgACK(0xd8,PMCtrl4);  //打开计量,保持潜动开启
            EnyB_InitCFInt();                   //开启CF脉冲中断
             //TIM2_Int_Init(99,3199); 
            guc_CfOpenFlg=false;
        }
    }

//    if(guc_FrsCalRTCDelay>0)                    //延时5s
//    {
//        guc_FrsCalRTCDelay--;
//        if(guc_FrsCalRTCDelay==0)
//        {
//            CalRTC();                           //上电第一次校正RTC需要等AD稳定
//        }
//    }

}
/*========================================================
* @function_name: EnyB_InitCFInt
* @function_file: EnergyBottom.c
* @描述: 初始化CF中断
* 
* @作者: lwb (2012/3/3)
* @备注: 
*----------------------------------------------------------
* @修改人: 
* @修改内容:  
============================================================*/
void EnyB_InitCFInt(void)
{
//  EIE|=BIT0;
//  ExInt2IE|=BIT7;
//  P9OE&=(~BIT6);                          //打开CF IO输出
//  P9FC|=(BIT6);
}
/*=========================================================================================\n
* @function_name: EnyB_SetMeterRule
* @function_file: EnergyBottom.c
* @描述: 设置计量模块流程
* 
* @参数: 
* @返回: 
* @作者:  Vango Application Teams
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_SetMeterRule(void)
{
    BroncoInit();
    EnyB_PurInit();         //计量模块上电初始化
}
/*=========================================================================================\n
* @function_name: EnyB_PurInit
* @function_file: EnergyBottom.c
* @描述: 上电初始化
* 
* @参数: 
* @返回: 
* @作者:   Vango Application Teams
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_PurInit(void)                                 
{
    Eny_RefVar();                                       //计量参数刷新
    IWDG_ReloadCounter();                         //喂狗
    Eny_RefCalcType();                                  //计量方式字刷新
    IWDG_ReloadCounter();                         //喂狗
    Eny_PurCheckRAMEny();                               //检测RAM电量
    IWDG_ReloadCounter();                         //喂狗
}
extern const uint8  InitPara5[sizeof(S_JBPM)-2];
/*=========================================================================================\n
* @function_name: EnyB_JbPm_Init
* @function_file: EnergyBottom.c
* @描述: 校表参数的初始化
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_JbPm_Init(void)                                
{                                                        
  //  memcpy((uint8*)&gs_JbPm.ui_MeterC,InitPara5,sizeof(S_JBPM)-2);  //拷贝Flash数据到RAM中
   // gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);     // 校表参数的CRC结果
//  V9281_InitDefaultPara();
}

/*=========================================================================================\n
* @function_name: EnyB_IntCF
* @function_file: EnergyBottom.c
* @描述: 脉冲中断
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-07)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_IntCF(uint8 power)                       
{
    uint8 *pCfZ;
    uint8 *pEyZ;
    if(power == active)
    {
        if(gs_MeterState.ui_MeterState2.RunCode2Word.PADIR)      //判断是否是反向
        {
            pEyZ=&gs_Energy.ucNz;
            pCfZ=&gs_PowerCf.uc_Nz;
        }else
        {
            pEyZ=&gs_Energy.ucPz;                                   //正向能量
            pCfZ=&gs_PowerCf.uc_Pz;                                 //正向脉冲数
        }


        (*pCfZ)++;                                                  //总能量累加
        if(*pCfZ>=guc_GateCF)                                       //判别是否需要走字
        {
            *pCfZ -=guc_GateCF;                                     //对CF脉冲进行分频，将分频结果进行能量累加
            *pEyZ +=guc_UnitCF; 
            gui_SystemEvent |= flgEtGlb_EnergyCalc;                 //设置能量处理事件
        }
    }
    else
    {
         if(gs_MeterState.ui_MeterState2.RunCode2Word.PADIR)      //判断是否是反向
        {
            pEyZ=&gs_NEnergy.ucNz;
            pCfZ=&gs_PowerCfN.uc_Nz;
        }else
        {
            pEyZ=&gs_NEnergy.ucPz;                                   //正向能量
            pCfZ=&gs_PowerCfN.uc_Pz;                                 //正向脉冲数
        }


        (*pCfZ)++;                                                  //总能量累加
        if(*pCfZ>=guc_GateCF)                                       //判别是否需要走字
        {
            *pCfZ -=guc_GateCF;                                     //对CF脉冲进行分频，将分频结果进行能量累加
            *pEyZ +=guc_UnitCF; 
            gui_SystemEvent |= flgEtGlb_NEnergyCalc;                 //设置能量处理事件
        }
    }
}

/*=========================================================================================\n
* @function_name: ReadRMSPara
* @function_file: EnergyBottom.c
* @描述: 读取显示比例系数k
* 
* 
* @参数: 
* @param:ucpos  
* 
* @返回: 
* @return: uint32 
* @作者:   lwb (2012-05-29)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
* @描述: 计算瞬时量
* 
* 
* @参数: 
* @param:addr  寄存器地址
* 
* @返回: 
* @return: uint32 返回有效值
* @作者:  Vango Application Teams
* @备注: 
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
          if(TempValue.byte[3]>0x7f)          //如果是负数的数，先变成正数
          {
              TempValue.lword=~TempValue.lword+1;
          }

          TempPara=gs_JbPm.ul_PG;        //读取比例系数

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
//               if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir==true)   //反向添加负号
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
          if(TempValue.byte[3]>0x7f)          //如果是负数的数，先变成正数
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
          //电流的计算
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
              //功率的计算
              /*if(ReadRaccoon(DATAP,1,phyadd))
              {
                  memcpy((uint8*)TempValue2.byte,gs_RacCtrl.ucBuf+3,4);
              }else
              {
                  TempValue2.lword=0;
              }

              if(TempValue2.byte[3]>0x7f)                         //如果是负数的数，先变成正数
              {
                  TempValue2.lword=~TempValue2.lword+1;
              }

              TempPara=gs_JbPm.ul_PG;        //读取比例系数

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
                  //如果功率小于启动功率，这是电流小于0.2A,电流清零
                  if(TempValue1<0x200)
                  {
                      TempValue1=0;
                  }*/
//              if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir==true)   //反向添加负号
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
* @描述: 
* 
* 
* @参数: 
* @param:addr  
* @param:value  
* 
* @返回: 
* @return: uint32 
* @作者:   lwb (2013-09-27)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint32 CalRMS2(uint16 addr,uint32 value)
{
    uint32 TempPara,TempValue1;
    switch (addr)
    {
        case DATAP:
        case DATAQ:
          TempPara=gs_JbPm.ul_PG;        //读取比例系数
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
              if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir==true)   //反向添加负号
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
          //电流的计算
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
              if(gs_MeterState.ui_MeterState1.RunCode1Word.PDir==true)   //反向添加负号
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
* @描述: 计算功率因数
* 
* @参数: 
* 
* @返回: 
* @return: uint16 
* @作者:  Vango Application Teams
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
* @描述: 检测功率是否反向
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_Check_PowRev(void)
{
//    int32 l_pa;
//    uint32 TempPara;
//    uint32 ulPower;
////
////    l_pa = EnyB_ReadMeterParaACK(DATAP);
//    if(l_pa>0)
//    {                                       //功率反向
////        TempPara=gs_JbPm.ul_PG>>8;          //读取比例系数
//        if(TempPara==0)
//        {                                   //校表参数功率计算值为0的话，直接判断功率原始值
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
////     gs_MeterState.ui_MeterState1.RunCode1Word.PDir = true;               //置功率反向标志
//       gs_MeterState.ui_MeterState2.RunCode2Word.PADIR= true;
//       if(gs_ReportStateFlg.ui_ReportState3.ReportWord3.PDirRpt == false    // 状态标志未置
//          && gs_ReportWord.ui_ReportWord3.ReportWord3.PDirRpt == 1)         // 需要主动上报
//       {
//           gs_ReportState.ui_ReportState3.ReportWord3.PDirRpt = true;       // 状态置位
//       }
//       if(gs_ReportStateFlg.ui_ReportState2A.ReportWord2.PDirRpt == false   // 状态标志未置
//          && gs_ReportWord.ui_ReportWord2.ReportWord2.PDirRpt == 1)         // 需要主动上报
//       {
//           gs_ReportState.ui_ReportState2A.ReportWord2.PDirRpt = true;      // 状态置位
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
////     gs_MeterState.ui_MeterState1.RunCode1Word.PDir = false;      //清功率反向标志
//       gs_MeterState.ui_MeterState2.RunCode2Word.PADIR= false;
//       gs_SysRunSt.PDirFR=false;
//       if(gs_ReportStateFlg.ui_ReportState3.ReportWord3.PDirRpt == true)     // 判断状态标志
//       {
//           gs_ReportStateFlg.ui_ReportState3.ReportWord3.PDirRpt = false;
//       }
//       if(gs_ReportStateFlg.ui_ReportState2A.ReportWord2.PDirRpt == true)     // 判断状态标志
//       {
//           gs_ReportStateFlg.ui_ReportState2A.ReportWord2.PDirRpt = false;
//       }
//    }
}
/*=========================================================================================\n
* @function_name: EnyB_Cal_Cos
* @function_file: EnergyBottom.c
* @描述: 计算功率因数
* 
* @参数: 
* 
* @返回: 
* @return: int16 
* @作者:   lwb (2012-03-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
* @描述: 刷新校表参数到SFR中
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_JbPm_Updata(void)
{
        UpdateBroncoPara();
}

/*=========================================================================================\n
* @function_name: EnyB_JbPm_Updata
* @function_file: EnergyBottom.c
* @描述: 刷新校表参数到SFR中
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-02)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void RecoverPowDirConfig(void)
{
    gs_PDirCnt.ucDirCnt=0;
    gs_PDirCnt.ucRevDirCnt=0;
    SysE2ReadData(EEP_PDIRTIME,(uint8*)&gs_PDirCtr.ucDelayTm,4);    //跟新反向功率判断延时和阀值 
    gs_PDirCtr.ucDelayTm=BCD2Hex(gs_PDirCtr.ucDelayTm);
    gs_PDirCtr.ul_PDirGate=BCD2Hex(gs_PDirCtr.ul_PDirGate&0x00ffffff);   
}

/*=============================================================================================\n
Function：自动校正算法
 ================================================================================================*/

/*=========================================================================================\n
* @function_name: CalculateSC
* @function_file: EnergyBottom.c
* @描述: 计算公式
* 
* 
* @参数: 
* @param:CurValue  
* @param:TarValue  
* @param:LastSC  
* 
* @返回: 
* @return: uint32 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
/*uint32 CalculateSC(int32 CurValue,int32 TarValue,int32 LastSC)
{
    //计算公式更换,雷文斌，2011年7月14日
    int16 Error;                    
    uint16 Stemp0remd,Stemp2remd;  
    int32 Stemp0,Stemp1,Stemp2,Stemp3;//
    uint32 Result;

    //公式说明 计算误差公式 S=2^31(1/(1+e)-1)+S1(1/(1+e)),如果e扩大1000倍后，
    //化解公式得:  S=  2^31(-e/(1000+e))+S1(1000/(1000+e)=S1(1000/(1000+e)-2^31(e/(1000+e))
    //下面计算  2^31(e/(1000+e))
    Error=(CurValue-TarValue)*1000/TarValue;            //计算误差值。。扩大1000
    Stemp0=0x80000000/(1000+Error);                     //先保留整数部分 2^31/(1000+e)=0x80000000/(1000+e)
    Stemp0remd=0x80000000%(1000+Error);                 //保留余数
    Stemp0remd=Stemp0remd*1000/(1000+Error);            //计算小数部分，小数部分扩大1000
    Stemp1=Stemp0*Error+(Stemp0remd*Error)/1000;        //把计算的整数和小数部分都乘以ERROR，得2^31(e/(1000+e))

    //下面计算S1*(1000/(1000+e)
    Stemp2=LastSC/(1000+Error);                         //计算整数部分
    Stemp2remd=LastSC%(1000+Error);                     //计算余数部分
    Stemp2remd=Stemp2remd*1000/(1000+Error);            //小数部分
    Stemp3=Stemp2*1000+Stemp2remd;                      //整数部分加小数部分  

    Result=Stemp3-Stemp1;                               //S=S1(1000/(1000+e)-2^31(e/(1000+e))
    return Result;
 
}*/


/*=========================================================================================\n
* @function_name: CalculateAC
* @function_file: EnergyBottom.c
* @描述: 计算公式
* 
* 
* @参数: 
* @param:CurValue  
* @param:TarValue  
* @param:LastSC  
* 
* @返回: 
* @return: uint32 
* @作者:   lwb (2012-06-18)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
/*uint8 CalculateAC(int32 CurValue,int32 TarValue)
{
    //计算公式更换,雷文斌，2011年7月14日
    int16 Error;                    
    uint8 Result;

    Error=(CurValue-TarValue)*1000/TarValue;            //计算误差值。。扩大1000
    Result = (int32)3011*Error/2000;

    return Result; 
}*/
uint8 CalculateAC( int16 Error)
{
    //计算公式更换            
    uint8 Result;
    Result = (int32)3011*Error*4/2000;
    return Result; 
}

 
/*=========================================================================================\n
* @function_name: EnyB_AutoChkPW
* @function_file: EnergyBottom.c
* @描述: 校正功率和误差
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_AutoChkPW(void)
{
//    uint8 ucData[12];                        //三相标准功率值
//    uint8 i;
//    int32 l_SdPower[3];                        //标准功率值
//    int16  err[3];
//    int32 pa[3];
//   BE_ReadP(EEP_AUTOCAL,ucData,12);         //读取标准功率值 
//   for(i=0;i<3;i++)
//   {
//       l_SdPower[i] = Uint8_To_Uint32(ucData + 4*i)/10;   //获得标准功率值
//       pa[i] = CalRMS2(DATAP,gs_power.ul_power[i]);      //实际功率值
//       pa[i] =BCD2Hex(pa[i]);
//       err[i] = (pa[i] -  l_SdPower[i])*1000/ l_SdPower[i] ;   //计算误差
//       *ul_pRacReg[i] = CalculateSC(err[i],*ul_pRacReg[i]); //计算新的比差值并存入校表尝试
//   }
//   
////  pa = EnyB_ReadMeterParaACK(DATAP);        //读取功率值做方向判断
////
////  if(pa>0)           //首先把表计读取的功率值转存正值
////  {
////     pa= -BCD2Hex(CalRMS(DATAP));//gs_EnyInsData.l_PowerPA;
////  }else
////  {
////     pa= BCD2Hex(CalRMS(DATAP));//-gs_EnyInsData.l_PowerPA;
////  }
//
//    pa=CalRMS(DATAP);               //读取功率
//    if(pa&0x800000)                 //判断方向
//    {
//        pa&=~0x800000;               
//    } 
//
//    pa =BCD2Hex(pa);
//    gs_JbPm.ul_SCP = CalculateSC(pa, l_SdPower, ul_Scp);   //计算比差值
// 
//    EnyB_SetMeterCfgACK(gs_JbPm.ul_SCP, SCP);                                 //设置新比差值
}

/*=========================================================================================\n
* @function_name: EnyB_AutoChkPW2
* @function_file: EnergyBottom.c
* @描述: 小信号校正功率和误差
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
//uint32 * const ul_pRacSReg[3] ={(uint32*)&gs_JbPm.gs_JBA.RacWWAPT,(uint32*)&gs_JbPm.gs_JBB.RacWWAPT,
 //                               (uint32*)&gs_JbPm.gs_JBC.RacWWAPT};
void EnyB_AutoChkPW2(void)
{
//    uint8 ucData[12],ucData1[4];                        //三相标准功率值
//    uint8 i;
//    int32 l_SdPower[3];                        //标准功率值
//    int32 l_SdPreg;
//    int32  err[3];
//    int32 pa[3];
//    BE_ReadP(EEP_AUTOCAL,ucData,12);         //读取标准功率值 
//    BE_ReadP(EEP_AUTOCAL+24,ucData1,4);         //读取标准功率值 
//    l_SdPreg = Uint8_To_Uint32(ucData1);
//    for(i=0;i<3;i++)
//    {
//       l_SdPower[i] = Uint8_To_Uint32(ucData + 4*i)/10;   //获得标准功率值
//       
//       pa[i] = CalRMS2(DATAP,gs_power.ul_power[i]);      //实际功率值
//       pa[i] =BCD2Hex(pa[i]);
//       err[i] = (pa[i] -  l_SdPower[i])*1000/ l_SdPower[i] ;   //计算误差
//       *ul_pRacSReg[i] = (0-err[i])*l_SdPreg/1000 ;
//    }
//    gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);         // 校表参数的CRC结果
//    SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));         //写入E2中
//    EnyB_JbPm_Updata();   
//    uint8 ucData[4];                        //单相标准功率值
//    uint8 i;
//    int32 l_SdPower;                        //标准功率值   
//    Word32 TempValue;
//    uint32 TempSum;
//    uint32 pa_sum,TempPara;
//    
//    pa_sum = 0;
//    TempSum = 0;
//    BE_ReadP(EEP_AUTOCAL,ucData,4);         //读取标准功率值  
//
//    l_SdPower = Uint8_To_Uint32(ucData);   //获得标准功率值
//
//    for(i=0;i<16;i++)
//    {
//        TempValue.lword=EnyB_ReadMeterParaACK(DATAIP); 
//        DelayXms(20);
//        CLRWDT();                           //喂狗
//        //DelayXms(255);
//        if(TempValue.byte[3]>0x7f)          //如果是负数的数，先变成正数
//        {
//            TempValue.lword=~TempValue.lword+1;
//        }
//        TempSum += TempValue.lword;
//    }   
//    
//    TempSum = TempSum>>4;              //读取功率寄存器值
//    TempPara = gs_JbPm.ul_PG/256;        //读取比例系数
//    pa_sum = (TempSum*2560)/TempPara;  //换算值
//      
//    TempValue.lword = l_SdPower*TempPara/2560;    //理论寄存器值
//    
//    if(l_SdPower > pa_sum)
//      gs_JbPm.ul_PARAPC = (TempValue.lword*(l_SdPower-pa_sum)/l_SdPower);       //计算二次补偿值
//    else
//      gs_JbPm.ul_PARAPC = -(TempValue.lword*(pa_sum-l_SdPower)/l_SdPower);   //计算二次补偿值
//    EnyB_SetMeterCfgACK(gs_JbPm.ul_PARAPC, PARAPC);                                 //设置新比差值
}
 
/*=========================================================================================\n
* @function_name: EnyB_AutoChkIrms
* @function_file: EnergyBottom.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
//    BE_ReadP(EEP_AUTOCAL+12,ucData,2);      //读取标准电流值
//    ul_SdCurrent=Uint8_To_Uint16(ucData);    //获得标准电流值
//    ul_Irmg = EnyB_ReadMeterParaACK(SCI1);                    //读取比差值
//
//    l_IARms= BCD2Hex(CalRMS(RMSI1));//gs_EnyInsData.ul_IARms;
//
//    gs_JbPm.ul_SCI1 = CalculateSC(l_IARms, ul_SdCurrent, ul_Irmg);   //计算比差值
//
//    EnyB_SetMeterCfgACK(gs_JbPm.ul_SCI1, SCI1);                                 //设置新比差值
//    uint8 ucData[6];                        //三相标准功率值
//    uint8 i;
//    int32 ul_SdCurrent[3];                        //标准电流值
//    int16  err[3];
//    int32 pa[3];
//    BE_ReadP(EEP_AUTOCAL+12,ucData,6);         //读取标准电流值
//    for(i=0;i<3;i++)
//    {
//        ul_SdCurrent[i] = Uint8_To_Uint16(ucData + 2*i);   //获得标准电流值
//        pa[i] = CalRMS2(RMSI1,gs_UI.ul_I[i]);      //实际电流
//        pa[i] =BCD2Hex(pa[i]);
//        err[i] = (pa[i] -  ul_SdCurrent[i])*1000/ ul_SdCurrent[i] ;   //计算误差
//        *ul_pRacIReg[i] = CalculateSC(err[i],*ul_pRacIReg[i]); //计算新的比差值并存入校表尝试
//    }

}

 
/*=========================================================================================\n
* @function_name: EnyB_AutoChkUrms
* @function_file: EnergyBottom.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
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
//    BE_ReadP(EEP_AUTOCAL+18,ucData,2);      //读取标准电压值
//    ul_SdVoltage=Uint8_To_Uint16(ucData);    //获得标准电压值
//    ul_Urmg = EnyB_ReadMeterParaACK(SCU);                    //读取比差值
//
//    l_URms= BCD2Hex(CalRMS(RMSU))*10;//gs_EnyInsData.ul_URms;
//
//    gs_JbPm.ul_SCU = CalculateSC(l_URms, ul_SdVoltage, ul_Urmg);   //计算比差值
//
//    EnyB_SetMeterCfgACK(gs_JbPm.ul_SCU, SCU);                                 //设置新比差值
//    uint8 ucData[6];                        //三相标准功率值
//    uint8 i;
//    int32 ul_SdVoltage[3];                        //标准电流值
//    int16  err[3];
//    int32 pa[3];
//    BE_ReadP(EEP_AUTOCAL+18,ucData,6);         //读取标准电流值
//    for(i=0;i<3;i++)
//    {
//        ul_SdVoltage[i] = Uint8_To_Uint16(ucData + 2*i);   //获得标准电流值
//        pa[i] = CalRMS2(RMSU,gs_UI.ul_U[i]);      //实际电流
//        pa[i] =BCD2Hex(pa[i])*10;
//        err[i] = (pa[i] -  ul_SdVoltage[i])*1000/ ul_SdVoltage[i] ;   //计算误差
//        *ul_pRacUReg[i] = CalculateSC(err[i],*ul_pRacUReg[i]); //计算新的比差值并存入校表尝试
//    }

}

/*=========================================================================================\n
* @function_name: EnyB_AutoChkAngle
* @function_file: EnergyBottom.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-06-18)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
//uint32 * const ul_pRacACReg[3] ={(uint32*)&gs_JbPm.gs_JBA.RacMTPARA1,(uint32*)&gs_JbPm.gs_JBB.RacMTPARA1,
//                                (uint32*)&gs_JbPm.gs_JBC.RacMTPARA1};

void EnyB_AutoChkAngle(void)
{
//    uint8 ucData[4];                        //单相标准功率值
//    int32 l_SdPower;                        //标准功率值
//    int32 pa;
//    
//    //EnyB_SetMeterCfgACK(0, PHCCtrl1);       //角差值写0
//    //需要延时等待，故放到通信写入E2前
//    BE_ReadP(EEP_AUTOCAL,ucData,4);         //读取标准功率值  
//   
//    l_SdPower = Uint8_To_Uint32(ucData)/10;   //获得标准功率值
////  pa = EnyB_ReadMeterParaACK(DATAP);        //读取功率值做方向判断
////
////  if(pa>0)           //首先把表计读取的功率值转存正值
////  {
////     pa= -BCD2Hex(CalRMS(DATAP));//gs_EnyInsData.l_PowerPA;
////  }else
////  {
////     pa= BCD2Hex(CalRMS(DATAP));//-gs_EnyInsData.l_PowerPA;
////  }
//
//    pa=CalRMS(DATAP);               //读取功率
//    if(pa&0x800000)                 //判断方向
//    {
//        pa&=~0x800000;               
//        pa =BCD2Hex(pa);
//    }else
//    {
//        pa =BCD2Hex(pa);
//    }
//
/*    uint8 ucData[12];                        //三相标准功率值
    uint8 i;
    int32 l_SdPower[3];                        //标准功率值
    int16  err[3];
    int32 pa[3];
     uint32 JbJc;
    BE_ReadP(EEP_AUTOCAL,ucData,12);         //读取标准功率值 
    for(i=0;i<3;i++)
    {
        l_SdPower[i] = Uint8_To_Uint32(ucData + 4*i)/10;   //获得标准功率值
        pa[i] = CalRMS2(DATAP,gs_power.ul_power[i]);      //实际功率值
        pa[i] =BCD2Hex(pa[i]);
        err[i] = (pa[i] -  l_SdPower[i])*1000/ l_SdPower[i] ;   //计算误差    
        JbJc = CalculateAC(err[i]); //计算新的角差值并存入校表尝试
        JbJc<<=8;
        (*ul_pRacACReg[i])&=0xfffB00ff;                          //角差位清零
        (*ul_pRacACReg[i])|=JbJc;
    }
    gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);         // 校表参数的CRC结果
    SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));         //写入E2中
    EnyB_JbPm_Updata();     
*/
//    
//    gs_JbPm.ui_JbCRC = do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2);  //计算CRC校验值
//
//    SysE2ParaSetManage(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));
//
//    EnyB_JbPm_Updata();                                       //跟新校表参数到计量芯片
}
/*=========================================================================================\n
* @function_name: EnyB_AutoChkInrms
* @function_file: EnergyBottom.c
* @描述: 校正零线电流
* 
* 
* @参数: 
* @param:ui_SdCurrent  
* @返回: 
* @作者:   lwb (2012-10-16)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_AutoChkInrms(uint16 ui_SdCurrent)
{
//    uint32  ul_Irmg;
//    int32   l_IARms;
//    int32   ul_SdCurrent;
//    
//    ul_SdCurrent=ui_SdCurrent;
//
//    ul_Irmg = EnyB_ReadMeterParaACK(SCI2);                    //读取比差值
//
//    l_IARms= BCD2Hex(CalRMS(RMSI2));//gs_EnyInsData.ul_IARms;
//
//    gs_JbPm.ul_SCI2 = CalculateSC(l_IARms, ul_SdCurrent, ul_Irmg);   //计算比差值
//
//    EnyB_SetMeterCfgACK(gs_JbPm.ul_SCI2, SCI2);                     //设置新比差值
//
//
//    gs_JbPm.ui_JbCRC = do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2);  //计算CRC校验值
//    BE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));        //写回校表参数
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
* @描述: 自动校表
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_AutoCheck(void)
{
//    EnyB_AutoChkPW();                                                   // 校正功率和误差
//    EnyB_AutoChkIrms();                                                 // 校正电流有效值
//    EnyB_AutoChkUrms();                                                 // 校正电压有效值
//    gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);         // 校表参数的CRC结果
 //   SysE2ParaSetManage(EEP_JBTOTAL,(uint8*)&gs_JbPm,sizeof(S_JBPM));         //写入E2中
 //   EnyB_JbPm_Updata();      
//    //if((0x00 == gs_JbPm.uc_JbOkFlag)&&(guc_FactoryType))                    // 没有禁止写入标志，同时处于生产模式，允许写校表参数
//    //{//写入数据
//        gs_JbPm.ui_JbCRC = do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2);  //计算CRC校验值
//
//        SysE2ParaSetManage(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));            //写回校表参数
//
////      BE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));            //写回校表参数
////
////  #if (_SW_SE_Type > 0)
////      SE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));
////  #else
////      BE_WriteP(EEP_JBTOTAL+EEP_BAK_LEN,(uint8*)&gs_JbPm, sizeof(S_JBPM));
////  #endif
//             
//        EnyB_JbPm_Updata();                                       //跟新校表参数到计量芯片
//    //}
}
 
/*=========================================================================================\n
* @function_name: EnyB_CloseAutoCheck
* @function_file: EnergyBottom.c
* @描述: 关闭自动校表，一旦校正完成后，需要关闭
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_CloseAutoCheck(void)
{
//    //需要加保护限制，要求在生产模式
//    if(guc_FactoryType==FactorMd)                                                    // 只有在生产模式下，才允许写入数据
//    {
//        gs_JbPm.ui_JbCRC = do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2);      //计算CRC校验值
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
* @描述: 打开自动校表
* 
* @参数: 
* @返回: 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_OpenAutoCheck(void)
{
//    if(guc_FactoryType==FactorMd)// 只有在生产模式下，才允许写入数据
//    {
//        
//        gs_JbPm.ui_JbCRC = do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2);  //计算CRC校验值
//        BE_WriteP(EEP_JBTOTAL,(uint8 *)&gs_JbPm,sizeof(S_JBPM));        //写入数据
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
* @描述: 检测是否大电流
* 
* @参数: 
* @返回: 
* @作者:   lm (2013-03-21)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void EnyB_Check_RMSI(void)
{
//    uint32 ul_rmsi;
//
//    if(gs_GiantSCP.ui_CRC == do_CRC((uint8*)&gs_GiantSCP.ul_Adjust,4)                  
//       && gs_GiantSCP.ul_Adjust != 0)                                        //大电流有补偿值需要补偿
//    {
//        ul_rmsi = CalRMS(RMSI1)&0x7fffffff;                                  //读取电流有效值
//        
//        if((ul_rmsi >= 0x048000 && guc_GiantSCPflag==0x55)                    //大于48A & 
//           || ul_rmsi >= 0x052000)                                           //大于52A 
//        {
//            if(guc_GiantSCPcnt>3)
//            {
//                if(gs_GiantSCP.ul_Adjust<0x014AFD6A && gs_GiantSCP.ul_Adjust>-0x014AFD6A) //保护
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
* @描述: 计算公式
* 
* 
* @参数: 
* @param:CurValue  
* @param:TarValue  
* @param:LastSC  
* 
* @返回: 
* @return: uint32 
* @作者:   lwb (2012-03-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
uint32 CalculateSC(int16 error,int32 LastSC)
{
    //计算公式更换,雷文斌，2011年7月14日
    int16 Error;                    
    uint16 Stemp0remd,Stemp2remd;  
    int32 Stemp0,Stemp1,Stemp2,Stemp3;//
    uint32 Result;
    //公式说明 计算误差公式 S=2^31(1/(1+e)-1)+S1(1/(1+e)),如果e扩大1000倍后，
    //化解公式得:  S=  2^31(-e/(1000+e))+S1(1000/(1000+e)=S1(1000/(1000+e)-2^31(e/(1000+e))
    //下面计算  2^31(e/(1000+e))
    Error=error;                                                               //计算误差值。。扩大1000
    Stemp0=0x80000000/(1000+Error);                     //先保留整数部分 2^31/(1000+e)=0x80000000/(1000+e)
    Stemp0remd=0x80000000%(1000+Error);                 //保留余数
    Stemp0remd=Stemp0remd*1000/(1000+Error);            //计算小数部分，小数部分扩大1000
    Stemp1=Stemp0*Error+(Stemp0remd*Error)/1000;        //把计算的整数和小数部分都乘以ERROR，得2^31(e/(1000+e))
    //下面计算S1*(1000/(1000+e)
    Stemp2=LastSC/(1000+Error);                         //计算整数部分
    Stemp2remd=LastSC%(1000+Error);                     //计算余数部分
    Stemp2remd=Stemp2remd*1000/(1000+Error);            //小数部分
    Stemp3=Stemp2*1000+Stemp2remd;                      //整数部分加小数部分  

    Result=Stemp3-Stemp1;                               //S=S1(1000/(1000+e)-2^31(e/(1000+e))
    return Result;
 
}

