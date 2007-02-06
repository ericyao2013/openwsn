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
#ifndef _TIMER_H_6828_
#define _TIMER_H_6828_

/******************************************************************************
 * @author zhangwei on 2006-08-11
 * Timer 
 * This object is the software mapping of MCU's hardware timer object.
 * attention that TTimer is not the 1:1 mapping of hardware timer. actually, one 
 * TTimer object corrspond to 1 separate channel of hardware timer.
 * 
 * @history
 * @modified by zhangwei 2006-09-05
 * @modified by tangwen on 2006-10-21
 * 	add channel parameter to construct function.
 *  and other modifications to support the channel(sub-timer).
 *
 * @modified by Shanlei on 200611
 * 
 *  timer����ͨ�������汾˵�����£�
 *  timer_setvalue �� timer_setcaptureΪ�պ�����
 *  timer_configure��priority������ʾ��timer�����Ƿ�ʹ���жϷ�ʽ��priority
 * Ϊ0-15��ʾʹ���жϷ�ʽ��>15��ʾ��ʹ���жϡ�
 *  timer_VICinitֻ�Ǹ���timer�����id �� priority ������VIC��ַ��priority
 * ��ֵֻ��Ϊ0-15������Χ�������ַ���˺�����timer_configure�����е��ã���
 * timer_configure�Ĳ���priority��0-15֮�䡣ԭ������timer��VIC��ַΪͬһ��
 *��timer_interrupt���������Է��֣�������timerͬʱ�����жϵ�ʱ����Ϊʹ��ͬһ
 * ��ַ���������ͻ������Ϊÿһ��timer����һ��VIC��ַ��Timer0_Int for timer0 
 * and Timer1_Int for timer1����������ֳ�ͻ��
 *  ��timer_VICinit�з����timer_VICenable��timer_VICdisable��������timer��
 * ʹ�ܻ��ֹ��������������timer_start��timer_stop�и���timer->state�����á�
 *  timer_setinterval�ļ�ʱ��С�����Ϊ1ms������interval��ֵ����ms����
 * repeat���ָܻ���repeatΪ0��ֻƥ��һ�Σ�repeatΪ1������ƥ�䡣
 *  timer_enable��timer_disable�Ƕ�timer��ÿ���Ƚ�ͨ���жϵ�ʹ�ܻ��ֹ����
 * ʹ��timer_expired�Ĳ�ѯ��ʽ�£�Ҳ��Ҫʹ�ܱȽ�ͨ�����жϣ��Ա�����жϱ�־��
 * ��ˣ�����������������Ϊtimer����ʹ���жϷ�ʽ�ı��֣����ǽ����ı�timer-
 * >state��
 *   timer_VICinit��timer_VICenable��timer_VICdisable��timer_enable��
 * timer_disable���ϲ㶼�ǲ��ɼ��ġ�
 *
 * modified by zhangwei on 200612
 * timer��������ֹͣ����ʹ��start/stop����Ҫʹ��disable��enable
 *
 *****************************************************************************/
 
#include "hal_foundation.h"

/* TTimer Object
 *  id		timer hardware id. usually it is 0, 1, 2. it depends on hardware
 *  channel	each sub-timer in timer hardware. usually 0-3
 *	state	bit 7 - bit 0
 *			bit 0: enabled or not
 *			bit 1: period trigger or not
 *			bit 2: raise interrupt or not
 *	data	used as the parameter for the timer callback function
 */
#define TIMER_STATE_ENABLED 	0x01
#define TIMER_STATE_PERIOD	 	0x02
#define TIMER_STATE_INTERRUPT 	0x04
#define TIMER_STATE_CAPTURE 	0x08

typedef struct{
  uint8 id;
  uint8 state;
  uint32 interval;
  void * data;
  TEventHandler callback;
  uint8 priority;
  uint32 lasttime;
  // @TODO 20061010
  // ����Ϊ�ƻ������Timer�ṹ�������ṹҪʵ�ֹ鲢������һ��Ϊ��  
  uint8 enabled;
  uint16 ctrl;
  uint16 init;
  void (* hhcallback)(void *);
}TTimer;

