#ifndef __LCDMIDDLE_H__
#define __LCDMIDDLE_H__


#ifndef LCDMIDDLEEXT
    #define LCDMIDDLEEXT extern
#else
    #define LCDMIDDLESELF
#endif

//Ӧ�ò�ӿں���
void Disp_Numb(uint32 data,uint32 type);
void Disp_String(uint16 para);
void Disp_UnitPoint(uint16 para);
void Disp_State(uint32 data);

#ifdef LCDMIDDLESELF


//COM�ں�SEG�ڶ�Ӧ��RAM����
#define     COM8        0x07
#define     COM7        0x06
#define     COM6        0x05
#define     COM5        0x04
#define     COM4        0x03
#define     COM3        0x02
#define     COM2        0x01
#define     COM1        0x00

#define     S00         (0*8)
#define     S01         (1*8)
#define     S02         (2*8)
#define     S03         (3*8)
#define     S04         (4*8)
#define     S05         (5*8)
#define     S06         (6*8)
#define     S07         (7*8)
#define     S08         (8*8)
#define     S09         (9*8)
#define     S10         (10*8)
#define     S11         (11*8)
#define     S12         (12*8)
#define     S13         (13*8)
#define     S14         (14*8)
#define     S15         (15*8)
#define     S16         (16*8)
#define     S17         (17*8)
#define     S18         (18*8)
#define     S19         (19*8)
#define     S20         (20*8)
#define     S21         (21*8)
#define     S22         (22*8)
#define     S23         (23*8)
#define     S24         (24*8)
#define     S25         (25*8)
#define     S26         (26*8)
#define     S27         (27*8)
#define     S28         (28*8)
#define     S29         (29*8)
#define     S30         (30*8)
#define     S31         (31*8)
#define     S32         (32*8)
#define     S33         (33*8)
#define     S34         (34*8)
#define     S35         (35*8)
#define     S36         (36*8)
#define     S37         (37*8)
#define     S38         (38*8)
#define     S39         (39*8)
 


/*==================================================
˵��������Ϊ������ʾ���ֵĶ��壬����������ʹ�������
====================================================*/
//��������������ڵ�RAM���ֽ�λ��
//hjj add
#define     number8H     8
#define     number8L     7
#define     number7H     27
#define     number7L     26
#define     number6H     25
#define     number6L      24
#define     number5H     33 
#define     number5L     32
#define     number4H     31
#define     number4L     30
#define     number3H     29
#define     number3L     28
#define     number2H     15
#define     number2L     14
#define     number1H     13
#define     number1L     12
//hjj add
const uint8   NumAddr1[8] = {number4H,number4L,number3H,number3L,number2H,number2L,number1H,number1L};
const uint8   NumAddr2[8] = {number8H,number8L,number7H,number7L,number6H,number6L,number5H,number5L};




/*=======================================================
˵��������Ϊ������ʾ���ֵĶ��壬����������ʹ�������
=========================================================*/
//hjj add
#define NUMC1    BIT1 
#define NUMG1    BIT2
#define NUMB1    BIT2


#define NUMD1    BIT0
#define NUME1    BIT1
#define NUMF1    BIT3
#define NUMA1    BIT3

#define  N_Resdh1 0x01
#define  N_Resdl1 0x00

 //abc 
 //hjj add
const uint8 NUMTABLE_H1[]=
{
    NUMB1+NUMC1+NUMA1,                          //0
    NUMB1+NUMC1,                                //1 
    NUMB1+NUMA1,                                //2
    NUMB1+NUMC1+NUMA1,                          //3
    NUMB1+NUMC1,                                //4
    NUMA1+NUMC1,                                //5
    NUMC1+NUMA1,                                //6
    NUMB1+NUMC1+NUMA1,                          //7
    NUMB1+NUMC1+NUMA1,                          //8
    NUMB1+NUMC1+NUMA1,                          //9
    NUMB1+NUMC1+NUMA1,                          //A
    NUMC1,                                      //B
    NUMA1,                                      //C
    NUMB1+NUMC1,                                //D
    NUMA1,                                      //E
    NUMA1,                                      //F
};

