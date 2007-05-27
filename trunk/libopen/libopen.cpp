//----------------------------------------------------------------------------
// @author zhou songli on 200612
// @note libopen
//	this is the middle ware between host application and hardware sensor networks.
//
// @history
// @modified by zhousongli on 200702
//	add support to GAINZ platform
//	test passed.
// @modified by zhangwei on 20070524
//  add new interface to manipuate the wsn network.  
//
//----------------------------------------------------------------------------

#include "configall.h"
#include "foundation.h"
//#include <windows.h>
//#include <stdio.h>
#include "service/svc_siocomm.h"
#include "service/svc_log4c.h"
#include "service/svc_dataqueue.h"
#include "libopen.h"
#include "global.h"

//----------------------------------------------------------------------------
// Interface of OpenWSN system
// generally, it's enough to use the following functions only for most of the 
// application.
//----------------------------------------------------------------------------

DLLAPI int _stdcall wsn_open( int mode )
{
	global_create( mode );
	net_open( g_network );
	return 0;
}

DLLAPI void _stdcall wsn_close()
{
	net_close( g_network );
	global_free();
}

DLLAPI TOpenNetwork* wsn_get_network()
{
	return g_network;
}

DLLAPI int _stdcall wsn_write( TOpenDataPacket * datapacket, uint8 opt )
{
	if (g_mode)
		return opennet_write( g_network, datapacket, opt );
	else
		return simunet_write( g_network, datapacket, opt );
}

DLLAPI int _stdcall wsn_read(  TOpenDataPacket * datapacket, uint8 opt )
{
	if (g_mode)
		return opennet_read( g_network, datapacket, opt );
	else
		return simunet_read( g_network, datapacket, opt );
}

DLLAPI int _stdcall wsn_rawwrite( char * buf, uint8 len, uint8 opt )
{
	if (g_mode)
		return opennet_rawwrite( g_network, buf, len, opt );
	else
		return simunet_rawwrite( g_network, buf, len, opt );
}

DLLAPI int _stdcall  wsn_rawread( char * buf, uint8 capacity, uint8 opt )
{
	if (g_mode)
		return opennet_rawread( g_network, buf, capacity, opt );
	else
		return simunet_rawread( g_network, buf, capacity, opt );
}

DLLAPI void _stdcall wsn_evolve()
{
	if (g_mode)
		return opennet_evolve( g_network );
	else
		return simunet_evolve( g_network );
}

DLLAPI void _stdcall wsn_probe()
{
	if (g_mode)
		return opennet_probe( g_network );
	else
		return simunet_probe( g_network );
}

DLLAPI void _stdcall wsn_probe_node( uint16 nodeid )
{
	if (g_mode)
		return opennet_probe_node( g_network, nodeid );
	else
		return simunet_probe_node( g_network, nodeid );
}

DLLAPI uint16 _stdcall wsn_get_node_count()
{
	if (g_mode)
		return opennet_get_node_count( g_network );
	else
		return simunet_get_node_count( g_network );
}

DLLAPI TOpenNode * _stdcall wsn_node( uint16 idx )
{
	if (g_mode)
		return opennet_node( g_network, idx );
	else
		return simunet_node( g_network, idx );
}

DLLAPI int _stdcall  wsn_get_neighbor_nodes( uint16 id, uint32 radius, uint16 * buf, uint16 capacity )
{
	if (g_mode)
		return opennet_get_neighbor_nodes( g_network, id, radius, buf, capacity );
	else
		return simunet_get_neighbor_nodes( g_network, id, radius, buf, capacity );
}

DLLAPI uint32 _stdcall wsn_distance_between( uint16 id1, uint16 id2 )
{
	if (g_mode)
		return opennet_distance_between( g_network, id1, id2 );
	else
		return simunet_distance_between( g_network, id1, id2 );
}

DLLAPI int _stdcall  wsn_generate()
{
	if (g_mode)
		return opennet_generate( g_network );
	else
		return simunet_generate( g_network );
}

DLLAPI int _stdcall  wsn_load( char * filename )
{
	if (g_mode)
		return opennet_load( g_network, filename );
	else
		return simunet_load( g_network, filename );
}

