#define AP_ENERGYEXT
#include "Includes.h"

/*=========================================================================================\n
* @function_name: Eny_JbPm_GetE2
* @function_file: EnergyBottom.c
* @����: ��ȡE2�е����ݣ�ͬʱ������صĴ���
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Eny_JbPm_GetE2(void)
{
    uint8 ucjb[sizeof(S_JBPM)];

    BE_ReadP(EEP_JBTOTAL,ucjb,sizeof(S_JBPM));

    if(Uint8_To_Uint16(ucjb+sizeof(S_JBPM)-2)==do_CRC(ucjb,sizeof(S_JBPM)-2))
    {
        Eny_JbPm_UpdateRam(ucjb);
        EnyB_JbPm_Updata();
    }
    else
    {
//    #if (_SW_SE_Type > 0)
//        SE_ReadP(EEP_JBTOTAL,ucjb,sizeof(S_JBPM));
//    #else
//        BE_ReadP(EEP_JBTOTAL+EEP_BAK_LEN,ucjb,sizeof(S_JBPM));
//    #endif
        if(Uint8_To_Uint16(ucjb+sizeof(S_JBPM)-2)==do_CRC(ucjb,sizeof(S_JBPM)-2))
        {
            Eny_JbPm_UpdateRam(ucjb);
            EnyB_JbPm_Updata();
        }
        else
        {
            if(gs_JbPm.ui_JbCRC == do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2))  //RAM�е������Ƿ�����
            {                                           //��Ϊ���²���Ҫ�����������ṹ������
                EnyB_JbPm_Updata();
            }
            else
            {
                EnyB_JbPm_Init();
                EnyB_JbPm_Updata();
            }
        }
    }
}
                                                     
 
/*=========================================================================================\n
* @function_name: Eny_JbPm_UpdateRam
* @function_file: EnergyBottom.c
* @����: У������ĸ��£���E2�и���
* 
* 
* @����: 
* @param:pjb  
* @����: 
* @����:   lwb (2012-03-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Eny_JbPm_UpdateRam(uint8 *pjb)                  
{                                                       // 
   uint8 i;                                            //
    uint8 *p;                                           //
    p = (uint8 *)&gs_JbPm;                              //
    for(i=0;i<sizeof(S_JBPM);i++)                       //
    {                                                   //
        *p = * pjb;                                     //
        p++;                                            //
        pjb++;                                          //
    }                                                   //
} 
/*=========================================================================================\n
* @function_name: Eny_RefVar
* @function_file: EnergyBottom.c
* @����: ��E2�ж���У�������ͬʱ����������У�飬�����ȷ�������ˢ�£�����ȷ���ó�ʼ������
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Eny_RefVar(void)
{
   /*static uint32 spiready;
    spiready=0;
        while(spiready!=0x100000ff)
        {
           WriteBronco(0x100000ff,0xc000);
              spiready=ReadBronco(0xc000);
        }
        for(uint8 i=0;i<56;i++)
        {
            WriteBronco(0x0,0xC800+i);
        } */
    UpdateBroncoPara();
    Eny_JbPm_MeterConst();                                                     //��������ĸ���
  /*  uint16 uitmp;
    BE_ReadW(EEP_JBTOTAL+sizeof(S_JBPM)-2, &uitmp);                             //��E2�е�CRCУ��ֵ
    IWDG_ReloadCounter();    
    if(uitmp == gs_JbPm.ui_JbCRC)                                               //�ж�����E2�е������Ƿ���RAM�е����
    {                                                                           //RAM�е�������E2�е���һ����
        if(gs_JbPm.ui_JbCRC == do_CRC((uint8 *)&gs_JbPm,sizeof(S_JBPM)-2))      //RAM�е������Ƿ�����
        {                                                                       //��Ϊ���²���Ҫ�����������ṹ������
            EnyB_JbPm_Updata();
        }
        else
        {
            Eny_JbPm_GetE2();                                                  // RAM�е�����Ҫ��ͨ����CRCУ�飬Ҫ����E2��CRC�Բ�����
        }
    }
    else
    {
        Eny_JbPm_GetE2();                                                      //RAM�е�����Ҫ��ͨ����CRCУ�飬Ҫ����E2��CRC�Բ�����
    }
    IWDG_ReloadCounter();    
    Eny_JbPm_MeterConst();                                                     //��������ĸ���
    */
} 
/*=========================================================================================\n
* @function_name: Eny_RefCalcType
* @function_file: Energy.c
* @����: ˢ�¼�����ʽ��
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-07)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Eny_RefCalcType(void)
{
    BE_ReadB(EEP_YGCALC,&guc_EPCalcType);               //ˢ�¼�����ʽ��    
}
/*=========================================================================================\n
* @function_name: Eny_JbPm_MeterConst
* @function_file: EnergyBottom.c
* @����: ���峣��
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Eny_JbPm_MeterConst(void)                           
{                                                        
    uint8 i;
    for(i=0;i<MeterConstLen;i++)                                // ��������ų���ʵ�����ֿ���
    {                                                           // ���ݱ�����ȷ��CF�����Ƶϵ���͵�λ������С
        if(gs_MeterConstT[i].ui_MC==gs_JbPm.ui_MeterC)
        {                                                       // �������峣����ȷ��CF��Ƶϵ���͵�λ������С
            guc_GateCF = gs_MeterConstT[i].uc_CFGate;           // �����������
            guc_UnitCF = gs_MeterConstT[i].uc_CFUinit;          // ��λ��С
            break;
        }
    }

    if(i>=MeterConstLen)                                        // ���ұ���У�����û�ж�Ӧ�ĳ����Ĵ���
    {                                                           // ��������û�ж�Ӧ�������״̬������
        guc_GateCF = 10;                                        // ��һ���Ƚϴ�����ݣ����׿���
        guc_UnitCF = 1;
    }
}
 
/*=========================================================================================\n
* @function_name: Eny_EnergyClrCur
* @function_file: Energy.c
* @����: �����ǰ�����й�����
*        ���RAM�е����ݣ����ǽṹ���е����ݣ������2kWh 
*        ���E2�е����ݣ��ǵ�ǰ�����й��ܵ�������
* @����: 
* @param:ucType  
* @����: 
* @����:   lwb (2012-03-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Eny_EnergyClrCur(uint8 ucType,uint8 power)
{                       
   uint16 *p,*q;
   uint32 *r;
    if(power == active)
    {
        p = (uint16*)&gs_EnergyData;
        q = (uint16*)&gs_EnergyData_bak;
        r = (uint32*)&gs_EnergyA;
    }
    else
    {
        p = (uint16*)&gs_NEnergyData;
        q = (uint16*)&gs_NEnergyData_bak;
        r = (uint32*)&gs_NEnergyA;
    }

    if(ucType & ENERGY_CLR_RAM)
    {
        memset((uint8*)p, 0, sizeof(ENERGYDATA)-2);    //��������
        p[2] = do_CRC((uint8 *)p, sizeof(ENERGYDATA)-2);
        memcpy((uint8*)q, (uint8*)p, sizeof(ENERGYDATA));
    }

    if(ucType & ENERGY_CLR_E2)
    {
        memset((uint8*)r, 0, sizeof(ENERGYACTIVE));
        r[1] = do_CRC((uint8*)r, sizeof(ENERGYACTIVE)- 4);
        //BE_WriteP(EEP_EPS,(uint8*)r, sizeof(ENERGYACTIVE));
        SysE2ParaSetManage(EEP_EPS,(uint8*)r, sizeof(ENERGYACTIVE));

   /* #if (_SW_SE_Type > 0)
        SE_WriteP(EEP_EPS,(uint8*)&r, sizeof(ENERGYACTIVE));
    #else
        BE_WriteP(EEP_EPS+EEP_BAK_LEN,(uint8*)r, sizeof(ENERGYACTIVE));
    #endif*/
    }
}
 
