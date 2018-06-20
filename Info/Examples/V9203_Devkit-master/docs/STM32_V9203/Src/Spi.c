#define SPIEXT
#include "Includes.h"


/*=========================================================================================\n
* @function_name: Initspi1
* @function_file: Spi.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   Vango Application Teams
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Initspi1(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); 
    //使能SPI1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;   //| GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

        SPI_Cmd(SPI1, DISABLE); 												//必须先禁能,才能改变MODE
        SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//两线全双工
        SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;							//从器件
        SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;						//16位
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;								//CPOL=1 时钟悬空高
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;							//CPHA=1 数据捕获第2个
        SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//软件NSS
       // SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;	//256分频,SPCK初始速度200K以内
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;						//高位在前
        SPI_InitStructure.SPI_CRCPolynomial = 7;								//CRC7

        SPI_Init(SPI1, &SPI_InitStructure);
        //SPI_SSOutputCmd(SPI1, ENABLE);

        //SPI_Cmd(SPI1, ENABLE); 																					
}

/*=========================================================================================\n
* @function_name: Initspi1_high
* @function_file: Spi.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   Vango Application Teams
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/

void Initspi1_high(unsigned int SPI_BRP)
{
        SPI_InitTypeDef  SPI_InitStructure;

        //使能SPI1时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

        SPI_Cmd(SPI1, DISABLE); 												//必须先禁能,才能改变MODE
        SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//两线全双工
        SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//主
        SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						//8位
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;								//CPOL=1 时钟悬空高
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;							//CPHA=1 数据捕获第2个
        SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//软件NSS
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BRP;	//128分频,SPCK初始速度200K以内
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;						//高位在前
        SPI_InitStructure.SPI_CRCPolynomial = 7;								//CRC7

        SPI_Init(SPI1, &SPI_InitStructure);
        SPI_SSOutputCmd(SPI1, ENABLE);

        SPI_Cmd(SPI1, ENABLE); 																					
}

/*=========================================================================================\n
* @function_name: Initspi2
* @function_file: Spi.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   Vango Application Teams
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
void Initspi2(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    //使能SPI2时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_SPI2);



    SPI_Cmd(SPI2, DISABLE); 												//必须先禁能,才能改变MODE
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//两线全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//主
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						//8位
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;								//CPOL=1 时钟悬空高
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;							//CPHA=1 数据捕获第1个
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//软件NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//64分频
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;						//低位位在前
    SPI_InitStructure.SPI_CRCPolynomial = 7;								//CRC7

    SPI_Init(SPI2, &SPI_InitStructure);
    SPI_SSOutputCmd(SPI2, ENABLE);
    SPI_Cmd(SPI2, ENABLE); 
		
}

/*=========================================================================================\n
* @function_name: Initspi3
* @function_file: Spi.c
* @描述: 
* 
* @参数: 
* @返回: 
* @作者:   Vango Application Teams
* @备注: 
*-------------------------------------------------------------------------------------------
* @修改人:  
* @修改内容: 
===========================================================================================*/
//===========SPI读写函数==============
unsigned int SPI1Byte(unsigned int byte)
{
	/*等待发送寄存器空*/
	while((SPI1->SR & SPI_I2S_FLAG_TXE)==RESET);
    /*发送一个字节*/
	
//	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	SPI1->DR = byte;
	/* 等待接收寄存器有效*/
	while((SPI1->SR & SPI_I2S_FLAG_RXNE)==RESET);
//	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	return(SPI1->DR);
	
}


//===========从器件============
unsigned int SPI2Byte(unsigned int byte)
{
	/*等待发送寄存器空*/
	while((SPI2->SR & SPI_I2S_FLAG_TXE)==RESET);
    /*发送一个字节*/
	SPI2->DR = byte;
	/* 等待接收寄存器有效*/
	while((SPI2->SR & SPI_I2S_FLAG_RXNE)==RESET);
	return(SPI2->DR);
}


//unsigned int SPI3Byte(unsigned int byte)
//{
//	assert_param(IS_SPI_ALL_PERIPH(SPI3));

	/*等待发送寄存器空*/
//	while((SPI3->SR & SPI_I2S_FLAG_TXE)==RESET);
    /*发送一个字节*/
//	SPI3->DR = byte;
	/* 等待接收寄存器有效*/
//	while((SPI3->SR & SPI_I2S_FLAG_RXNE)==RESET);
//	SPI3->SR;
//	SPI3->DR;
//	return(SPI3->DR);
//}

/*void InitSPI_JTAG(void)
{
	SPI2Byte(0XFF);
	SPI1Byte(0XFF);	
}*/



/*****************************************************/
//older driver
/****************************************************/
static unsigned char tdo_m,tdo_c;
static unsigned int  tdo_d;
static unsigned char spi_err;

