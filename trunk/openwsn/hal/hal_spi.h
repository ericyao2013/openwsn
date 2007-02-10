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
#ifndef _SPI_H_4289_
#define _SPI_H_4289_

/***************************************************************************** 
 * @author zhangwei on 2006-07-20
 * TSPIDriver 
 *
 * @modified by zhangwei on 20061010
 * based on Huanghuan's mature code. 
 * support multiple SPI channel.
 * 
 ****************************************************************************/

#include "hal_foundation.h"
#include "hal_cpu.h"

typedef struct{
  uint8 id;
}TSpiDriver;

/* global variables defined in global.* */
extern TSpiDriver * g_spi0;
extern TSpiDriver * g_spi1;

TSpiDriver * spi_construct( uint8 id, char * buf, uint8 size );
void  spi_destroy( TSpiDriver * spi );
void  spi_configure( TSpiDriver * spi );

uint8 spi_read( TSpiDriver * spi, char * buf, uint8 capacity, uint8 opt );
uint8 spi_write( TSpiDriver * spi, char * buf, uint8 len, uint8 opt );
void  spi_put( TSpiDriver * spi, char ch );
int8  spi_get( TSpiDriver * spi, char * pc );
void  spi_wait( TSpiDriver * spi );

/* @attention
 * obsolete functions. pls use the above function instead
 */
void FASTSPI_TX_WORD_LE(TSpiDriver * spi, uint16 x) ;
void FASTSPI_TX_WORD(TSpiDriver * spi, uint16 x); 
void FASTSPI_RX_WORD_LE(TSpiDriver * spi, uint16 *x); 
void FASTSPI_RX_WORD(TSpiDriver * spi, uint16 *x); 


#endif /*_SPI_H_*/