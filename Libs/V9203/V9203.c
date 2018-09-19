
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
V9203_defSet_t V9203_defSet; //Default calibr


//Write flash
HAL_StatusTypeDef V9203_wr_flash(uint8_t channel, uint16_t addrReg, uint32_t data);
//Read flash
HAL_StatusTypeDef V9203_rd_flash(uint8_t channel, uint16_t addrReg, uint32_t* data);
//Setup registers
static void V9203_setupReg(uint8_t channel, V9203_calibrate_t *calibr);

//--------------------------------------------------------------------------------------------------
//Init
void V9203_init(SPI_HandleTypeDef *hspi, uint8_t* channelEn, uint8_t* activeCh, V9203_calibrate_t* ch_set)
{
  V9203_hspi = hspi;  
  
  for (int i=0; i<V9203_COUNT_CHANNELS; i++)
  {
    if (*channelEn & (1<<i))
      if (V9203_initDev(i, &ch_set[i]) == HAL_OK) //Init dev
        *activeCh |= (1<<i);
  }
  
}
//--------------------------------------------------------------------------------------------------
//Init dev
HAL_StatusTypeDef V9203_initDev(uint8_t channel, V9203_calibrate_t *calibr)
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
  
  V9203_setupReg(channel, calibr); //Setup registers
    
  DC_debugOut("# V9203 INIT OK CH:%d\r\n", channel);
  
  return HAL_OK;
}
//--------------------------------------------------------------------------------------------------
//Setup registers
static void V9203_setupReg(uint8_t channel, V9203_calibrate_t *calibr)
{
  uint32_t checkSum = 0;
  
  // clear ram
  V9203_wr_flash(channel, RegMTPARA0, 0xAA000000);
  vTaskDelay(20);
  
  //**********************************Metering data reg*********************************************
  // Metering data reg
  V9203_wr_flash(channel, RegMTPARA0, calibr->V9203_defSet.MTPARA0); //Enable digital input
  checkSum += calibr->V9203_defSet.MTPARA0;
  V9203_wr_flash(channel, RegMTPARA1, calibr->V9203_defSet.MTPARA1);
  checkSum += calibr->V9203_defSet.MTPARA1;
  V9203_wr_flash(channel, RegMTPARA2, calibr->V9203_defSet.MTPARA2); //MT_PARA2 - Unsegmented,distribute 32bit separately twice,DMA enable UA,non-did,AD all-enable
  checkSum += calibr->V9203_defSet.MTPARA2;
  
  //**********************************Analog control register***************************************
  V9203_wr_flash(channel, RegANCtrl0, calibr->V9203_defSet.ANCtrl0);
  checkSum += calibr->V9203_defSet.ANCtrl0;
  V9203_wr_flash(channel, RegANCtrl1, calibr->V9203_defSet.ANCtrl1);
  checkSum += calibr->V9203_defSet.ANCtrl1;
  V9203_wr_flash(channel, RegANCtrl2, calibr->V9203_defSet.ANCtrl2);
  checkSum += calibr->V9203_defSet.ANCtrl2;
  V9203_wr_flash(channel, RegANCtrl3, calibr->V9203_defSet.ANCtrl3);
  checkSum += calibr->V9203_defSet.ANCtrl3;
  
  //**********************************Threshold*****************************************************
  // Low threshold
  V9203_wr_flash(channel, RegZZEGYTHL, calibr->cal_energyThrdDetect);
  checkSum += calibr->cal_energyThrdDetect;
  // High threshold
  V9203_wr_flash(channel, RegZZEGYTHH, 0x00000000);
  checkSum += 0x00000000;
  // enable threshold limit
  V9203_wr_flash(channel, RegCTHH, calibr->V9203_defSet.CTHH);
  checkSum += calibr->V9203_defSet.CTHH;
  // enable threshold lower limit
  V9203_wr_flash(channel, RegCTHL, calibr->V9203_defSet.CTHL);
  checkSum += calibr->V9203_defSet.CTHL;
  //Current threshold
  V9203_wr_flash(channel, RegZZDCUM, calibr->cal_currThrdDetect);
  checkSum += calibr->cal_currThrdDetect; 

  //**********************************Calibration***************************************************
  // Phase A Total
  // gain voltage RMS
  V9203_wr_flash(channel, RegWARTUA, calibr->calTotalPhase[LINE_A].Cal_WARTU);
  checkSum += calibr->calTotalPhase[LINE_A].Cal_WARTU;
  // gain current RMS
  V9203_wr_flash(channel, RegWARTIA, calibr->calTotalPhase[LINE_A].Cal_WARTI);
  checkSum += calibr->calTotalPhase[LINE_A].Cal_WARTI;
  // gain active power coef RMS
  V9203_wr_flash(channel, RegWAPTA, calibr->calTotalPhase[LINE_A].Cal_WAPT);
  checkSum += calibr->calTotalPhase[LINE_A].Cal_WAPT;
  // gain reactive power coef RMS
  V9203_wr_flash(channel, RegWAQTA, calibr->calTotalPhase[LINE_A].Cal_WAQT);
  checkSum += calibr->calTotalPhase[LINE_A].Cal_WAQT;
  // offset voltage RMS
  V9203_wr_flash(channel, RegWWARTUA, calibr->calTotalPhase[LINE_A].Cal_WWARTU);
  checkSum += calibr->calTotalPhase[LINE_A].Cal_WARTU;
  // offset current RMS
  V9203_wr_flash(channel, RegWWARTIA, calibr->calTotalPhase[LINE_A].Cal_WWARTI);
  checkSum += calibr->calTotalPhase[LINE_A].Cal_WARTI;
  // offset active power
  V9203_wr_flash(channel, RegWWAPTA, calibr->calTotalPhase[LINE_A].Cal_WWAPT);
  checkSum += calibr->calTotalPhase[LINE_A].Cal_WAPT;
  // offset reactive power
  V9203_wr_flash(channel, RegWWAQTA, calibr->calTotalPhase[LINE_A].Cal_WWAQT);
  checkSum += calibr->calTotalPhase[LINE_A].Cal_WAQT;
  
  // Phase B Total
  // gain voltage RMS
  V9203_wr_flash(channel, RegWARTUB, calibr->calTotalPhase[LINE_B].Cal_WARTU);
  checkSum += calibr->calTotalPhase[LINE_B].Cal_WARTU;
  // gain current RMS
  V9203_wr_flash(channel, RegWARTIB, calibr->calTotalPhase[LINE_B].Cal_WARTI);
  checkSum += calibr->calTotalPhase[LINE_B].Cal_WARTI;
  // gain active power coef RMS
  V9203_wr_flash(channel, RegWAPTB, calibr->calTotalPhase[LINE_B].Cal_WAPT);
  checkSum += calibr->calTotalPhase[LINE_B].Cal_WAPT;
  // gain reactive power coef RMS
  V9203_wr_flash(channel, RegWAQTB, calibr->calTotalPhase[LINE_B].Cal_WAQT);
  checkSum += calibr->calTotalPhase[LINE_B].Cal_WAQT;
  // offset voltage RMS
  V9203_wr_flash(channel, RegWWARTUB, calibr->calTotalPhase[LINE_B].Cal_WWARTU);
  checkSum += calibr->calTotalPhase[LINE_B].Cal_WARTU;
  // offset current RMS
  V9203_wr_flash(channel, RegWWARTIB, calibr->calTotalPhase[LINE_B].Cal_WWARTI);
  checkSum += calibr->calTotalPhase[LINE_B].Cal_WARTI;
  // offset active power
  V9203_wr_flash(channel, RegWWAPTB, calibr->calTotalPhase[LINE_B].Cal_WWAPT);
  checkSum += calibr->calTotalPhase[LINE_B].Cal_WAPT;
  // offset reactive power
  V9203_wr_flash(channel, RegWWAQTB, calibr->calTotalPhase[LINE_B].Cal_WWAQT);
  checkSum += calibr->calTotalPhase[LINE_B].Cal_WAQT;
  
  // Phase C Total
  // gain voltage RMS
  V9203_wr_flash(channel, RegWARTUC, calibr->calTotalPhase[LINE_C].Cal_WARTU);
  checkSum += calibr->calTotalPhase[LINE_C].Cal_WARTU;
  // gain current RMS
  V9203_wr_flash(channel, RegWARTIC, calibr->calTotalPhase[LINE_C].Cal_WARTI);
  checkSum += calibr->calTotalPhase[LINE_C].Cal_WARTI;
  // gain active power coef RMS
  V9203_wr_flash(channel, RegWAPTC, calibr->calTotalPhase[LINE_C].Cal_WAPT);
  checkSum += calibr->calTotalPhase[LINE_C].Cal_WAPT;
  // gain reactive power coef RMS
  V9203_wr_flash(channel, RegWAQTC, calibr->calTotalPhase[LINE_C].Cal_WAQT);
  checkSum += calibr->calTotalPhase[LINE_C].Cal_WAQT;
  // offset voltage RMS
  V9203_wr_flash(channel, RegWWARTUC, calibr->calTotalPhase[LINE_C].Cal_WWARTU);
  checkSum += calibr->calTotalPhase[LINE_C].Cal_WARTU;
  // offset current RMS
  V9203_wr_flash(channel, RegWWARTIC, calibr->calTotalPhase[LINE_C].Cal_WWARTI);
  checkSum += calibr->calTotalPhase[LINE_C].Cal_WARTI;
  // offset active power
  V9203_wr_flash(channel, RegWWAPTC, calibr->calTotalPhase[LINE_C].Cal_WWAPT);
  checkSum += calibr->calTotalPhase[LINE_C].Cal_WAPT;
  // offset reactive power
  V9203_wr_flash(channel, RegWWAQTC, calibr->calTotalPhase[LINE_C].Cal_WWAQT);
  checkSum += calibr->calTotalPhase[LINE_C].Cal_WAQT;
  
 
  //**********************************Others********************************************************
  
  // Full-wave / fundamental combined phase power combination 	 A+B+C 
  V9203_wr_flash(channel, ZZAPPA, 0x00000007);
  checkSum += 0x00000007;
  
  // Start the creep mode selection according to the current rms value
  V9203_wr_flash(channel, ZZCPSEL, 0x00000001);
  checkSum += 0x00000001;
  
  // Angle difference
  V9203_wr_flash(channel, RegWAEC0, calibr->V9203_defSet.WAEC0);
  checkSum += calibr->V9203_defSet.WAEC0;
  
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
void V9203_setDefaultReg(uint8_t channel, V9203_calibrate_t *calibr)
{    
  //Default calibr
  calibr->V9203_defSet.CTHH = V9203_DEF_CTHH; // Top judgment threshold register
  calibr->V9203_defSet.CTHL = V9203_DEF_CTHL; // Bottom judgment threshold register
  calibr->V9203_defSet.WAEC0 = V9203_DEF_WAEC0; // Angle difference 0
  calibr->V9203_defSet.MTPARA0 = V9203_DEF_MTPARA0; // Metering data reg 0
  calibr->V9203_defSet.MTPARA1 = V9203_DEF_MTPARA1; // Metering data reg 1
  calibr->V9203_defSet.MTPARA2 = V9203_DEF_MTPARA2; // Metering data reg 2
  calibr->V9203_defSet.ANCtrl0 = V9203_DEF_ANCTRL0; //Analog control register 0
  calibr->V9203_defSet.ANCtrl1 = V9203_DEF_ANCTRL1; //Analog control register 1
  calibr->V9203_defSet.ANCtrl2 = V9203_DEF_ANCTRL2; //Analog control register 2
  calibr->V9203_defSet.ANCtrl3 = V9203_DEF_ANCTRL3; //Analog control register 3
  
  //Total calibrate
  //Gaint
  calibr->calTotalPhase[LINE_A].Cal_WARTU = V9203_DEF_CAL_WARTU;
  calibr->calTotalPhase[LINE_A].Cal_WARTI = V9203_DEF_CAL_WARTI;
  calibr->calTotalPhase[LINE_A].Cal_WAPT = V9203_DEF_CAL_WAPT;
  calibr->calTotalPhase[LINE_A].Cal_WAQT = V9203_DEF_CAL_WAQT;
  //Offset
  calibr->calTotalPhase[LINE_A].Cal_WWARTU = V9203_DEF_CAL_WWARTU;
  calibr->calTotalPhase[LINE_A].Cal_WWARTI = V9203_DEF_CAL_WWARTI;
  calibr->calTotalPhase[LINE_A].Cal_WWAPT = V9203_DEF_CAL_WWAPT;
  calibr->calTotalPhase[LINE_A].Cal_WWAQT = V9203_DEF_CAL_WWAQT;
  
  calibr->calTotalPhase[LINE_B] = calibr->calTotalPhase[LINE_A];
  calibr->calTotalPhase[LINE_C] = calibr->calTotalPhase[LINE_A];
  
  //Findamental calibrate
  //Gaint
  calibr->calFundPhase[LINE_A].Cal_WBRTU = V9203_DEF_CAL_WBRTU;
  calibr->calFundPhase[LINE_A].Cal_WBRTI = V9203_DEF_CAL_WBRTI;
  calibr->calFundPhase[LINE_A].Cal_WBPT = V9203_DEF_CAL_WBPT;
  calibr->calFundPhase[LINE_A].Cal_WBQT = V9203_DEF_CAL_WBQT;
  //Offset
  calibr->calFundPhase[LINE_A].Cal_WWBRTU = V9203_DEF_CAL_WWBRTU;
  calibr->calFundPhase[LINE_A].Cal_WWBRTI = V9203_DEF_CAL_WWBRTI;
  calibr->calFundPhase[LINE_A].Cal_WWBPT = V9203_DEF_CAL_WWBPT;
  calibr->calFundPhase[LINE_A].Cal_WWBQT = V9203_DEF_CAL_WWBQT;
  
  calibr->calFundPhase[LINE_B] = calibr->calFundPhase[LINE_A];
  calibr->calFundPhase[LINE_C] = calibr->calFundPhase[LINE_A];
  
  //Proportion coeff
  float datReg = V9203_DEF_PROP_VOLTAGE;
  calibr->calPropVoltage = *(uint32_t*)&datReg;
  datReg = V9203_DEF_PROP_CURRENT;
  calibr->calPropCurrent = *(uint32_t*)&datReg;
  datReg = V9203_DEF_PROP_POWER;
  calibr->calPropPower = *(uint32_t*)&datReg;
  datReg = V9203_DEF_PROP_RPOWER;
  calibr->calPropRPower = *(uint32_t*)&datReg;
  datReg = V9203_DEF_PROP_COSFI;
  calibr->calPropCOSfi = *(uint32_t*)&datReg;
  datReg = V9203_DEF_PROP_FREQ;
  calibr->calPropFreq = *(uint32_t*)&datReg;
  
  //Threshold
  calibr->cal_currThrdDetect = V9203_DEF_THRD_CURRENT_DETECT;
  calibr->cal_energyThrdDetect = V9203_DEF_THRD_POWER_DETECT;
}
//--------------------------------------------------------------------------------------------------
//Set chip select
HAL_StatusTypeDef V9203_set_CS(uint8_t channel, uint8_t state)
{
  uint8_t pin;

//  PCA9555_digitalWrite(PCA9555_DEF_ADDR, PCA9555_PIN_CS1, HIGH_LEVEL);
//  PCA9555_digitalWrite(PCA9555_DEF_ADDR, PCA9555_PIN_CS2, HIGH_LEVEL);
//  PCA9555_digitalWrite(PCA9555_DEF_ADDR, PCA9555_PIN_CS3, HIGH_LEVEL);
//  PCA9555_digitalWrite(PCA9555_DEF_ADDR, PCA9555_PIN_CS4, HIGH_LEVEL);
//  
  switch(channel)
  {
  case 0: pin = PCA9555_PIN_CS1; break;
  case 1: pin = PCA9555_PIN_CS2; break;
  case 2: pin = PCA9555_PIN_CS3; break;
  case 3: pin = PCA9555_PIN_CS4; break;
  default: DC_debugOut("# V9203 CS channel num ERROR\r\n"); return HAL_ERROR;
  }
  
  taskENTER_CRITICAL();
  if (state == LOW_LEVEL)
  {
    PCA9555_digitalWrite(PCA9555_DEF_ADDR, pin, LOW_LEVEL);
  }else{
    PCA9555_digitalWrite(PCA9555_DEF_ADDR, pin, HIGH_LEVEL);
  }
  taskEXIT_CRITICAL();
  
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

  vTaskDelay(1);
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
  
  return regData*V9203_FREQ_MES_RES*(*(float*)&DC_calibr.channel_cal[channel].calPropFreq);
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
  
  return (double)regData*(*(float*)&DC_calibr.channel_cal[channel].calPropVoltage);
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
  
  return (float)regData*(*(float*)&DC_calibr.channel_cal[channel].calPropCurrent);
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
  
  return (float)regData*(*(float*)&DC_calibr.channel_cal[channel].calPropPower);
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
  
  return (float)regData*(*(float*)&DC_calibr.channel_cal[channel].calPropRPower);
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
  
  return (uint64_t)round(value*(*(float*)&DC_calibr.channel_cal[channel].calPropRPower));
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
  
  return (float)regData*(*(float*)&DC_calibr.channel_cal[channel].calPropCOSfi);
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