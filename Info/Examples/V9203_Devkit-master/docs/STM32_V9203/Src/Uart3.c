#define UART3_EXT
#include "Includes.h"

//变量声明
uint8   guc_DyUart3Over;        //模拟串口超时保护
/*=========================================================================================\n
* @function_name: Sys_InitUart3
* @function_file: Uart3.c
* @描述: 初始化UART3
* 
* 
* @参数: 
* @param:bound  
* @返回: 
* @作者:   lwb (2013-07-03)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Sys_InitUart3(uint32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    USART_DeInit(USART3);  //复位串口3

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



   //USART 初始化设置

    USART_InitStructure.USART_BaudRate = bound;                     //一般设置为2400;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;     //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;          //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_Even;           //偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式

    USART_Init(USART3, &USART_InitStructure);               //初始化串口

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                         //根据指定的参数初始化VIC寄存器
//  USART_HalfDuplexCmd(USART3,ENABLE);                     //设置为半双工
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);          //开启中断
//  USART_ITConfig(USART3, USART_IT_TXE, ENABLE);           //开启中断
//  USART_ITConfig(USART3, USART_IT_TC,ENABLE);             //开启发送中断
//  USART_ClearFlag(USART3,USART_FLAG_TC);
    USART_Cmd(USART3, ENABLE);                              //使能串口
//  USART_ClearITPendingBit(USART2, USART_IT_TC);
}
/*=========================================================================================\n
* @function_name: USART1_IRQHandler
* @function_file: Uart3.c
* @描述: UART3中断
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-08)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void USART3_IRQHandler(void)
{
    //接收中断
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        Uart3_Receive();
        USART_ClearITPendingBit(USART3,USART_IT_RXNE);
    }
//   //接收中断
//  if(USART_GetFlagStatus(USART3, USART_IT_RXNE) != RESET)
//  {
//      Uart3_Receive();
//      USART_ClearITPendingBit(USART3,USART_IT_RXNE);
//  }

    
    //发送中断
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
* @描述: 模拟串口的超时保护
*       为防止锁死，定时保护，当没有任何数据发送和接收的时候，初始化
* @参数:
* @返回:
* @作者:   lwb (2012-03-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Uart3_Dy10ms(void)
{
    guc_DyUart3Over--;
    if(0x00== guc_DyUart3Over)
    {
        Sys_InitUart3(9600);
        ComBom_Init(ComIndex_Uart3);    //将uart6 端口对应的 COM初始化
    }
    
    if(guc_BodeDely>0)                  //波特率切换
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
* @描述: UART3接收,在中断中调用该函数
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Uart3_Receive(void)
{
    guc_DyUart3Over = Const_DyUart3Over;//端口超时保护

    //处于空闲状态或已经处于uart接收状态
    gs_ComGroup[ComIndex_Uart3].ucPort   = Port_Uart3;

    gs_ComGroup[ComIndex_Uart3].ucRecvTmr = Const_DyRvPack;     //设置数据包接收超时时间

    if(gs_ComGroup[ComIndex_Uart3].ucStt == ComStt_Idle)        //当前是空闲的状态的话，判断是否是帧头68
    {
        if(((uint8)USART_ReceiveData(USART3))==COMHEAD)               //接受到起始字节
        {
            gs_ComGroup[ComIndex_Uart3].ucStt = ComStt_Recv;
        }
    }

    if(gs_ComGroup[ComIndex_Uart3].ucStt == ComStt_Recv)            //这个判断是防止发送的时候有接收中断进入的可能
    {
        if(gs_ComGroup[ComIndex_Uart3].ucLen < Const_MaxBufLen)     //判断 com中的buf是否溢出
        {                                                           //防止缓存溢出
            gs_ComGroup[ComIndex_Uart3].ucBuf[gs_ComGroup[ComIndex_Uart3].ucLen++] = (uint8)USART_ReceiveData(USART3);//数据存入缓冲区，指针加加

            if(gs_ComGroup[ComIndex_Uart3].ucLen>COMINITLEN)
            {
                if(gs_ComGroup[ComIndex_Uart3].ucBuf[7]!=COMHEAD)           //如果第7个不是68返回错误
                {
                    gs_ComGroup[ComIndex_Uart3].ucStt=ComStt_Idle;
                }else if(gs_ComGroup[ComIndex_Uart3].ucLen>=(gs_ComGroup[ComIndex_Uart3].ucBuf[9]+COMINITLEN+2))   //长度符合要求
                {
                    if(((uint8)USART_ReceiveData(USART3))==COMTAIL)
                    {
                        guc_ComSendDy=3;                                        //延时30ms发送
                        gs_ComGroup[ComIndex_Uart3].ucStt=ComStt_WaitProc;
                        gui_SystemEvent |= flgEtGlb_NeedPrtclChk;               // 设置标志
                        gs_ComGroup[ComIndex_Uart3].ucRecvTmr=0;
                        USART_ITConfig(USART3, USART_IT_RXNE,DISABLE);          //关闭中断
                        //关闭接收
                    }
                }
            }
        }
    }
}
/*=========================================================================================\n
* @function_name: Uart3_Transmit
* @function_file: Uart3.c
* @描述: UART3发送,中断中调用
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Uart3_Transmit(void)
{
    guc_DyUart3Over = Const_DyUart3Over;//端口超时保护

    if(gs_ComGroup[ComIndex_Uart3].ucPort   == Port_Uart3)
    {   
        //处于空闲状态或已经处于uart接收状态
        if(gs_ComGroup[ComIndex_Uart3].ucStt == ComStt_Send)
        {
            if(gs_ComGroup[ComIndex_Uart3].ucPos >= gs_ComGroup[ComIndex_Uart3].ucLen)
            {   
                //发送完毕,初始化通讯控制状态
                Uart3_RevEn();
                USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);          //开启中断
                USART_ITConfig(USART3, USART_IT_TC,DISABLE);            //关闭发送中断
                ComBom_Init(ComIndex_Uart3);//将uart2 端口对应的 COM初始化
            }else
            {
                Uart3_SendEn();//控制端口设置为发送模式（三线制通讯使用）  //还有未发送数据则写入外设发送缓存
                if(gs_ComGroup[ComIndex_Uart3].ucFrmHeadCnt<4)
                {                    
                    USART_SendData(USART3,0xFE);    //发送数据
                    gs_ComGroup[ComIndex_Uart3].ucFrmHeadCnt++;
                }else
                {
                    USART_SendData(USART3,gs_ComGroup[ComIndex_Uart3].ucBuf[gs_ComGroup[ComIndex_Uart3].ucPos++]);    //发送数据
                }
            }
        }else
        {   //如果在发送中断标记中，进入但是状态不正确，则初始化
            ComBom_Init(ComIndex_Uart3);//将uart3 端口对应的 COM初始化
            Uart3_RevEn();
        }
    }
}




