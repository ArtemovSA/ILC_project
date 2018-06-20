#ifndef __DATA_PROTECTEXT_H__
#define __DATA_PROTECTEXT_H__

#ifndef DATAPROTECTEXT
    #define DATAPROTECTEXT extern 
#endif

#define E2_PROTECT_LEN  0x40            //64�ֽ�,��ĿǰE2�����������й�

#define JBLEN (sizeof(S_JBPM))
#define RTC_LEN 28
#define TIME_LEN 9
#define LOAD_LEN 16

typedef struct s_datachk
{//���ݰ�ȫ�Լ�����,���ڶ�����ԱȽϵ��������ֵ����Сֵ
    uint8   ucMin;
    uint8   ucMax;
}S_DATACHK;

#define DATA_CHECK_USE_DEF  0x80        //��Ҫʹ��Ĭ��ֵ

typedef struct s_e2parachk
{//��СE2��Ӧ�ı�־λ�趨��gui_BEParaCheck��gui_SEParaCheck֮��
    uint8   ucLen;                      //����У�����������ܳ���,����crc�����ֽ�
    uint32  uiAddr;                     //��Ӧ�Ĵ�СE2�Ĳ������׵�ַ
}S_E2PARACHK;

typedef struct s_dmachk
{
    uint8  ucNeedOpen;
    uint8  ucSaveFlg;
    uint8  ucOffset;
    uint8  ucPhase[6];
    uint8  ucZBpt;
    int16 uiDCValue[6];
}S_DMACHK;

DATAPROTECTEXT S_DMACHK gs_DMACheck;
DATAPROTECTEXT uint8 Flg_FirstPowerUp;

DATAPROTECTEXT uint32   gul_E2ParaStt;          //E2��Ӧ�Ĳ���������crcУ��ȶ�
DATAPROTECTEXT uint8  guc_RtcConflict;          //rtcУ����ͻ��־λ
//DATAPROTECTEXT const uint8 SampTab[];
//DATAPROTECTEXT const uint16 SampE2SaveTab[];


uint8 DataCheck(uint8 ucNum, uint8* p, S_DATACHK * psDC);
void RefreshPara(void);
void DataSecurityInit(void);
void SysE2ParaSetManage(uint32 uiAddr, uint8* pucBuf,uint8 ucLen);
void SysE2ReadData(uint32 uiAddr, uint8* pucBuf,uint8 ucLen);
void E2ParaCheck(void);
void CalcZBProc(void);
uint8 ComCheckFormat(uint8 *Databuf,uint8 type,uint8 len);
void WriteDMA2EEP(uint8 ucCnt);
#endif
