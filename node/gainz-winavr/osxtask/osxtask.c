/*******************************************************************************
 * osxdemo
 * osx is an component based, lightweight operating system kernel. 
 * this program demostrates how to develop an complete program and separate runnable services.
 *
 * @status
 *	- release
 *
 * @author zhangwei(TongJi University) on 20090706
 *	- first created
 * @modified by zhangwei(TongJi University) on 20091006
 *	- finished osx kernel, this demo program and compile them passed
 * @modified by yan shixing(TongJi University) on 20091112
 *	- tested success.
 ******************************************************************************/

#include "asv_configall.h"
#include <string.h>
#include "../../common/openwsn/rtl/rtl_foundation.h"
#include "../../common/openwsn/rtl/rtl_debugio.h"
#include "../../common/openwsn/hal/hal_foundation.h"
#include "../../common/openwsn/hal/hal_cpu.h"
#include "../../common/openwsn/hal/hal_interrupt.h"
#include "../../common/openwsn/hal/hal_targetinit.h"
#include "../../common/openwsn/hal/hal_led.h"
#include "../../common/openwsn/hal/hal_assert.h"
#include "../../common/openwsn/hal/hal_timer.h"
#include "../../common/openwsn/hal/hal_debugio.h"
#include "../../common/openwsn/hal/hal_led.h"
#include "../../common/openwsn/osx/osx_kernel.h"
#include "../../common/openwsn/hal/hal_luminance.h"
#include "../../common/openwsn/hal/hal_rtc.h"
#include "../../common/openwsn/osx/osx_taskpool.h"
#include "../../common/openwsn/osx/osx_taskheap.h"
#include "asv_foundation.h"
#include "appsvc1.h"
#include "appsvc2.h"


#define CONFIG_AUTO_STOP
#undef  CONFIG_AUTO_STOP

#define CONFIG_DISPATCHER_TEST_ENABLE

#define CONFIG_UART_ID              0
#define CONFIG_TIMER_ID             1

TiAppService1                       m_svcmem1;
TiAppService2                       m_svcmem2;

//TiTimerAdapter                      m_timer;
   uint16                            g_count=0;
   uint16                            rtccount=0;
   int16                             tasktimeline=0;

static TiRtc g_rtc;

TiAdcAdapter  g_adc;
TiLightSensor g_light;
TiUartAdapter g_uart;


TiOsxTaskPool m_taskpool;
TiOsxTaskHeap m_taskheap;

int16 tasktimeline;

TiOsxTaskPool * tpl;
TiOsxTaskHeap * heap;


void on_timer_expired( void * object, TiEvent * e );
void _osx_priority_queue_popfront_test( TiOsxTaskHeap * heap );
void heap_task_evovle(TiOsxTaskHeap *heap,TiRtc *rtc);

/******************************************************************************* 
 * main()
 ******************************************************************************/

int main()
{
	TiAppService1 * asv1;
	TiAppService2 * asv2;

	TiRtc * rtc;

    int8 idx;

	target_init();

	tpl = osx_taskpool_construct( (void *)&m_taskpool, sizeof(TiOsxTaskPool) );
	heap = osx_taskheap_open( &m_taskheap, tpl );//构造一个堆

	osx_assert( tpl != NULL );
	osx_assert( heap != NULL );	

	asv1 = asv1_open( &m_svcmem1, sizeof(TiAppService1) );//打开两个task
	asv2 = asv2_open( &m_svcmem2, sizeof(TiAppService2) );

	TiOsxTaskHeapItem item;
	tasktimeline=3;
	memset( &item, 0x00, sizeof(item) );//此时先产生一个task1，并把它放入堆中
	item.taskfunction =asv1_evolve;
	item.taskdata = NULL;
	item.timeline = tasktimeline;//让其定时器打开后三秒运行
	item.priority = 1;

	idx = osx_taskheap_insert( heap, &item );


	rtc=rtc_construct( (void *)&g_rtc, sizeof(g_rtc) );//构造一个定时器
	rtc_setinterval(rtc,0,2,0x01);//定时周期为一秒 
	rtc_open( rtc, on_timer_expired, NULL, 0x01 );
	hal_enable_interrupts();
	rtc_start( rtc );

	while(1)//暂时用RTC的中断来实现控制task的执行，也就是on_timer_expired,heap_task_evole应该更适用一般，此时就可以不用on_timer_expired
	{
		heap_task_evovle(heap,rtc);
	}

	
}


void on_timer_expired( void * object, TiEvent * e )//每秒钟过后执行这个函数
{
	led_toggle(LED_YELLOW);

	g_count ++;
/*	TiOsxTaskHeapItem item;

	if(!osx_taskheap_empty(heap))//非空的话取出来
	{
		item = osx_taskheap_root(heap);
		rtl_assert( item->heapindex == 0 );

		if(g_count>=item->timeline&&item->taskfunction != NULL)//如果计时达到预定时间时（秒级，可能不精确）就执行
		{
		    osx_taskheap_deleteroot( heap );
			
			
			for(int i=0;i<heap->count;++i)//此时应该把堆里余下的task的时间减去过去的时间
			{
				heap->items[i]->timeline=heap->items[i]->timeline-g_count;
			}
			g_count=0;
			
			item->taskfunction(item->taskdata,item);
		}
		else return;

	}
*/

}

void _osx_priority_queue_popfront_test( TiOsxTaskHeap * heap )
{
	TiOsxTaskHeapItem * item;
	while (!osx_taskheap_empty(heap))
	{
		item = osx_taskheap_root(heap);
		rtl_assert( item->heapindex == 0 );
		// _osx_taskheap_item_dump( heap, item->heapindex );
		dbc_uint16( item->timeline );

		osx_taskheap_deleteroot( heap );

	}
}
void heap_task_evovle(TiOsxTaskHeap *heap,TiRtc *rtc)//原理同上 我想用rtc里的函数代替g_count来计时，见注释部分
{
	TiOsxTaskHeapItem *item;
	if(!osx_taskheap_empty(heap))
	{
		item = osx_taskheap_root(heap);
		rtl_assert( item->heapindex == 0 );

		if(g_count>=item->timeline&&item->taskfunction != NULL)
		{
			osx_taskheap_deleteroot( heap );
			
			
			for(int i=0;i<heap->count;++i)
			{
				heap->items[i]->timeline=heap->items[i]->timeline-g_count;
			}
			g_count=0;

			item->taskfunction(item->taskdata,item);
		}
		else return;

/***********************************************************
        int16 currenttime=rtc->current_time->sec+
			              rtc->current_time->min*60+
						  rtc->current_time->hour*3600;

		if(currenttime)>=item->timeline&&item->taskfunction!=NULL)
		{
		    osx_taskheap_deleteroot( heap );
			
			
			for(int i=0;i<heap->count;++i)
			{
				heap->items[i]->timeline-=currenttime;
			}

			currenttime=0;
			rtc->current_time->sec = 0;
			rtc->current_time->min = 0;
			rtc->current_time->hour = 0;
			rtc->current_time->day = 1;
			rtc->current_time->month = 1;
			rtc->current_time->year = 2000;
			
			item->taskfunction(item->taskdata,item);
		}
		*****************************************************/

	}
}
