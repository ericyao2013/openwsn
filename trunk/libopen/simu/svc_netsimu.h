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

#include "../service/svc_configall.h"
#include "../service/svc_netnode.h"
#include "../service/svc_network.h"
#include "svc_nodesimu.h"

//----------------------------------------------------------------------------
// TSimuNetwork
//----------------------------------------------------------------------------

// this simulation network has the same interface with TOpenNode and TSimuNetwork 
// define in svc_netnode.h / svc_network.h

#define TSimuNetwork TOpenNetwork 

DLLAPI TSimuNetwork * _stdcall simunet_create();
DLLAPI void _stdcall simunet_free( TSimuNetwork * net );
DLLAPI int _stdcall simunet_open( TSimuNetwork * net );
DLLAPI void _stdcall simunet_close( TSimuNetwork * net );
DLLAPI int _stdcall simunet_write( TSimuNetwork * net, TOpenDataPacket * datapacket, uint8 opt );
DLLAPI int _stdcall simunet_read(  TSimuNetwork * net, TOpenDataPacket * datapacket, uint8 opt );
DLLAPI int _stdcall simunet_rawwrite( TSimuNetwork * net, char * buf, uint8 len, uint8 opt );
DLLAPI int _stdcall simunet_rawread( TSimuNetwork * net, char * buf, uint8 capacity, uint8 opt );
DLLAPI void _stdcall simunet_evolve( TSimuNetwork * net );
DLLAPI void _stdcall simunet_probe( TSimuNetwork * net );
DLLAPI void _stdcall simunet_probe_node( TSimuNetwork * net, uint16 nodeid );
DLLAPI uint16 _stdcall simunet_get_node_count( TSimuNetwork * net );
DLLAPI TOpenNode * _stdcall simunet_node( TSimuNetwork * net, uint16 idx );
DLLAPI int _stdcall simunet_get_neighbor_nodes( TSimuNetwork * net, uint16 id, uint32 radius, uint16 * buf, uint16 capacity );
DLLAPI uint32 _stdcall simunet_distance_between( TSimuNetwork * net, uint16 id1, uint16 id2 );
DLLAPI int _stdcall simunet_generate( TSimuNetwork * net );
DLLAPI int _stdcall simunet_load( TSimuNetwork * net, char * filename );
DLLAPI int _stdcall simunet_save( TSimuNetwork * net, char * filename );
DLLAPI int _stdcall simunet_sleep( TSimuNetwork * net );
DLLAPI int _stdcall simunet_wakeup( TSimuNetwork * net );

//----------------------------------------------------------------------------
#endif