//fged
//hjj add
const uint8   NUMTABLE_L1[]=
{
    NUME1+NUMF1+NUMD1,                          //0
    0,                                          //1 
    NUMG1+NUME1+NUMD1,                          //2 
    NUMG1+NUMD1,                                //3 
    NUMF1 + NUMG1,                              //4
    NUMG1+NUMD1+NUMF1,                          //5
    NUMG1+NUMD1+NUME1+NUMF1,                    //6
    0,                                          //7
    NUMG1+NUMD1+NUME1+NUMF1,                    //8
    NUMG1+NUMD1+NUMF1,                          //9
    NUMG1+NUME1+NUMF1,                          //A
    NUMD1+NUME1+NUMF1 + NUMG1,                  //B
    NUMD1+NUME1+NUMF1,                          //C
    NUMD1+NUME1+ NUMG1,                         //D
    NUMG1+NUMD1+NUME1+NUMF1,                    //E
    NUMG1+NUME1+NUMF1,                          //F
};


#define LCD_CHARL1_G  (NUMG1)                   //'-'
#define LCD_CHARH1_G  (0)                       //'-'
#define LCD_CHARL1_L  (NUMD1+NUME1+NUMF1)       //'L'
#define LCD_CHARH1_L  (0)                       //'L'
#define LCD_CHARL1_r  (NUMG1+NUME1)             //'r'
#define LCD_CHARH1_r  (0)                       //'r'
#define LCD_CHARL1_n  (NUME1+NUMG1)             //'n'
#define LCD_CHARH1_n  (NUMC1)                   //'n'
#define LCD_CHARL1_o  (NUMD1+NUME1+NUMG1)       //Сд��ĸ'o',������
#define LCD_CHARH1_o  (NUMC1)                   //Сд��ĸ'o',������
#define LCD_CHARL1_C  (NUMD1+NUME1+NUMF1)       //'C'
#define LCD_CHARH1_C  (NUMA1)                   //'C'
#define LCD_CHARL1_E  (NUMG1+NUMD1+NUME1+NUMF1) //'E'
#define LCD_CHARH1_E  (NUMA1)                   //'E'






/*==================================================
˵����״̬��ʾλ
===================================================*/
//8��ʾ��ַ����ڸ�5λ������3λ��,������ϵ�ֵ��ʾ�������ַ�ж�Ӧ��ŵ�λ
//hjj add
#define     Ut_X1                   (S05+COM1) //���ͷ���ʷ���
#define     Ut_X2                   (S36+COM1) //�绰
#define     Ut_X3                   (S16+COM1) //��̼�
#define     Ut_X4                   (S16+COM2) //��
#define     Ut_X5                   (S16+COM3) //���
#define     St_Reserved         (S39+COM8) //�չܽ�
#define     Ut_NIa                  (S06+COM1) //-(Ia)
#define     Ut_Ia                     (S13+COM1) //Ia
#define     Ut_NIb                  (S37+COM1) //-(Ib)
#define     Ut_Ib                    (S37+COM2) //Ib
#define     Ut_NIc                  (S37+COM3) //-(Ic)
#define     Ut_Ic                     (S37+COM4) //Ic
#define     Ut_Ua                   (S36+COM4) //Ua
#define     Ut_Ub                   (S36+COM3) //Ub
#define     Ut_Uc                   (S36+COM2) //Uc
#define     Cn_Nixiangxu      (S35+COM4) //������


#define     St_Len     16
const   uint16   St_Table[St_Len]=
{
    Ut_Ia,                    //0x01        
    Ut_Ib,                  //0x02 
    Ut_Ic,                 //0x04 
    Ut_NIa,             //0x08
    Ut_NIb,           //0x10
    Ut_NIc,          //0x20
    Ut_Ua,            //0x40
    Ut_Ub,           //0x80
    Ut_Uc,           //0x100
    St_Reserved,     //0x200
    Ut_X4,             //0x400
    Ut_X2,            //0x800
    Ut_X3,           //0x1000
    Ut_X5,           //0x2000
    Ut_X1,          //0x4000
    Cn_Nixiangxu,   //0x8000
};