DLLAPI int _stdcall  wsn_save( char * filename )
{
	if (g_mode)
		return opennet_save( g_network, filename );
	else
		return simunet_save( g_network, filename );
}

DLLAPI int _stdcall  wsn_sleep()
{
	if (g_mode)
		return opennet_sleep( g_network );
	else
		return simunet_sleep( g_network );
}

DLLAPI int _stdcall  wsn_wakeup()
{
	if (g_mode)
		return opennet_wakeup( g_network );
	else
		return simunet_wakeup( g_network );
}


//----------------------------------------------------------------------------
// Interface of TOpenNetwork 
//----------------------------------------------------------------------------

DLLAPI TOpenNetwork * _stdcall net_create( TSioComm * sio, TTimer * timer )
{
	return simunet_create( sio, timer );
}

DLLAPI void _stdcall net_free( TOpenNetwork * net )
{
	return simunet_free( net );
}

DLLAPI int _stdcall  net_open( TOpenNetwork * net )
{
	if (net->mode == OPENNET_MODE_REALNETWORK)
		return opennet_open( net );
	else
		return simunet_open( net );
}

DLLAPI void _stdcall net_close( TOpenNetwork * net )
{
	if (net->mode == OPENNET_MODE_REALNETWORK)
		return opennet_close( net );
	else
		return simunet_close( net );
}

DLLAPI int _stdcall  net_write( TOpenNetwork * net, TOpenDataPacket * datapacket, uint8 opt )
{
	if (net->mode == OPENNET_MODE_REALNETWORK)
		return opennet_write( net, datapacket, opt );
	else
		return simunet_write( net, datapacket, opt );
}

DLLAPI int _stdcall  net_read(  TOpenNetwork * net, TOpenDataPacket * datapacket, uint8 opt )
{
	if (net->mode == OPENNET_MODE_REALNETWORK)
		return opennet_read( net, datapacket, opt );
	else
		return simunet_read( net, datapacket, opt );
}

DLLAPI int _stdcall  net_rawwrite( TOpenNetwork * net, char * buf, uint8 len, uint8 opt )
{
	if (net->mode == OPENNET_MODE_REALNETWORK)
		return opennet_rawwrite( net, buf, len, opt );
	else
		return simunet_rawwrite( net, buf, len, opt );
}

DLLAPI int _stdcall  net_rawread( TOpenNetwork * net, char * buf, uint8 capacity, uint8 opt )
{
	if (net->mode == OPENNET_MODE_REALNETWORK)
		return opennet_rawread( net, buf, capacity, opt );
	else
		return simunet_rawread( net, buf, capacity, opt );
}

DLLAPI void _stdcall net_evolve( TOpenNetwork * net )
{
	if (net->mode == OPENNET_MODE_REALNETWORK)
		return opennet_evolve( net );
	else
		return simunet_evolve( net );
}

DLLAPI void _stdcall net_probe( TOpenNetwork * net )
{
	if (net->mode == OPENNET_MODE_REALNETWORK)
		return opennet_probe( net );
	else
		return simunet_probe( net );
}

DLLAPI void _stdcall net_probe_node( TOpenNetwork * net, uint16 nodeid )
{
	if (net->mode == OPENNET_MODE_REALNETWORK)
		return opennet_probe_node( net, nodeid );
	else
		return simunet_probe_node( net, nodeid );
}

DLLAPI uint16 _stdcall net_get_node_count( TOpenNetwork * net )
{
	if (net->mode == OPENNET_MODE_REALNETWORK)
		return opennet_get_node_count( net );
	else
		return simunet_get_node_count( net );
}

DLLAPI TOpenNode * _stdcall net_node( TOpenNetwork * net, uint16 idx )
{
	if (net->mode == OPENNET_MODE_REALNETWORK)
		return opennet_node( net, idx );
	else
		return simunet_node( net, idx );
}

DLLAPI int _stdcall  net_get_neighbor_nodes( TOpenNetwork * net, uint16 id, uint32 radius, uint16 * buf, uint16 capacity )
{
	if (net->mode == OPENNET_MODE_REALNETWORK)
		return opennet_get_neighbor_nodes( net, id, radius, buf, capacity );
	else
		return simunet_get_neighbor_nodes( net, id, radius, buf, capacity );
}

