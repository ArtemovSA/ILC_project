
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
HAL_StatusTypeDef CL_init()
{  
#ifdef CL_USE_NTP 
  //Init SNTP
  sntp_init();
  
  //Set server name
  sntp_setservername(0, DC_set.netNTP_server);
  
  uint8_t try_sec = 3;
  
  while (try_sec--)
  {
    if (sntp_enabled())
      break;
    
    vTaskDelay(1000);
  }
  DC_debugOut("NTP client enabled\r\n");
#endif

  return HAL_OK;
}
//------------------------------------------------------------------------------------------------
//Get system timestamp
HAL_StatusTypeDef CL_getSystem_Timestamp(time_t *timestamp)
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
  
  return HAL_OK;
}