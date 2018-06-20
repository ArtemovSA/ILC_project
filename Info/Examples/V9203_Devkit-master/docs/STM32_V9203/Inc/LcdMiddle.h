#ifndef __LCDMIDDLE_H__
#define __LCDMIDDLE_H__


#ifndef LCDMIDDLEEXT
    #define LCDMIDDLEEXT extern
#else
    #define LCDMIDDLESELF
#endif

//应用层接口函数
void Disp_Numb(uint32 data,uint32 type);
void Disp_String(uint16 para);
void Disp_UnitPoint(uint16 para);
void Disp_State(uint32 data);

#ifdef LCDMIDDLESELF


//COM口和SEG口对应的RAM区间
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
说明：以下为数字显示部分的定义，有数字区域和代码区域
====================================================*/
//定义各个数字所在的RAM的字节位置
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
说明：以下为数字显示部分的定义，有数字区域和代码区域
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
#define LCD_CHARL1_o  (NUMD1+NUME1+NUMG1)       //小写字母'o',不是零
#define LCD_CHARH1_o  (NUMC1)                   //小写字母'o',不是零
#define LCD_CHARL1_C  (NUMD1+NUME1+NUMF1)       //'C'
#define LCD_CHARH1_C  (NUMA1)                   //'C'
#define LCD_CHARL1_E  (NUMG1+NUMD1+NUME1+NUMF1) //'E'
#define LCD_CHARH1_E  (NUMA1)                   //'E'






/*==================================================
说明：状态显示位
===================================================*/
//8表示地址存放于高5位（左移3位）,后面加上的值表示在这个地址中对应存放的位
//hjj add
#define     Ut_X1                   (S05+COM1) //左箭头功率反向
#define     Ut_X2                   (S36+COM1) //电话
#define     Ut_X3                   (S16+COM1) //编程键
#define     Ut_X4                   (S16+COM2) //锁
#define     Ut_X5                   (S16+COM3) //电池
#define     St_Reserved         (S39+COM8) //空管脚
#define     Ut_NIa                  (S06+COM1) //-(Ia)
#define     Ut_Ia                     (S13+COM1) //Ia
#define     Ut_NIb                  (S37+COM1) //-(Ib)
#define     Ut_Ib                    (S37+COM2) //Ib
#define     Ut_NIc                  (S37+COM3) //-(Ic)
#define     Ut_Ic                     (S37+COM4) //Ic
#define     Ut_Ua                   (S36+COM4) //Ua
#define     Ut_Ub                   (S36+COM3) //Ub
#define     Ut_Uc                   (S36+COM2) //Uc
#define     Cn_Nixiangxu      (S35+COM4) //逆相序


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
说明：小数点符号的显示
========================================================*/
//负号及小数点的各个位段的定义,P表示POINT
//8表示地址存放于高5位（左移3位），后面加上的值表示在这个地址中对应存放的位
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


//小数点（包括"-"）在显示真值表中的地址和对应位，高5位表示地址，低3位表示在对应内存的某一位
//最大长度是16，目前只用了7，其他空
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

//单位符号的最基本单元的对应RAM地址空间
//hjj add
#define     Ut_Reserved     (S39+COM8) //预留
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

//最多可显示如下16个单位，根据入口参数(0x00到0x0f)确定
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
说明：中文显示内容
================================================*/
//中文字符的各个对应位段的定义，对应的RAM地址空间，当显示屏调整后
//8表示地址存放于高5位（左移3位），后面加上的值表示在这个地址中对应存放的位
/*三相表字符定义hjj add*/
#define     Cn_Dandqian         (S05+COM3) //当前
#define     Cn_Shang            (S05+COM4) //上
#define     Cn_Shangyue         (S06+COM4) //上月
#define     Cn_Xue              (S34+COM4) //需
#define     Cn_Dian             (S34+COM3) //电
#define     Cn_Liang            (S34+COM2) //量
#define     Cn_Ya               (S34+COM1) //压
#define     Cn_Liu              (S35+COM1) //流
#define     Cn_Shijian          (S35+COM2) //时间
#define     Cn_Juhao            (S35+COM3) // 局号

