#ifndef __UART3_H__
    #define __UART3_H__

#ifndef UART3_EXT
#define UART3_EXT extern
#endif

#define Const_DyUart3Over   0xff                        // 模拟口的溢出保护，在10ms中执行，等于0xff则初始化

#define Uart3_RevEn()    //{GPIO_ResetBits(GPIOD, GPIO_Pin_7);}
#define Uart3_SendEn()   //{GPIO_SetBits(GPIOD, GPIO_Pin_7);}   


void Uart3_Dy10ms(void);
void Uart3_Receive(void);
void Uart3_Transmit(void);
void Sys_InitUart3(uint32 bound);

#endif
