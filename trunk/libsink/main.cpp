
#include "foundation.h"
#include <windows.h>
#include "service/svc_siocomm.h"
#include "service/svc_log4c.h"
#include "libsink.h"

//extern CWinThread *pThread; 
//extern TSioComm  *g_pTSioComm;

TSioComm   m_TSioComm;
TSioComm  *g_pTSioComm;
TUartDriver  m_TUartDriver;
TUartDriver *g_pTUartDriver;

//TSvrData svrData;
//CCriticalSection Section; 

typedef struct{
	uint16            id;
	char			type;
	int				state;
/*	stack<TProfileStackItem> stack;
	tstring			caption;
	tstring			dirfrom;
	tstring			dirto;
	volatile int    foldercount;
	volatile int	folderdone;
	volatile int    filecount;
	volatile int	filedone;
	volatile int	action;
    volatile bool   cancel;
	volatile int	option;

    HANDLE			thread;
	tstring			quefile;
	TFileQueue *	queue;
	TActionService * asv;
*/
	// @modified by zhangwei on 200701
	// zhangwei decorated these two variable as volatile
	volatile bool   cancel;
	volatile HANDLE hthread;
	TSvrData  svrData;	
//	CCriticalSection Section;
}TSvcPacket;

//bool bend = FALSE;//�����ȫ�ֱ��������ڿ����̵߳����� 
DLLAPI void * __stdcall  svc_create( uint16 id, uint16 opt )
{
	TSvcPacket * pSvcPack;
	uint16 opt1 = opt;
	pSvcPack = new TSvcPacket();
	pSvcPack->id = id;
	pSvcPack->type = 0;
	pSvcPack->cancel = false;
	pSvcPack->state = 0;
	memset((char *)&pSvcPack->svrData,0x00,sizeof(TSvrData));
	g_pTUartDriver = uart_construct(0, (char *)&m_TUartDriver, sizeof(TUartDriver));
    g_pTSioComm = sio_construct((char *)&m_TSioComm, 
			sizeof(TSioComm), 
			g_pTUartDriver, 
			SIO_OPTION_FRAME_SUPPORT);

	
	return (void *)pSvcPack;
}
DLLAPI void svc_start(void * svc)
{
   TSvcPacket *pSvcPack = (TSvcPacket *) svc;
  //start_thread( sink_service_thread )
	//memset((char *)&svrData,0,sizeof(TSvrData));
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
//	pThread=AfxBeginThread(svc_thread,&svrData);//�����߳� 
//	pThread->m_bAutoDelete=FALSE;//�߳�Ϊ�ֶ�ɾ�� 	
}

DLLAPI void svc_stop(void *svc)
{
	
	TSvcPacket *pSvcPack = (TSvcPacket *)svc;

	if ( pSvcPack != NULL && pSvcPack->hthread != NULL)
	{
	  pSvcPack->cancel = true;	
	//bend=TRUE;//�ı�������߳̽��� 	
	  //Sleep(1000);  
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
			{
				TerminateThread( pSvcPack->hthread, -1 );
				CloseHandle( pSvcPack->hthread );
			}
		pSvcPack->hthread = NULL;

	  }
	Beep(100,100); 
	}
}

