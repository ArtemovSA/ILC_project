#ifndef __UART2_H__
    #define __UART2_H__

#ifndef UART2_EXT
#define UART2_EXT extern
#endif

#define Const_DyUart2Over   0xff                       // 模拟口的溢出保护，在10ms中执行，等于0xff则初始化
//extern uint8   guc_DyUart2Over;        //模拟串口超时保护

#define Uart2_RevEn()
#define Uart2_SendEn()

#define CONST_UART2_DEFBPS  Const_BPS_1200


//void Init_Uart2(void);
void Uart2_Dy10ms(void);
void Uart2_Receive(void);
void Uart2_Transmit(void);
void Sys_InitUart2(uint32 bound);


#endif