DLLAPI uint32 _stdcall net_distance_between( TOpenNetwork * net, uint16 id1, uint16 id2 )
{
	if (net->mode == OPENNET_MODE_REALNETWORK)
		return opennet_distance_between( net, id1, id2 );
	else
		return simunet_distance_between( net, id1, id2 );
}

DLLAPI int _stdcall  net_generate( TOpenNetwork * net )
{
	if (net->mode == OPENNET_MODE_REALNETWORK)
		return opennet_generate( net );
	else
		return simunet_generate( net );
}

DLLAPI int _stdcall  net_load( TOpenNetwork * net, char * filename )
{
	if (net->mode == OPENNET_MODE_REALNETWORK)
		return opennet_load( net, filename );
	else
		return simunet_load( net, filename );
}

DLLAPI int _stdcall  net_save( TOpenNetwork * net, char * filename )
{
	if (net->mode == OPENNET_MODE_REALNETWORK)
		return opennet_save( net, filename );
	else
		return simunet_save( net, filename );
}

DLLAPI int _stdcall  net_sleep( TOpenNetwork * net )
{
	if (net->mode == OPENNET_MODE_REALNETWORK)
		return opennet_sleep( net );
	else
		return simunet_sleep( net );
}

DLLAPI int _stdcall  net_wakeup( TOpenNetwork * net )
{
	if (net->mode == OPENNET_MODE_REALNETWORK)
		return opennet_wakeup( net );
	else
		return simunet_wakeup( net );
}


//----------------------------------------------------------------------------
// Old Version Source Code
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
#ifdef CONFIG_VERSION_10

//extern CWinThread *pThread; 
//extern TSioComm  *g_pTSioComm;

TSioComm   m_TSioComm;
TUartDriver  m_TUartDriver;
TUartDriver *g_pTUartDriver;
TSioComm  *g_pTSioComm;
//TSvrData svrData;
//CCriticalSection Section; 


//bool bend = FALSE;//�����ȫ�ֱ��������ڿ����̵߳����� 
DLLAPI void * svc_create( uint16 id, uint16 opt )
{
	TSvcPacket * pSvcPack;
	uint16 opt1 = opt;
	pSvcPack = new TSvcPacket();
	if (pSvcPack == 0) return pSvcPack;

	pSvcPack->id = id;
	pSvcPack->type = 0;
	pSvcPack->cancel = false;
	pSvcPack->state = 0;
	memset((char *)&pSvcPack->svrData,0x00,sizeof(TSvrData));
	g_pTUartDriver = uart_construct(0, (char *)&m_TUartDriver, sizeof(TUartDriver));

	uart_configure(g_pTUartDriver,57600,8,1,0,0);
	//uart_open(g_pTUartDriver,"COM1");	

	pSvcPack->pSioComm = sio_construct((char *)&m_TSioComm, 
			sizeof(TSioComm), 
			g_pTUartDriver, 
			0);//SIO_OPTION_FRAME_SUPPORT);	
	
	return (void *)pSvcPack;
}
DLLAPI void svc_start(void * svc)
{
   TSvcPacket *pSvcPack = (TSvcPacket *) svc;
   if (pSvcPack->hthread == NULL)
   {
		//InitializeCriticalSection(&pSvcPack->Section); 

		pSvcPack->hthread = CreateThread(
        			NULL,							// default security attributes
					0,								// use default stack size
					svc_thread,           // thread function
					pSvcPack,						// argument to thread function
					0,								// use default creation flags
					NULL);							// returns the thread identifier

		
		pSvcPack->cancel = false;
	}
}