//The Thread Function 
DLLAPI DWORD _stdcall svc_thread(LPVOID pParam)
{
	uint8 cnt;
	//uint8  dataType;
	//uint8  AddrCnt;
	TSvcPacket *pSvcPack = (TSvcPacket *)pParam;
	TSvrData *pSvrData ;
	char tempData[128];
	FILE * stream_source;
	
	RF_PAYLOAD *pRfPayload = (RF_PAYLOAD *) &tempData[0];       
	memset(tempData,0,sizeof(tempData));
	pSvrData = &pSvcPack->svrData;
	while(!pSvcPack->cancel) 
	{
	   //Beep(100,100); 
	//read data from rs232;
	   //cnt = sio_read(g_pTSioComm, &tempData[0], 128, 0);
	   cnt = 0;
	   if (cnt >0){

	/* phase the data read;push the route data into sinkServRoute and received data into 
	   rxDataQueue;
        */ 
  /*          dataType = BF(pRfPayload->FrameControl,DATA_TYPE_BM,0);
            AddrCnt = BF(pRfPayload->FrameControl,ADDRLIST_LENTH, ADDRLIST_BM);
            cnt = pRfPayload->pData[0];
	    switch(dataType)
	    {
	       case RouteFeedback:
			   svc_routetable_add_addrItem(&pSvrData->sinkServRoute,
		   pRfPayload->routeAddr,AddrCnt,0);
		   break;
	       case QueryFeekback:
		 				   svc_write_rxDataQueue(&pSvrData->rxDataQueue,
				   (char *)&pRfPayload->pData[1], cnt, 0);		   
		   break;
	    }		
		*/
		 svc_write_rxDataQueue(&pSvrData->rxDataQueue,(char *)&tempData[0], cnt, 0);	
		
		// @modified by zhangwei 
		// ����ڽ���threadһ��ʼ��fopen���˳�ʱclose����ҪƵ��open/close

		stream_source=fopen("D:\\source\\sio_raw_revcived.txt","w+");
		fseek(stream_source,0L,SEEK_SET); 						
		fwrite(&tempData[0],cnt,1,stream_source);			
		fclose(stream_source);	

                        
	  } else{ /* write the data into sio_txbuf if there are txQueueData sent.*/ 
		  //cnt = svc_read_txDataQueue(&pSvrData->txDataQueue,&tempData[0], 128,0);
	    //if (cnt >0)	sio_write(g_pTSioComm, &tempData[0], cnt, 0);
	 
	  }
	Sleep(1000); 
	} 

	free(tempData);
	pRfPayload = NULL;

	// @modified by zhangwei on 200701
	pSvcPack->hthread = NULL;
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

void svc_timer_handler()
{

}

void svc_optmize_path( TSinkService * svc )
{
}

DLLAPI uint8 _stdcall svc_write_rxDataQueue(TDataQueue * rxDataQueue, char *buf, uint8 len, uint16 opt)
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

DLLAPI uint8 _stdcall svc_read_rxDataQueue(TDataQueue *rxDataQueue, char *buf, uint8 capacity, uint16 opt)
{
	uint8 ptr;	
	TDataItem *pDataItem;
	uint8 count;
	if (rxDataQueue == NULL) return 0;
	if (buf == NULL) return 0;
	if (capacity==0) return 0;
  	if (rxDataQueue->totalCnt <1) return 0;//no data;

	//EnterCriticalSection(&Section); 
        ptr = rxDataQueue->ptrhead;
	pDataItem = &rxDataQueue->dataItem[ptr];
	count = MIN(pDataItem->totalen,capacity);
	
	memmove(buf,&pDataItem->data[0],count);
	pDataItem->totalen -= count ;
	if (pDataItem->totalen >0)
	{
           memmove(&pDataItem->data[0],&pDataItem[count],pDataItem->totalen - count);   		   
	}else{
		rxDataQueue->totalCnt--;
		rxDataQueue->ptrhead++;
		if (rxDataQueue->ptrhead >= MAX_DATAITEM_NUMBER -1)	
		rxDataQueue->ptrhead = 0;
	}
        //LeaveCriticalSection(&Section);
	return count;

}

 uint8 _stdcall svc_read_txDataQueue(TDataQueue *txDataQueue, char *buf, uint8 capacity, uint16 opt)
{
	uint8 ptr;	
	TDataItem *pDataItem;
	uint8 count;
	uint16 opt1 = opt;
	FILE *stream_source;
	
	if (txDataQueue == NULL) return 0;
	if (buf == NULL) return 0;
	if (capacity==0) return 0;
	if (txDataQueue->totalCnt <1)
	{

		return 0;//no data;
	}

    
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
           memmove(&pDataItem->data[0],&pDataItem[count],pDataItem->totalen - count);   		   
	}else{
		txDataQueue->totalCnt--;
		txDataQueue->ptrhead++;
		if (txDataQueue->ptrhead >= MAX_DATAITEM_NUMBER -1)	
		txDataQueue->ptrhead = 0;
	}
		stream_source=fopen("D:\\source\\sio_raw_to_be_sent(bak).txt","w+");
		fwrite(&txDataQueue->dataItem[0].data[0],txDataQueue->dataItem[0].totalen +count,1,stream_source);
		fclose(stream_source);
        //LeaveCriticalSection(&Section);
	return count;
}
DLLAPI uint8 _stdcall svc_write_txDataQueue(TDataQueue * txDataQueue, char *buf, uint8 len, uint16 opt)
{
        uint8 ptr;	
	TDataItem *pDataItem;
	uint16 opt1 = opt;
	FILE* stream_source;
	
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
		

    stream_source=fopen("D:\\source\\sio_raw_to_be_sent(txDataQueue).txt","w+");
	fseek(stream_source,0L,SEEK_SET); 		
    fwrite(&pDataItem->data[0],len,1,stream_source);
	fclose(stream_source);
    //LeaveCriticalSection(&Section);
	return len;
}

