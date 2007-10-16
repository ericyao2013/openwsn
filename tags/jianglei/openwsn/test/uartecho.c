/*****************************************************************************
 * This file is part of OpenWSN, the Open Wireless Sensor Network System.
 *
 * Copyright (C) 2005,2006,2007,2008 zhangwei (openwsn@gmail.com)
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

#include "..\foundation.h"
#include "..\hal\hal.h"
#include "..\global.h"
#include "uartecho.h"

/*****************************************************************************
 * @author zhangwei on 2006-07-25
 *
 * ������ֻ����ARM 7�ڲ�������UART�������ÿһ���յ����ַ�ԭ�ⲻ�����ͻ�ȥ��
 * ��ͨ�������նˡ����ڵ������ֵȲ��Ա�����Ч��
 * 
 * @state
 * test passed
 * 
 ****************************************************************************/ 

#define UART_BUF_SIZE 256

void uartecho_run( void )
{

	char memory[UART_BUF_SIZE * 2];
	char * buf1 = (char *)( &memory[0] );
	char * buf2 = buf1 + UART_BUF_SIZE;
	uint8 rxlen1, rxlen2, count;
	char * msgboot = "uartecho started...\n";
	char * inputmsg = "please input some word in the terminal and press <Enter>\n";

	target_init();
	global_construct();
	
	// make all the LED blank for 1 time to indicate the program is running!
	//
	led_off( LED_ALL );
	led_on( LED_ALL );
	hal_delay( 500 );
	led_off( LED_ALL );
    
	uart_write( g_uart, msgboot, strlen(msgboot)+1, 0x00 );
	uart_configure( g_uart, 9600, 0, 0, 0, 0 );
	uart_write( g_uart, inputmsg, strlen(inputmsg)+1, 0 );
	
	memset( buf1, 0x61, UART_BUF_SIZE ); 
	memset( buf2, 0x00, UART_BUF_SIZE ); 
	rxlen1 = 3;
	rxlen2 = 0;
	
	// do repeat accepting from the UART.
	// once receiving some data, send them back. that's why it was called "Echo".
	//
	while (1)
	{
		count = uart_write( g_uart, buf1, rxlen1, 0 );
	    if (count > 0)
		{
			rxlen1 -= count;
			memmove( buf1, buf1 + count, rxlen1 );
			//led_twinkle(LED_GREEN,3);
			//rxlen1 -= count;
		}
			
		/*count = uart_write( g_debuguart, buf2, rxlen2, 0 );
		if (count > 0)
		{
			rxlen2 -= count;
			memmove( buf2, buf2 + count, rxlen2 );
		}
		*/
		
		count = uart_read( g_uart, (char*)buf1 + rxlen1, UART_BUF_SIZE - rxlen1, 0 );
	    rxlen1 += count;
	    /* 
	    if (count>0) 
	    {   
	    	uart_write( g_uart, "xxxxxx", 6, 0 );
	        uart_putchar( g_uart, count);
	    }
	    */
	        
	    IRQEnable();
	        
	    //led_twinkle(LED_GREEN,3);
	    //led_twinkle(LED_GREEN,3);
	    //led_twinkle(LED_GREEN,3);
		
		//count = uart_read( g_debuguart, (char*)buf2 + rxlen2, UART_BUF_SIZE - rxlen2, 0 );
		//rxlen2 += count;
	}
		
	global_destroy();	
	
	return;
} 