DLLAPI void svc_stop(void *svc)
{
	TSvcPacket *pSvcPack = (TSvcPacket *)svc;

	if ( pSvcPack != NULL && pSvcPack->hthread != NULL)
	{ 
	  pSvcPack->cancel = true;	
	//bend=TRUE;//�ı�������߳̽��� 	
	  Sleep(1000);
	  if (pSvcPack->hthread != NULL)
	  {
	  //WaitForSingleObject(pSvcPack->hthread,INFINITE);//�ȴ��߳̽��� 

	  // @modified by zhangwei on 200701
	  // for zhou songli
	  // you use CreateThread to allocate resource for thread. you cannot call delete to free the 
	  // resource. i remember i had given you the template on how to canncel a thread.
	  //
	  //delete pSvcPack->hthread;//ɾ���߳� 
		
		//DWORD exitcode = 0;
		//if (GetExitCodeThread( pSvrPack->thread, &exitcode) != 0)
			//{
				TerminateThread( pSvcPack->hthread, -1 );
				CloseHandle( pSvcPack->hthread );
			//}
		pSvcPack->hthread = NULL;
		//Beep(100,100); 

	  }
	}
}

//The Thread Function 
DWORD __stdcall svc_thread(LPVOID pParam)
{
	uint8 cnt;
	TSvcPacket *pSvcPack = (TSvcPacket *)pParam;
	TSvrData *pSvrData = NULL;
	char tempData[128];
	memset(tempData,0,sizeof(tempData));
	pSvrData = &pSvcPack->svrData;
	while(!pSvcPack->cancel) 
	{
	   //Beep(100,100); 
	//read data from rs232;
		cnt = sio_read(pSvcPack->pSioComm,&tempData[0], 128, 0);
	   if (cnt >0){
		
		svc_write_rxDataQueue(&pSvrData->rxDataQueue,(char *)&tempData[0], cnt, 0);	
                        
	  } else{ /* write the data into sio_txbuf if there are txQueueData sent.*/ 
		  cnt = svc_read_txDataQueue(&pSvrData->txDataQueue,(char*)&tempData[0], 128,0);
	    if (cnt >0)	sio_write(pSvcPack->pSioComm, &tempData[0], cnt, 0);
	 
	  }
	  //Sleep(1000); 
	} 

	free(tempData);
	return 0; 

}

DLLAPI uint8 svc_read( void * svc, char * buf, uint8 capacity, uint16 opt )
{
	TSvcPacket * pSvcPack = (TSvcPacket *)svc;
	TSvrData * pTSvrData = &pSvcPack->svrData;
     if (svc == NULL)	 return 0;
     if (buf == NULL)  return 0;	     
	 return (svc_read_rxDataQueue(&pTSvrData->rxDataQueue, buf,capacity,opt));
}

DLLAPI uint8 svc_write( void * svc, char * buf, uint8 len, uint16 opt )
{
	TSvcPacket * pSvcPack = (TSvcPacket *)svc;
	TSvrData * pTSvrData = &pSvcPack->svrData;
     if (svc == NULL)	 return 0;
     if (buf == NULL)  return 0;	 
     return (svc_write_txDataQueue(&pTSvrData->txDataQueue, buf,len,opt));     
}