DLLAPI bool  _stdcall svc_routetable_add_addrItem(TSinkService *pTSinkServ,RouteAddr routeAddr,uint8 addrLen,uint8 opt)
{
        WORD nodeid;	
	uint8 ptr;	
	uint8 i;
        TRoutePathCache *pTRoutePathCache;	
	TRoutePathCacheItem *pTRouteCacheItem;	
	
	if (&routeAddr == NULL)	return FALSE;
	if (pTSinkServ == NULL) return FALSE;
	if (addrLen == 0) return FALSE;

	nodeid = routeAddr.dstAddr;
	pTRoutePathCache = &pTSinkServ->pathCache;
	if (pTRoutePathCache->totalCnt >= MAX_ROUTE_PATH_NUMBER) return FALSE;

	/*Ѱ�Ҹ�·�ɵ�ַ�Ƿ��Ѿ������ڵ�ַ���У�����Ѿ���
	�ڣ��򷵻�true;�������б��������·�ɵ�ַ��*/
	for(i=0;i< pTRoutePathCache->totalCnt;i++)
	{
	        pTRouteCacheItem = &pTRoutePathCache->RoutePathItem[i];
		if (pTRouteCacheItem->nodeid != nodeid) continue;	
		if (pTRouteCacheItem->length != addrLen) continue;
		for (ptr =0;ptr <addrLen;ptr++)
		{
			if (pTRouteCacheItem->path[ptr] != routeAddr.dstAddrItem[ptr].addr) break;                     				
		}

		if (ptr == addrLen -1)
		{/*��Ҫ�����·�ɵ�ַ�Ѿ�������·�ɻ������ڣ�*/
			return TRUE;
		}
	}

	/*�������в�����·�ɵ�ַ����Ҫ�Ѵ����·�ɵ�ַ���뵽�������ڣ�*/
        pTRouteCacheItem = &pTRoutePathCache->RoutePathItem[pTRoutePathCache->totalCnt++];
	pTRouteCacheItem ->nodeid = nodeid;
	pTRouteCacheItem->length = addrLen;
	for (i=0;i<addrLen;i++)
	{
		pTRouteCacheItem->path[i] = routeAddr.dstAddrItem[i].addr;
	}
	return TRUE;
}
bool _stdcall svc_select_routeCache(TSinkService *pTSinkServ,WORD srcNodeId,WORD destNodeId,TRoutePathCache *pRouteCache,uint8 opt)
 {
	return TRUE;
}
bool _stdcall svc_select_routeOptimalCache(TSinkService *pTSinkServ,WORD srcNodeId,WORD destNodeId,TRouteOptimalPathCache *pRouteCache,uint8 opt)
{
	return TRUE;
}

