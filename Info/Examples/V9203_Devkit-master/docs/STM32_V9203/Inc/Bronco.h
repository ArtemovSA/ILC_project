#ifndef __BRONCO_H__
#define __BRONCO_H__


#ifndef  BRONCOEXT
    #define BRONCOEXT extern
#else
    #define BRONCOEXT
#endif



#define Work_normal  0x11
//=============================================================
//Broncoģ����ƼĴ���
//=============================================================
#define   RegANCtrl0   0x8000
#define   RegANCtrl1   0x8001
#define   RegANCtrl2   0x8002
#define   RegANCtrl3   0x8003


//=============================================================
//Bronco �������üĴ���  
//=============================================================
#define   RegMTPARA0   0xC000
#define   RegMTPARA1   0xC001
#define   RegMTPARA2   0xC002
#define   RegMTPARA3   0xC003

#define   ZZCPSEL     0xEC15
#define   ZZPA0          0xEC23
#define   ZZPA1          0xEC24
#define   ZZQA0         0xEC47
#define   ZZQA1         0xEC48
#define   ZZAPPA       0xEC05
#define   ZZPCF0A     0XEC34


//=============================================================
//Bronco ֱ�������Ĵ���  
//=============================================================
#define   RegDCAU       0xF02C
#define   RegDCAI        0xF02D
#define   RegDCBU       0xF02E
#define   RegDCBI        0xF02F
#define   RegDCCU       0xF030
#define   RegDCCI        0xF031

//=============================================================
//Bronco Ƶ����λ�Ĵ���  
//=============================================================

#define   RegAFREQ      0xC008
#define   RegBFREQ      0xC009
#define   RegCFREQ      0xC00A

#define   RegBUPHA      0xC00B
#define   RegCUPHA      0xC00C
#define   RegAIPHA       0xC00D
#define   RegBIPHA       0xC00E
#define   RegCIPHA       0xC00F


//=============================================================
//Bronco ��ѹ/����˲ʱ��Чֵ�Ĵ���  
//=============================================================
#define   RegARTIA     0xE83E
#define   RegARTIB    0xE83F
#define   RegARTIC     0xE840

#define   RegARTUA     0xE842
#define   RegARTUB    0xE843
#define   RegARTUC     0xE844


//=============================================================
//Bronco �й�/�޹����ʼĴ���  
//=============================================================
#define   RegMAPSUM0      0xE8EE
#define   RegMAPSUM1      0xE8EF

#define   RegMAQSUM0      0xE8F3
#define   RegMAQSUM1      0xE8F4




//=============================================================
//Bronco ��ѹ/������ЧֵУ���Ĵ���  
//=============================================================
#define   RegWARTIA     0xE968
#define   RegWARTIB     0xE969
#define   RegWARTIC     0xE96A

#define   RegWARTUA     0xE96C
#define   RegWARTUB     0xE96D
#define   RegWARTUC     0xE96E


//=============================================================
//Bronco ȫ���й�/�޹�����У���Ĵ���  
//=============================================================
#define   RegWAPTA0      0xE95A
#define   RegWAPTB0      0xE95E
#define   RegWAPTC0      0xE962

#define   RegWAQTA      0xE965
#define   RegWAQTB      0xE966
#define   RegWAQTC      0xE967

#define  RegWAEC0      0XE954       //�ǲ�


//=============================================================
//Bronco �����й�/�޹�����У���Ĵ���  
//=============================================================
#define   RegWBPTA      0xE970   
#define   RegWBPTB      0xE971
#define   RegWBPTC      0xE972

#define   RegWBQTA      0xE973
#define   RegWBQTB      0xE974
#define   RegWBQTC      0xE975


//=============================================================
//Bronco ȫ���й�/�޹����ʶ��β���У���Ĵ���  
//=============================================================
#define   RegWWAPTA     0xE98E
#define   RegWWAPTB     0xE98F
#define   RegWWAPTC     0xE990

#define   RegWWAQTA      0xE991
#define   RegWWAQTB      0xE992
#define   RegWWAQTC      0xE993


//=============================================================
//Bronco ����ֵ�Ĵ���  
//=============================================================
#define   RegZZEGYTHH     0xEC1E
#define   RegZZEGYTHL     0xEC1F

#define   RegCTHH       0xE8AA
#define   RegCTHL       0xE8AB


//=============================================================
//Bronco У��ͼĴ���  
//=============================================================
#define   RegCKSUM     0xC003
//=============================================================
//Bronco �����������ֵ�Ĵ���  
//=============================================================
#define  RegZZDCUM    0xEC1D


//=============================================================
//Bronco ȫ�����������Ĵ���
//=============================================================
#define RegMAFA  0xE8E4
#define RegMAFB  0xE8E5
#define RegMAFC  0xE8E6
#define RegMAFS   0xE8E7




