/*****************************************************************************
* This file is part of OpenWSN, the Open Wireless Sensor Network System.
*
* Copyright (C) 2005,2006,2007 zhangwei (openwsn@gmail.com)
* 
* OpenWSN is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free
* Software Foundation; either version 2 or (at your option) any later version.
* 
* OpenWSN is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
* for more details.
* 
* You should have received a copy of the GNU General Public License along
* with eCos; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
* 
* As a special exception, if other files instantiate templates or use macros
* or inline functions from this file, or you compile this file and link it
* with other works to produce a work based on this file, this file does not
* by itself cause the resulting work to be covered by the GNU General Public
* License. However the source code for this file must still be made available
* in accordance with section (3) of the GNU General Public License.
* 
* This exception does not invalidate any other reasons why a work based on
* this file might be covered by the GNU General Public License.
* 
****************************************************************************/ 
/***************************************************************************** 
 * @author zhangwei on 2006-07-20
 * TSPIDriver 
 *
 * @modified by zhangwei on 20061010
 * based on Huanghuan's mature code. 
 * support multiple SPI channel.
 * 
 ****************************************************************************/
#include "hal_spi.h"
#include "hal_led.h"


TSpiDriver * spi_construct( uint8 id, char * buf, uint8 size )
{
	TSpiDriver * spi;
	
	if (sizeof(TSpiDriver) <= size)
		spi = (TSpiDriver *)buf;
	else
		spi = NULL;
		
	if (spi != NULL)
	{
		memset( buf, 0x00, size );
		spi->id = id;
	}
	
	// @modified by zhangwei on 20061025
	//
	// I delete spi_configure(spi)
	// this line is in huanghuan's code 
	
	return spi;
}

void spi_destroy( TSpiDriver * spi )
{
}

/* configure the SPI here. for example, the interrupt settings 
 */
void spi_configure( TSpiDriver * spi )
{
        if (spi->id == 0)
        {
          PINSEL0 = (PINSEL0 & 0xffff00ff) | 0x00005500;
          SPI_SPCCR = 0x52;		               // ����SPIʱ�ӷ�Ƶ
 	  SPI_SPCR  = (0 << 3) |		       // CPHA = 0, ������SCK �ĵ�һ��ʱ���ز���
 			        (0 << 4) |	       // CPOL = 0, SCK Ϊ����Ч
 			        (1 << 5) |	       // MSTR = 1, SPI ������ģʽ
 			        (0 << 6) |	       // LSBF = 0, SPI ���ݴ���MSB (λ7)����
 			        (0 << 7);	       // SPIE = 0, SPI �жϱ���ֹ
        }
        
        
        
        else if (spi->id == 1)
        {
        
          PINSEL1 = (PINSEL1 & (~(0xFF << 2))) | (0xAA << 2);
        
          SSPCR0 = (0x01 << 8) |              // SCR  ����SPIʱ�ӷ�Ƶ
             (0x00 << 7) |              // CPHA ʱ�������λ,��SPIģʽ��Ч 
             (0x00 << 6) |              // CPOL ʱ���������,��SPIģʽ��Ч
             (0x00 << 4) |              // FRF  ֡��ʽ 00=SPI,01=SSI,10=Microwire,11=����
             (0x07 << 0);               // DSS  ���ݳ���,0000-0010=����,0011=4λ,0111=8λ,1111=16λ

          SSPCR1 = (0x00 << 3) |              // SOD  �ӻ��������,1=��ֹ,0=����
             (0x00 << 2) |              // MS   ����ѡ��,0=����,1=�ӻ�
             (0x01 << 1) |              // SSE  SSPʹ��,1=����SSP�������豸ͨ��
             (0x00 << 0);               // LBM  ��дģʽ
             
          SSPCPSR = 0x52;                     // PCLK��Ƶֵ
          //SSPIMSC = 0x07;                     // �ж����μĴ���
          SSPICR  = 0x03;                     // �ж�����Ĵ��� 
        }
	
}

uint8 spi_read(TSpiDriver * spi, char * buf, uint8 capacity, uint8 opt )
{
        UINT8 spiCnt = 0;
        
        for (spiCnt = 0; spiCnt < capacity; spiCnt++) { 
            spi_get(spi,buf + spiCnt); 
        } 
       
        return 0;
}

uint8 spi_write(TSpiDriver * spi,  char * buf, uint8 len, uint8 opt )
{
        uint8 spiCnt = 0;
        for (spiCnt = 0; spiCnt < len; spiCnt++) { 
            spi_put(spi,((BYTE*)(buf))[spiCnt]); 
        } 
      
        return 0;
}

void spi_put(TSpiDriver * spi, char ch )
{
        if (spi->id == 0)
        {
          SPI_SPDR = ch; 
          spi_wait(spi);
        }
        else if (spi->id == 1)
        {
          SSPDR = ch;
          spi_wait(spi);	
        }
}

int8 spi_get(TSpiDriver * spi, char * pc )
{
        if(spi->id == 0)
        {
          SPI_SPDR = 0; 
          spi_wait(spi); 
          *pc = SPI_SPDR; 
        }
        if(spi->id == 1)
        {
          SSPDR = 0;
          spi_wait(spi);
          *pc = SSPDR; 	
        }
        
    return 0;
}


void spi_wait(TSpiDriver * spi) 
{ 
        if (spi->id == 0) 
        {	
        	while (!(SPI_SPSR & BM(7)));	
    	}
        else if(spi->id == 1) 
        {
        	while (!(SSPSR & 0x01));
        }
}


void FASTSPI_TX_WORD_LE(TSpiDriver * spi, uint16 x) 
    { 
        spi_put(spi,x); 
        spi_put(spi,(x) >> 8); 
    }
    
void FASTSPI_TX_WORD(TSpiDriver * spi, uint16 x) 
    { 
        spi_put(spi,(x) >> 8); 
        spi_put(spi,(BYTE)(x)); 
    }
    

void FASTSPI_RX_WORD_LE(TSpiDriver * spi, uint16 *x) 
    { 
    	char temp;
    	spi_get(spi,&temp);
    	*x = (WORD)temp;
    	spi_get(spi,&temp);
    	*x |= ((WORD)temp) << 8;
        //spi_put(spi,0);
        //*x = (WORD)SPI_SPDR; 
        //spi_put(spi,0);
        //*x |= ((WORD)(SPI_SPDR)) << 8; 
    } 

void FASTSPI_RX_WORD(TSpiDriver * spi,uint16 *x) 
    { 
        char temp;
    	spi_get( spi, &temp );
    	*x = ((WORD)temp) << 8;
    	spi_get(spi,&temp);
    	*x |= ((WORD)temp);
        //spi_put(0); 
        //*x = ((WORD)SPI_SPDR) << 8; 
        //spi_put(0);
        //*x |= (WORD)SPI_SPDR; 
    } 

