#ifndef __SPI_H__
    #define __SPI_H__

#ifndef SPIEXT
    #define SPIEXT extern
#endif


#define BUFFSIZE 128*6*5
//#define BUFFSIZE 128*6

SPIEXT uint16 SPI1_Buffer_Rx[BUFFSIZE*2];
SPIEXT int16 SPI1_SaveBuf_Rx[3][5][128][6];
SPIEXT int16 DMA_Buf[128*5];
//SPIEXT int32 DMA_result[BUFFSIZE];



void Initspi1(void);
void Initspi1_high(unsigned int SPI_BRP);
void Initspi2(void);
unsigned int SPI1Byte(unsigned int byte);
void SPI_Idle(void);
void set_data_cmd_flash(unsigned char cmd, unsigned int dat);
void spi_wr_flash(unsigned int addr, unsigned int dat_h, unsigned int dat_l);
unsigned long spi_rd_flash(unsigned int addr);
void set_data_cmd_flash_PC(unsigned char cmd, unsigned int dat,unsigned char cksum);
void DMA_Configuration(void);


#endif