/*
void svc_timer_handler()
{

}

void svc_optmize_path( TSinkService * svc )
{
}
*/
/*
uint8  svc_write_rxDataQueue(TDataQueue * rxDataQueue, char *buf, uint8 len, uint16 opt)
{
    uint8 ptr;	
	TDataItem *pDataItem;
	if (rxDataQueue == NULL) return 0;
	if (buf == NULL) return 0;
	if (len ==0) return 0;
	//EnterCriticalSection(&Section); 
        ptr = rxDataQueue->ptrtail++;
	if (ptr >= MAX_DATAITEM_NUMBER -1)
	{
		ptr = 0;
	}	
	pDataItem = &rxDataQueue->dataItem[ptr];
	pDataItem->totalen = len;
	memmove(&pDataItem->data[0],buf,len);
	
	rxDataQueue->totalCnt++;
        //LeaveCriticalSection(&Section);
	return len;
}

uint8  svc_read_rxDataQueue(TDataQueue *rxDataQueue, char *buf, uint8 capacity, uint16 opt)
{
	uint8 ptr;	
	TDataItem *pDataItem;
	uint8 count;
	uint8 i;
	if (rxDataQueue == NULL) return 0;
	if (buf == NULL) return 0;
	if (capacity==0) return 0;
  	if (rxDataQueue->totalCnt <1) return 0;//no data;
	

	//EnterCriticalSection(&Section); 
    ptr = rxDataQueue->ptrhead;
	pDataItem = &rxDataQueue->dataItem[ptr];
	count = MIN(pDataItem->totalen,capacity);
	
	for (i=0;i<count;i++)
		buf[i] = pDataItem->data[i];
	//memmove(buf,&pDataItem->data[0],count);
	pDataItem->totalen -= count ;
	if (pDataItem->totalen >0)
	{
		memmove(&pDataItem->data[0],&pDataItem->data[count],pDataItem->totalen);   		   		
	}else{
		rxDataQueue->totalCnt--;
		rxDataQueue->ptrhead++;
		if (rxDataQueue->ptrhead >= MAX_DATAITEM_NUMBER -1)	
		rxDataQueue->ptrhead = 0;
	}
        //LeaveCriticalSection(&Section);
	return count;

}

 uint8  svc_read_txDataQueue(TDataQueue *txDataQueue, char *buf, uint8 capacity, uint16 opt)
{
	uint8 ptr;	
	TDataItem *pDataItem;
	uint8 count;
	uint16 opt1 = opt;
	//FILE *stream_source;
	
	if (txDataQueue == NULL) return 0;
	if (buf == NULL) return 0;
	if (capacity==0) return 0;
	if (txDataQueue->totalCnt <1) return 0;//no data;
	    
	//EnterCriticalSection(&Section); 
        ptr = txDataQueue->ptrhead;
	if (ptr >= MAX_DATAITEM_NUMBER -1)
	{
		ptr = 0;
	}	
	pDataItem = &txDataQueue->dataItem[ptr];
	count = MIN(pDataItem->totalen,capacity);
	
	memmove(buf,&pDataItem->data[0],count);
	pDataItem->totalen -= count ;
	if (pDataItem->totalen >0)
	{
		memmove(&pDataItem->data[0],&pDataItem->data[count],pDataItem->totalen);   		   
	}else{
		txDataQueue->totalCnt--;
		txDataQueue->ptrhead++;
		if (txDataQueue->ptrhead >= MAX_DATAITEM_NUMBER -1)	
		txDataQueue->ptrhead = 0;
	}
        //LeaveCriticalSection(&Section);
	return count;
}

uint8  svc_write_txDataQueue(TDataQueue * txDataQueue, char *buf, uint8 len, uint16 opt)
{
    uint8 ptr;	
	TDataItem *pDataItem;
	uint16 opt1 = opt;

	if (txDataQueue == NULL) return 0;
	if (buf == NULL) return 0;
	if (len ==0) return 0;
	//EnterCriticalSection(&Section); 
        ptr = txDataQueue->ptrtail++;
	if (ptr >= MAX_DATAITEM_NUMBER -1)
	{
		ptr = 0;
	}	
	pDataItem = &txDataQueue->dataItem[ptr];
	pDataItem->totalen = len;
	memmove(&pDataItem->data[0],buf,len);	
	txDataQueue->totalCnt++;
    //LeaveCriticalSection(&Section);
	return len;
}
*/

DLLAPI int8 svc_uart_open( void * svc, char * name)
{
  TSvcPacket *pSvcPacket = (TSvcPacket *) svc;
  TUartDriver * uart = pSvcPacket->pSioComm->uart;
  return uart_open(uart,name);
}



DLLAPI int8 svc_uart_close(void *svc)
{
  TSvcPacket *pSvcPacket = (TSvcPacket *) svc;
  TUartDriver * uart = pSvcPacket->pSioComm->uart;
  return uart_close(uart);
}

DLLAPI int8 svc_uart_configure (void *svc,uint32 baudrate, uint8 databits, uint8 stopbits, uint8 parity, uint8 optflag )
{
  TSvcPacket *pSvcPacket = (TSvcPacket *) svc;
  TUartDriver * uart = pSvcPacket->pSioComm->uart;
  return uart_configure(uart,baudrate,databits,stopbits,parity,optflag);
}

#endif /* CONFIG_VERSION_10 */
//----------------------------------------------------------------------------