
#include "Network.h"
#include "lwip.h"
#include "dns.h"

//Get ip from domen address
HAL_StatusTypeDef NW_getIP_byDomen(char* domen, uint8_t* ip)
{
  ip_addr_t ipAddress;
  err_t retVal = dns_gethostbyname(domen, &ipAddress, NULL, NULL);
  
  if (retVal == ERR_OK)
  {
    *ip = (ipAddress.addr & 0xff000000) >> 24;  //ip4_addr1
    *(ip+1) = (ipAddress.addr & 0x00ff0000) >> 16;
    *(ip+2) = (ipAddress.addr & 0x0000ff00) >> 8;
    *(ip+3) = (ipAddress.addr & 0x000000ff);
    
    return HAL_OK;
  }
  
  return HAL_ERROR;
}