/*=========================================================================================\n
* @function_name: Eny_PurCheckRAMEny
* @function_file: Energy.c
* @����: �ϵ���RAM�����Ƿ���ȷ
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-02)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Eny_PurCheckRAMEny(void)
{
    ENERGYACTIVE sEA;
    if(ENERGY_ERR_RAM==Eny_CheckEnergyActiveRAM(active))
    {                                                                       //RAM��������ȷ
        //BE_ReadP(EEP_EPS,(uint8*)&sEA,sizeof(ENERGYACTIVE));                //��ȡE2�еĵ���
        DelayXms(1000);
        DelayXms(1000);
        DelayXms(1000);
        DelayXms(1000);
        DelayXms(1000);
   DelayXms(1000);
        DelayXms(1000);
        DelayXms(1000);
        DelayXms(1000);
        DelayXms(1000);
   DelayXms(1000);
        DelayXms(1000);
        DelayXms(1000);
        DelayXms(1000);
        DelayXms(1000);
        SysE2ReadData(EEP_EPS,(uint8*)&sEA,sizeof(ENERGYACTIVE)); 
       
        //SysE2ParaSetManage(EEP_EPS,(uint8*)&sEA,sizeof(ENERGYACTIVE));  
         //memcpy((uint8*)&gs_EnergyA, (uint8*)&sEA, sizeof(ENERGYACTIVE));   
        if(do_CRC((uint8*)&sEA, sizeof(ENERGYACTIVE)- 4) != sEA.uiCRC)      //���E2�е���CRC�Ƿ���ȷ
        {                                                                   //��E2��������ȷ
//        #if (_SW_SE_Type > 0)
//            SE_ReadP(EEP_EPS+EEP_BAK_LEN,(uint8*)&sEA,sizeof(ENERGYACTIVE));
//        #else
//            BE_ReadP(EEP_EPS+EEP_BAK_LEN,(uint8*)&sEA,sizeof(ENERGYACTIVE)); //��ȡE2�еĵ���
//        #endif 
//            if(do_CRC((uint8*)&sEA, sizeof(ENERGYACTIVE) - 4) == sEA.uiCRC)
//            {                                                               //����E2������ȷ
//                memcpy((uint8*)&gs_EnergyA, (uint8*)&sEA, sizeof(ENERGYACTIVE));            //����E2���ݵ�����RAM��
 //           }
 //            return;
             SysE2ReadData(EEP_EPS,(uint8*)&sEA,sizeof(ENERGYACTIVE)); 
        }
  //      else
        {
            memcpy((uint8*)&gs_EnergyA, (uint8*)&sEA, sizeof(ENERGYACTIVE));                //������E2������RAM��
        }
    }
}

/*=========================================================================================\n
* @function_name: Eny_CheckEnergy
* @function_file: Energy.c
* @����: ���ṹ���е������Ƿ���ȷ
*       ��������crcУ�����������,0�ض���У����ȷ,1��У�������ݶ�,2����
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-07)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Eny_CheckEnergy(uint8 power)
{
    uint8   ucErr = 0;
    uint16  uiCRC;
    uint16 *p,*q;
    if(power == active)
    {
        p =(uint16*)& gs_EnergyData;
        q = (uint16*)&gs_EnergyData_bak;
    }
    else
    {
        p = (uint16*)&gs_NEnergyData;
        q = (uint16*)&gs_NEnergyData_bak;
    }
    uiCRC = do_CRC((uint8 *)p, sizeof(ENERGYDATA)-2);          // ��������CRCУ��
    if (uiCRC != p[2])                                       
    {                                                                       
        ucErr++;                                                            
        uiCRC = do_CRC((uint8 *)q, sizeof(ENERGYDATA)-2);  // ���ݵ�������CRCУ��
        if (uiCRC != q[2])                               
        {                                                                   
            ucErr++;                                                        
        }                                                                   
    }                                                                       

    if (ucErr & 1)                                                          
    {                                                                       // ��һ�����
        memcpy((uint8*)p, (uint8*)q, sizeof(ENERGYDATA));     // ��������
    }
    else if(ucErr >= 2)                                                     // ������ͱ������򶼳���
    {                                                                       // ������                                                               
        //Eny_EnergyClrCur(ENERGY_CLR_RAM,power);                                       // ����
    }                                                                       
}
 
/*=========================================================================================\n
* @function_name: Eny_CheckEnergyActiveRAM
* @function_file: Energy.c
* @����: ���RAM�����Ƿ���ȷ
* 
* @����: 
* 
* @����: 
* @return: uint8 
* @����:   lwb (2012-03-07)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 Eny_CheckEnergyActiveRAM(uint8 power)
{
    //��֤����CRCУ��
    uint32 *p;
    if(power == active)
    {
        p = (uint32*)&gs_EnergyA;
    }
    else
    {
        p = (uint32*)&gs_NEnergyA;
    }
    if(do_CRC((uint8*)p, sizeof(ENERGYACTIVE)- 4) != p[1])
    {
        return ENERGY_ERR_RAM;
    }
    
    return ENERGY_ERR_NONE;
}
 
/*=========================================================================================\n
* @function_name: Eny_GetEp1
* @function_file: Energy.c
* @����: ���㵱ǰ�������й��ܵ����������ַ���)
* 
* 
* @����: 
* @param:index  0-��ϣ�����״̬�־�����, 1-ԭ��, 2-��
* @param:fee   �Ʒѵķ���
* 
* @����: 
* @return: int16 
* @����:   lwb (2012-03-07)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  hjj 
* @�޸�����: ����������λ����phyadd
===========================================================================================*/
int16 Eny_GetEp1(uint8 index, uint8 fee,uint8 phyadd,uint8 power)                 
{
    int16 iRet=0;
    uint16 *pEyP;
    uint16 *pEyN;
     fee = 0;
    if(power == active)
    {
        Eny_CheckEnergy(active);                                                  //������������Ч�ԣ��Ƿ�ͨ�Ĺ�CRCУ��
        switch(phyadd)
            {
                case APhy:
                    //pEyP=&gs_EnergyData.uiEPZ[phyadd];           //����������
                    //pEyN=&gs_EnergyData.uiENZ[phyadd];           //����������
                    break;
                case BPhy:
                   // pEyP=&gs_EnergyData.uiEPZ[phyadd>>3];           //����������
                    //pEyN=&gs_EnergyData.uiENZ[phyadd>>3];           //����������
                    break;
                case CPhy:
                    //pEyP=&gs_EnergyData.uiEPZ[phyadd>>1];           //����������
                    //pEyN=&gs_EnergyData.uiENZ[phyadd>>1];           //����������
                    break;
                case ABCPhy:
                default:
                   /* if((gs_FunCfg.uc_CfSaveCfg&0x0f)==0x05)                            //CF����ַ��ʼ���
                    {
                        if(fee == 0)
                        {
                            pEyP=&gs_EnergyData.uiEPZ[phyadd];           //����������
                            pEyN=&gs_EnergyData.uiENZ[phyadd];           //����������
                        }
                        else
                        {
                            pEyP=(&gs_EnergyData.uiEP1[phyadd] + (fee - 1));
                            pEyN=(&gs_EnergyData.uiEN1[phyadd] + (fee - 1));
                        }       
                    }*/
                   // else
                    {
                        pEyP=&gs_EnergyData.uiEPZ;           //����������
                        pEyN=&gs_EnergyData.uiENZ;           //����������
                    }
                    break;
            }

    }
    else
    {
        Eny_CheckEnergy(reactive);                                                  //������������Ч�ԣ��Ƿ�ͨ�Ĺ�CRCУ��
        switch(phyadd)
            {
                case APhy:
                    //pEyP=&gs_NEnergyData.uiEPZ[phyadd];           //����������
                    //pEyN=&gs_NEnergyData.uiENZ[phyadd];           //����������
                    break;
                case BPhy:
                    //pEyP=&gs_NEnergyData.uiEPZ[phyadd>>3];           //����������
                    //pEyN=&gs_NEnergyData.uiENZ[phyadd>>3];           //����������
                    break;
                case CPhy:
                    //pEyP=&gs_NEnergyData.uiEPZ[phyadd>>1];           //����������
                    //pEyN=&gs_NEnergyData.uiENZ[phyadd>>1];           //����������
                    break;
                case ABCPhy:
                default:
                    /*if((gs_FunCfg.uc_CfSaveCfg&0x0f)==0x05)                            //CF����ַ��ʼ���
                    {
                        if(fee == 0)
                        {
                            pEyP=&gs_NEnergyData.uiEPZ[phyadd];           //����������
                            pEyN=&gs_NEnergyData.uiENZ[phyadd];           //����������
                        }
                        else
                        {
                            pEyP=(&gs_NEnergyData.uiEP1[phyadd] + (fee - 1));
                            pEyN=(&gs_NEnergyData.uiEN1[phyadd] + (fee - 1));
                        }       
                    }*/
                   // else
                    {
                        pEyP=&gs_NEnergyData.uiEPZ;           //����������
                        pEyN=&gs_NEnergyData.uiENZ;           //����������
                    }
                    break;
            }
    }
    
    if(index==1)
    {
        iRet = *pEyP;                               //���������������
    }else if(index==0)
    {   
                                                    //�����������й����ռ�����ʽ��������
        //������������
        if(guc_EPCalcType&BIT0)
        {
            iRet = *pEyP;
        }
        else if(guc_EPCalcType&BIT1)
        {
            iRet -= *pEyP;
        }
        //������������
        if(guc_EPCalcType&BIT2)
        {
            iRet += *pEyN;
        }
        else if(guc_EPCalcType&BIT3)
        {
            iRet -= *pEyN;
        }
                       
    }else if(index==2)
    {
        iRet =*pEyN;                                             //���ط����������
    }
    return iRet;
}
 
