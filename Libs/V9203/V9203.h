
#ifndef V9203_H
#define V9203_H

#include "stm32f4xx_hal.h"

//****************************************DEV*******************************************************

#define V9203_SPI_TIMEOUT       500
#define V9203_TRY_COUNT         3      //1000 ms

//****************************************Parametrs*************************************************

#define V9203_FREQ_MES_RES      0.0008 //Hz

//****************************************Cmd*******************************************************

#define V9203_CMD_WRITE_LO_WORD    0x08
#define V9203_CMD_WRITE_HI_WORD    0x0A
#define V9203_CMD_WRITE_TO_REG     0x0C
#define V9203_CMD_READ_REG         0x10
#define V9203_CMD_READ_LO_WORD     0x12
#define V9203_CMD_READ_HI_WORD     0x14

//****************************************Registers*************************************************

#define Work_normal  0x11
//=============================================================
//Analog control register
//=============================================================
#define   RegANCtrl0   0x8000
#define   RegANCtrl1   0x8001
#define   RegANCtrl2   0x8002
#define   RegANCtrl3   0x8003


//=============================================================
//Metering configuration register
//=============================================================
#define   RegMTPARA0   0xC000
#define   RegMTPARA1   0xC001
#define   RegMTPARA2   0xC002
#define   RegMTPARA3   0xC003

#define   ZZCPSEL     0xEC15
#define   ZZPA0       0xEC23
#define   ZZPA1       0xEC24
#define   ZZQA0       0xEC47
#define   ZZQA1       0xEC48
#define   ZZAPPA      0xEC05
#define   ZZPCF0A     0XEC34


//=============================================================
//DC component register  
//=============================================================
#define   RegDCAU       0xF02C
#define   RegDCAI        0xF02D
#define   RegDCBU       0xF02E
#define   RegDCBI        0xF02F
#define   RegDCCU       0xF030
#define   RegDCCI        0xF031

//=============================================================
//Frequency phase register  
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
//Voltage / current instantaneous rms register  
//=============================================================
#define   RegARTIA     0xE83E
#define   RegARTIB    0xE83F
#define   RegARTIC     0xE840

#define   RegARTUA     0xE842
#define   RegARTUB    0xE843
#define   RegARTUC     0xE844


//=============================================================
//Active / reactive power register 
//=============================================================
#define   RegMAPSUM0      0xE8EE
#define   RegMAPSUM1      0xE8EF

#define   RegMAQSUM0      0xE8F3
#define   RegMAQSUM1      0xE8F4




//=============================================================
//Voltage / current rms correction register  
//=============================================================
#define   RegWARTIA     0xE968
#define   RegWARTIB     0xE969
#define   RegWARTIC     0xE96A

#define   RegWWARTIA    0xE994
#define   RegWWARTIB    0xE995
#define   RegWWARTIC    0xE996

#define   RegWARTUA     0xE96C
#define   RegWARTUB     0xE96D
#define   RegWARTUC     0xE96E

#define   RegWWARTUA     0xE998
#define   RegWWARTUB     0xE999
#define   RegWWARTUC     0xE99A

//=============================================================
//Full wave active / reactive power correction register 
//=============================================================
#define   RegWAPTA      0xE95A
#define   RegWAPTB      0xE95E
#define   RegWAPTC      0xE962

#define   RegWAQTA      0xE965
#define   RegWAQTB      0xE966
#define   RegWAQTC      0xE967

#define   RegWAEC0      0XE954       // Angle difference

//=============================================================
//Baseband active / reactive power correction register
//=============================================================
#define   RegWBPTA      0xE970   
#define   RegWBPTB      0xE971
#define   RegWBPTC      0xE972

#define   RegWBQTA      0xE973
#define   RegWBQTB      0xE974
#define   RegWBQTC      0xE975


//=============================================================
//Full wave active / reactive power secondary compensation correction register
//=============================================================
#define   RegWWAPTA     0xE98E
#define   RegWWAPTB     0xE98F
#define   RegWWAPTC     0xE990

#define   RegWWAQTA      0xE991
#define   RegWWAQTB      0xE992
#define   RegWWAQTC      0xE993


//=============================================================
//Threshold register  
//=============================================================
#define   RegZZEGYTHH     0xEC1E
#define   RegZZEGYTHL     0xEC1F

#define   RegCTHH       0xE8AA
#define   RegCTHL       0xE8AB


//=============================================================
//Checksum register 
//=============================================================
#define   RegCKSUM     0xC003
//=============================================================
//Current sense threshold register  
//=============================================================
#define  RegZZDCUM    0xEC1D