/*======================================================
˵����С������ŵ���ʾ
========================================================*/
//���ż�С����ĸ���λ�εĶ���,P��ʾPOINT
//8��ʾ��ַ����ڸ�5λ������3λ����������ϵ�ֵ��ʾ�������ַ�ж�Ӧ��ŵ�λ
//hjj add
#define     Pt_Reserved (S39+COM8)//

//hjj add
#define     Ut_P1                    (S15+COM1) //P1
#define     Ut_P2                    (S29+COM1) //P2
#define     Ut_P3                    (S31+COM1) //P3
#define     Ut_P4                    (S33+COM1) //P4
#define     Ut_P5                    (S08+COM1) //P5
#define     Ut_P6                    (S25+COM1) //P6
#define     Ut_P7                    (S27+COM1) //P7


//С���㣨����"-"������ʾ��ֵ���еĵ�ַ�Ͷ�Ӧλ����5λ��ʾ��ַ����3λ��ʾ�ڶ�Ӧ�ڴ��ĳһλ
//��󳤶���16��Ŀǰֻ����7��������
//hjj add
#define PointLen    7
const   uint16   PointTable[PointLen]=
{
    Ut_P1,      //  0x01
    Ut_P2,      //  0x02
    Ut_P3,      //  0x04
    Ut_P4,      //  0x08
    Ut_P5,      //  0x10
    Ut_P6,      //  0x20
    Ut_P7,      //  0x40
};

//��λ���ŵ��������Ԫ�Ķ�ӦRAM��ַ�ռ�
//hjj add
#define     Ut_Reserved     (S39+COM8) //Ԥ��
#define     Ut_KW           (S09+COM4) //KW
#define     Ut_h            (S09+COM3) //h
#define     Ut_Imp          (S16+COM4) //imp/
#define     Ut_V            (S09+COM2) //V
#define     Ut_a            (S09+COM1) //A


#define     Len_UnitBit 5
const uint8   UnitBitTable[Len_UnitBit]=
{
    Ut_KW,      //  0x01
    Ut_h,       //  0x02
    Ut_Imp,     //  0x04
    Ut_V,       //  0x08
    Ut_a,       //  0x10
};

//������ʾ����16����λ��������ڲ���(0x00��0x0f)ȷ��
#define UnitTableLen 7
const uint16   UnitTable[UnitTableLen]=
{ //bit15--------------------------------------->bit0
    0x00,
    0x01+0x02,          //KWh
    0x01,               //KW
    //0x04 +0x01+0x02 ,   //imp/KWh
    0x08,               //V
    0x10,               //A
    0X10+0X08,    //VA
    0X10+0X08+0X02, //VAH
    
};


/*==============================================
˵����������ʾ����
================================================*/
//�����ַ��ĸ�����Ӧλ�εĶ��壬��Ӧ��RAM��ַ�ռ䣬����ʾ��������
//8��ʾ��ַ����ڸ�5λ������3λ����������ϵ�ֵ��ʾ�������ַ�ж�Ӧ��ŵ�λ
/*������ַ�����hjj add*/
#define     Cn_Dandqian         (S05+COM3) //��ǰ
#define     Cn_Shang            (S05+COM4) //��
#define     Cn_Shangyue         (S06+COM4) //����
#define     Cn_Xue              (S34+COM4) //��
#define     Cn_Dian             (S34+COM3) //��
#define     Cn_Liang            (S34+COM2) //��
#define     Cn_Ya               (S34+COM1) //ѹ
#define     Cn_Liu              (S35+COM1) //��
#define     Cn_Shijian          (S35+COM2) //ʱ��
#define     Cn_Juhao            (S35+COM3) // �ֺ�

#define     Ut_A                (S06+COM3) //A
#define     Ut_B                (S06+COM2) //B
#define     Ut_C                (S05+COM2) //C
#define     Cn_Reserved         (S39+COM8) //Ԥ��
/*hjj add over*/


#define Len_ChineseTable 14
//�����ַ�����ʾ��ֵ���еĵ�ַ�Ͷ�Ӧλ����5λ��ʾ��ַ����3λ��ʾ�ڶ�Ӧ�ڴ��ĳһλ
const   uint16   ChineseTable[Len_ChineseTable]=
{
    Cn_Dandqian,        //  0x01    ��ǰ
    Cn_Shang,           //  0x02    ��
    Cn_Shangyue,        //  0x04    ����
    Cn_Xue,             //  0x08    ��
    Cn_Dian,            //  0x10    ��
    Cn_Liang,           //  0x20    ��
    Cn_Ya,              //  0x40    ѹ
    Cn_Liu,             //  0x80    ��
    Cn_Reserved,        //  0x100   ����
    Cn_Shijian,         //  0x200   ʱ��
    Cn_Juhao,           //  0x400   �ֺ�
    Ut_A,               //  0x800A
    Ut_B,               //  0x1000B
    Ut_C,               //  0x2000C

};

//������ʾ 0x96�����ݣ�ͨ��һ���ֽڵ���ڲ�����ʵ��
#define Len_LCDChinaCode    0x24
const uint32   LCD_ChinaCode[Len_LCDChinaCode] =
{
    0x0000,                         //              00h
    0x01+0x10+0x20+0x800,           // ��ǰA����    01h
    0x01+0x10+0x20+0x1000,          // ��ǰB����    02h
    0x01+0x10+0x20+0x2000,          // ��ǰC����    03h
    0x01+0x10+0x20,                 // ��ǰ����     04h
    0x04+0x10+0x20+0x800,           // ����A����    05h
    0x04+0x10+0x20+0x1000,          // ����B����    06h
    0x04+0x10+0x20+0x2000,          // ����C����    07h
    0x04+0x10+0x20,                 // ���µ���     08h

    0x02+0x04+0x10+0x20+0x800,      // ������A����  09h
    0x02+0x04+0x10+0x20+0x1000,     // ������B����  0ah
    0x02+0x04+0x10+0x20+0x2000,     // ������C����  0bh
    0x02+0x04+0x10+0x20,            // �����µ���   0ch

    0x01+0x20+0x800+0x08,           // ��ǰA����    0dh
    0x01+0x20+0x1000+0x08,          // ��ǰB����    0eh
    0x01+0x20+0x2000+0x08,          // ��ǰC����    0fh
    0x01+0x20+0x08,                 // ��ǰ����     10h
       
    0x02+0x20+0x800+0x08,           // ����A����    11h
    0x02+0x20+0x1000+0x08,          // ����B����    12h
    0x02+0x20+0x2000+0x08,          // ����C����    13h
    0x02+0x20+0x08,                 // ��������     14h
                                     
    0x02+0x04+0x20+0x800+0x08,      // ����A����    15h
    0x02+0x04+0x20+0x1000+0x08,     // ����B����    16h
    0x02+0x04+0x20+0x2000+0x08,     // ����C����    17h
    0x02+0x04+0x20+0x08,            // ��������     18h       

    0x800+0x10+0x80,                // A����        19h
    0x1000+0x10+0x80,               // B����        1ah
    0x2000+0x10+0x80,               // C����        1bh
    0x800+0x10+0x40,                // A��ѹ        1ch
    0x1000+0x10+0x40,               // B��ѹ        1dh
    0x2000+0x10+0x40,               // C��ѹ        1eh
    0x200,                          // ʱ��         1fh
    0x400,                          // �ֺ�         20h
    0x800,                          // A            21h
    0x1000,                         // B            22h
    0x2000,                         // C            23h
};

#endif

#endif