/*
@author zhangwei on 20061105
==========================================================================

Ŀ��
==========================================================================
������OpenM2M/OpenWSN��ϵ�У��ṩsensor network��computer network(Internet)֮���һ��gateway����������������ͨ·��һ�����У������������sensor networks��packet�����һ����ڱ��أ��ȴ�SCADA��������ȡ��һ�����У��������SCADA������packet�����ҷ���sensor network��


���˼��
==========================================================================

�ؼ��㣺ʵ������communication channel
һ��channel��ͨ��UART��sink nodeͨ�ţ���������packet������packet��
һ��channel����SCADA Admin����ͨ��

Sink Service����������ѯ����channel�����Ƿ������ݵ����scada channel������������ͨ��sink uart channel���ͳ�ȥ����uart channel�����������򻺴��ڱ���RAM�У��ȴ�Scada Admin����ȡ��

char m_txpacket[]
char m_rxpacket[];
queue m_rxqueue;
table m_routetable[];

svc_main()
{
  start_thread( sink_service_thread )
}

svc_thread()
{
  if (uart_read() > 0)
  {
    �Ƿ����������ݰ���������ǣ�������copy��m_rxpacket[]
    ���m_rxpacket���Ѿ���һ��������packet����ִ����Ӳ�����push��m_rxqueue�У�
    ����յ������ݰ���·��ά��request�ķ���response���򲻱�������У�ֱ�ӷ���routing table���ɡ�
       �Ž�ȥ֮���������·���Ż����� svc_optimize_routing()
  }

  if (m_txpacket���д���������)
  {
    ͨ��UART���͸�sink node���͸��������硣
  }
}



����m_rxqueue��������STL queue�����Լ�ʵ�֡�

svc��Ҫ�ṩ����������SCADA Admin����

uint16 svc_read( void * svc, char * buf, uint16 capacity, uint16 opt )
�ú�����admin���ã���ִ�г��Ӳ������������յ������ݴ�queue��ȡ��������copy��buf��ָ��Ŀռ��У�����ʵ��copy���ֽ���


uint16 svc_write( void * svc, char * buf, uint16 len, uint16 opt )
�ú�����admin���ã���buf����д�뵽m_txpacket�У�ע��Ҫд��ȫ��д�룬����Ͳ�д������д�ɹ�ʱ����0��admin�Ḻ��retry�ġ�


·����Ϣ��ά����ҪҲ����svc�����ġ�������Ƶ�Ŀ����Ϊ�˼�sensor node��������������sensor node�����
·��ά���Ļ���˼·��svcͨ��sink node�����Թ㲥path request���ݰ���ÿ��sensor�յ���packet��Ҫ��һ���㲥��ȥ����������������node�����յ�����packet�ڴ����������Ҫ��¼����������path��ÿ���յ���packet��sensor node��Ҫ�������м�¼��path������һ��packet��sink node��sink node�յ����sink svc������,sink svc���ܻ�õ�����node����ߵ�path��Ϣ���Ժ�svc/adminҪ��������ʱ��ֻҪ�򵥵Ĵӱ����ѯһ��path���ɡ�sink svcҲ���Զ��յ���path�����Ż����������ÿ��node�����path��

service���ں�һ��timer����(VC�е�)�������Եĵ���svc_timer_handler��ִ������ά����������·��ά��
void svc_timer_handler()
{
  ����svc_write()����·��ά��packet
  svc_optmize_path()
}


typedef TRoutePathCache{
  nodeid;
  length;   // path length
  path[15]; // path. we don't consider those exceed 15 hop 
};

typedef TRouteOptimalPath{
  nodeid;
  path[10];  
}

typedef TSinkService{
  sinknode;
  maxhop;
  updateperiod;
  TRouteOptimalPath path[50]
  TRoutePathCache pathcache[200];
};

svc_optmize_path( TSinkService * svc )
{
  ����TRoutePathCache�д洢��path��Ϣ���鲢����sink��ÿ��destination sensor node������path��
  ���ҽ�optimal path����TRouteOptimalPath�С�
}

ע�⣺����path�ı�׼ÿ���˸��в�ͬ�����������Ƕ��ԣ�������ۺϿ�������·�ϵ�energy��path length.
���ǣ����ǵĵ�������Ҳ�Ǹ����⣬��ȻĿǰ�ڶ���Ӳ����֧�֣����������㻹���е��Ѷȡ�
�����������������Ŀǰ�Ȱ������·ȥ���ɣ�����Ҳ��Ϊ�˽����Ѷȡ�


���ճɹ�����
==========================================================================

һ���ɹ�SCADA Admin GUI���õĿ�(DLL��ʽ)��������һ�����Զ������е�windows service��
���ڼ�㣬�����һ����������DLL��ʽȥ���ɡ�

���������ĺͻƻ��Լ����˶�����������ɹ���������ÿ���˶�������һ�ס�
����Ӳ���ʹ�������д�����⣬��Ҳ��Ҫ�ƻ��ȶ�е�һЩ������������һ�£������깤��


���������棬��ǿ�ҽ�������������C#ȥ������������ͼ��Ŀ������⣩��ֻ�м򵥲��Ḵ�ӡ�����������3�˵��ǿ����Լ����Լ��ģ�����ͳһ��

����������Ŀ�ܳ�����򵥵ľ��Ǹ��Ի������ҪƯ���ģ�����download DevExpress�ؼ� for C#���зǳ��õ�Example���������ġ�
*/
