
#ifndef _SVC_NETDATA_H_8989_
#define _SVC_NETDATA_H_8989_

//----------------------------------------------------------------------------
// @author zhangwei on 20070515
// @note
//	�涨���������ݰ������ݸ�ʽ
//  �ṩ���ڽṹ�������ݺ�buf��������֮��ת���Ĺ��ߺ���
//
// @modified by zhangwei on 20070515
//	first created
//----------------------------------------------------------------------------
#include "svc_configall.h"
#include "svc_foundation.h"

#ifndef DLLAPI
#error "please define macro DLLAPI first"
#endif
/*
void test2();


#define DLLAPI extern "C" _declspec(dllexport)



// TNetAppData��net_read()/net_write()�����ݵĸ�ʽ
// net_read()/net_write()���������ݵĸ�ʽ�����ṹ���������ݸ�ʽ�Լ�ÿ����Ա�����ĺ���
//
typedef struct{
  //unsigned short nodeid;
  //unsigned int x,y,z;
  //unsigned int timestamp;
  //unsigned int sen_temprature;
  //unsigned int sen_strain;
  unsigned char reserved[4];
}TNetAppData;  


// ��TNetAppData�ṹ��read/write��Ҫ���{buf,capacity}������֮����и�ʽת��
// assemble���ڽ�struct�е���������ֽ�memmove��buf�������н����ŷ�
// disassembleִ�з������

DLLAPI int __stdcall net_assemble33332( void  * payload, char * buf, uint16 capacity );

DLLAPI int _stdcall net_assemble( TNetAppData * payload, char * buf, uint16 capacity );
DLLAPI int _stdcall net_disassemble( char * buf, uint16 len, TNetAppData * payload );
*/
//----------------------------------------------------------------------------
#endif