/*=========================================================================================\n
* @function_name: Eny_EnergyActiveSave
* @function_file: Energy.c
* @����: ��ǰ����д��E2��
*           ���ﻹ����һ������,�������ݷַ��ʺ����ܵ�������ȵĻ�,������д���ݵ�E2��
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-07)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Eny_EnergyActiveSave(uint8 power)
{
    uint8 ucFlg = 0;
    uint32 *p;
    uint16 *q;
    uint32 e2_addr;//,e2_addr_bak;
    ENERGYACTIVE sEA;
    if(power == active)
    {
        e2_addr = EEP_EPS;
        //e2_addr_bak = EEP_EPS + EEP_BAK_LEN;
        p = (uint32*)&gs_EnergyA;
        q = (uint16*)&gs_EnergyData;
    }
    else
    {
        e2_addr = EEP_NEPS;
        //e2_addr_bak = EEP_EPS + EEP_BAK_LEN + 100;
        p = (uint32*)&gs_NEnergyA;
        q = (uint16*)&gs_NEnergyData;
    }
    //����֤E2����CRC�Ƿ���ȷ,�����һ����ȷ,����Ϊ�ⲿ��������Ч
    //BE_ReadP(e2_addr,(uint8*)&sEA,sizeof(ENERGYACTIVE));
    //SysE2ReadData(e2_addr,(uint8*)&sEA,sizeof(ENERGYACTIVE));
   // memcpy((void*)p, (void*)&sEA, sizeof(ENERGYACTIVE));

/*    if(do_CRC((uint8*)&sEA, sizeof(ENERGYACTIVE)- 4) == sEA.uiCRC)
    {
        ucFlg = 1;
    }
    
    //���E2����CRC��Ч,����E2���ݸ���RAM����,��E2����CRC����ȷʱ,RAM����CRC��ȷ,��ô����RAM����
    //���������ȷ,����������E2�����ݸ���RAM����
    if(ucFlg == 0)
    {
        if(do_CRC ((uint8*)p, sizeof(ENERGYACTIVE) - 4) != p[1])
        {
            //�������������ȷ���򶼲�����ֱ�ӷ��ء�
            return;
        }
    }

    if(ucFlg == 1)
    {
        memcpy((void*)p, (void*)&sEA, sizeof(ENERGYACTIVE));
    }
*/
    {
       p[0]   += q[0];
    }

    //����CRC
    p[1] = do_CRC((uint8*)p, sizeof(ENERGYACTIVE) - 4);
    //��ҪRAM�����Ϸ�����дE2
    if(Eny_CheckEnergyActiveRAM(power) == ENERGY_ERR_NONE)
    {
        //BE_WriteP(e2_addr,(uint8*)p,sizeof(ENERGYACTIVE));
        SysE2ParaSetManage(e2_addr,(uint8*)p,sizeof(ENERGYACTIVE));
    }
    else
    {
        return;
    }
    //����жϼ���,����������������ж�����Ϊ������
    if(Eny_CheckEnergyActiveRAM(power) == ENERGY_ERR_NONE)
    {
        Eny_EnergyClrCur(ENERGY_CLR_RAM,power);
    }
}
 
