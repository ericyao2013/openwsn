
#include "../service/svc_configall.h"
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <math.h>
#include "../rtl/rtl_random.h"
#include "../service/svc_network.h"
#include "svc_netsimu.h"
 
//----------------------------------------------------------------------------
// TSimuNetwork
//----------------------------------------------------------------------------

// ����һ��TSimuNetwork����ִ�б���ĳ�ʼ��
DLLAPI TSimuNetwork * _stdcall simunet_create( TSioComm * sio, TTimer * timer )
{
	TSimuNetwork * net = (TSimuNetwork *)malloc(sizeof(TSimuNetwork));
	if (net)
	{
		memset( net, 0x00, sizeof(TSimuNetwork) );
		net->mode = OPENNET_MODE_SIMULATION;
		net->root = 0xFFFF;
		net->sensing_radius = 5;
		net->comm_radius = 100;
	}
	return net;
}
 
// �ͷ�һ���������
DLLAPI void _stdcall simunet_free( TSimuNetwork * net )
{
	if (net)
	{
		free( net );
	}
}
 
// ��һ���������׼����д
DLLAPI int _stdcall simunet_open( TSimuNetwork * net )
{
	net->read_cursor = 0;
	simunet_generate( net );
	return 0;
}

DLLAPI void _stdcall simunet_close( TSimuNetwork * net )
{
	NULL;
}
 
// �������ж�һ������������ÿ��read����Ҫô����0����ʾû�ж������ݣ�Ҫôͨ��buf����һ�����ݰ�
// ����ֵ��ʾ�����ݰ��ĳ��ȡ�������󲻹����ݰ��ĸ�ʽ�ͽ��͡�����Ҫ�Լ����͡�
DLLAPI int _stdcall simunet_read(  TSimuNetwork * net, TOpenDataPacket * datapacket, uint8 opt )
{
	TOpenNode * node;
	bool found;
	int ret = 0;

	found = false;
	while (!found)
	{
		if (net->read_cursor >= CONFIG_NETSIMU_MAX_NODE)
			net->read_cursor = 0;
		else
			net->read_cursor ++;

		node = simunet_node( net, net->read_cursor );
		if (node->state != NODE_STATE_FREE) 
		{
			datapacket->id = net->read_cursor;
			ret = netnode_read( node, &(datapacket->data[0]), CONFIG_DATAPACKET_DATASIZE, opt );
		}
	}

	return ret;
}
 
DLLAPI int _stdcall simunet_rawread( TSimuNetwork * net, char * buf, uint8 capacity, uint8 opt )
{
	TOpenNode * node;
	bool found;
	int ret = 0;

	found = false;
	while (!found)
	{
		if (net->read_cursor >= CONFIG_NETSIMU_MAX_NODE)
			net->read_cursor = 0;
		else
			net->read_cursor ++;

		node = simunet_node( net, net->read_cursor );
		if (node->state != NODE_STATE_FREE) 
		{
			buf[0] = net->read_cursor & 0x00FF;
			buf[1] = (net->read_cursor & 0xFF00) >> 8;
			ret = netnode_read( node, buf+2, CONFIG_DATAPACKET_DATASIZE, opt );
		}
	}

	return ret;
}
 
// дһ�����ݰ������磬Ҳ���Ƿ������ݰ���ȥ
DLLAPI int _stdcall simunet_write( TSimuNetwork * net, TOpenDataPacket * datapacket, uint8 opt )
{
	TOpenNode * node;

	assert( datapacket->id < CONFIG_NETSIMU_MAX_NODE );
	node = simunet_node( net, datapacket->id );
	return netnode_write( node, datapacket->data, CONFIG_DATAPACKET_DATASIZE, opt );
}

DLLAPI int _stdcall simunet_rawwrite( TSimuNetwork * net, char * buf, uint8 len, uint8 opt )
{
	TOpenNode * node;
	uint16 id;

	id = buf[0] | (buf[1]<<8);
	assert( id < CONFIG_NETSIMU_MAX_NODE );
	node = simunet_node( net, id );
	return netnode_write( node, buf, CONFIG_DATAPACKET_DATASIZE, opt );
}

// not used now.
// evolve the state to the next state
//
DLLAPI void _stdcall simunet_evolve( TSimuNetwork * net )
{
	NULL;
}
 
// not used now.
// probe the neighbor nodes based on the root node
// 
DLLAPI void _stdcall simunet_probe( TSimuNetwork * net )
{
	TOpenNode * root = &(net->nodes[net->root]);
	root = root;
	NULL;
}
 
