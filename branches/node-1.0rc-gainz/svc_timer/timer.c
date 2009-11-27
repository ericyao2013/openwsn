/*******************************************************************************
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
 ******************************************************************************/ 

/*******************************************************************************
 * @attention
 * the "cc2420" chip has address identification mechanism built. if you want to 
 * use it as a sniffer frontier, you must turn off it. 
 *
 * @author zhangwei(TongJi University) on 20070423
 * 	- revision today.
 * @modified by yan-shixing(TongJi University) on 20091105
 *  - revision. 
 ******************************************************************************/ 

#include "../common/svc/svc_configall.h"
#include "../common/hal/hal_cpu.h"
#include "../common/hal/hal_timer.h"
#include "../common/hal/hal_debugio.h"
#include "../common/hal/hal_uart.h"
#include "../common/hal/hal_led.h"
#include "../common/hal/hal_assert.h"
#include "../common/svc/svc_foundation.h"
#include "../common/svc/svc_timer.h"

#define VTM_RESOLUTION 7

static TiTimerAdapter g_timeradapter;
static TiTimerManager g_vtm;

void vti_listener1( void * vtmptr, TiEvent * e );
void vti_listener2( void * vtmptr, TiEvent * e );

int main(void)
{
	TiTimerAdapter * timeradapter;
	TiTimerManager * vtm;
	TiTimer * vti1;
	TiTimer * vti2;

	target_init();
	wdt_disable();

	led_open();
	led_on( LED_ALL );
	hal_delay( 500 );
	led_off( LED_ALL );

	dbo_open(0, 38400);

	timeradapter = timer_construct( (void *)(&g_timeradapter), sizeof(g_timeradapter) );
	vtm = vtm_construct( (void*)&g_vtm, sizeof(g_vtm) );
	vtm = vtm_open( vtm, timeradapter, VTM_RESOLUTION );

	vti1 = vtm_apply( vtm );
	hal_assert( vti1 != NULL );
	vti_open( vti1, vti_listener1, vti1 );
	vti_setscale( vti1, 1 );
	vti_setinterval( vti1, 100, 0x01 );
	vti_start( vti1 );

	vti2 = vtm_apply( vtm );
	hal_assert( vti2 != NULL );
	vti_open( vti2, vti_listener2, vti2 );
	vti_setscale( vti2, 1 );
	vti_setinterval( vti2, 400, 0x01 );
	vti_start( vti2 );

	hal_enable_interrupts();
	while(1){}

	return 0;
}

/* This function is called by the virtual timer(vti) object */
void vti_listener1( void * vtmptr, TiEvent * e )
{
	led_toggle( LED_RED );
}

/* This function is called by the virtual timer(vti) object */
void vti_listener2( void * vtmptr, TiEvent * e )
{
	led_toggle( LED_YELLOW );
}