#define APhy  0x00
#define BPhy  0x01
#define CPhy  0x02
#define ABCPhy 0x03


#define RMSUA        0xE944
#define RMSUB        0xE945
#define RMSUC        0xE946

#define RMSI1A       0xE90E
#define RMSI1B       0xE90F
#define RMSI1C       0xE910

#define DATAPA       0xE8E8
#define DATAPB      0xE8E9
#define DATAPC       0xE8EA
#define DATAPABC       0xE8EE

#define DATAQA        0xE8F0
#define DATAQB        0xE8F1
#define DATAQC        0xE8F2
#define DATAQABC       0xE8F3

#define DATAFREQA    0xC008
#define DATAFREQB    0xC009
#define DATAFREQC    0xC00A


#define DATAFREQ    0xC008




//������Ϊ�˱���ͨ��
#define DATAP       0x0119
#define DATAQ       0x10d7
#define RMSU        0x011B
#define RMSI1       0x011C
#define RMSI2       0x0117

#define RAP         0x0102  //ȫ���й�����ԭʼֵ
#define RAQ         0x0103  //ȫ���޹�����ԭʼֵ
#define ARRTU       0x0104  //ȫ��������Чֵ��ԭʼֵ
#define ARRTI       0x0105  //ȫ����ѹ��Чֵ��ԭʼֵ
//������Ϊ�˱���ͨ��





enum
{
    Addr_UA=0,
    Addr_IA,
    Addr_UB,
    Addr_IB,
    Addr_UC,
    Addr_IC,
};

typedef struct
{
    uint32  RacWARTU;           //ȫ����ѹ��Чֵ�Ȳ�Ĵ���
    uint32  RacWARTI;           //ȫ��������Чֵ�Ȳ�Ĵ���
    uint32  RacWAPT;            //ȫ���й����ʱȲ�Ĵ���
    uint32  RacWWAPT;           //ȫ���й����ʶ��β����Ĵ���                
    uint32  RacREWWAPT;         //ȫ���޹������ʶ��β����Ĵ���                
}S_JBRE;


typedef struct 
{
    uint16 ui_MeterC;           // ����
    uint16 ui_Un;               // ��Ƶ�ѹ
    uint16 ui_Ib;               // ��Ƶ���
    uint16 ui_Resve1;           //����
    
    uint32  RacEGYTH;           //�й�����ֵ�Ĵ���
    uint32  RacCTHH;             //��/Ǳ���ж�����ֵ�Ĵ���
    uint32  RacCTHL;             //��/Ǳ���ж�����ֵ�Ĵ���
    uint32 RacZZDCUM;        //�����������ֵ
    uint32  RacWAEC0;         //�ǲ�0
    uint32 RacMTPARA0;
    uint32 RacMTPARA1;
    uint32 RacMTPARA2;
    uint32  RacANCtrl0;         //ģ����ƼĴ���0
    uint32  RacANCtrl1;         //ģ����ƼĴ���1
    uint32  RacANCtrl2;         //ģ����ƼĴ���2
    uint32  RacANCtrl3;         //ģ����ƼĴ���2
    S_JBRE  gs_JBA;             //A��Ȳ���
    S_JBRE  gs_JBB;             //B��Ȳ���
    S_JBRE  gs_JBC;             //C��Ȳ���
    uint32 ul_PG;               //���ʱ���ϵ��
    uint32 ul_URmG;             //��ѹͨ������ϵ��
    uint32 ul_I1RmG;            //����ͨ��1����ϵ��

    uint16 ui_Resve2;           //����
    uint16 ui_JbCRC;            // У�������CRC���
}S_JBPM;

BRONCOEXT S_JBPM  gs_JbPm;            //У�����

typedef struct 
{       //���츴���ṹ
    float  real;
    float  imag;
} COMPLEX;

BRONCOEXT COMPLEX  fftx[256];

BRONCOEXT volatile uint8 guc_CommDelayTime;                                 //ͨѶ�ȴ���ʱ
BRONCOEXT uint8 guc_CommDelayFlag;
BRONCOEXT uint8 guc_Phy;          //��λ
BRONCOEXT uint8 guc_DODMAFlg;
//BRONCOEXT uint8 guc_FFT_Flg;


uint8  WriteBronco(uint32 Data,uint16 Addr);
void Bronco_PMCtrl(uint8 pm);
uint32  ReadBronco(uint16 Addr);
void BroncoInit(void);
void UpdateBroncoPara(void);
void JbpmInit(void);
uint8 Brc_CheckDMAData(int16* DMAaddr, uint8 ucType);
void Recording(void);
void FFT_Task(int16 *indata);
void Data_FFTProc(void);
#endif