// update the information of network node specified by "nodeid" through communication
// no use in the simulation. just keep it for future.
//
DLLAPI void _stdcall simunet_probe_node( TSimuNetwork * net, uint16 nodeid )
{
	NULL;
}

// ��ȡ�����н�����������get_node_desc����ʹ�ã�����������ѭ������
DLLAPI uint16 _stdcall simunet_get_node_count( TSimuNetwork * net )
{
	return CONFIG_NETSIMU_MAX_NODE;
}

DLLAPI TOpenNode * _stdcall simunet_node( TSimuNetwork * net, uint16 idx )
{
	assert( idx < CONFIG_NETSIMU_MAX_NODE );
	return &(net->nodes[idx]);
}
 
// ��ȡĳ�����ڽ��id����
// ��ʱ����
DLLAPI int _stdcall simunet_get_neighbor_nodes( TSimuNetwork * net, uint16 id, uint32 radius, uint16 * buf, uint16 capacity )
{
	// �ж��Ƿ�neighbor nodes�Ǹ���net->comm_radius��location(x,y,z)�жϵ�
	// not developed yet!
	int n, found=0;
	uint16 idx;

	assert( false );

	rand_vector16( net->random_sequence, CONFIG_NETSIMU_MAX_NODE );
	#ifdef GDEBUG
	for (n=0; n<CONFIG_NETSIMU_MAX_NODE; n++)
		net->random_sequence[n] = n;
	#endif
	
	for (n=0; n<CONFIG_NETSIMU_MAX_NODE; n++)
	{
		idx = net->random_sequence[n];
		if (simunet_distance_between(net, id, idx) <= radius)
		{
			if (found < capacity)
				buf[found] = idx;
			
			found ++;
		}
	}

	if (found < capacity)
		return found;
	else
		return found - capacity;
}
 
DLLAPI uint32 _stdcall simunet_distance_between( TSimuNetwork * net, uint16 id1, uint16 id2 )
{
	TOpenNode * n1,*n2;
	n1 = simunet_node( net, id1 );
	n2 = simunet_node( net, id2 );
	long tmp = (n1->x-n2->x)*(n1->x-n2->x) + (n1->y-n2->y)*(n1->y-n2->y);
	return uint32(sqrt(double(tmp)));
}

// �ڷ���������һ�����磬�������������е����н�㣬���ǵ�λ�����ݵ�
DLLAPI int _stdcall simunet_generate( TSimuNetwork * net )
{
	TOpenNode * node;
	int range_min=0, range_max=100, n;

	assert( net != NULL );

	// Seed the random-number generator with current time so that
	// the numbers will be different every time we run.
	//
	srand( (unsigned)time( NULL ) );

	// initialize nodes location randomly
	for (n=0; n<CONFIG_NETSIMU_MAX_NODE; n++)
	{
		node = &( net->nodes[n] );
		node->id = n;
		node->parent = 0;
		node->state = NODE_STATE_ACTIVE;
		node->x = rand() % range_max + range_min;
		node->y = rand() % range_max + range_min;
		node->z = rand() % range_max + range_min;
		node->timestamp = 0;
		node->radius = 10;
		node->txlen = 0;
		node->rxlen = 0;
	}

	// initialize node sensor values randomly
	for (n=0; n<CONFIG_NETSIMU_MAX_NODE; n++)
	{
		node = &( net->nodes[n] );
		node->sen_temprature = rand() % 200 - 100; // a random value ranging from -100 to 100
		node->sen_strain = rand() % 200 - 100; // a random value ranging from -100 to 100
	}
	for (n=0; n<CONFIG_NETSIMU_MAX_NODE; n++)
	{
		if (n % 3 == 0)
		{
			node = &( net->nodes[n] );
			node->sen_temprature = rand() % 200 - 100; // a random value ranging from -100 to 100
			node->sen_strain = rand() % 200 - 100; // a random value ranging from -100 to 100
		}
	}

	net->root = 0;
	net->sensing_radius = 4;
	net->comm_radius = 10;
	net->last_probetime = 0;
	return 0;
}

// ��������Ϣ���浽�ļ���
DLLAPI int _stdcall simunet_load( TSimuNetwork * net, char * filename )
{
	return 0;
}

DLLAPI int _stdcall simunet_save( TSimuNetwork * net, char * filename )
{
	return 0;
}

// ��ʱ����
DLLAPI int _stdcall simunet_sleep( TSimuNetwork * net )
{
	return 0;
}

// ��ʱ����
DLLAPI int _stdcall simunet_wakeup( TSimuNetwork * net )
{
	return 0;
}