/* global variables define in "hal_global.c". They will be initialized in  
 * hal_global_construct(). */

extern TTimer * g_timer0;
extern TTimer * g_timer1;
extern TTimer * g_timer2;

/* This macro defines how many clocks the Timer run in one milli-seconds.
 * since we often use milli-seconds when dealing with scheduling and communication
 * problems, this macro is quite good for such conversion.
 */
#define TIMER_CLOCKS_PER_MILLISECOND  (Fpclk / 1000)
#define timer_msecof(clocks) (clocks/TIMER_CLOCKS_PER_MILLISECOND)
#define timer_clocksof(ms) (ms*TIMER_CLOCKS_PER_MILLISECOND)

TTimer* timer_construct( char * buf, uint8 size );
void   timer_destroy( TTimer * timer );
void   timer_init( TTimer * timer, uint8 id, uint8 channel );
void   timer_configure( TTimer * timer, TEventHandler callback, void * data, uint8 priority );
void   timer_setvalue( TTimer * timer, uint32 value );
uint32 timer_getvalue( TTimer * timer );
void   timer_setinterval( TTimer * timer, uint32 interval, uint8 repeat );
void   timer_setcapture( TTimer * timer, uint8 opt );
void   timer_start( TTimer * timer );
void   timer_stop( TTimer * timer );
void   timer_restart( TTimer * timer, uint32 interval, uint8 repeat );
boolean timer_expired( TTimer * timer );
uint32 timer_elapsed( TTimer * timer );
void   timer_enable( TTimer * timer );
void   timer_disable( TTimer * timer );
uint32 timer_clocksperms( TTimer * timer );
void   timer_VICinit( TTimer * timer );
void   timer_VICenable( TTimer * timer);
void   timer_VICdisable( TTimer * timer);

void __irq Timer0_Int (void);
void __irq Timer1_Int (void);

  

// @TODO
// ����Ϊ�ƻ��ṩ���´��� 20061010
// these are obsolete source code! you should not use them in the future!
//
// should be integrated with the above in the near future
// to support one timer object only



void INTERRUPT_LPC_TIMER0_T0MR0(void);
void INTERRUPT_LPC_TIMER0_T0MR2(void);
void INTERRUPT_LPC_TIMER0_CAPT(void);


#define MR0IF 0
#define MR1IF 1
#define MR2IF 2
#define MR3IF 3
#define CR0IF 4
#define CR1IF 5
#define CR2IF 6
#define CR3IF 7
  
#define CAP0RE  0
#define CAP0FE  1
#define CAP0I   2
#define CAP1RE  3
#define CAP1FE  4
#define CAP1I   5
#define CAP2RE  6
#define CAP2FE  7
#define CAP2I   8

#define MR0I  0
#define MR0R  1
#define MR1I  3
#define MR1R  4
#define MR2I  6
#define MR2R  7

void LPC_TIMER0_STOP(void);
void LPC_TIMER0_START(void);
void LPC_TIMER0_INIT(void);
void LPC_TIMER0_SET_T0MR0_VALUE(DWORD value);
void LPC_TIMER0_SET_T0MR1_VALUE(DWORD value);
void LPC_TIMER0_SET_T0MR2_VALUE(DWORD value);
void LPC_TIMER0_CLEAR_T0MR0(void);
void LPC_TIMER0_CLEAR_T0MR1(void);
void LPC_TIMER0_CLEAR_T0MR2(void);

void DISABLE_LPC_TIMER0_T0MR0_INT(void);
void ENABLE_LPC_TIMER0_T0MR0_INT(void);
void DISABLE_LPC_TIMER0_T0MR2_INT(void);
void ENABLE_LPC_TIMER0_T0MR2_INT(void);
void DISABLE_LPC_TIMER0_T0CAP2_INT(void);
void ENABLE_LPC_TIMER0_T0CAP2_INT(void);
void ENABLE_LPC_TIMER0_INT(void);
void DISABLE_LPC_TIMER0_INT(void);


#endif /* _TIMER_H_6828_ */
