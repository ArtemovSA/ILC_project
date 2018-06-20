#ifndef __DATA_PROTECTEXT_H__
#define __DATA_PROTECTEXT_H__

#ifndef DATAPROTECTEXT
    #define DATAPROTECTEXT extern 
#endif

#define E2_PROTECT_LEN  0x40            //64字节,跟目前E2参数的排列有关

#define JBLEN (sizeof(S_JBPM))
#define RTC_LEN 28
#define TIME_LEN 9
#define LOAD_LEN 16

typedef struct s_datachk
{//数据安全性检验用,用于定义可以比较的数据最大值与最小值
    uint8   ucMin;
    uint8   ucMax;
}S_DATACHK;

#define DATA_CHECK_USE_DEF  0x80        //需要使用默认值

typedef struct s_e2parachk
{//大小E2对应的标志位设定在gui_BEParaCheck和gui_SEParaCheck之中
    uint8   ucLen;                      //整个校验数据区的总长度,包括crc两个字节
    uint32  uiAddr;                     //对应的大小E2的参数区首地址
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

DATAPROTECTEXT uint32   gul_E2ParaStt;          //E2对应的参数区进行crc校验比对
DATAPROTECTEXT uint8  guc_RtcConflict;          //rtc校正冲突标志位
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
