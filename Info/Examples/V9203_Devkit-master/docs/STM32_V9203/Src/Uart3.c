#define UART3_EXT
#include "Includes.h"

//��������
uint8   guc_DyUart3Over;        //ģ�⴮�ڳ�ʱ����
/*=========================================================================================\n
* @function_name: Sys_InitUart3
* @function_file: Uart3.c
* @����: ��ʼ��UART3
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
void Sys_InitUart3(uint32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    USART_DeInit(USART3);  //��λ����3

    // Connect Pb.06 to UART TX3 
    //GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
    
    // Connect Pb.07 to UART RX3
    //GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
     GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);

    //USART3_RX   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        // 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;             
     GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
    GPIO_Init(GPIOC, &GPIO_InitStructure);     

    //USART3_TX  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;      // 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
     GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
    GPIO_Init(GPIOC, &GPIO_InitStructure);              



   //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;                     //һ������Ϊ2400;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;     //�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;          //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_Even;           //żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ

    USART_Init(USART3, &USART_InitStructure);               //��ʼ������

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                         //����ָ���Ĳ�����ʼ��VIC�Ĵ���
//  USART_HalfDuplexCmd(USART3,ENABLE);                     //����Ϊ��˫��
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);          //�����ж�
//  USART_ITConfig(USART3, USART_IT_TXE, ENABLE);           //�����ж�
//  USART_ITConfig(USART3, USART_IT_TC,ENABLE);             //���������ж�
//  USART_ClearFlag(USART3,USART_FLAG_TC);
    USART_Cmd(USART3, ENABLE);                              //ʹ�ܴ���
//  USART_ClearITPendingBit(USART2, USART_IT_TC);
}
/*=========================================================================================\n
* @function_name: USART1_IRQHandler
* @function_file: Uart3.c
* @����: UART3�ж�
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-08)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void USART3_IRQHandler(void)
{
    //�����ж�
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        Uart3_Receive();
        USART_ClearITPendingBit(USART3,USART_IT_RXNE);
    }
//   //�����ж�
//  if(USART_GetFlagStatus(USART3, USART_IT_RXNE) != RESET)
//  {
//      Uart3_Receive();
//      USART_ClearITPendingBit(USART3,USART_IT_RXNE);
//  }

    
    //�����ж�
    if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
    {
         Uart3_Transmit();
         USART_ClearITPendingBit(USART3, USART_IT_TC);
    }
    
    
//  if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
//  {
//      Uart3_Transmit();
//      USART_ClearITPendingBit(USART3,USART_IT_TXE);
//  }
   
//    if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
//    {
//        USART_ClearITPendingBit(USART3, USART_IT_ORE);
//    }
}


/*=========================================================================================\n
* @function_name: Uart3_Dy10ms
* @function_file: Uart3.c
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
void Uart3_Dy10ms(void)
{
    guc_DyUart3Over--;
    if(0x00== guc_DyUart3Over)
    {
        Sys_InitUart3(9600);
        ComBom_Init(ComIndex_Uart3);    //��uart6 �˿ڶ�Ӧ�� COM��ʼ��
    }
    
    if(guc_BodeDely>0)                  //�������л�
    {
        guc_BodeDely--;
        if(guc_BodeDely==0)
        {
            gui_RefreshEvent|=flgEtPara_Bode;
        }
    }
}
/*=========================================================================================\n
* @function_name: Uart3_Receive
* @function_file: Uart3.c
* @����: UART3����,���ж��е��øú���
*
* @����:
* @����:
* @����:   lwb (2012-03-28)
* @��ע:
*-------------------------------------------------------------------------------------------
* @�޸���:
* @�޸�����:
===========================================================================================*/
void Uart3_Receive(void)
{
    guc_DyUart3Over = Const_DyUart3Over;//�˿ڳ�ʱ����

    //���ڿ���״̬���Ѿ�����uart����״̬
    gs_ComGroup[ComIndex_Uart3].ucPort   = Port_Uart3;

    gs_ComGroup[ComIndex_Uart3].ucRecvTmr = Const_DyRvPack;     //�������ݰ����ճ�ʱʱ��

    if(gs_ComGroup[ComIndex_Uart3].ucStt == ComStt_Idle)        //��ǰ�ǿ��е�״̬�Ļ����ж��Ƿ���֡ͷ68
    {
        if(((uint8)USART_ReceiveData(USART3))==COMHEAD)               //���ܵ���ʼ�ֽ�
        {
            gs_ComGroup[ComIndex_Uart3].ucStt = ComStt_Recv;
        }
    }

    if(gs_ComGroup[ComIndex_Uart3].ucStt == ComStt_Recv)            //����ж��Ƿ�ֹ���͵�ʱ���н����жϽ���Ŀ���
    {
        if(gs_ComGroup[ComIndex_Uart3].ucLen < Const_MaxBufLen)     //�ж� com�е�buf�Ƿ����
        {                                                           //��ֹ�������
            gs_ComGroup[ComIndex_Uart3].ucBuf[gs_ComGroup[ComIndex_Uart3].ucLen++] = (uint8)USART_ReceiveData(USART3);//���ݴ��뻺������ָ��Ӽ�

            if(gs_ComGroup[ComIndex_Uart3].ucLen>COMINITLEN)
            {
                if(gs_ComGroup[ComIndex_Uart3].ucBuf[7]!=COMHEAD)           //�����7������68���ش���
                {
                    gs_ComGroup[ComIndex_Uart3].ucStt=ComStt_Idle;
                }else if(gs_ComGroup[ComIndex_Uart3].ucLen>=(gs_ComGroup[ComIndex_Uart3].ucBuf[9]+COMINITLEN+2))   //���ȷ���Ҫ��
                {
                    if(((uint8)USART_ReceiveData(USART3))==COMTAIL)
                    {
                        guc_ComSendDy=3;                                        //��ʱ30ms����
                        gs_ComGroup[ComIndex_Uart3].ucStt=ComStt_WaitProc;
                        gui_SystemEvent |= flgEtGlb_NeedPrtclChk;               // ���ñ�־
                        gs_ComGroup[ComIndex_Uart3].ucRecvTmr=0;
                        USART_ITConfig(USART3, USART_IT_RXNE,DISABLE);          //�ر��ж�
                        //�رս���
                    }
                }
            }
        }
    }
}
/*=========================================================================================\n
* @function_name: Uart3_Transmit
* @function_file: Uart3.c
* @����: UART3����,�ж��е���
*
* @����:
* @����:
* @����:   lwb (2012-03-28)
* @��ע:
*-------------------------------------------------------------------------------------------
* @�޸���:
* @�޸�����:
===========================================================================================*/
void Uart3_Transmit(void)
{
    guc_DyUart3Over = Const_DyUart3Over;//�˿ڳ�ʱ����

    if(gs_ComGroup[ComIndex_Uart3].ucPort   == Port_Uart3)
    {   
        //���ڿ���״̬���Ѿ�����uart����״̬
        if(gs_ComGroup[ComIndex_Uart3].ucStt == ComStt_Send)
        {
            if(gs_ComGroup[ComIndex_Uart3].ucPos >= gs_ComGroup[ComIndex_Uart3].ucLen)
            {   
                //�������,��ʼ��ͨѶ����״̬
                Uart3_RevEn();
                USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);          //�����ж�
                USART_ITConfig(USART3, USART_IT_TC,DISABLE);            //�رշ����ж�
                ComBom_Init(ComIndex_Uart3);//��uart2 �˿ڶ�Ӧ�� COM��ʼ��
            }else
            {
                Uart3_SendEn();//���ƶ˿�����Ϊ����ģʽ��������ͨѶʹ�ã�  //����δ����������д�����跢�ͻ���
                if(gs_ComGroup[ComIndex_Uart3].ucFrmHeadCnt<4)
                {                    
                    USART_SendData(USART3,0xFE);    //��������
                    gs_ComGroup[ComIndex_Uart3].ucFrmHeadCnt++;
                }else
                {
                    USART_SendData(USART3,gs_ComGroup[ComIndex_Uart3].ucBuf[gs_ComGroup[ComIndex_Uart3].ucPos++]);    //��������
                }
            }
        }else
        {   //����ڷ����жϱ���У����뵫��״̬����ȷ�����ʼ��
            ComBom_Init(ComIndex_Uart3);//��uart3 �˿ڶ�Ӧ�� COM��ʼ��
            Uart3_RevEn();
        }
    }
}




