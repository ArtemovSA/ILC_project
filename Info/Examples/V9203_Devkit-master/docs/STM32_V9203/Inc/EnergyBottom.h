#ifndef _DR_ENERGY_H_
    #define _DR_ENERGY_H_

#ifndef ENERGYBOTTOMEXT
    #define ENERGYBOTTOMEXT extern
#else
    #define DR_ENERGYSELF
#endif

 
#define Const_MaxCf     0x05//���������
#define ADCFRE   819200
#define PLEVEL   65         //��������
#define IALEVEL  25         //��������
#define IBLEVEL  70
#define ULEVEL   200         //��ʼ����ʾ�ĵ�ѹ20.0V
#define CRIULEVEL1  1694
#define CRIULEVEL2  2640
 

//ENERGYBOTTOMEXT uint32 u32PMdatal;
//ENERGYBOTTOMEXT uint8  u8PMdatah;

/*typedef struct
{
    int32 ul_Adjust;    // ������Ȳ��
    uint16 ui_CRC;       // CRC
}S_GSCP;*/

//ENERGYBOTTOMEXT S_GSCP gs_GiantSCP;
//ENERGYBOTTOMEXT uint8 guc_GiantSCPflag;
//ENERGYBOTTOMEXT uint8 guc_GiantSCPcnt;

//����Ϊ����������޹������Ľṹ�嶨�壬
//��Ӧ�ò�ӿڵ�һ�º�ͳһ���������ݵ�λΪ0.01kwh
typedef struct                   
{
    uint8 uc_Pz;       // ����������������
    //uint8 uc_P1[4];       // �����й�����1
   // uint8 uc_P2[4];       // �����й�����2����������
   // uint8 uc_P3[4];       // �����й�����3����������
    //uint8 uc_P4[4];       // �����й�����3����������

    uint8 uc_Nz;       // ����������������
    //uint8 uc_N1[4];       // �����й�����1
    //uint8 uc_N2[4];       // �����й�����2����������
    //uint8 uc_N3[4];       // �����й�����3����������
    //uint8 uc_N4[4];       // �����й�����3����������
} S_POWERCF; 

ENERGYBOTTOMEXT S_POWERCF gs_PowerCf,gs_PowerCfN;
//ENERGYBOTTOMEXT S_POWERCF gs_Energy;

//ENERGYBOTTOMEXT  uint8 guc_PowRCnt;        //���ʷ������
//ENERGYBOTTOMEXT  uint8 guc_PowRExitCnt;    //���ʷ����˳�����

typedef struct
{
    uint32 Pa;                          //����2λС�� ��λΪW
    uint16 Ua;                          //����2λС�� ��λΪV
    uint16 Ia;                          //����3ΪС�� ��λΪA
}S_CHECKMETER;

ENERGYBOTTOMEXT S_CHECKMETER gs_Checkmeter;//У���ʱȶ�����


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


//��������
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
