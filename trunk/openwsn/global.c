//----------------------------------------------------------------------------
// @author zhangwei on 2006-07-25
// Global Variables with initialization and finalization
//
// 	This module is very important in the whole application. It does not only 
// declares and initializes all the global objects, but also create the 
// relationships between different objects.
// 	In TinyOS and nesC systems, these relationships are constructed by a separate
// Configurator object. As you have seem here, this is not necessary. You should
// understand and adopt the OOP programming model and use it in such conditions.
//  
//----------------------------------------------------------------------------
// @history
// @modified by zhangwei on 2006-08-14
//	将TWirelessComm后面的SafeBuf区域从256 byte减少到128 Byte.以减小内存SRAM消耗。
//
//----------------------------------------------------------------------------

#include "foundation.h"
#include "global.h"

static TSioComm 			m_sio;
static TWirelessComm 		m_wireless;
static TConsole *			m_console;
static TConfigure 			m_configure;
static TActionScheduler  	m_actsche;
static TDebugIo 			m_debugio;

TActionScheduler *			g_actsche = NULL;
TSioComm *					g_sio = NULL;
TWirelessComm *				g_wls = NULL;
TConsole *					g_console = NULL;
TConfigure * 				g_config = NULL;
TDebugIo * 					g_debugio = NULL;


// an global variable to record whether the system is successfully initialized.
uint8 						g_init = FALSE;

// @attention
//	though the debugio_xxx() used the g_debugio object, it has already declared
// in the debugio module to facilitate programming. so you need NOT declare 
// g_debugio here.
//
// @return
//	0:	success
//	-1 or other negtive:	failed
//
int8 global_construct( void )
{
	TConfigStore * data;
	g_init = TRUE;
	
	if (!hal_global_construct())
		return -1;
	
	g_sio 		= sio_construct( (char*)(&m_sio), sizeof(TSioComm), g_uart1, SIO_OPTION_FRAME_SUPPORT );
	//g_console 	= console_construct( (char*)(&m_console), sizeof(TConsole), g_uart0 );
	g_config 	= config_construct( (char*)(&m_configure), sizeof(TConfigure), g_console );
	//g_actsche 	= acts_construct( (char*)(&m_actsche), sizeof(TActionScheduler) );
	//g_wls 		= wls_construct( (char*)(&m_wireless), sizeof(TWirelessComm), g_cc2420, g_actsche );
	g_debugio 	= debug_construct( (char*)(&m_debugio), sizeof(TDebugIo), g_debuguart );
	/*
	if ( (g_debuguart == NULL) || (g_sio == NULL) || (g_wls == NULL)
		|| (g_console == NULL) || (g_config == NULL) || (g_debugio == NULL))
	{
		g_init = FALSE;
		assert( FALSE );
	}
	
	// after constructing all the global objects successfully and link them 
	// successfully, you should do more work to configure them to work.
	// here is a good place for you to check and start the hardware.
	//
	if (g_init)
	{
		config_load( g_config, CONFIG_FLASHSTORE_ADDR, CONFIG_FLASHSTORE_SIZE );
		data = &(g_config->data);
		
		spi_configure( g_spi0 );
		spi_configure( g_spi1 );
		cc2420_configure( g_cc2420, CC2420_CONFIG_PANID, data->panid, 1 );
 		cc2420_configure( g_cc2420, CC2420_CONFIG_LOCALADDRESS, data->localaddress, 2 );
		cc2420_configure( g_cc2420, CC2420_CONFIG_TUNNING_POWER, (uint16) data->wlspower, 1 );
		cc2420_configure( g_cc2420, CC2420_CONFIG_CHANNEL, (uint16) data->wlschannel, 1 );
		cc2420_configure( g_cc2420, CC2420_CONFIG_APPLY, NULL, 0 );
		timer_configure( g_timer0, NULL, NULL, 1 );
		timer_configure( g_timer1, NULL, NULL, 1 );
		timer_configure( g_timer2, NULL, NULL, 1 );
		uart_configure( g_uart0, data->uart_baudrate, data->uart_databits, data->uart_stopbits, data->uart_parity, 0x01 );
		uart_configure( g_uart1, 9600, 8, 1, 0, 0x01 );
		watchdog_configure( g_watchdog, 0x00 );
				
		acts_configure( g_actsche, g_timer1, acts_harddrive_default_timer_callback );
		// sio_configure( g_sio, )
		// wls_configure()
		// console_configure()
		// debug_configure()
		
		#ifdef GDEBUG
		watchdog_disable( g_watchdog );
		#endif
		#ifndef GDEBUG
		watchdog_enable( g_watchdog );
		#endif
	}
	*/
	return (g_init == TRUE) ? 0 : -1;
}

int8 global_destroy( void )
{
	debug_destroy( g_debugio );
	config_destroy( g_config );
	console_destroy( g_console );
	wls_destroy( g_wls );
	sio_destroy( g_sio );
	acts_destroy( g_actsche );
	hal_global_destroy();
	return 0;
}
