#define UART2_EXT
#include "Includes.h"

//��������
uint8   guc_DyUart2Over;        //ģ�⴮�ڳ�ʱ����

/*=========================================================================================\n
* @function_name: Sys_InitUart2
* @function_file: Uart2.c
* @����: ��ʼ��UART2
* 
* 
* @����: 
* @param:bound  
* @����: 
* @����:   lwb (2013-07-03)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Sys_InitUart2(uint32 bound)
{
   //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    USART_DeInit(USART2);  //��λ����2

    // Connect Pd.05 to UART TX2 
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    
    // Connect Pd.06 to UART RX2
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

    //USART2_TX  Pd.05
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //�����������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
    GPIO_Init(GPIOD, &GPIO_InitStructure);              //��ʼ��Pd5  TXD

    //USART2_RX   Pd.06
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
    GPIO_Init(GPIOD, &GPIO_InitStructure);              //��ʼ��Pd6 RXD

  

   //USART ��ʼ������
    USART_InitStructure.USART_BaudRate = bound;                     //һ������Ϊ2400;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;     //�ֳ�Ϊ9λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;          //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_Even;           //żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure);               //��ʼ������

   //Usart2 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;      //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                         //����ָ���Ĳ�����ʼ��VIC�Ĵ���
//  USART_HalfDuplexCmd(USART2,ENABLE);                     //����Ϊ��˫��
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);          //�����ж�
//  USART_ITConfig(USART2, USART_IT_TXE, ENABLE);           //�����ж�
//  USART_ClearFlag(USART2,USART_FLAG_TC);
//  USART_ITConfig(USART2, USART_IT_TC,ENABLE);             //���������ж�
    USART_Cmd(USART2, ENABLE);                              //ʹ�ܴ��� 
//  USART_ClearITPendingBit(USART2, USART_IT_TC);

}
/*=========================================================================================\n
* @function_name: Uart2_Dy10ms
* @function_file: Uart2.c
* @����: ģ�⴮�ڵĳ�ʱ����
*       Ϊ��ֹ��������ʱ��������û���κ����ݷ��ͺͽ��յ�ʱ�򣬳�ʼ��
* @����:
* @����:
* @����:   lwb (2012-03-28)
* @��ע:
*-------------------------------------------------------------------------------------------
* @�޸���:
* @�޸�����:
===========================================================================================*/
void Uart2_Dy10ms(void)
{
    guc_DyUart2Over--;
      if(0x00== guc_DyUart2Over)
      {
          Sys_InitUart2(9600);
          ComBom_Init(ComIndex_Uart2);    //��uart6 �˿ڶ�Ӧ�� COM��ʼ��
      }
      
     /* if(guc_BodeDely>0)                  //�������л�
      {
          guc_BodeDely--;
          if(guc_BodeDely==0)
          {
              gui_RefreshEvent|=flgEtPara_Bode;
          }
      }*/

}
/*=========================================================================================\n
* @function_name: USART2_IRQHandler
* @function_file: Uart2.c
* @����: ����2�ж�
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-04)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void USART2_IRQHandler(void)
{  
    //�����ж�
//    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
//    {
//        USART_ClearFlag(USART2,USART_FLAG_ORE);        //��SR
//        USART_ReceiveData(USART2);                                //��DR
//    }
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        Uart2_Receive();
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    }     
    //�����ж�
    if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
    {
         Uart2_Transmit();
         USART_ClearITPendingBit(USART2, USART_IT_TC);
    }    
}
/*=========================================================================================\n
* @function_name: Uart2_Receive
* @function_file: Uart2.c
* @����: UART4����,���ж��е��øú���
*
* @����:
* @����:
* @����:   lwb (2012-03-28)
* @��ע:
*-------------------------------------------------------------------------------------------
* @�޸���:
* @�޸�����:
===========================================================================================*/
void Uart2_Receive(void)
{
    guc_DyUart2Over = Const_DyUart2Over;//�˿ڳ�ʱ����
    
       //���ڿ���״̬���Ѿ�����uart����״̬
       gs_ComGroup[ComIndex_Uart2].ucPort   = Port_Uart2;
    
       gs_ComGroup[ComIndex_Uart2].ucRecvTmr = Const_DyRvPack;     //�������ݰ����ճ�ʱʱ��
    
       if(gs_ComGroup[ComIndex_Uart2].ucStt == ComStt_Idle)        //��ǰ�ǿ��е�״̬�Ļ����ж��Ƿ���֡ͷ68
       {
           if(((uint8)USART_ReceiveData(USART2))==COMHEAD)               //���ܵ���ʼ�ֽ�
           {
               gs_ComGroup[ComIndex_Uart2].ucStt = ComStt_Recv;
           }
       }
    
       if(gs_ComGroup[ComIndex_Uart2].ucStt == ComStt_Recv)            //����ж��Ƿ�ֹ���͵�ʱ���н����жϽ���Ŀ���
       {
           if(gs_ComGroup[ComIndex_Uart2].ucLen < Const_MaxBufLen)     //�ж� com�е�buf�Ƿ����
           {                                                           //��ֹ�������
               gs_ComGroup[ComIndex_Uart2].ucBuf[gs_ComGroup[ComIndex_Uart2].ucLen++] = (uint8)USART_ReceiveData(USART2);//���ݴ��뻺������ָ��Ӽ�
    
               if(gs_ComGroup[ComIndex_Uart2].ucLen>COMINITLEN)
               {
                   if(gs_ComGroup[ComIndex_Uart2].ucBuf[7]!=COMHEAD)           //�����7������68���ش���
                   {
                       gs_ComGroup[ComIndex_Uart2].ucStt=ComStt_Idle;
                   }else if(gs_ComGroup[ComIndex_Uart2].ucLen>=(gs_ComGroup[ComIndex_Uart2].ucBuf[9]+COMINITLEN+2))   //���ȷ���Ҫ��
                   {
                       if(((uint8)USART_ReceiveData(USART2))==COMTAIL)
                       {
                           guc_ComSendDy=3;                                        //��ʱ30ms����
                           gs_ComGroup[ComIndex_Uart2].ucStt=ComStt_WaitProc;
                           gui_SystemEvent |= flgEtGlb_NeedPrtclChk;               // ���ñ�־
                           gs_ComGroup[ComIndex_Uart2].ucRecvTmr=0;
                           USART_ITConfig(USART2, USART_IT_RXNE,DISABLE);          //�ر��ж�
                           //�رս���
                       }
                   }
               }
           }
       }

}
/*=========================================================================================\n
* @function_name: Uart2_Transmit
* @function_file: Uart2.c
* @����: UART2����,�ж��е���
*
* @����:
* @����:
* @����:   lwb (2012-03-28)
* @��ע:
*-------------------------------------------------------------------------------------------
* @�޸���:
* @�޸�����:
===========================================================================================*/
void Uart2_Transmit(void)
{
   guc_DyUart2Over = Const_DyUart2Over;//�˿ڳ�ʱ����

    if(gs_ComGroup[ComIndex_Uart2].ucPort   == Port_Uart2)
    {   
        //���ڿ���״̬���Ѿ�����uart����״̬
        if(gs_ComGroup[ComIndex_Uart2].ucStt == ComStt_Send)
        {
            if(gs_ComGroup[ComIndex_Uart2].ucPos >= gs_ComGroup[ComIndex_Uart2].ucLen)
            {   
                //�������,��ʼ��ͨѶ����״̬
                USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);          //�����ж�
                USART_ITConfig(USART2, USART_IT_TC,DISABLE);            //�رշ����ж�
                ComBom_Init(ComIndex_Uart2);//��uart2 �˿ڶ�Ӧ�� COM��ʼ��
            }else
            {
                
                if(gs_ComGroup[ComIndex_Uart2].ucFrmHeadCnt<4)
                {                    
                    USART_SendData(USART2,0xFE);    //��������
                    gs_ComGroup[ComIndex_Uart2].ucFrmHeadCnt++;
                }else
                {
                    USART_SendData(USART2,gs_ComGroup[ComIndex_Uart2].ucBuf[gs_ComGroup[ComIndex_Uart2].ucPos++]);    //��������
                }
            }
        }else
        {   //����ڷ����жϱ���У����뵫��״̬����ȷ�����ʼ��
            ComBom_Init(ComIndex_Uart2);//��uart3 �˿ڶ�Ӧ�� COM��ʼ��
        }
    }
}



