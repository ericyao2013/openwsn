
#ifndef _SVC_NETSIMU_H_7388_
#define _SVC_NETSIMU_H_7388_

//----------------------------------------------------------------------------
// @author zhangwei on 20070515
// @note
// ��ģ�������ṩһϵ�нӿڣ�ʹ��host�ϵ�service/ui������Դ�����
// ��ȡ���ݻ���ͨ�����緢�����ݡ�����ģ���ṩ�Ĳ��Ƕ���ʵ�����read/write������
// ������һ�����硣��д������ͨ�������ʽ���ɡ�
//
// @modified by zhangwei on 20070515
//	first created
//----------------------------------------------------------------------------

#include "..\configall.h"
#include "..\foundation.h"
#include "svc_netnode.h"

#define CONFIG_NETSIMU_MAX_NODE 100
#define CONFIG_DATAPACKET_DATASIZE 100

//----------------------------------------------------------------------------
// TOpenNetwork
//----------------------------------------------------------------------------

// ģ��һ������
//
typedef struct{
  uint16 root;
  uint32 sensing_radius;
  uint32 comm_radius;
  TOpenNode nodes[CONFIG_NETSIMU_MAX_NODE];
  uint16 random_sequence[CONFIG_NETSIMU_MAX_NODE];
  uint16 read_cursor;
  uint32 last_probetime;
}TOpenNetwork;

typedef struct{
  uint16 id;
  char data[CONFIG_DATAPACKET_DATASIZE];
}TOpenDataPacket;

DLLAPI TOpenNetwork * _stdcall net_create();
DLLAPI void _stdcall net_free( TOpenNetwork * net );
DLLAPI int _stdcall net_open( TOpenNetwork * net );
DLLAPI void _stdcall net_close( TOpenNetwork * net );
DLLAPI int _stdcall net_write( TOpenNetwork * net, TOpenDataPacket * datapacket, uint8 opt );
DLLAPI int _stdcall net_read(  TOpenNetwork * net, TOpenDataPacket * datapacket, uint8 opt );
DLLAPI int _stdcall net_rawwrite( TOpenNetwork * net, char * buf, uint8 len, uint8 opt );
DLLAPI int _stdcall net_rawread( TOpenNetwork * net, char * buf, uint8 capacity, uint8 opt );
DLLAPI void _stdcall net_evolve( TOpenNetwork * net );
DLLAPI void _stdcall net_probe( TOpenNetwork * net );
DLLAPI void _stdcall net_probe_node( TOpenNetwork * net, uint16 nodeid );
DLLAPI uint16 _stdcall net_get_node_count( TOpenNetwork * net );
DLLAPI TOpenNode * _stdcall net_node( TOpenNetwork * net, uint16 idx );
DLLAPI int _stdcall net_get_neighbor_nodes( TOpenNetwork * net, uint16 id, uint32 radius, uint16 * buf, uint16 capacity );
DLLAPI uint32 _stdcall net_distance_between( TOpenNetwork * net, uint16 id1, uint16 id2 );
DLLAPI int _stdcall net_generate( TOpenNetwork * net );
DLLAPI int _stdcall net_load( TOpenNetwork * net, char * filename );
DLLAPI int _stdcall net_save( TOpenNetwork * net, char * filename );
DLLAPI int _stdcall net_sleep( TOpenNetwork * net );
DLLAPI int _stdcall net_wakeup( TOpenNetwork * net );

//----------------------------------------------------------------------------
#endif