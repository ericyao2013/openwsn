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