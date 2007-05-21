
#ifndef _SVC_NETSIMU_H_7388_
#define _SVC_NETSIMU_H_7388_

//----------------------------------------------------------------------------
// @author zhangwei on 20070515
// @note
// 该模块用于提供一系列接口，使得host上的service/ui程序可以从网络
// 读取数据或者通过网络发送数据。但本模块提供的不是对真实网络的read/write，而是
// 虚拟了一个网络。读写的数据通过随机方式生成。
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

// 模拟一个网络
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
