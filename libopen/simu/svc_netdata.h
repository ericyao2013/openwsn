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

#ifndef _SVC_NETDATA_H_8989_
#define _SVC_NETDATA_H_8989_

//----------------------------------------------------------------------------
// @author zhangwei on 20070515
// @note
//	�涨���������ݰ������ݸ�ʽ
//  �ṩ���ڽṹ�������ݺ�buf��������֮��ת���Ĺ��ߺ���
//  ��ģ��������ڷ���
//
// @modified by zhangwei on 20070515
//	first created
//----------------------------------------------------------------------------

#include "..\configall.h"
#include "..\foundation.h"

// TNetAppData��net_read()/net_write()�����ݵĸ�ʽ
// net_read()/net_write()���������ݵĸ�ʽ�����ṹ���������ݸ�ʽ�Լ�ÿ����Ա�����ĺ���
//
typedef struct{
  unsigned short nodeid;
  unsigned int x,y,z;
  unsigned int timestamp;
  unsigned int sen_temprature;
  unsigned int sen_strain;
}TNetAppData;

// ��TNetAppData�ṹ��read/write��Ҫ���{buf,capacity}������֮����и�ʽת��
// assemble���ڽ�struct�е���������ֽ�memmove��buf�������н����ŷ�
// disassembleִ�з������

DLLAPI int _stdcall net_assemble( TNetData * payload, char * buf, uint16 capacity );
DLLAPI int _stdcall net_disassemble( char * buf, uint16 len, TNetData * payload );


//----------------------------------------------------------------------------
#endif
