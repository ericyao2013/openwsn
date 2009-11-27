#ifndef _RTL_LIGHTQUEUE_H_9032_
#define _RTL_LIGHTQUEUE_H_9032_

/* TiLightQueue Data Structure
 * this is a C language based queue data structure. it's actaully an ring queue.
 * different to TiDynaQueue, the TiLightQueue has fixed number of items/elements.
 * It will not increase memory when it's full. It's mainly used in kernel developing
 * or embedded developing without dynamic memory allocator support.
 *
 * another advantage of TiLightQueue is that it can be moved to another place
 * inside the address space and can still work.
 *
 * portability
 *	- ansi c language
 *  - linux, windows, and embedded platforms with c compiler only
 *
 * @state
 *	finished but not tested yet
 *
 * @author zhangwei in 200503
 * @modified by zhangwei in 200803
 *	- eliminate the call of malloc() and free().
 * @modified by zhangwei in 200905
 *	- change the name as TiLightQueue. the original name is TiRingQueue
 *    the new predix "lwque_" means "light weight queue"
 */

#include "rtl_configall.h"

#define LIGHTQUEUE_HOPESIZE(itemsize,capacity) (sizeof(TiLightQueue) + (itemsize)*(capacity))


#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
	uint8 front;
	uint8 rear;
	uint8 capacity;
	uint8 count;
	uint16 itemsize;
}TiLightQueue;

TiLightQueue *  lwque_construct( void * buf, uint16 size, uint16 itemsize );
void		    lwque_destroy( TiLightQueue * que );
uint8           lwque_count( TiLightQueue * que );
uint8           lwque_capacity( TiLightQueue * que );
bool            lwque_empty( TiLightQueue * que );
bool            lwque_full( TiLightQueue * que );
void *          lwque_getbuf( TiLightQueue * que, uint8 idx );
void *          lwque_front( TiLightQueue * que );
void *          lwque_rear( TiLightQueue * que );
bool            lwque_pushback( TiLightQueue * que, void * item );
bool            lwque_pushfront( TiLightQueue * que, void * item );
bool            lwque_popfront( TiLightQueue * que );
bool            lwque_poprear( TiLightQueue * que );
/* bool            lwque_extend( TiLightQueue * que, uint16 newsize ); */


#ifdef __cplusplus
}
#endif

#endif