/*=========================================================================================\n
* @function_name: Eny_EnergyProc
* @function_file: Energy.c
* @����: �����������յ��ײ��������ݺ�ת����RAM��ͬʱ����CRCУ��
*       ͬʱ�б��Ƿ���Ҫ��RAM�еĵ���ת����E2��
* 
* @����: 
* @����: 
* @����:   lwb (2012-03-07)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Eny_EnergyProc(uint8 power)
{
    uint8   i;                                           
    uint8   ucDelta;  
    uint8*  p;                                          // �����й�������ָ��
    uint16*  p1;
    uint16*  p2;
    uint8 ucSaveflg;
                                             
    ucSaveflg=0;                                                    
    Eny_CheckEnergy(power);                                  // ��������Ч��
    if(power == active)
    {
        p = (uint8*)&gs_Energy;                             // ��������;
        p1 = (uint16*)&gs_EnergyData;
    }
    else
    {
        p = (uint8*)&gs_NEnergy;                             // ��������;
        p1 = (uint16*)&gs_NEnergyData;
    }                       
    for (i=0; i<sizeof(S_ENERGY); i++,p++,p1++)
    {                                                   // ����й������־�������ﵽMAX_E����EEPROM��дһ�Σ�
        if ( *p > 0)                                    
        {                                               // �е�������
            __disable_interrupt();                      // ���ݱ���
            ucDelta = *p;                               // ���ж��в����������ƶ���RAM�����Ĵ�����
            *p = 0;                                     
            __enable_interrupt();                           
            *p1 += ucDelta;                             // ���жϲ��������������ۼӵ�RAM��
        }                                               
                                                        
        if (*p1 > MAX_E)                                // RAM�е���������Ϊ��Ч����
        {                                               
            Eny_CheckEnergy(power);                              
        }                                               

        if (*p1 > SAVEVALUE)                            // �б��Ƿ���Ҫд��
        { 
            ucSaveflg=0x55;                                              
        }                                               
    }         
    if(power == active)
    {
        p2 = (uint16*)&gs_EnergyData_bak;                             // ��������;
        p1 = (uint16*)&gs_EnergyData;
    }
    else
    {
        p2 = (uint16*)&gs_NEnergyData_bak;                             // ��������;
        p1 = (uint16*)&gs_NEnergyData;
    }
    p1[2] = do_CRC((uint8 *)p1, sizeof(ENERGYDATA)-2);  // ��������CRCУ��
    memcpy((void*)p2, (void*)p1, sizeof(ENERGYDATA));
    if(ucSaveflg==0x55)
    {
        Eny_EnergyActiveSave(power);
    }
}
/*=========================================================================================\n
* @function_name: WriteHisEP
* @function_file: Energy.c
* @����: д����ʷ����
*
* @����:
* @����:
* @����:   lwb (2012-03-25)
* @��ע:
*-------------------------------------------------------------------------------------------
* @�޸���:
* @�޸�����:
===========================================================================================*/
void WriteHisEP(void)
{
 /*   if(Eny_CheckEnergyActiveRAM(active) != ENERGY_ERR_NONE)
    {
        BE_ReadP(EEP_EPS, (uint8*)&gs_EnergyA, sizeof(ENERGYACTIVE));
        if(Eny_CheckEnergyActiveRAM(active) != ENERGY_ERR_NONE)
        {
        #if (_SW_SE_Type > 0)
            SE_ReadP(EEP_EPS, (uint8*)&gs_EnergyA, sizeof(ENERGYACTIVE));
        #else
            BE_ReadP(EEP_EPS+EEP_BAK_LEN, (uint8*)&gs_EnergyA, sizeof(ENERGYACTIVE));
        #endif
        }
    }
    DataProcWriteRound2(R2_MOVE,LSDLID,(uint8*)&gs_EnergyA);    //д����ʷ����*/
}

