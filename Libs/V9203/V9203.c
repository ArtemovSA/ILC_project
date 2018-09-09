
#include "V9203.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"
#include "String.h"
#include "DevCTRL.h"
#include "PCA9555.h"
#include "cmsis_os.h"
#include "math.h"

//Var
SPI_HandleTypeDef *V9203_hspi; //SPI pointer
V9203_defSet_t V9203_defSet; //Default settings


//Write flash
HAL_StatusTypeDef V9203_wr_flash(uint8_t channel, uint16_t addrReg, uint32_t data);
//Read flash
HAL_StatusTypeDef V9203_rd_flash(uint8_t channel, uint16_t addrReg, uint32_t* data);
//CRC16
static uint16_t V9203_crc16(const void *data, unsigned data_size);
//Setup registers
static void V9203_setupReg(uint8_t channel, V9203_settings_t *settings);

//--------------------------------------------------------------------------------------------------
//Init
void V9203_init(SPI_HandleTypeDef *hspi, uint8_t* channelEn, uint8_t* activeCh)
{
  V9203_hspi = hspi;  
  
  for (int i=0; i<V9203_COUNT_CHANNELS; i++)
  {
    if (*channelEn & (1<<i))
      if (V9203_initDev(i, &DC_set.V9203_ch_set[i]) == HAL_OK) //Init dev
        *activeCh |= (1<<i);
  }
  
}
//--------------------------------------------------------------------------------------------------
//Init dev
HAL_StatusTypeDef V9203_initDev(uint8_t channel, V9203_settings_t *settings)
{
  uint8_t try_count = V9203_TRY_COUNT;
  uint32_t readyFlag = 0;
  
  while (--try_count)
  {
    V9203_wr_flash(channel, RegMTPARA0, 0x100000ff);
    V9203_rd_flash(channel, RegMTPARA0, &readyFlag);
    
    if (readyFlag == 0x100000ff)
      break;
    
    vTaskDelay(1000);
  }
  
  if (try_count == 0x0)
  {
    DC_debugOut("# V9203 INIT CH:%d  ERROR\r\n", channel);
    return HAL_ERROR;
  }
  
  //Clear
  for(unsigned char i=0;i<56;i++)
  {
    V9203_wr_flash(channel, (0xC800+i), 0);
  } 
  
  V9203_setupReg(channel, settings); //Setup registers
    
  DC_debugOut("# V9203 INIT OK CH:%d\r\n", channel);
  
  return HAL_OK;
}
//--------------------------------------------------------------------------------------------------
//Setup registers
static void V9203_setupReg(uint8_t channel, V9203_settings_t *settings)
{
  uint32_t checkSum = 0;
  
  // clear ram
  V9203_wr_flash(channel, RegMTPARA0, 0xAA000000);
  vTaskDelay(20);
  
  //**********************************Metering data reg*********************************************
  // Metering data reg
  V9203_wr_flash(channel, RegMTPARA0, settings->V9203_defSet.MTPARA0); //Enable digital input
  checkSum += settings->V9203_defSet.MTPARA0;
  V9203_wr_flash(channel, RegMTPARA1, settings->V9203_defSet.MTPARA1);
  checkSum += settings->V9203_defSet.MTPARA1;
  V9203_wr_flash(channel, RegMTPARA2, settings->V9203_defSet.MTPARA2); //MT_PARA2 - Unsegmented,distribute 32bit separately twice,DMA enable UA,non-did,AD all-enable
  checkSum += settings->V9203_defSet.MTPARA2;
  
  //**********************************Analog control register***************************************
  V9203_wr_flash(channel, RegANCtrl0, settings->V9203_defSet.ANCtrl0);
  checkSum += settings->V9203_defSet.ANCtrl0;
  V9203_wr_flash(channel, RegANCtrl1, settings->V9203_defSet.ANCtrl1);
  checkSum += settings->V9203_defSet.ANCtrl1;
  V9203_wr_flash(channel, RegANCtrl2, settings->V9203_defSet.ANCtrl2);
  checkSum += settings->V9203_defSet.ANCtrl2;
  V9203_wr_flash(channel, RegANCtrl3, settings->V9203_defSet.ANCtrl3);
  checkSum += settings->V9203_defSet.ANCtrl3;
  
  //**********************************Threshold*****************************************************
  // Low threshold
  V9203_wr_flash(channel, RegZZEGYTHL, settings->cal_energyThrdDetect);
  checkSum += settings->cal_energyThrdDetect;
  // High threshold
  V9203_wr_flash(channel, RegZZEGYTHH, 0x00000000);
  checkSum += 0x00000000;
  // enable threshold limit
  V9203_wr_flash(channel, RegCTHH, settings->V9203_defSet.CTHH);
  checkSum += settings->V9203_defSet.CTHH;
  // enable threshold lower limit
  V9203_wr_flash(channel, RegCTHL, settings->V9203_defSet.CTHL);
  checkSum += settings->V9203_defSet.CTHL;
  //Current threshold
  V9203_wr_flash(channel, RegZZDCUM, settings->cal_currThrdDetect);
  checkSum += settings->cal_currThrdDetect; 

  //**********************************Calibration***************************************************
  // Phase A Total
  // gain voltage RMS
  V9203_wr_flash(channel, RegWARTUA, settings->calTotalPhaseA.Cal_WARTU);
  checkSum += settings->calTotalPhaseA.Cal_WARTU;
  // gain current RMS
  V9203_wr_flash(channel, RegWARTIA, settings->calTotalPhaseA.Cal_WARTI);
  checkSum += settings->calTotalPhaseA.Cal_WARTI;
  // gain active power coef RMS
  V9203_wr_flash(channel, RegWAPTA, settings->calTotalPhaseA.Cal_WAPT);
  checkSum += settings->calTotalPhaseA.Cal_WAPT;
  // gain reactive power coef RMS
  V9203_wr_flash(channel, RegWAQTA, settings->calTotalPhaseA.Cal_WAQT);
  checkSum += settings->calTotalPhaseA.Cal_WAQT;
  // offset voltage RMS
  V9203_wr_flash(channel, RegWWARTUA, settings->calTotalPhaseA.Cal_WWARTU);
  checkSum += settings->calTotalPhaseA.Cal_WARTU;
  // offset current RMS
  V9203_wr_flash(channel, RegWWARTIA, settings->calTotalPhaseA.Cal_WWARTI);
  checkSum += settings->calTotalPhaseA.Cal_WARTI;
  // offset active power
  V9203_wr_flash(channel, RegWWAPTA, settings->calTotalPhaseA.Cal_WWAPT);
  checkSum += settings->calTotalPhaseA.Cal_WAPT;
  // offset reactive power
  V9203_wr_flash(channel, RegWWAQTA, settings->calTotalPhaseA.Cal_WWAQT);
  checkSum += settings->calTotalPhaseA.Cal_WAQT;
  
  // Phase B Total
  // gain voltage RMS
  V9203_wr_flash(channel, RegWARTUB, settings->calTotalPhaseB.Cal_WARTU);
  checkSum += settings->calTotalPhaseB.Cal_WARTU;
  // gain current RMS
  V9203_wr_flash(channel, RegWARTIB, settings->calTotalPhaseB.Cal_WARTI);
  checkSum += settings->calTotalPhaseB.Cal_WARTI;
  // gain active power coef RMS
  V9203_wr_flash(channel, RegWAPTB, settings->calTotalPhaseB.Cal_WAPT);
  checkSum += settings->calTotalPhaseB.Cal_WAPT;
  // gain reactive power coef RMS
  V9203_wr_flash(channel, RegWAQTB, settings->calTotalPhaseB.Cal_WAQT);
  checkSum += settings->calTotalPhaseB.Cal_WAQT;
  // offset voltage RMS
  V9203_wr_flash(channel, RegWWARTUB, settings->calTotalPhaseB.Cal_WWARTU);
  checkSum += settings->calTotalPhaseB.Cal_WARTU;
  // offset current RMS
  V9203_wr_flash(channel, RegWWARTIB, settings->calTotalPhaseB.Cal_WWARTI);
  checkSum += settings->calTotalPhaseB.Cal_WARTI;
  // offset active power
  V9203_wr_flash(channel, RegWWAPTB, settings->calTotalPhaseB.Cal_WWAPT);
  checkSum += settings->calTotalPhaseB.Cal_WAPT;
  // offset reactive power
  V9203_wr_flash(channel, RegWWAQTB, settings->calTotalPhaseB.Cal_WWAQT);
  checkSum += settings->calTotalPhaseB.Cal_WAQT;
  
  // Phase C Total
  // gain voltage RMS
  V9203_wr_flash(channel, RegWARTUC, settings->calTotalPhaseC.Cal_WARTU);
  checkSum += settings->calTotalPhaseC.Cal_WARTU;
  // gain current RMS
  V9203_wr_flash(channel, RegWARTIC, settings->calTotalPhaseC.Cal_WARTI);
  checkSum += settings->calTotalPhaseC.Cal_WARTI;
  // gain active power coef RMS
  V9203_wr_flash(channel, RegWAPTC, settings->calTotalPhaseC.Cal_WAPT);
  checkSum += settings->calTotalPhaseC.Cal_WAPT;
  // gain reactive power coef RMS
  V9203_wr_flash(channel, RegWAQTC, settings->calTotalPhaseC.Cal_WAQT);
  checkSum += settings->calTotalPhaseC.Cal_WAQT;
  // offset voltage RMS
  V9203_wr_flash(channel, RegWWARTUC, settings->calTotalPhaseC.Cal_WWARTU);
  checkSum += settings->calTotalPhaseC.Cal_WARTU;
  // offset current RMS
  V9203_wr_flash(channel, RegWWARTIC, settings->calTotalPhaseC.Cal_WWARTI);
  checkSum += settings->calTotalPhaseC.Cal_WARTI;
  // offset active power
  V9203_wr_flash(channel, RegWWAPTC, settings->calTotalPhaseC.Cal_WWAPT);
  checkSum += settings->calTotalPhaseC.Cal_WAPT;
  // offset reactive power
  V9203_wr_flash(channel, RegWWAQTC, settings->calTotalPhaseC.Cal_WWAQT);
  checkSum += settings->calTotalPhaseC.Cal_WAQT;
  
 
  //**********************************Others********************************************************
  
  // Full-wave / fundamental combined phase power combination 	 A+B+C 
  V9203_wr_flash(channel, ZZAPPA, 0x00000007);
  checkSum += 0x00000007;
  
  // Start the creep mode selection according to the current rms value
  V9203_wr_flash(channel, ZZCPSEL, 0x00000001);
  checkSum += 0x00000001;
  
  // Angle difference
  V9203_wr_flash(channel, RegWAEC0, settings->V9203_defSet.WAEC0);
  checkSum += settings->V9203_defSet.WAEC0;
  
  // Active and phase 0 configuration
  V9203_wr_flash(channel, ZZPA0, 0x00000015);
  checkSum += 0x00000015;

  // Active and phase 1 configuration
  V9203_wr_flash(channel, ZZPA1, 0x0000002A);
  checkSum += 0x0000002A;
  
  // Reactive and phase 0 configuration
  V9203_wr_flash(channel, ZZQA0, 0x00000015);
  checkSum += 0x00000015;

  // Reactive and phase 1 configuration
  V9203_wr_flash(channel, ZZQA1, 0x0000002A);
  checkSum += 0x0000002A;
  
  // Visual and phase configuration
  V9203_wr_flash(channel, ZZAPPA, 0x00000007);
  checkSum += 0x00000007;
  
  // High-speed CF source selection
  V9203_wr_flash(channel, ZZPCF0A, 0x00002211);
  checkSum += 0x00002211;  
  
  // interrupted 0
  V9203_wr_flash(channel, 0xA000, 0x00008000);
  checkSum += 0x00008000;  
  
  // interrupted 1
  V9203_wr_flash(channel, 0xA001, 0x00008000);
  checkSum += 0x00008000;  
  
  checkSum += 0xff000000;	  //ANA2
  checkSum += 0x00000005;	  //ANA3
  checkSum = 0xffffffff - checkSum;
  
  V9203_wr_flash(channel, RegCKSUM, checkSum); //mt_para3 Self-test and  SUM+x=0xffffffff
  vTaskDelay(10);
  V9203_wr_flash(channel, 0xa002, 0); // Interrupt flag is cleared to 0
}
//--------------------------------------------------------------------------------------------------
//Defaul reg val
void V9203_setDefaultReg(uint8_t channel, V9203_settings_t *settings)
{    
  //Default settings
  settings->V9203_defSet.CTHH = V9203_DEF_CTHH; // Top judgment threshold register
  settings->V9203_defSet.CTHL = V9203_DEF_CTHL; // Bottom judgment threshold register
  settings->V9203_defSet.WAEC0 = V9203_DEF_WAEC0; // Angle difference 0
  settings->V9203_defSet.MTPARA0 = V9203_DEF_MTPARA0; // Metering data reg 0
  settings->V9203_defSet.MTPARA1 = V9203_DEF_MTPARA1; // Metering data reg 1
  settings->V9203_defSet.MTPARA2 = V9203_DEF_MTPARA2; // Metering data reg 2
  settings->V9203_defSet.ANCtrl0 = V9203_DEF_ANCTRL0; //Analog control register 0
  settings->V9203_defSet.ANCtrl1 = V9203_DEF_ANCTRL1; //Analog control register 1
  settings->V9203_defSet.ANCtrl2 = V9203_DEF_ANCTRL2; //Analog control register 2
  settings->V9203_defSet.ANCtrl3 = V9203_DEF_ANCTRL3; //Analog control register 3
  
  //Total calibrate
  //Gaint
  settings->calTotalPhaseA.Cal_WARTU = V9203_DEF_CAL_WARTU;
  settings->calTotalPhaseA.Cal_WARTI = V9203_DEF_CAL_WARTI;
  settings->calTotalPhaseA.Cal_WAPT = V9203_DEF_CAL_WAPT;
  settings->calTotalPhaseA.Cal_WAQT = V9203_DEF_CAL_WAQT;
  //Offset
  settings->calTotalPhaseA.Cal_WWARTU = V9203_DEF_CAL_WWARTU;
  settings->calTotalPhaseA.Cal_WWARTI = V9203_DEF_CAL_WWARTI;
  settings->calTotalPhaseA.Cal_WWAPT = V9203_DEF_CAL_WWAPT;
  settings->calTotalPhaseA.Cal_WWAQT = V9203_DEF_CAL_WWAQT;
  
  settings->calTotalPhaseB = settings->calTotalPhaseA;
  settings->calTotalPhaseC = settings->calTotalPhaseA;
  
  //Findamental calibrate
  //Gaint
  settings->calFundPhaseA.Cal_WBRTU = V9203_DEF_CAL_WBRTU;
  settings->calFundPhaseA.Cal_WBRTI = V9203_DEF_CAL_WBRTI;
  settings->calFundPhaseA.Cal_WBPT = V9203_DEF_CAL_WBPT;
  settings->calFundPhaseA.Cal_WBQT = V9203_DEF_CAL_WBQT;
  //Offset
  settings->calFundPhaseA.Cal_WWBRTU = V9203_DEF_CAL_WWBRTU;
  settings->calFundPhaseA.Cal_WWBRTI = V9203_DEF_CAL_WWBRTI;
  settings->calFundPhaseA.Cal_WWBPT = V9203_DEF_CAL_WWBPT;
  settings->calFundPhaseA.Cal_WWBQT = V9203_DEF_CAL_WWBQT;
  
  settings->calFundPhaseB = settings->calFundPhaseA;
  settings->calFundPhaseC = settings->calFundPhaseA;
  
  //Proportion coeff
  settings->calPropVoltage = V9203_DEF_PROP_VOLTAGE;
  settings->calPropCurrent = V9203_DEF_PROP_CURRENT;
  settings->calPropPower = V9203_DEF_PROP_POWER;
  
  //Threshold
  settings->cal_currThrdDetect = V9203_DEF_THRD_CURRENT_DETECT;
  settings->cal_energyThrdDetect = V9203_DEF_THRD_POWER_DETECT;
}
//--------------------------------------------------------------------------------------------------
//Set chip select
HAL_StatusTypeDef V9203_set_CS(uint8_t channel, uint8_t state)
{
  uint8_t pin;

  HAL_GPIO_WritePin(PW_CS_GPIO_Port, PW_CS_Pin, GPIO_PIN_SET); //CS SPI
  
  PCA9555_digitalWrite(PCA9555_DEF_ADDR, PCA9555_PIN_CS1, HIGH_LEVEL);
  PCA9555_digitalWrite(PCA9555_DEF_ADDR, PCA9555_PIN_CS2, HIGH_LEVEL);
  PCA9555_digitalWrite(PCA9555_DEF_ADDR, PCA9555_PIN_CS3, HIGH_LEVEL);
  PCA9555_digitalWrite(PCA9555_DEF_ADDR, PCA9555_PIN_CS4, HIGH_LEVEL);
  
  if (state == LOW_LEVEL)
  {
    switch(channel)
    {
    case 0: pin = PCA9555_PIN_CS1; break;
    case 1: pin = PCA9555_PIN_CS2; break;
    case 2: pin = PCA9555_PIN_CS3; break;
    case 3: pin = PCA9555_PIN_CS4; break;
    default: DC_debugOut("# V9203 CS channel num ERROR\r\n"); return HAL_ERROR;
    };
    
    PCA9555_digitalWrite(PCA9555_DEF_ADDR, pin, LOW_LEVEL);
    HAL_GPIO_WritePin(PW_CS_GPIO_Port, PW_CS_Pin, GPIO_PIN_RESET); //CS SPI
  }
  
  return HAL_OK;
}
//--------------------------------------------------------------------------------------------------
//Set data|cmd
HAL_StatusTypeDef V9203_data_cmd_flash(uint8_t channel, uint8_t cmd, uint16_t dataTx, uint16_t *dataRx)
{
  HAL_StatusTypeDef state;
  uint8_t txBuf[4];
  uint8_t rxBuf[4];

  txBuf[0] = (0x3f & cmd) | 0x80;
  txBuf[1] = dataTx >> 8;
  txBuf[2] = dataTx & 0x00ff;
  txBuf[3] = ~((dataTx & 0x00ff) + (dataTx >> 8) + txBuf[0]);
  
  vTaskDelay(50);
  if ((state = V9203_set_CS(channel, LOW_LEVEL)) != HAL_OK)
    return state;
  
  if ((state = HAL_SPI_TransmitReceive(V9203_hspi, txBuf, rxBuf, 4, V9203_SPI_TIMEOUT)) != HAL_OK)
    return state;

  if ((state = V9203_set_CS(channel, HIGH_LEVEL)) != HAL_OK)
    return state;
  
  if (dataRx != NULL)
    *dataRx = ADD(rxBuf[1], rxBuf[2]);
  
  return state;
}
//--------------------------------------------------------------------------------------------------
//Write flash
HAL_StatusTypeDef V9203_wr_flash(uint8_t channel, uint16_t addrReg, uint32_t data)
{
  HAL_StatusTypeDef state;
  
  if ((state = V9203_data_cmd_flash(channel, V9203_CMD_WRITE_LO_WORD, data & 0x0000FFFF, NULL)) != HAL_OK)
    return state;
  
  if ((state = V9203_data_cmd_flash(channel, V9203_CMD_WRITE_HI_WORD, data >> 16, NULL)) != HAL_OK)
    return state;
  
  if ((state = V9203_data_cmd_flash(channel, V9203_CMD_WRITE_TO_REG, addrReg, NULL)) != HAL_OK)
    return state;
  
  return state;
}
//--------------------------------------------------------------------------------------------------
//Read flash
HAL_StatusTypeDef V9203_rd_flash(uint8_t channel, uint16_t addrReg, uint32_t* data)
{
  HAL_StatusTypeDef state;
  uint16_t word_lo, word_hi;
  
  if ((state = V9203_data_cmd_flash(channel, V9203_CMD_READ_REG, addrReg, NULL)) != HAL_OK)
    return state;
  
  if ((state = V9203_data_cmd_flash(channel, V9203_CMD_READ_LO_WORD, addrReg, &word_lo)) != HAL_OK)
    return state;
  
  if ((state = V9203_data_cmd_flash(channel, V9203_CMD_READ_HI_WORD, addrReg, &word_hi)) != HAL_OK)
    return state;
  
  *data = ADD16(word_hi, word_lo);
  
  return state;
}
//----------------------------------------------------------------------------------
//CRC16
const uint16_t V9203_crc16_table[256]={ // X16+X12+X5+1
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
  0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
  0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
  0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
  0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
  0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
  0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
  0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
  0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
  0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
  0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
  0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
  0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
  0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
  0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
  0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
  0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
  0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
  0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
  0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
  0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
  0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
  0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
  0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
  0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
  0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
  0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
  0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
  0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

static uint16_t V9203_crc16(const void *data, unsigned data_size)
{
  if (!data || !data_size)
    return 0;
  unsigned short crc = 0;
  unsigned char* buf = (unsigned char*)data;
  while (data_size--)
    crc = (crc >> 8) ^ V9203_crc16_table[(unsigned char)crc ^ *buf++];
  return crc;
}
//***********************************************API************************************************
//Get frequency
float V9203_getFreq(uint8_t channel, V9203_line_t line)
{
  uint16_t regAddr;
  uint32_t regData;
  
  //Check channel
  if (channel > V9203_COUNT_CHANNELS)
  {
    DC_debugOut("# Channel num ERROR\r\n");
    return -1;
  }
  
  //Get register address
  switch(line)
  {
  case LINE_A: regAddr = DATAFREQA; break;
  case LINE_B: regAddr = DATAFREQB; break;
  case LINE_C: regAddr = DATAFREQC; break;
  default: DC_debugOut("# Line num ERROR\r\n"); return -1;
  }
  
  if (V9203_rd_flash(channel, regAddr, &regData) != HAL_OK)
  {
    DC_debugOut("# Frequncy read ERROR\r\n");
    return -1;
  }
  
  //Check return data
  if (regData >= 0xFFFF)
    return -1;
  
  return regData*V9203_FREQ_MES_RES;
}   
//----------------------------------------------------------------------------------
//Get RMS voltage
float V9203_getRMS_Voltage(uint8_t channel, V9203_line_t line)
{
  uint16_t regAddr;
  uint32_t regData;
  
  //Check channel
  if (channel > V9203_COUNT_CHANNELS)
  {
    DC_debugOut("# Channel num ERROR\r\n");
    return -1;
  }
  
  //Get register address
  switch(line)
  {
  case LINE_A: regAddr = RMSUA; break;
  case LINE_B: regAddr = RMSUB; break;
  case LINE_C: regAddr = RMSUC; break;
  default: DC_debugOut("# Line num ERROR\r\n"); return -1;
  }
  
  if (V9203_rd_flash(channel, regAddr, &regData) != HAL_OK)
  {
    DC_debugOut("# Voltage read ERROR\r\n");
    return -1;
  }
  
  //Check return data
  if (regData == 0xFFFFFFFF)
    return -1;
  
  return (float)regData/V9203_DEF_PROP_VOLTAGE;
}
//----------------------------------------------------------------------------------
//Get RMS current
float V9203_getRMS_Current(uint8_t channel, V9203_line_t line)
{
  uint16_t regAddr;
  uint32_t regData;
  
  //Check channel
  if (channel > V9203_COUNT_CHANNELS)
  {
    DC_debugOut("# Channel num ERROR\r\n");
    return -1;
  }
  
  //Get register address
  switch(line)
  {
  case LINE_A: regAddr = RMSIA; break;
  case LINE_B: regAddr = RMSIB; break;
  case LINE_C: regAddr = RMSIC; break;
  case LINE_N: regAddr = RMSIN; break;
  default: DC_debugOut("# Line num ERROR\r\n"); return -1;
  }
  
  if (V9203_rd_flash(channel, regAddr, &regData) != HAL_OK)
  {
    DC_debugOut("# Current read ERROR\r\n");
    return -1;
  }
  
  //Check return data
  if (regData == 0xFFFFFFFF)
    return -1;
  
  return (float)regData/V9203_DEF_PROP_CURRENT;
}
//----------------------------------------------------------------------------------
//Get power
float V9203_getRMS_Power(uint8_t channel, V9203_line_t line)
{
  uint16_t regAddr;
  uint32_t regData;
  
  //Check channel
  if (channel > V9203_COUNT_CHANNELS)
  {
    DC_debugOut("# Channel num ERROR\r\n");
    return -1;
  }
  
  //Get register address
  switch(line)
  {
  case LINE_A: regAddr = DATAPA; break;
  case LINE_B: regAddr = DATAPB; break;
  case LINE_C: regAddr = DATAPC; break;
  default: DC_debugOut("# Line num ERROR\r\n"); return -1;
  }
  
  if (V9203_rd_flash(channel, regAddr, &regData) != HAL_OK)
  {
    DC_debugOut("# Active power ERROR\r\n");
    return -1;
  }
  
  //Check return data
  if (regData == 0xFFFFFFFF)
    return -1;
  
  return (float)regData/V9203_DEF_PROP_RPOWER;
}
//----------------------------------------------------------------------------------
//Get reactive power
float V9203_getRMS_reactivePower(uint8_t channel, V9203_line_t line)
{
  uint16_t regAddr;
  uint32_t regData;
  
  //Check channel
  if (channel > V9203_COUNT_CHANNELS)
  {
    DC_debugOut("# Channel num ERROR\r\n");
    return -1;
  }
  
  //Get register address
  switch(line)
  {
  case LINE_A: regAddr = DATAQA; break;
  case LINE_B: regAddr = DATAQB; break;
  case LINE_C: regAddr = DATAQC; break;
  default: DC_debugOut("# Line num ERROR\r\n"); return -1;
  }
  
  if (V9203_rd_flash(channel, regAddr, &regData) != HAL_OK)
  {
    DC_debugOut("# Reactive power ERROR\r\n");
    return -1;
  }
  
  //Check return data
  if (regData == 0xFFFFFFFF)
    return -1;
  
  return (float)regData/V9203_DEF_PROP_POWER;
}
//----------------------------------------------------------------------------------
//Get S Consamption
uint64_t V9203_getSCons(uint8_t channel, V9203_line_t line)
{
  uint64_t value = 0;
  uint16_t regAddrHI, regAddrLO;
  uint32_t regDataHI, regDataLO;
  
  //Check channel
  if (channel > V9203_COUNT_CHANNELS)
  {
    DC_debugOut("# Channel num ERROR\r\n");
    return 0;
  }
  
  //Get register address
  switch(line)
  {
  case LINE_A: regAddrHI = RegEGYAPPAH; regAddrLO = RegEGYAPPAL; break;
  case LINE_B: regAddrHI = RegEGYAPPBH; regAddrLO = RegEGYAPPBL; break;
  case LINE_C: regAddrHI = RegEGYAPPCH; regAddrLO = RegEGYAPPCL; break;
  case LINE_S: regAddrHI = RegEGYAPPSH; regAddrLO = RegEGYAPPSL;  break;
  default: DC_debugOut("# Line num ERROR\r\n"); return -1;
  }

  if (V9203_rd_flash(channel, regAddrHI, &regDataHI) != HAL_OK)
  {
    DC_debugOut("# Full cons ERROR\r\n");
    return 0;
  }

  if (V9203_rd_flash(channel, regAddrLO, &regDataLO) != HAL_OK)
  {
    DC_debugOut("# Full cons ERROR\r\n");
    return 0;
  }
  
  value = (uint64_t)(regDataHI << 31)|regDataLO;
  
  //Check return data
  if (value >= 0xFFFFFFFF)
    return 0;
  
  
  return value;
}
//----------------------------------------------------------------------------------
//Get active Consamption
uint64_t V9203_getPCons(uint8_t channel, V9203_line_t line)
{
  uint64_t value = 0;
  uint16_t regAddrHI, regAddrLO;
  uint32_t regDataHI, regDataLO;
  
  //Check channel
  if (channel > V9203_COUNT_CHANNELS)
  {
    DC_debugOut("# Channel num ERROR\r\n");
    return 0;
  }
  
  //Get register address
  switch(line)
  {
  case LINE_A: regAddrHI = RegEGYPAH; regAddrLO = RegEGYPAL; break;
  case LINE_B: regAddrHI = RegEGYPBH; regAddrLO = RegEGYPBL; break;
  case LINE_C: regAddrHI = RegEGYPCH; regAddrLO = RegEGYPCL; break;
  case LINE_S: regAddrHI = RegEGYPS0H; regAddrLO = RegEGYPS0L;  break;
  default: DC_debugOut("# Line num ERROR\r\n"); return -1;
  }

  if (V9203_rd_flash(channel, regAddrHI, &regDataHI) != HAL_OK)
  {
    DC_debugOut("# Active cons ERROR\r\n");
    return 0;
  }

  if (V9203_rd_flash(channel, regAddrLO, &regDataLO) != HAL_OK)
  {
    DC_debugOut("# Active cons ERROR\r\n");
    return 0;
  }
  
  value = (uint64_t)(regDataHI << 31)|regDataLO;
  
  //Check return data
  if (value >= 0xFFFFFFFF)
    return 0;
  
  return value;
}
//----------------------------------------------------------------------------------
//Get reactive Consamption
uint64_t V9203_getQCons(uint8_t channel, V9203_line_t line)
{
  uint64_t value = 0;
  uint16_t regAddrHI, regAddrLO;
  uint32_t regDataHI, regDataLO;
  
  //Check channel
  if (channel > V9203_COUNT_CHANNELS)
  {
    DC_debugOut("# Channel num ERROR\r\n");
    return 0;
  }
  
  //Get register address
  switch(line)
  {
  case LINE_A: regAddrHI = RegEGYQAH; regAddrLO = RegEGYQAL; break;
  case LINE_B: regAddrHI = RegEGYQBH; regAddrLO = RegEGYQBL; break;
  case LINE_C: regAddrHI = RegEGYQCH; regAddrLO = RegEGYQCL; break;
  case LINE_S: regAddrHI = RegEGYQS0H; regAddrLO = RegEGYQS0L;  break;
  default: DC_debugOut("# Line num ERROR\r\n"); return -1;
  }

  if (V9203_rd_flash(channel, regAddrHI, &regDataHI) != HAL_OK)
  {
    DC_debugOut("# Reactive cons ERROR\r\n");
    return 0;
  }

  if (V9203_rd_flash(channel, regAddrLO, &regDataLO) != HAL_OK)
  {
    DC_debugOut("# Reactive cons ERROR\r\n");
    return 0;
  }
  
  value = (uint64_t)(regDataHI << 31)|regDataLO;
  
  //Check return data
  if (value >= 0xFFFFFFFF)
    return 0;
  
  return value;
}
//----------------------------------------------------------------------------------
//Get cos Fi
float V9203_getCOSfi(uint8_t channel, V9203_line_t line)
{
  uint16_t regAddr;
  uint32_t regData;
  
  //Check channel
  if (channel > V9203_COUNT_CHANNELS)
  {
    DC_debugOut("# Channel num ERROR\r\n");
    return -1;
  }
  
  //Get register address
  switch(line)
  {
  case LINE_A: regAddr = RegMAFA; break;
  case LINE_B: regAddr = RegMAFB; break;
  case LINE_C: regAddr = RegMAFC; break;
  case LINE_S: regAddr = RegMAFS; break;
  default: DC_debugOut("# Line num ERROR\r\n"); return -1;
  }
  
  if (V9203_rd_flash(channel, regAddr, &regData) != HAL_OK)
  {
    DC_debugOut("# Reactive power ERROR\r\n");
    return -1;
  }
  
  //Check return data
  if (regData >= 0x9FFFFFFF)
    return -1;
  
  return (float)regData/V9203_DEF_PROP_COSFI;
}
//----------------------------------------------------------------------------------
//Clear P consamption
HAL_StatusTypeDef V9203_clearPCons(uint8_t channel, V9203_line_t line)
{
  uint16_t regAddrHI, regAddrLO;
  
  //Check channel
  if (channel > V9203_COUNT_CHANNELS)
  {
    DC_debugOut("# Channel num ERROR\r\n");
    return HAL_ERROR;
  }
  
  //Get register address
  switch(line)
  {
  case LINE_A: regAddrHI = RegEGYPAH; regAddrLO = RegEGYPAL; break;
  case LINE_B: regAddrHI = RegEGYPBH; regAddrLO = RegEGYPBL; break;
  case LINE_C: regAddrHI = RegEGYPCH; regAddrLO = RegEGYPCL; break;
  case LINE_S: regAddrHI = RegEGYPS0H; regAddrLO = RegEGYPS0L;  break;
  default: DC_debugOut("# Line num ERROR\r\n"); return HAL_ERROR;
  }

  if (V9203_wr_flash(channel, regAddrHI, 0) != HAL_OK)
  {
    DC_debugOut("# Active cons clear ERROR\r\n");
    return HAL_ERROR;
  }

  if (V9203_wr_flash(channel, regAddrLO, 0) != HAL_OK)
  {
    DC_debugOut("# Active cons clear ERROR\r\n");
    return HAL_ERROR;
  }

  return HAL_OK;
}
//----------------------------------------------------------------------------------
//Clear Q consamption
HAL_StatusTypeDef V9203_clearQCons(uint8_t channel, V9203_line_t line)
{
  uint16_t regAddrHI, regAddrLO;
  
  //Check channel
  if (channel > V9203_COUNT_CHANNELS)
  {
    DC_debugOut("# Channel num ERROR\r\n");
    return HAL_ERROR;
  }
  
  //Get register address
  switch(line)
  {
  case LINE_A: regAddrHI = RegEGYQAH; regAddrLO = RegEGYQAL; break;
  case LINE_B: regAddrHI = RegEGYQBH; regAddrLO = RegEGYQBL; break;
  case LINE_C: regAddrHI = RegEGYQCH; regAddrLO = RegEGYQCL; break;
  case LINE_S: regAddrHI = RegEGYQS0H; regAddrLO = RegEGYQS0L;  break;
  default: DC_debugOut("# Line num ERROR\r\n"); return HAL_ERROR;
  }

  if (V9203_wr_flash(channel, regAddrHI, 0) != HAL_OK)
  {
    DC_debugOut("# Reactive cons clear ERROR\r\n");
    return HAL_ERROR;
  }

  if (V9203_wr_flash(channel, regAddrLO, 0) != HAL_OK)
  {
    DC_debugOut("# Reactive cons clear ERROR\r\n");
    return HAL_ERROR;
  }
  
  return HAL_OK; 
}
//----------------------------------------------------------------------------------
//Clear S consamption
HAL_StatusTypeDef V9203_clearSCons(uint8_t channel, V9203_line_t line)
{
  uint16_t regAddrHI, regAddrLO;
  
  //Check channel
  if (channel > V9203_COUNT_CHANNELS)
  {
    DC_debugOut("# Channel num ERROR\r\n");
    return HAL_ERROR;
  }
  
  //Get register address
  switch(line)
  {
  case LINE_A: regAddrHI = RegEGYAPPAH; regAddrLO = RegEGYAPPAL; break;
  case LINE_B: regAddrHI = RegEGYAPPBH; regAddrLO = RegEGYAPPBL; break;
  case LINE_C: regAddrHI = RegEGYAPPCH; regAddrLO = RegEGYAPPCL; break;
  case LINE_S: regAddrHI = RegEGYAPPSH; regAddrLO = RegEGYAPPSL;  break;
  default: DC_debugOut("# Line num ERROR\r\n"); return HAL_ERROR;
  }

  if (V9203_wr_flash(channel, regAddrHI, 0) != HAL_OK)
  {
    DC_debugOut("# Full cons clear ERROR\r\n");
    return HAL_ERROR;
  }

  if (V9203_wr_flash(channel, regAddrLO, 0) != HAL_OK)
  {
    DC_debugOut("# Full cons clear ERROR\r\n");
    return HAL_ERROR;
  }

  return HAL_OK;
}