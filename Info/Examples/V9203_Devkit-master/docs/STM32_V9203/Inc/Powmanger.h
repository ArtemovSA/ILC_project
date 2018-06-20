
#ifndef _POWMANGE_H_
#define _POWMANGE_H_

#ifndef POWMANGE_EXT
#define POWMANGE_EXT extern
#endif


POWMANGE_EXT uint16 gs_powerErr;
POWMANGE_EXT uint32 ADC_ConvertedValue;
POWMANGE_EXT uint8 adc_cnt;
POWMANGE_EXT uint32 time_cnt;

void PowDownChk(void);
uint8 PowerDnPro(void);
uint8 JudgePowerDnValue(uint32 RMSUValue);

#endif