//=============================================================
//Full wave power factor register
//=============================================================
#define RegMAFA  0xE8E4
#define RegMAFB  0xE8E5
#define RegMAFC  0xE8E6
#define RegMAFS  0xE8E7

#define BRTUA 0xE883
#define BRTUB 0xE884
#define BRTUC 0xE885


#define APhy  0x00
#define BPhy  0x01
#define CPhy  0x02
#define ABCPhy 0x03


#define RMSUA        0xE944
#define RMSUB        0xE945
#define RMSUC        0xE946

#define RMSIA       0xE90E
#define RMSIB       0xE90F
#define RMSIC       0xE910
#define RMSIN       0xE911

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


//Full consaption
#define RegEGYAPPAH 0xE8AC
#define RegEGYAPPAL 0xE8AD
#define RegEGYAPPBH 0xE8AE
#define RegEGYAPPBL 0xE8AF
#define RegEGYAPPCH 0xE8B0
#define RegEGYAPPCL 0xE8B1
#define RegEGYAPPSH 0xE8B2
#define RegEGYAPPSL 0xE8B3
//Active consamtion
#define RegEGYPAH 0xE8D0
#define RegEGYPAL 0xE8D1
#define RegEGYPBH 0xE8D2
#define RegEGYPBL 0xE8D3
#define RegEGYPCH 0xE8D4
#define RegEGYPCL 0xE8D5
#define RegEGYPS0H 0xE8D6
#define RegEGYPS0L 0xE8D7
//Reactive consaption
#define RegEGYQAH 0xE8DA
#define RegEGYQAL 0xE8DB
#define RegEGYQBH 0xE8DC
#define RegEGYQBL 0xE8DD
#define RegEGYQCH 0xE8DE
#define RegEGYQCL 0xE8DF
#define RegEGYQS0H 0xE8E0
#define RegEGYQS0L 0xE8E1

//The following is for the compiler through
#define DATAP       0x0119
#define DATAQ       0x10d7
#define RMSU        0x011B
#define RMSI1       0x011C
#define RMSI2       0x0117

#define RAP         0x0102  // Full-wave active power original value
#define RAQ         0x0103  // Full-wave reactive power original value
#define ARRTU       0x0104  // Full-wave current rms original value
#define ARRTI       0x0105  // full wave voltage rms original value
// The above is for compile through

//****************************************Default values********************************************

//Default settings
#define V9203_DEF_CTHH        0x000221E5	// Top judgment threshold register
#define V9203_DEF_CTHL        0x0001EB4E	// Bottom judgment threshold register
#define V9203_DEF_WAEC0       0x00000000	// Angle difference 0
#define V9203_DEF_MTPARA0     0x000000ff	// Metering data reg 0
#define V9203_DEF_MTPARA1     0x00000000	// Metering data reg 1
#define V9203_DEF_MTPARA2     0x070080ff	// Metering data reg 2
#define V9203_DEF_ANCTRL0     0x00000333	// Analog control register 0
#define V9203_DEF_ANCTRL1     0x00000000	// Analog control register 1
#define V9203_DEF_ANCTRL2     0x77005400	// Analog control register 2
#define V9203_DEF_ANCTRL3     0x00000406	// Analog control register 3

//Default calibrate
//Total
#define V9203_DEF_CAL_WARTU   0x00000000	// gain voltage RMS
#define V9203_DEF_CAL_WARTI   0x21A8301B	// gain current RMS
#define V9203_DEF_CAL_WAPT    0x21E51894	// gain active power coef RMS
#define V9203_DEF_CAL_WAQT    0x00000000	// gain reactive power coef RMS
#define V9203_DEF_CAL_WWARTU  0x00000000	// offset voltage RMS
#define V9203_DEF_CAL_WWARTI  0x00000000	// offset current RMS
#define V9203_DEF_CAL_WWAPT   0x00000000	// offset active power
#define V9203_DEF_CAL_WWAQT   0x00000000	// offset reactive power
//Fundamental
#define V9203_DEF_CAL_WBRTU   0x00000000
#define V9203_DEF_CAL_WBRTI   0x00000000
#define V9203_DEF_CAL_WBPT    0x00000000
#define V9203_DEF_CAL_WBQT    0x00000000
#define V9203_DEF_CAL_WWBRTU  0x00000000
#define V9203_DEF_CAL_WWBRTI  0x00000000
#define V9203_DEF_CAL_WWBPT   0x00000000
#define V9203_DEF_CAL_WWBQT   0x00000000

