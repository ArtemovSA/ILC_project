
#include "V9203.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"
#include "String.h"
#include "DevCTRL.h"
#include "PCA9555.h"
#include "cmsis_os.h"

//Var
SPI_HandleTypeDef *V9203_hspi;
JBPM_t JbPm_val;

//Init dev
HAL_StatusTypeDef V9203_initDev(uint8_t channel);
//Setup registers
void V9203_setupReg(uint8_t channel);
//Write flash
HAL_StatusTypeDef V9203_wr_flash(uint8_t channel, uint16_t addrReg, uint32_t data);
//Read flash
HAL_StatusTypeDef V9203_rd_flash(uint8_t channel, uint16_t addrReg, uint32_t* data);
//CRC16
uint16_t V9203_crc16(const void *data, unsigned data_size);

//Defaul reg val
void V9203_initRegVal();

//--------------------------------------------------------------------------------------------------
//Init
void V9203_init(SPI_HandleTypeDef *hspi)
{
  V9203_hspi = hspi;
  
  for (int i=0; i<V9203_COUNT_CHANNELS; i++)
  {
    V9203_initDev(i);
  }
}
//--------------------------------------------------------------------------------------------------
//Init dev
HAL_StatusTypeDef V9203_initDev(uint8_t channel)
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
    DC_debugOut(" #V9203 INIT CH:%d  ERROR\r\n", channel);
    return HAL_ERROR;
  }
  
  //Clear
  for(unsigned char i=0;i<56;i++)
  {
    V9203_wr_flash(channel, (0xC800+i), 0);
  } 
  
  V9203_initRegVal();
  V9203_setupReg(channel); //Setup registers
    
  DC_debugOut(" #V9203 INIT OK CH:%d\r\n", channel);
  
  return HAL_OK;
}
//--------------------------------------------------------------------------------------------------
//Setup registers
void V9203_setupReg(uint8_t channel)
{
  uint32_t checkSum = 0;
  
  V9203_wr_flash(channel, RegMTPARA0, 0xaa000000); //clear ram
  vTaskDelay(20);
  V9203_wr_flash(channel, RegMTPARA0, JbPm_val.RacMTPARA0); //Enable digital input
  checkSum += JbPm_val.RacMTPARA0;
  
  //Config registers
  V9203_wr_flash(channel, RegMTPARA1, JbPm_val.RacMTPARA1);
  checkSum += JbPm_val.RacMTPARA1;
  V9203_wr_flash(channel, RegMTPARA2, JbPm_val.RacMTPARA2); //MT_PARA2 - Unsegmented,distribute 32bit separately twice,DMA enable UA,non-did,AD all-enable
  checkSum += JbPm_val.RacMTPARA2;
  
  // Full-wave / fundamental combined phase power combination 	 A+B+C 
  V9203_wr_flash(channel, ZZAPPA, 0x00000007);
  checkSum += 0x00000007;
  
  // Start the creep mode selection according to the current rms value
  V9203_wr_flash(channel, ZZCPSEL, 0x00000001);
  checkSum += 0x00000001;
  
  // Analog control register 0 0x000f1111 Voltage gain double Current gain 4 times
  V9203_wr_flash(channel, RegANCtrl0, JbPm_val.RacANCtrl0);
  checkSum += JbPm_val.RacANCtrl0;
  
  // Analog control register 1
  V9203_wr_flash(channel, RegANCtrl1, JbPm_val.RacANCtrl1);
  checkSum += JbPm_val.RacANCtrl1;
  
  // Analog control register 2
  V9203_wr_flash(channel, RegANCtrl2, JbPm_val.RacANCtrl2);
  checkSum += JbPm_val.RacANCtrl2;
  
  // Analog control register 3
  V9203_wr_flash(channel, RegANCtrl3, JbPm_val.RacANCtrl3);
  checkSum += JbPm_val.RacANCtrl3;
  
  // Low threshold
  V9203_wr_flash(channel, RegZZEGYTHL, JbPm_val.RacEGYTH);
  checkSum += JbPm_val.RacEGYTH;
  
  // High threshold
  V9203_wr_flash(channel, RegZZEGYTHH, 0x00000000);
  checkSum += 0x00000000;
   
  // enable threshold limit
  V9203_wr_flash(channel, RegCTHH, JbPm_val.RacCTHH);
  checkSum += JbPm_val.RacCTHH;
  
  // enable threshold lower limit
  V9203_wr_flash(channel, RegCTHL, JbPm_val.RacCTHL);
  checkSum += JbPm_val.RacCTHL;

  // Active A difference ratio 0	
  V9203_wr_flash(channel, RegWAPTA0, JbPm_val.gs_JBA.RacWAPT);
  checkSum += JbPm_val.gs_JBA.RacWAPT;
  
  // Active B difference ratio 0	
  V9203_wr_flash(channel, RegWAPTB0, JbPm_val.gs_JBB.RacWAPT);
  checkSum += JbPm_val.gs_JBB.RacWAPT;

  // Active C difference ratio 0	
  V9203_wr_flash(channel, RegWAPTC0, JbPm_val.gs_JBC.RacWAPT);
  checkSum += JbPm_val.gs_JBC.RacWAPT;  
  
  // Reactive A difference ratio
  V9203_wr_flash(channel, RegWAQTA, JbPm_val.gs_JBA.RacWAPT);
  checkSum += JbPm_val.gs_JBA.RacWAPT;  

  // Reactive B difference ratio
  V9203_wr_flash(channel, RegWAQTB, JbPm_val.gs_JBB.RacWAPT);
  checkSum += JbPm_val.gs_JBB.RacWAPT;  
  
  // Reactive C difference ratio
  V9203_wr_flash(channel, RegWAQTC, JbPm_val.gs_JBC.RacWAPT);
  checkSum += JbPm_val.gs_JBC.RacWAPT;
  
  //A Current rms difference
  V9203_wr_flash(channel, RegWARTIA, JbPm_val.gs_JBA.RacWARTI);
  checkSum += JbPm_val.gs_JBA.RacWARTI;
  
  //B Current rms difference
  V9203_wr_flash(channel, RegWARTIB, JbPm_val.gs_JBB.RacWARTI);
  checkSum += JbPm_val.gs_JBB.RacWARTI;

  //C Current rms difference
  V9203_wr_flash(channel, RegWARTIC, JbPm_val.gs_JBC.RacWARTI);
  checkSum += JbPm_val.gs_JBC.RacWARTI;  
  
  //A RMS voltage difference
  V9203_wr_flash(channel, RegWARTUA, JbPm_val.gs_JBA.RacWARTU);
  checkSum += JbPm_val.gs_JBA.RacWARTU;
  
  //B RMS voltage difference
  V9203_wr_flash(channel, RegWARTUB, JbPm_val.gs_JBB.RacWARTU);
  checkSum += JbPm_val.gs_JBB.RacWARTU;
  
  //C RMS voltage difference
  V9203_wr_flash(channel, RegWARTUC, JbPm_val.gs_JBC.RacWARTU);
  checkSum += JbPm_val.gs_JBC.RacWARTU;
  
  // Angle difference
  V9203_wr_flash(channel, RegWAEC0, JbPm_val.RacWAEC0);
  checkSum += JbPm_val.RacWAEC0;
  
  //UMChannel DC component  preset negative value  current detection interrupted
  V9203_wr_flash(channel, RegZZDCUM, JbPm_val.RacZZDCUM);
  checkSum += JbPm_val.RacZZDCUM;  
  
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
void V9203_initRegVal()
{
  JbPm_val.ui_MeterC=1200;               // Table constant
  JbPm_val.ui_Un=22000;                  // Nominal voltage
  JbPm_val.ui_Ib=5000;                   // Nominal current
  JbPm_val.ui_Resve1=0;
  
  JbPm_val.RacEGYTH  = 0x2fd3ff5;      // Energy accumulation threshold  0x2C7BDF00  0x04fBDF00 0x5aa8c57
  JbPm_val.RacCTHH= 0x000221E5;      //Enable,creep threshold
  JbPm_val.RacCTHL= 0x0001EB4E;      // Enable,creep threshold
  JbPm_val.RacZZDCUM =  0xfff00000;       //0x0134 Current detection threshold
  JbPm_val.RacWAEC0 = 0x00000000;        // Angle difference
  
  JbPm_val.RacMTPARA0 = 0x000000ff;
  JbPm_val.RacMTPARA1 = 0x00000000;
  JbPm_val.RacMTPARA2 = 0x070080ff;
  JbPm_val.RacANCtrl0 = 0x00000333;     
  JbPm_val.RacANCtrl1 = 0x00000000;      
  JbPm_val.RacANCtrl2 = 0x77005400;      //0x77005400;  0xF7005400;  
  JbPm_val.RacANCtrl3 = 0x00000406;      //0x00000406; 
  
  JbPm_val.gs_JBA.RacWARTU = 1;//0xFC9A0D98;  // Full-wave voltage rms ratio difference register 0xFC9A0D98
  JbPm_val.gs_JBA.RacWARTI = 0x21A8301B;  // Full-wave current rms ratio difference register
  JbPm_val.gs_JBA.RacWAPT = 0x21E51894;   // Full-wave active power ratio difference register 0xEBA74B27
  JbPm_val.gs_JBA.RacWWAPT = 0x00000000;  // Full-wave active power secondary compensation register
  JbPm_val.gs_JBA.RacREWWAPT = 0x00000000;  // Full-wave reactive power secondary compensation register
  
  JbPm_val.gs_JBB.RacWARTU = 1;//0xFD6F2E2F;  // Full-wave voltage rms ratio difference register
  JbPm_val.gs_JBB.RacWARTI = 0xE4913EB;  // Full-wave current rms ratio difference register
  JbPm_val.gs_JBB.RacWAPT = 0xF5DC2F3;   // Full-wave active power ratio difference register 0xECC04599
  JbPm_val.gs_JBB.RacWWAPT = 0x00000000;  // Full-wave active power secondary compensation register
  JbPm_val.gs_JBB.RacREWWAPT = 0x00000000;  // Full-wave reactive power secondary compensation register
  
  JbPm_val.gs_JBC.RacWARTU = 1;//0xFD1996B1;  // Full-wave voltage rms ratio difference register
  JbPm_val.gs_JBC.RacWARTI = 0xE38E38E;  // Full-wave current rms ratio difference register
  JbPm_val.gs_JBC.RacWAPT = 0xEF92325;   //Full-wave active power ratio difference register0xEC4A811B
  JbPm_val.gs_JBC.RacWWAPT = 0x00000000;  //Full-wave active power secondary compensation register
  JbPm_val.gs_JBC.RacREWWAPT = 0x00000000;  //Full-wave reactive power secondary compensation register 
  
  JbPm_val.ui_Resve2 = 0;
  JbPm_val.ul_PG = 0x10B;               // Power proportional coefficient
  JbPm_val.ul_URmG = 0x513b;            // Voltage channel proportional coefficient
  JbPm_val.ul_I1RmG = 0x1A2C0;          // Current channel 1 proportional coefficient
  
  JbPm_val.ui_JbCRC = V9203_crc16((uint8_t*)&JbPm_val, sizeof(JBPM_t)-2);   // The CRC result of the calibration parameter
}
//--------------------------------------------------------------------------------------------------
//Set chip select
HAL_StatusTypeDef V9203_set_CS(uint8_t channel, uint8_t state)
{
  uint8_t pin;

  HAL_GPIO_WritePin(PW_CS_GPIO_Port, PW_CS_Pin, GPIO_PIN_RESET); //CS SPI
  
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
  
  if ((state = V9203_set_CS(channel, LOW_LEVEL)) != HAL_OK)
    return state;
  
  vTaskDelay(1);
  
  if ((state = HAL_SPI_TransmitReceive(V9203_hspi, txBuf, rxBuf, 4, V9203_SPI_TIMEOUT)) != HAL_OK)
    return state;
  
  vTaskDelay(1);
  
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

uint16_t V9203_crc16(const void *data, unsigned data_size)
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
  if (regData > 0xFFFF)
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
//  if (regData > 0xFFFFFFF)
//    return -1;
  
  return regData/JbPm_val.ul_URmG/10;
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
  case LINE_A: regAddr = RMSI1A; break;
  case LINE_B: regAddr = RMSI1B; break;
  case LINE_C: regAddr = RMSI1C; break;
  default: DC_debugOut("# Line num ERROR\r\n"); return -1;
  }
  
  if (V9203_rd_flash(channel, regAddr, &regData) != HAL_OK)
  {
    DC_debugOut("# Current read ERROR\r\n");
    return -1;
  }
  
  //Check return data
//  if (regData > 0xFFFFFFF)
//    return -1;
  
  return regData/JbPm_val.ul_I1RmG;
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
//  if (regData > 0xFFFFFFF)
//    return -1;
  
  return regData/JbPm_val.ul_PG;
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
//  if (regData > 0xFFFFFFF)
//    return -1;
  
  return regData/JbPm_val.ul_PG;
}