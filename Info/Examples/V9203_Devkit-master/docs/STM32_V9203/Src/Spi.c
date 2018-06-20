#define SPIEXT
#include "Includes.h"


/*=========================================================================================\n
* @function_name: Initspi1
* @function_file: Spi.c
* @����: 
* 
* @����: 
* @����: 
* @����:   Vango Application Teams
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Initspi1(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); 
    //ʹ��SPI1ʱ��
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

        SPI_Cmd(SPI1, DISABLE); 												//�����Ƚ���,���ܸı�MODE
        SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//����ȫ˫��
        SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;							//������
        SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;						//16λ
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;								//CPOL=1 ʱ�����ո�
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;							//CPHA=1 ���ݲ����2��
        SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//���NSS
       // SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;	//256��Ƶ,SPCK��ʼ�ٶ�200K����
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;						//��λ��ǰ
        SPI_InitStructure.SPI_CRCPolynomial = 7;								//CRC7

        SPI_Init(SPI1, &SPI_InitStructure);
        //SPI_SSOutputCmd(SPI1, ENABLE);

        //SPI_Cmd(SPI1, ENABLE); 																					
}

/*=========================================================================================\n
* @function_name: Initspi1_high
* @function_file: Spi.c
* @����: 
* 
* @����: 
* @����: 
* @����:   Vango Application Teams
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/

void Initspi1_high(unsigned int SPI_BRP)
{
        SPI_InitTypeDef  SPI_InitStructure;

        //ʹ��SPI1ʱ��
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

        SPI_Cmd(SPI1, DISABLE); 												//�����Ƚ���,���ܸı�MODE
        SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//����ȫ˫��
        SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//��
        SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						//8λ
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;								//CPOL=1 ʱ�����ո�
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;							//CPHA=1 ���ݲ����2��
        SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//���NSS
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BRP;	//128��Ƶ,SPCK��ʼ�ٶ�200K����
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;						//��λ��ǰ
        SPI_InitStructure.SPI_CRCPolynomial = 7;								//CRC7

        SPI_Init(SPI1, &SPI_InitStructure);
        SPI_SSOutputCmd(SPI1, ENABLE);

        SPI_Cmd(SPI1, ENABLE); 																					
}

/*=========================================================================================\n
* @function_name: Initspi2
* @function_file: Spi.c
* @����: 
* 
* @����: 
* @����: 
* @����:   Vango Application Teams
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Initspi2(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    //ʹ��SPI2ʱ��
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



    SPI_Cmd(SPI2, DISABLE); 												//�����Ƚ���,���ܸı�MODE
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//����ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//��
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						//8λ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;								//CPOL=1 ʱ�����ո�
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;							//CPHA=1 ���ݲ����1��
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//���NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//64��Ƶ
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;						//��λλ��ǰ
    SPI_InitStructure.SPI_CRCPolynomial = 7;								//CRC7

    SPI_Init(SPI2, &SPI_InitStructure);
    SPI_SSOutputCmd(SPI2, ENABLE);
    SPI_Cmd(SPI2, ENABLE); 
		
}

/*=========================================================================================\n
* @function_name: Initspi3
* @function_file: Spi.c
* @����: 
* 
* @����: 
* @����: 
* @����:   Vango Application Teams
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
//===========SPI��д����==============
unsigned int SPI1Byte(unsigned int byte)
{
	/*�ȴ����ͼĴ�����*/
	while((SPI1->SR & SPI_I2S_FLAG_TXE)==RESET);
    /*����һ���ֽ�*/
	
//	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	SPI1->DR = byte;
	/* �ȴ����ռĴ�����Ч*/
	while((SPI1->SR & SPI_I2S_FLAG_RXNE)==RESET);
//	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	return(SPI1->DR);
	
}


//===========������============
unsigned int SPI2Byte(unsigned int byte)
{
	/*�ȴ����ͼĴ�����*/
	while((SPI2->SR & SPI_I2S_FLAG_TXE)==RESET);
    /*����һ���ֽ�*/
	SPI2->DR = byte;
	/* �ȴ����ռĴ�����Ч*/
	while((SPI2->SR & SPI_I2S_FLAG_RXNE)==RESET);
	return(SPI2->DR);
}


//unsigned int SPI3Byte(unsigned int byte)
//{
//	assert_param(IS_SPI_ALL_PERIPH(SPI3));

	/*�ȴ����ͼĴ�����*/
//	while((SPI3->SR & SPI_I2S_FLAG_TXE)==RESET);
    /*����һ���ֽ�*/
//	SPI3->DR = byte;
	/* �ȴ����ռĴ�����Ч*/
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
    
    DMA_DeInit(DMA1_Channel2);                                                  //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr = SPI1_DR_Address;//(u32)&(SPI1->DR); ;                //ԴͷBUF ���� (&(USART1->DR)) 
    DMA_InitStructure.DMA_MemoryBaseAddr =(uint32)SPI1_Buffer_Rx;                //Ŀ��BUF ����Ҫд���ĸ�������֮��
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //������Դͷ//��������Ϊ���ݴ����Ŀ�ĵػ�����Դ
    DMA_InitStructure.DMA_BufferSize = BUFFSIZE*2;                                        //DMA����Ĵ�С ��λ���±��趨
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;               //�����ַ�Ĵ���������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                        //�ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//HalfWord;        //�����ֽ�Ϊ��λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//HalfWord;            //�ڴ��ֽ�Ϊ��λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                                //������ѭ������ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                            //4���ȼ�֮һ��(������) VeryHigh/High/Medium/Low
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                  //���ڴ浽�ڴ�
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);                                    //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��1�Ĵ���
    //DMA�ж�����//
    DMA_ITConfig(DMA1_Channel2,(DMA_IT_TC | DMA_IT_TE), ENABLE);			     //DMA2ͨ��1 spi_rx ȫ�������������ж�
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

