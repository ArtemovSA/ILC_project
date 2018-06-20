#ifndef _DR_ENERGY_H_
    #define _DR_ENERGY_H_

#ifndef ENERGYBOTTOMEXT
    #define ENERGYBOTTOMEXT extern
#else
    #define DR_ENERGYSELF
#endif

 
#define Const_MaxCf     0x05//最大脉冲数
#define ADCFRE   819200
#define PLEVEL   65         //启动功率
#define IALEVEL  25         //启动电流
#define IBLEVEL  70
#define ULEVEL   200         //开始有显示的电压20.0V
#define CRIULEVEL1  1694
#define CRIULEVEL2  2640
 

//ENERGYBOTTOMEXT uint32 u32PMdatal;
//ENERGYBOTTOMEXT uint8  u8PMdatah;

/*typedef struct
{
    int32 ul_Adjust;    // 大电流比差补偿
    uint16 ui_CRC;       // CRC
}S_GSCP;*/

//ENERGYBOTTOMEXT S_GSCP gs_GiantSCP;
//ENERGYBOTTOMEXT uint8 guc_GiantSCPflag;
//ENERGYBOTTOMEXT uint8 guc_GiantSCPcnt;

//以下为能量脉冲和无功能量的结构体定义，
//与应用层接口的一致和统一，以下数据单位为0.01kwh
typedef struct                   
{
    uint8 uc_Pz;       // 正向总能量脉冲数
    //uint8 uc_P1[4];       // 正向有功费率1
   // uint8 uc_P2[4];       // 正向有功费率2能量脉冲数
   // uint8 uc_P3[4];       // 正向有功费率3能量脉冲数
    //uint8 uc_P4[4];       // 正向有功费率3能量脉冲数

    uint8 uc_Nz;       // 反向总能量脉冲数
    //uint8 uc_N1[4];       // 反向有功费率1
    //uint8 uc_N2[4];       // 反向有功费率2能量脉冲数
    //uint8 uc_N3[4];       // 反向有功费率3能量脉冲数
    //uint8 uc_N4[4];       // 反向有功费率3能量脉冲数
} S_POWERCF; 

ENERGYBOTTOMEXT S_POWERCF gs_PowerCf,gs_PowerCfN;
//ENERGYBOTTOMEXT S_POWERCF gs_Energy;

//ENERGYBOTTOMEXT  uint8 guc_PowRCnt;        //功率反向计数
//ENERGYBOTTOMEXT  uint8 guc_PowRExitCnt;    //功率反向退出计数

typedef struct
{
    uint32 Pa;                          //保留2位小数 单位为W
    uint16 Ua;                          //保留2位小数 单位为V
    uint16 Ia;                          //保留3为小数 单位为A
}S_CHECKMETER;

ENERGYBOTTOMEXT S_CHECKMETER gs_Checkmeter;//校表功率比对数据


ENERGYBOTTOMEXT uint8 guc_OpenCfTmCnt;
ENERGYBOTTOMEXT uint8 guc_CfOpenFlg;


typedef struct
{
    uint8   ucDelayTm;
    uint32  ul_PDirGate;
}S_PDIRCTR;
ENERGYBOTTOMEXT S_PDIRCTR gs_PDirCtr;

typedef struct
{
    uint8   ucDirCnt;
    uint8   ucRevDirCnt;
}S_PDIRCNT;

ENERGYBOTTOMEXT S_PDIRCNT gs_PDirCnt;

enum
{
    active,
    reactive,
};


//函数声明
void EnyB_PurInit(void);
void EnyB_JbPm_Init(void);



void EnyB_AutoChkPW(void);                                              
void EnyB_AutoChkIrms(void);
void EnyB_AutoChkUrms(void);
void EnyB_AutoChkAngle(void);
void EnyB_AutoCheck(void);
void EnyB_AutoCheck2(void);
void EnyB_CloseAutoCheck(void);
void EnyB_OpenAutoCheck(void);
void EnyB_AutoChkInrms(uint16 ui_SdCurrent);
void EnyB_JbPm_Updata(void);
void EnyB_InitCFInt(void);
//void EnyB_IntCF(void);
void EnyB_IntCF(uint8 power);   
void EnyB_AutoChkPW2(void);

void EnyB_Check_RMSI(void); 
void EnyB_Check_PowRev(void);
int16 EnyB_Cal_Cos(void);
void EnyB_SetMeterRule(void);
void EnyB_Open_Cf(void);
void EnyB_FstPluseProc(void);

uint32 CalRMS(uint16 addr);
uint32 CalRMS2(uint16 addr,uint32 value);
uint32 CalCos(uint8 phy);

void RecoverPowDirConfig(void);
uint32 CalculateSC(int16 error,int32 LastSC);
uint8 CalculateAC( int16 Error);






#endif