#define     Ut_A                (S06+COM3) //A
#define     Ut_B                (S06+COM2) //B
#define     Ut_C                (S05+COM2) //C
#define     Cn_Reserved         (S39+COM8) //预留
/*hjj add over*/


#define Len_ChineseTable 14
//中文字符在显示真值表中的地址和对应位，高5位表示地址，低3位表示在对应内存的某一位
const   uint16   ChineseTable[Len_ChineseTable]=
{
    Cn_Dandqian,        //  0x01    当前
    Cn_Shang,           //  0x02    上
    Cn_Shangyue,        //  0x04    上月
    Cn_Xue,             //  0x08    需
    Cn_Dian,            //  0x10    电
    Cn_Liang,           //  0x20    量
    Cn_Ya,              //  0x40    压
    Cn_Liu,             //  0x80    流
    Cn_Reserved,        //  0x100   保留
    Cn_Shijian,         //  0x200   时间
    Cn_Juhao,           //  0x400   局号
    Ut_A,               //  0x800A
    Ut_B,               //  0x1000B
    Ut_C,               //  0x2000C

};

//最多可显示 0x96项内容，通过一个字节的入口参数来实现
#define Len_LCDChinaCode    0x24
const uint32   LCD_ChinaCode[Len_LCDChinaCode] =
{
    0x0000,                         //              00h
    0x01+0x10+0x20+0x800,           // 当前A电量    01h
    0x01+0x10+0x20+0x1000,          // 当前B电量    02h
    0x01+0x10+0x20+0x2000,          // 当前C电量    03h
    0x01+0x10+0x20,                 // 当前电量     04h
    0x04+0x10+0x20+0x800,           // 上月A电量    05h
    0x04+0x10+0x20+0x1000,          // 上月B电量    06h
    0x04+0x10+0x20+0x2000,          // 上月C电量    07h
    0x04+0x10+0x20,                 // 上月电量     08h

    0x02+0x04+0x10+0x20+0x800,      // 上上月A电量  09h
    0x02+0x04+0x10+0x20+0x1000,     // 上上月B电量  0ah
    0x02+0x04+0x10+0x20+0x2000,     // 上上月C电量  0bh
    0x02+0x04+0x10+0x20,            // 上上月电量   0ch

    0x01+0x20+0x800+0x08,           // 当前A需量    0dh
    0x01+0x20+0x1000+0x08,          // 当前B需量    0eh
    0x01+0x20+0x2000+0x08,          // 当前C需量    0fh
    0x01+0x20+0x08,                 // 当前需量     10h
       
    0x02+0x20+0x800+0x08,           // 上月A需量    11h
    0x02+0x20+0x1000+0x08,          // 上月B需量    12h
    0x02+0x20+0x2000+0x08,          // 上月C需量    13h
    0x02+0x20+0x08,                 // 上月需量     14h
                                     
    0x02+0x04+0x20+0x800+0x08,      // 上月A需量    15h
    0x02+0x04+0x20+0x1000+0x08,     // 上月B需量    16h
    0x02+0x04+0x20+0x2000+0x08,     // 上月C需量    17h
    0x02+0x04+0x20+0x08,            // 上月需量     18h       

    0x800+0x10+0x80,                // A电流        19h
    0x1000+0x10+0x80,               // B电流        1ah
    0x2000+0x10+0x80,               // C电流        1bh
    0x800+0x10+0x40,                // A电压        1ch
    0x1000+0x10+0x40,               // B电压        1dh
    0x2000+0x10+0x40,               // C电压        1eh
    0x200,                          // 时间         1fh
    0x400,                          // 局号         20h
    0x800,                          // A            21h
    0x1000,                         // B            22h
    0x2000,                         // C            23h
};

#endif

#endif
