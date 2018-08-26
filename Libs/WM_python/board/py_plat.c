/*
# This file is Copyright 2006, 2007, 2009 Dean Hall, 2010 Oscar Lindberg.
#
# This file is part of the Python-on-a-Chip program.
# Python-on-a-Chip is free software: you can redistribute it and/or modify
# it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE Version 2.1.
#
# Python-on-a-Chip is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# A copy of the GNU LESSER GENERAL PUBLIC LICENSE Version 2.1
# is seen in the file COPYING up one directory from this.
*/


#undef __FILE_ID__
#define __FILE_ID__ 0x70


/** PyMite platform-specific routines for MoSync target */

//#include <py_ma.h>

#include "py_pm.h"
#include "cmsis_os.h"
#include "Memory.h"
#include "string.h"
#include "USB_ctrl.h"
#include "USB_port.h"
#include "DevCTRL.h"

extern SemaphoreHandle_t muxSRAM1;
extern SemaphoreHandle_t muxSRAM2;
char PM_str_buf[PM_STRING_SIZE]; //Global string buffer

//Error description
const PM_error_codes_t PM_error_codes[PM_ERROR_CODE_LEN] = {
  
  //Errors
  {0x00, "All ok"},
  {0xFF, "General no result"},
  {0xFD, "General failure"},
  {0xFE, "Return val for stub fxn"},
  {0xFC, "Assertion failure"},
  
  //Exceptions
  {0xE0, "General exception"},
  {0xE1, "System exit"},
  {0xE2, "Input/output error"},
  {0xE3, "Zero division error"},
  {0xE4, "Assertion error"},
  {0xE5, "Attribute error"},
  {0xE6, "Import error"},
  {0xE7, "Index error"},
  {0xE8, "Key error"},
  {0xE9, "Memory error"},
  {0xEA, "Name error"},
  {0xEB, "Syntax error"},
  {0xEC, "System error"},
  {0xED, "Type error"},
  {0xEE, "Value error"},
  {0xEF, "Stop iteration"},
  {0xF0, "Warning"}
};

PM_error_t PM_error; //Error return variable

/* MoSync target requires no initiation. */
PmReturn_t
plat_init(void)
{
  return PM_RET_OK;
}


/* No cleaning up */
PmReturn_t
plat_deinit(void)
{
    return PM_RET_OK;
}

/*
 * Gets a byte from the address in the designated memory space
 * Post-increments *paddr.
 */
uint8_t
plat_memGetByte(PmMemSpace_t memspace, uint8_t const **paddr)
{
  uint8_t b = 0;
  
  switch (memspace)
  {
  case MEMSPACE_RAM:
    b = **paddr;
    *paddr += 1;
    return b;
  case MEMSPACE_SRAM1:
    if ( xSemaphoreTake(muxSRAM1, 100) == pdTRUE ) {
      MEM_SRAM_readData(MEM_ID_SRAM1, (uint32_t)(*paddr), &b, 1); //Read data SRAM
      xSemaphoreGive(muxSRAM1);
      *paddr += 1;
    }
    return b;
  case MEMSPACE_SRAM2:
    if ( xSemaphoreTake(muxSRAM2, 100) == pdTRUE ) {
      MEM_SRAM_readData(MEM_ID_SRAM2, (uint32_t)(*paddr), &b, 1); //Read data SRAM
      xSemaphoreGive(muxSRAM2);
      *paddr += 1;
    }
    return b;
  default:
    return 0;
  }
}


/* MoSync does not have an obvious stdin, so getByte is empty. 
 * Suggestions are welcome.
 */
PmReturn_t
plat_getByte(uint8_t *b)
{
    PmReturn_t retval = PM_RET_OK;
    
    if (USBP_mode == USBP_MODE_SCRIPT) {
      //*b = (uint8_t)USB_rx_buf[USB_rx_count];
      //USB_rx_count--;
    }
    
    return retval;
}

#define PRINTBUFSIZE 80

/* Prints to the log.
 * A newline is added by the platform to every write to log.
 * So the bytes are buffered and logged at newline or if buffer is full. */

PmReturn_t
plat_putByte(uint8_t b)
{
  PmReturn_t retval = PM_RET_OK;
  
  //Режим работы с коммандной строкой питона
  if (USBP_mode == USBP_MODE_SCRIPT) {
    USBP_Send(&b,1);
  }else{
    DC_debugOut(&b);
  }
  
  return retval;
}


PmReturn_t
plat_getMsTicks(uint32_t *r_ticks)
{
    *r_ticks = pm_timerMsTicks;

    return PM_RET_OK;
}


/* Copied from Desktop platform */
void
plat_reportError(PmReturn_t result)
{
  char buf[100];
  
    //Режим работы с коммандной строкой питона
    if (USBP_mode == USBP_MODE_SCRIPT) {
      
      //Отправить сообщение об ошибке
      for (int i=0; i<PM_ERROR_CODE_LEN; i++){
        if (PM_error_codes[i].error_code == result) {
          sprintf(buf, "\n\rError #%02X - %s",result, PM_error_codes[i].description);
          USBP_Send((uint8_t*)buf,strlen(buf));
          break;
        }
      }
      sprintf(buf,"  Release: 0x%02X\n\r", gVmGlobal.errVmRelease);
      USBP_Send((uint8_t*)buf,strlen(buf));
      sprintf(buf,"  FileId:  0x%02X\n\r", gVmGlobal.errFileId);
      USBP_Send((uint8_t*)buf,strlen(buf));
      sprintf(buf,"  LineNum: %d\n\r", gVmGlobal.errLineNum);
      USBP_Send((uint8_t*)buf,strlen(buf));
    }
    
    /* Print traceback */
    {
        pPmObj_t pframe;
        pPmObj_t pstr;
        PmReturn_t retval;

        sprintf(buf,"Traceback (top first):\n\r");
        USBP_Send((uint8_t*)buf,strlen(buf));

        /* Get the top frame */
        pframe = (pPmObj_t)gVmGlobal.pthread->pframe;

        /* If it's the native frame, print the native function name */
        if (pframe == (pPmObj_t)&(gVmGlobal.nativeframe))
        {

            /* The last name in the names tuple of the code obj is the name */
            retval = tuple_getItem((pPmObj_t)gVmGlobal.nativeframe.nf_func->
                                   f_co->co_names, -1, &pstr);
            if ((retval) != PM_RET_OK)
            {
                sprintf(buf,"  Unable to get native func name.\n\r");
                USBP_Send((uint8_t*)buf,strlen(buf));
                return;
            }
            else
            {
              
              sprintf(buf,"  %s() __NATIVE__\n\r", ((pPmString_t)pstr)->val);
              USBP_Send((uint8_t*)buf,strlen(buf));
            }

            /* Get the frame that called the native frame */
            pframe = (pPmObj_t)gVmGlobal.nativeframe.nf_back;
        }

        /* Print the remaining frame stack */
        for (;
             pframe != C_NULL;
             pframe = (pPmObj_t)((pPmFrame_t)pframe)->fo_back)
        {
            /* The last name in the names tuple of the code obj is the name */
            retval = tuple_getItem((pPmObj_t)((pPmFrame_t)pframe)->
                                   fo_func->f_co->co_names, -1, &pstr);
            if ((retval) != PM_RET_OK) break;

            sprintf(buf,"  %s()\n\r", ((pPmString_t)pstr)->val);
            USBP_Send((uint8_t*)buf,strlen(buf));
        }
        
        sprintf(buf,"  <module>.\n\r");
        USBP_Send((uint8_t*)buf,strlen(buf));
    }
}
