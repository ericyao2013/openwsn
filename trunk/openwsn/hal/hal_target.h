
#ifndef _HAL_TARGET_H_3792_
#define _HAL_TARGET_H_3792_

/***************************************************************************** 
 * @author zhangwei on 2006-07-20
 * Target utilities
 * ͨ����target.*ģ���а���Ŀ��Ӳ��ϵͳ�����ã���Щ����ͨ���ǵ�·��ϵͳ�������ӣ�
 * ���Ƿ���MCUģ���в������ʣ���Ϊ��Щ��Ϣ�������ض���MCU�йء�target�еĴ���
 * �����������RTOS���ᵽ��BSP(board support package)���
 *
 * @modified by zhangwei on 20061013
 * first created
 * move the old PORT_INIT() and UART0_INIT() here.
 * 
 ****************************************************************************/

#include "hal_foundation.h"

/* initialize the target hardware 
 * this function MUST run successfully or the later code will dead.
 */
void target_init( void );

#endif