//Proportional coeff
#define V9203_DEF_PROP_VOLTAGE	  0x00000000
#define V9203_DEF_PROP_CURRENT	  0x00000000
#define V9203_DEF_PROP_POWER	  0x00000000

//Threshold detect
#define V9203_DEF_THRD_CURRENT_DETECT	0x00000000
#define V9203_DEF_THRD_POWER_DETECT		0x00000000

//****************************************Calibration struct****************************************
//Calibtation total struct
typedef struct
{
  uint32_t  Cal_WARTU;
  uint32_t  Cal_WARTI;
  uint32_t  Cal_WAPT;
  uint32_t  Cal_WAQT;
  uint32_t  Cal_WWARTU;
  uint32_t  Cal_WWARTI;
  uint32_t  Cal_WWAPT;
  uint32_t  Cal_WWAQT;
}V9203_Total_cal_t;

//Calibtation Fundamental struct
typedef struct
{
  uint32_t  Cal_WBRTU;
  uint32_t  Cal_WBRTI;
  uint32_t  Cal_WBPT;
  uint32_t  Cal_WBQT;
  uint32_t  Cal_WWBRTU;
  uint32_t  Cal_WWBRTI;
  uint32_t  Cal_WWBPT;
  uint32_t  Cal_WWBQT;
}V9203_Fund_cal_t;

//****************************************Data structs**********************************************

//Line num
typedef enum
{
  LINE_A = 0,
  LINE_B,
  LINE_C,
  LINE_N,
  LINE_S
}V9203_line_t;

//Default settings struct
typedef struct 
{  
  uint32_t  CTHH;                    // Top judgment threshold register
  uint32_t  CTHL;                    // Bottom judgment threshold register
  uint32_t  WAEC0;                   // Angle difference 0
  uint32_t  MTPARA0;                 // Metering data reg 0
  uint32_t  MTPARA1;                 // Metering data reg 1
  uint32_t  MTPARA2;                 // Metering data reg 2
  uint32_t  ANCtrl0;                 // Analog control register 0
  uint32_t  ANCtrl1;                 // Analog control register 1
  uint32_t  ANCtrl2;                 // Analog control register 2
  uint32_t  ANCtrl3;                 // Analog control register 3
}V9203_defSet_t;

//Calibration channel coefficients
typedef struct
{
  //Default settings
  V9203_defSet_t V9203_defSet;
  
  //Calibtation total struct
  V9203_Total_cal_t calTotalPhaseA;
  V9203_Total_cal_t calTotalPhaseB;
  V9203_Total_cal_t calTotalPhaseC;
  
  //Calibtation Fundamental struct
  V9203_Fund_cal_t calFundPhaseA;
  V9203_Fund_cal_t calFundPhaseB;
  V9203_Fund_cal_t calFundPhaseC;
  
  //Proportion coefficents
  uint32_t calPropPower;                //Proportiona Power coeff
  uint32_t calPropVoltage;              //Proportiona U coeff
  uint32_t calPropCurrent;              //Proportiona I coeff
  
  //Threshold
  uint32_t cal_currThrdDetect;          //Threshold current detect
  uint32_t cal_energyThrdDetect;        //Threshold energy meter detect
  
}V9203_settings_t;

//***********************************************API************************************************
//Init
void V9203_init(SPI_HandleTypeDef *hspi);
//Init dev
HAL_StatusTypeDef V9203_initDev(uint8_t channel, V9203_settings_t *settings);
//Defaul reg val
void V9203_setDefaultReg(uint8_t channel, V9203_settings_t *settings);
//Get frequency
float V9203_getFreq(uint8_t channel, V9203_line_t line);
//Get RMS voltage
float V9203_getRMS_Voltage(uint8_t channel, V9203_line_t line);
//Get RMS current
float V9203_getRMS_Current(uint8_t channel, V9203_line_t line);
//Get power
float V9203_getRMS_Power(uint8_t channel, V9203_line_t line);
//Get reactive power
float V9203_getRMS_reactivePower(uint8_t channel, V9203_line_t line);
//Get S Consamption
uint64_t V9203_getSCons(uint8_t channel, V9203_line_t line);
//Get active Consamption
uint64_t V9203_getPCons(uint8_t channel, V9203_line_t line);
//Get reactive Consamption
uint64_t V9203_getQCons(uint8_t channel, V9203_line_t line);
//Get cos Fi
float V9203_getCOSfi(uint8_t channel, V9203_line_t line);

#endif