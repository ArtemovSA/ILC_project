#define UART2_EXT
#include "Includes.h"

//变量声明
uint8   guc_DyUart2Over;        //模拟串口超时保护

/*=========================================================================================\n
* @function_name: Sys_InitUart2
* @function_file: Uart2.c
* @描述: 初始化UART2
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
void Sys_InitUart2(uint32 bound)
{
   //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    USART_DeInit(USART2);  //复位串口2

    // Connect Pd.05 to UART TX2 
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    
    // Connect Pd.06 to UART RX2
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

    //USART2_TX  Pd.05
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用推挽输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
    GPIO_Init(GPIOD, &GPIO_InitStructure);              //初始化Pd5  TXD

    //USART2_RX   Pd.06
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
    GPIO_Init(GPIOD, &GPIO_InitStructure);              //初始化Pd6 RXD

  

   //USART 初始化设置
    USART_InitStructure.USART_BaudRate = bound;                     //一般设置为2400;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;     //字长为9位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;          //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_Even;           //偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式

    USART_Init(USART2, &USART_InitStructure);               //初始化串口

   //Usart2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;      //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                         //根据指定的参数初始化VIC寄存器
//  USART_HalfDuplexCmd(USART2,ENABLE);                     //设置为半双工
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);          //开启中断
//  USART_ITConfig(USART2, USART_IT_TXE, ENABLE);           //开启中断
//  USART_ClearFlag(USART2,USART_FLAG_TC);
//  USART_ITConfig(USART2, USART_IT_TC,ENABLE);             //开启发送中断
    USART_Cmd(USART2, ENABLE);                              //使能串口 
//  USART_ClearITPendingBit(USART2, USART_IT_TC);

}
/*=========================================================================================\n
* @function_name: Uart2_Dy10ms
* @function_file: Uart2.c
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
void Uart2_Dy10ms(void)
{
    guc_DyUart2Over--;
      if(0x00== guc_DyUart2Over)
      {
          Sys_InitUart2(9600);
          ComBom_Init(ComIndex_Uart2);    //将uart6 端口对应的 COM初始化
      }
      
     /* if(guc_BodeDely>0)                  //波特率切换
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
* @描述: 串口2中断
* 
* @参数: 
* @返回: 
* @作者:   lwb (2013-07-04)
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void USART2_IRQHandler(void)
{  
    //接收中断
//    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
//    {
//        USART_ClearFlag(USART2,USART_FLAG_ORE);        //读SR
//        USART_ReceiveData(USART2);                                //读DR
//    }
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        Uart2_Receive();
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    }     
    //发送中断
    if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
    {
         Uart2_Transmit();
         USART_ClearITPendingBit(USART2, USART_IT_TC);
    }    
}
/*=========================================================================================\n
* @function_name: Uart2_Receive
* @function_file: Uart2.c
* @描述: UART4接收,在中断中调用该函数
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Uart2_Receive(void)
{
    guc_DyUart2Over = Const_DyUart2Over;//端口超时保护
    
       //处于空闲状态或已经处于uart接收状态
       gs_ComGroup[ComIndex_Uart2].ucPort   = Port_Uart2;
    
       gs_ComGroup[ComIndex_Uart2].ucRecvTmr = Const_DyRvPack;     //设置数据包接收超时时间
    
       if(gs_ComGroup[ComIndex_Uart2].ucStt == ComStt_Idle)        //当前是空闲的状态的话，判断是否是帧头68
       {
           if(((uint8)USART_ReceiveData(USART2))==COMHEAD)               //接受到起始字节
           {
               gs_ComGroup[ComIndex_Uart2].ucStt = ComStt_Recv;
           }
       }
    
       if(gs_ComGroup[ComIndex_Uart2].ucStt == ComStt_Recv)            //这个判断是防止发送的时候有接收中断进入的可能
       {
           if(gs_ComGroup[ComIndex_Uart2].ucLen < Const_MaxBufLen)     //判断 com中的buf是否溢出
           {                                                           //防止缓存溢出
               gs_ComGroup[ComIndex_Uart2].ucBuf[gs_ComGroup[ComIndex_Uart2].ucLen++] = (uint8)USART_ReceiveData(USART2);//数据存入缓冲区，指针加加
    
               if(gs_ComGroup[ComIndex_Uart2].ucLen>COMINITLEN)
               {
                   if(gs_ComGroup[ComIndex_Uart2].ucBuf[7]!=COMHEAD)           //如果第7个不是68返回错误
                   {
                       gs_ComGroup[ComIndex_Uart2].ucStt=ComStt_Idle;
                   }else if(gs_ComGroup[ComIndex_Uart2].ucLen>=(gs_ComGroup[ComIndex_Uart2].ucBuf[9]+COMINITLEN+2))   //长度符合要求
                   {
                       if(((uint8)USART_ReceiveData(USART2))==COMTAIL)
                       {
                           guc_ComSendDy=3;                                        //延时30ms发送
                           gs_ComGroup[ComIndex_Uart2].ucStt=ComStt_WaitProc;
                           gui_SystemEvent |= flgEtGlb_NeedPrtclChk;               // 设置标志
                           gs_ComGroup[ComIndex_Uart2].ucRecvTmr=0;
                           USART_ITConfig(USART2, USART_IT_RXNE,DISABLE);          //关闭中断
                           //关闭接收
                       }
                   }
               }
           }
       }

}
/*=========================================================================================\n
* @function_name: Uart2_Transmit
* @function_file: Uart2.c
* @描述: UART2发送,中断中调用
*
* @参数:
* @返回:
* @作者:   lwb (2012-03-28)
* @备注:
*-------------------------------------------------------------------------------------------
* @修改人:
* @修改内容:
===========================================================================================*/
void Uart2_Transmit(void)
{
   guc_DyUart2Over = Const_DyUart2Over;//端口超时保护

    if(gs_ComGroup[ComIndex_Uart2].ucPort   == Port_Uart2)
    {   
        //处于空闲状态或已经处于uart接收状态
        if(gs_ComGroup[ComIndex_Uart2].ucStt == ComStt_Send)
        {
            if(gs_ComGroup[ComIndex_Uart2].ucPos >= gs_ComGroup[ComIndex_Uart2].ucLen)
            {   
                //发送完毕,初始化通讯控制状态
                USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);          //开启中断
                USART_ITConfig(USART2, USART_IT_TC,DISABLE);            //关闭发送中断
                ComBom_Init(ComIndex_Uart2);//将uart2 端口对应的 COM初始化
            }else
            {
                
                if(gs_ComGroup[ComIndex_Uart2].ucFrmHeadCnt<4)
                {                    
                    USART_SendData(USART2,0xFE);    //发送数据
                    gs_ComGroup[ComIndex_Uart2].ucFrmHeadCnt++;
                }else
                {
                    USART_SendData(USART2,gs_ComGroup[ComIndex_Uart2].ucBuf[gs_ComGroup[ComIndex_Uart2].ucPos++]);    //发送数据
                }
            }
        }else
        {   //如果在发送中断标记中，进入但是状态不正确，则初始化
            ComBom_Init(ComIndex_Uart2);//将uart3 端口对应的 COM初始化
        }
    }
}



