
#include "Clock.h"
#include "DevCTRL.h"
#include "cmsis_os.h"
#include "lwip.h"
#include "ip_addr.h"
#include "sntp.h"

void sntp_serverFound(const char *name, ip_addr_t *ipaddr, void *arg);
extern RTC_HandleTypeDef hrtc;

//------------------------------------------------------------------------------------------------
//Init clock
DEV_Status_t CL_init()
{  
#ifdef CL_USE_NTP 
  
  //Set server name
  sntp_setservername(0, DC_set.netNTP_server);
  
  //Init SNTP
  sntp_init();
  
  uint8_t try_sec = 3;
  
  while (try_sec--)
  {
    if (sntp_enabled())
      break;
    
    vTaskDelay(1000);
  }
  DC_debugOut("# NTP client enabled\r\n");
#endif

  return DEV_OK;
}
//------------------------------------------------------------------------------------------------
//Set system timestamp
void CL_setSystem_Timestamp(uint32_t sec)
{
  time_t timestamp = (time_t)sec;
  
  RTC_TimeTypeDef currentTime;
  RTC_DateTypeDef currentDate;
  struct tm time_tm = *(localtime(&timestamp));
  
  currentTime.Hours = (uint8_t)time_tm.tm_hour;
  currentTime.Minutes = (uint8_t)time_tm.tm_min;
  currentTime.Seconds = (uint8_t)time_tm.tm_sec;
  
  if (time_tm.tm_wday == 0) { time_tm.tm_wday = 7; }  // the chip goes mon tue wed thu fri sat sun
  currentDate.WeekDay = (uint8_t)time_tm.tm_wday;
  currentDate.Month = (uint8_t)time_tm.tm_mon+1; //momth 1-12. This is why date math is frustrating.
  currentDate.Date = (uint8_t)time_tm.tm_mday;
  currentDate.Year = (uint16_t)(time_tm.tm_year+1900-2000);  // time.h is years since 1900, chip is years since 2000
  
  HAL_RTC_SetTime(&hrtc, &currentTime, RTC_FORMAT_BIN);
  HAL_RTC_SetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);
  
  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0,0x32F2); // lock it in with the backup registers
}
//------------------------------------------------------------------------------------------------
//Get system timestamp
DEV_Status_t CL_getSystem_Timestamp(time_t *timestamp)
{
  RTC_TimeTypeDef currentTime;
  RTC_DateTypeDef currentDate;
  struct tm currTime;

  HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);
  
  currTime.tm_year = currentDate.Year + 100;  // In fact: 2000 + 18 - 1900
  currTime.tm_mday = currentDate.Date;
  currTime.tm_mon  = currentDate.Month - 1;
  
  currTime.tm_hour = currentTime.Hours;
  currTime.tm_min  = currentTime.Minutes;
  currTime.tm_sec  = currentTime.Seconds;
  
  *timestamp = mktime(&currTime);
  
  return DEV_OK;
}
//------------------------------------------------------------------------------------------------
//Get dateTime
DEV_Status_t CL_getDateTime(uint8_t* year, uint8_t* month, uint8_t* date, uint8_t* hours, uint8_t* minutes, uint8_t* sec)
{
  RTC_TimeTypeDef currentTime;
  RTC_DateTypeDef currentDate;
  
  if (HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN) != HAL_OK)
    return DEV_ERROR;
  
  if (HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN) != HAL_OK)
    return DEV_ERROR;

  *year = currentDate.Year;
  *date = currentDate.Date;
  *month = currentDate.Month - 1;
  
  *hours = currentTime.Hours;
  *minutes = currentTime.Minutes;
  *sec = currentTime.Seconds;
  
  return DEV_OK;
}
//------------------------------------------------------------------------------------------------
//Get format date time
DEV_Status_t CL_getFormat_DateTime(char* buf)
{
  RTC_TimeTypeDef currentTime;
  RTC_DateTypeDef currentDate;
  struct tm currTime;

  HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);
  
  currTime.tm_year = currentDate.Year + 100;  // In fact: 2000 + 18 - 1900
  currTime.tm_mday = currentDate.Date;
  currTime.tm_mon  = currentDate.Month - 1;
  
  currTime.tm_hour = currentTime.Hours;
  currTime.tm_min  = currentTime.Minutes;
  
  strftime(buf, 26, "%Y-%m-%d %H:%M:%S", &currTime);
  
  return DEV_OK;
}