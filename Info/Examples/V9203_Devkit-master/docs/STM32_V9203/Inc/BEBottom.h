#ifndef __BEBottom_H__
#define __BEBottom_H__

#define _SW_SE_Type     0

#define _SW_BE_Type  Const_AT24C512

#define Const_AT24C04_08_16     16
#define Const_AT24C64           64
#define Const_AT24C256          256
#define Const_AT24C512          512

//��������
#if(_SW_BE_Type==Const_AT24C04_08_16)
    #define BE_Const_DyClk      0       // ��ʱ10us��ʱ�䣨ÿ����λ��5us��
    #define BE_Const_DyWrite    10      // ��ʱ2ms��ʱ�䣨ÿ����λ��5us��
    #define BE_Const_Page       16      // ������E2��һҳ�����ֽ���
    #define BE_Const_ID         0xa0    // ������E2��������ַ
#elif(_SW_BE_Type==Const_AT24C64)
    #define BE_Const_DyClk      0       // ��ʱ10us��ʱ�䣨ÿ����λ��5us��
    #define BE_Const_DyWrite    40      // ��ʱ2ms��ʱ�䣨ÿ����λ��5us��
    #define BE_Const_Page       32      // ������E2��һҳ�����ֽ���
    #define BE_Const_ID         0xa0    // ������E2��������ַ
#else
    #define BE_Const_DyClk      1       // ��ʱ10us��ʱ�䣨ÿ����λ��5us��
    #define BE_Const_DyWrite    160      // ��ʱ5ms��ʱ�䣨ÿ����λ��5us�� 
    #define BE_Const_Page       64      // ������E2��һҳ�����ֽ���
    #define BE_Const_ID         0xa0    // ������E2��������ַ
#endif



//�˿ڶ���
#define BE_I2C_CLK_1()      {GPIOB->BSRRL = GPIO_Pin_10;} //CLK=1
#define BE_I2C_CLK_0()      {GPIOB->BSRRH = GPIO_Pin_10;} //CLK=0


#define BE_I2C_SDA_1()      {GPIOB->BSRRL = GPIO_Pin_11;GPIOB->MODER|=0x400000;} //SDA=1    
#define BE_I2C_SDA_0()      {GPIOB->BSRRH = GPIO_Pin_11;GPIOB->MODER|=0x400000;} //SDA=0 

#define BE_I2C_SDA_IN()     {GPIOB->MODER&=~0xC00000;} //���ó�����ģʽ
//#define SCL_read      GPIOB->IDR  & GPIO_Pin_0
#define BE_I2C_SDA_ST       (GPIOB->IDR  & GPIO_Pin_11)//(P9ID&BIT4)                                 //��ȡ�������ϵ��ź�


#define BE_WP_1()           {BE_I2C_SDA_1();BE_I2C_CLK_1();}
#define BE_WP_0()           {;}

uint8_t BE_WriteP(uint32_t address,uint8_t *buff,uint8_t Len);




uint8_t BE_ReadP(uint32_t address,uint8_t *buff,uint8_t Len);
uint8_t BE_ReadB(uint32_t address,uint8_t *buff);
uint8_t BE_ReadW(uint32_t address,uint16_t *buff);
void BE_I2C_Init(void);
void WREEPROM(void);
void Delay_Unit5us(void);
//#define Feed_WDT()     ;//CLRWDT() 


#endif


