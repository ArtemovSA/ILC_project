
#include "devTCP.h"
#include "DevCTRL.h"
#include "stm32f4xx_hal_def.h"

//--------------------------------------------------------------------------------------------------
//Set TCP settings
HAL_StatusTypeDef devTCP_set(uint8_t* devIP, uint8_t* devMASK, uint8_t* devGATEWAY)
{
  struct netif fsl_netif0;
  ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
  
  IP4_ADDR(&fsl_netif0_ipaddr, *devIP, *(devIP+1), *(devIP+2), *(devIP+3));
  IP4_ADDR(&fsl_netif0_netmask, *devMASK, *(devMASK+1), *(devMASK+2), *(devMASK+3));
  IP4_ADDR(&fsl_netif0_gw, *devGATEWAY, *(devGATEWAY+1), *(devGATEWAY+2), *(devGATEWAY+3));
  
  netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, NULL, ethernetif_init, ethernet_input);
  netif_set_default(&fsl_netif0);
  netif_set_up(&fsl_netif0);
  
  DC_debugOut("# Setted TCP settings\r\n");
  
  return HAL_OK;
}