/*=========================================================================================\n
* @function_name: MonthLYDL
* @function_file: Energy.c
* @����: �¶���ϵ�������
* 
* @����: 
* @����: 
* @����:   lwb (2013-03-01)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void MonthLYDL(uint8 uc_Save)
{
   // uint32 ulData;
//    uint8  ucPt;
//    int16 iDelta[2];
//    Word32 JSSJTime;
//    
//    //BE_ReadB(EEP_SYZHLYDL_PT, (uint8*)&ucPt);       //ָ��
//    if(ucPt == 0)
//    {
//        ucPt = 1;
//    }
//    else
//    {
//        ucPt--;
//    }
//
//    iDelta[0] = Eny_GetEp1(1, 0,ABCPhy,active);                   //��ǰ�����������
//    iDelta[1] = Eny_GetEp1(2, 0,ABCPhy,active);                   //��ǰ�����������
//    ulData=gs_EnergyA.ulP[0]+gs_EnergyA.ulN[0]+iDelta[0]+iDelta[1];
//    //BE_WriteP(EEP_SYZHLYDLADDR+ucPt*4,(uint8*)&ulData,4);
//    BE_WriteP(EEP_SYZHLYDL_PT, (uint8*)&ucPt,1);    //д��ָ������  
//   
//    if(uc_Save == 1)
//    {
//        JSSJTime.byte[0]=gs_DateTime.ucHour;
//        JSSJTime.byte[1]=gs_DateTime.ucDay;
//        JSSJTime.byte[2]=gs_DateTime.ucMonth;
//        JSSJTime.byte[3]=gs_DateTime.ucYear;
//        Eny_EnergyActiveSave(active);
//        WriteHisEP();
//        SaveJSTime(JSSJTime.lword);
//    }
}

/*=========================================================================================\n
* @function_name: EnergyCBRManage
* @function_file: Energy.c
* @����: �����մ���,һСʱ���ñ������жϳ�����ʱ��һ��
* 
* @����: 
* @����: 
* @����:   lwb (2012-05-22)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void EnergyCBRManage(void)
{
    uint8 i,j;
    Word32 JSSJTime;
    Word16 W16_JSR[3];

    BE_ReadP(EEP_CBR1, W16_JSR[0].byte,6);              // �õ������յ����ں�Сʱ
                                                    
    for(i=0;i<2;i++)                                    //ȥ����ͬ�ĳ�����
    {
        for(j=i+1;j<3;j++)
        {
            if(W16_JSR[i].word==W16_JSR[j].word)
            {
                W16_JSR[j].word=0x9999;
            }
        }
    }


    for(i=0;i<3;i++)
    {
        if(W16_JSR[i].word==0x9999)
        {
            continue;
        }
        //��Ҫ��Ҫ���������Ƿ�Ϸ�

        if(W16_JSR[i].byte[0] == gs_DateTime.ucHour && W16_JSR[i].byte[1]  == gs_DateTime.ucDay)
        {
            if(i==0)                                            //����һ�������գ�������й������õ���
            {
                MonthLYDL(1);
            }
            else
            {
            //������ʱ�䵽,��ʼ����ת��
                JSSJTime.byte[0]=W16_JSR[i].byte[0];
                JSSJTime.byte[1]=W16_JSR[i].byte[1];
                JSSJTime.byte[2]=gs_DateTime.ucMonth;
                JSSJTime.byte[3]=gs_DateTime.ucYear;
                Eny_EnergyActiveSave(active);
                WriteHisEP();
                SaveJSTime(JSSJTime.lword);
            }
            break;                                  //ͬһ��ʱ�̲�����ת��ü���           
        }
    }
}
/*=========================================================================================\n
* @function_name: SaveJSTime
* @function_file: Energy.c
* @����: �������ʱ��
* 
* 
* @����: 
* @param:time  
* @����: 
* @����:   xuqf (2013-1-5)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void SaveJSTime(uint32 time)
{
    DataProcWriteRound2(R2_MOVE,JSSJJLID,(uint8*)&time);
}
