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

#include "../foundation.h"
#include "../hal/hal_cc2420.h"
#include "../hal/hal_global.h"
#include "../service/svc_debugio.h"
#include "../global.h"
#include "sniffer.h"

/*****************************************************************************
 * @author zhangwei on 20070423
 * revision today.
 * 
 * the "cc2420" chip has address identification mechanism built. if you want to 
 * use it as a sniffer frontier, you must turn off it. 
 * 
 ****************************************************************************/ 

#define MAX_BUFFER_SIZE 0xFF

void sniffer_run( void )
{
	char rxbuf[MAX_BUFFER_SIZE];
	char * buf;
	uint8 rxlen, count;
	TSioComm sio;
	char * msg = "sniffer started...\n";
	
    target_init();
    global_construct();
	led_init();
    spi_configure( g_spi );
    uart_configure( g_uart, CONFIG_UART_BAUDRATE, 0, 0, 0, 0 );
    cc2420_configure( g_cc2420, CC2420_BASIC_INIT, 0, 0 );
    cc2420_configure( g_cc2420, CC2420_CONFIG_PANID, 0x2420, 0 );
    cc2420_configure( g_cc2420, CC2420_CONFIG_LOCALADDRESS, 0x5678, 0);

    //uart_write( g_uart, "program run1..", 15, 0 );
    // this configure will disable the address recognition mechanism so that 
    // the transceiver "cc2420" can receive all the frames in transmission, even
    // the destination address isn't the current node. 
	// hardware address recognition can be enabled or disabled using 
	// MDMCTRL0.ADR_DECODE bit in register CC2420_MDMCTRL0 (p.39, cc2420 datasheet) 
	//FAST2420_SETREG(spi,CC2420_MDMCTRL0,0x02E2) ;

    cc2420_configure( g_cc2420, CC2420_CONFIG_SNIFFER_MODE, 0, 0);
	
	sio_construct( (char*)&sio, sizeof(TSioComm), g_uart, 0x00 );
	sio_configure( &sio, NULL, 0x00, 27 );
    sio_write( &sio, msg, strlen(msg)+1, 0x00 );

	//cc2420_relation( g_spi0 );
	memset( (char*)(&rxbuf[0]), 0x00, MAX_BUFFER_SIZE ); 
	rxlen = 0;   
    cc2420_receive_on(g_cc2420);  
    IRQEnable();
		
	// assume: you should have construct g_cc2420, g_sio, g_uart successfully now
	// each time the sniffer received a frame, the LED will toggle its state	
	sio_write( &sio, msg, strlen(msg), 0x00 );
	while (TRUE)
	{
		led_toggle( LED_RED );
		count = cc2420_rawread( g_cc2420, buf, rxlen, 0 );
		if (count > 0)
		{
			sio_write(&sio, buf, count, 0 );
		}
	}
		
	global_destroy();	
	return;
}