//======PC4=====
#define SPICS_H		//GPIO_SetBits(GPIOC,GPIO_Pin_4)		  
#define SPICS_L		//GPIO_ResetBits(GPIOC,GPIO_Pin_4)
//======PC2=====
#define SPIRST_H	//GPIO_SetBits(GPIOC,GPIO_Pin_2)	  
#define SPIRST_L	//GPIO_ResetBits(GPIOC,GPIO_Pin_2)
//======PC7=====
#define SPIDFT_H	//GPIO_SetBits(GPIOC,GPIO_Pin_7)		 
#define SPIDFT_L   	//GPIO_ResetBits(GPIOC,GPIO_Pin_7)
//======PA0====
#define SPIEER		//GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	  

//==========1us===========
static void Delayus(void)
{
	unsigned long i;
  	for(i=13;i>1;i--);
}
//==========xus============
static void Delayxus(unsigned long us)
{
   unsigned long i;
   for(i=0;i<us;i++)
   	Delayus();

}

void SPI_Idle(void)
{
	unsigned char i;
	for(i=0;i<4;i++)
		SPI1Byte(0xff);
}


void set_data_cmd_flash(unsigned char cmd, unsigned int dat)
{

    unsigned char cksum,cmdb;
    unsigned int send_dat;
    cmdb = (0x3f&cmd)|0x80;
    cksum = ~((dat&0x00ff) + (dat>>8) + cmdb);
 //	cksum = 0;
	send_dat=dat;
	tdo_m=SPI2Byte(cmdb);
	tdo_d=SPI2Byte((send_dat>>8));
	tdo_d=tdo_d<<8;
	tdo_d+=SPI2Byte((send_dat));
	tdo_c=SPI2Byte(cksum);	
}

void spi_wr_flash(unsigned int addr, unsigned int dat_h, unsigned int dat_l)
{
    set_data_cmd_flash(0x08,dat_l);
    set_data_cmd_flash(0x0a,dat_h);
    set_data_cmd_flash(0x0c,addr );
}

unsigned long spi_rd_flash(unsigned int addr)
{
    unsigned long mt_dout;

    set_data_cmd_flash(0x10, addr); 
    set_data_cmd_flash(0x12, addr);
    mt_dout = (unsigned long)tdo_d;
    if((unsigned char)(tdo_m+(unsigned char)(tdo_d&0x00ff)+(unsigned char)(tdo_d>>8))!=(unsigned char)(~tdo_c))
	{
        spi_err = 1;
    }
	else
	{
        spi_err = 0;
    }
    set_data_cmd_flash(0x14, addr);
    mt_dout += (((unsigned long)tdo_d)<<16);
    if((unsigned char)(tdo_m+(unsigned char)(tdo_d&0x00ff)+(unsigned char)(tdo_d>>8))!=(unsigned char)(~tdo_c))
	{
        spi_err = 1;
    }
	else
	{
        spi_err = 0;
    }
    
    return mt_dout;
}

void set_data_cmd_flash_PC(unsigned char cmd, unsigned int dat,unsigned char cksum)
{

    unsigned char cmdb;
    unsigned int send_dat;
    cmdb = (0x3f&cmd)|0x80;
 //   cksum = ~((dat&0x00ff) + (dat>>8) + cmdb);
 //	cksum = 0;
 	SPICS_H;
	Delayxus(2);
	SPICS_L;
	send_dat=dat;
	tdo_m=SPI1Byte(cmdb);
	tdo_d=SPI1Byte((send_dat>>8));
	tdo_d=tdo_d<<8;
	tdo_d+=SPI1Byte((send_dat));
	tdo_c=SPI1Byte(cksum);	
}


#define SPI1_DR_Address    ((uint32_t)0x4001300C)

void DMA_Configuration(void)
{
    DMA_InitTypeDef   DMA_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    DMA_DeInit(DMA1_Channel2);                                                  //将DMA的通道1寄存器重设为缺省值
    DMA_InitStructure.DMA_PeripheralBaseAddr = SPI1_DR_Address;//(u32)&(SPI1->DR); ;                //源头BUF 既是 (&(USART1->DR)) 
    DMA_InitStructure.DMA_MemoryBaseAddr =(uint32)SPI1_Buffer_Rx;                //目标BUF 既是要写在哪个个数组之中
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //外设作源头//外设是作为数据传输的目的地还是来源
    DMA_InitStructure.DMA_BufferSize = BUFFSIZE*2;                                        //DMA缓存的大小 单位在下边设定
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;               //外设地址寄存器不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                        //内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//HalfWord;        //外设字节为单位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//HalfWord;            //内存字节为单位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                                //工作在循环缓存模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                            //4优先级之一的(高优先) VeryHigh/High/Medium/Low
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                  //非内存到内存
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);                                    //根据DMA_InitStruct中指定的参数初始化DMA的通道1寄存器
    //DMA中断配置//
    DMA_ITConfig(DMA1_Channel2,(DMA_IT_TC | DMA_IT_TE), ENABLE);			     //DMA2通道1 spi_rx 全部传完或传输错误中断
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);                              
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
    
    // SPI_CalculateCRC(SPI3, DISABLE);
    //SPI_Cmd(SPI1, ENABLE);
    DMA_Cmd(DMA1_Channel2, ENABLE);
}

