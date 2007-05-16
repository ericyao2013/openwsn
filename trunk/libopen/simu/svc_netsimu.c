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

#include "svc_netsimu.h"
 
 // ����һ��TOpenNetwork����ִ�б���ĳ�ʼ��
 TOpenNetwork * net_create()
{
}
 
 // �ͷ�һ���������
 void net_free( TOpenNetwork * net );
 
 // ��һ���������׼����д
 int net_open( TOpenNetwork * net );
 net_close( TOpenNetwork * net );
 
 // �������ж�һ������������ÿ��read����Ҫô����0����ʾû�ж������ݣ�Ҫôͨ��buf����һ�����ݰ�
 // ����ֵ��ʾ�����ݰ��ĳ��ȡ�������󲻹����ݰ��ĸ�ʽ�ͽ��͡�����Ҫ�Լ����͡�
 int net_read( TOpenNetwork * net, char * buf, uint8 capacity );
 
 // дһ�����ݰ������磬Ҳ���Ƿ������ݰ���ȥ
 int net_write( TOpenNetwork * net, char * buf, uint8 len );
 
 // ��ʱ����
 void net_evolve( TOpenNetwork * net );
 
 // ��ʱ����
 void net_probe( TOpenNetwork * net )
 
 // ��ȡ�����н�����������get_node_desc����ʹ�ã�����������ѭ������
 int net_get_node_count( TOpenNetwork * net )
 TOpenNode * net_get_node_desc( TOpenNetwork * net, uint16 id )
 
 // ��ȡĳ�����ڽ��id����
 // ��ʱ����
 int net_get_neighbor_nodes( TOpenNetwork * net, uint16 id, uint16 * buf, uint16 capacity );
 
 // �ڷ���������һ�����磬�������������е����н�㣬���ǵ�λ�����ݵ�
 int net_generate( TOpenNetwork * net );
 
 // ��������Ϣ���浽�ļ���
 int net_load( TOpenNetwork * net, char * filename );
 int net_save( TOpenNetwork * net, char * filename );
 
 // ��ʱ����
 int net_sleep( TOpenNetwork * net );
 
 // ��ʱ����
 int net_wakeup( TOpenNetwork * net );

