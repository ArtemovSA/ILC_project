#ifndef __UART2_H__
    #define __UART2_H__

#ifndef UART2_EXT
#define UART2_EXT extern
#endif

#define Const_DyUart2Over   0xff                       // ģ��ڵ������������10ms��ִ�У�����0xff���ʼ��
//extern uint8   guc_DyUart2Over;        //ģ�⴮�ڳ�ʱ����

#define Uart2_RevEn()
#define Uart2_SendEn()

#define CONST_UART2_DEFBPS  Const_BPS_1200


//void Init_Uart2(void);
void Uart2_Dy10ms(void);
void Uart2_Receive(void);
void Uart2_Transmit(void);
void Sys_InitUart2(uint32 bound);


#endif
