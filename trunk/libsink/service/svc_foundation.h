#ifndef _FOUNDATION_H_4278_
#define _FOUNDATION_H_4278_

/***************************************************************************** 
 * @author zhangwei on 2006-08-20
 * this is the foundation file of service layer.
 * every service modules in this directory should include this one as their first file.
 * 
 *****************************************************************************/

#include "../foundation.h"
#include "svc_configall.h"

#ifdef STANDARD_APPLICATION
#define DLLAPI extern "C" _declspec(dllimport)
#else
#define DLLAPI extern "C" _declspec(dllexport)
#endif

// macro _AFXDLL will tell the compiler and linker to compile the project as DLL and using ATL/MFC
// attention that MFC application can NOT include "windows.h".
//
// macro _CRT_SECURE_NO_DEPRECATE will tell the compile do NOT report warning messages will unsafe string 
// functions.
//
#define _AFXDLL
#define _CRT_SECURE_NO_DEPRECATE

#ifdef _AFXDLL
//#include <afx.h>
#endif

#ifndef _AFXDLL
//#define WIN32_LEAN_AND_MEAN		
//#include <windows.h>
#endif


#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif

#define addrof(arr) (&(arr[0]))
#define msgbox(text,flag) MessageBox(NULL,(text),_T("libfsys"),(flag))


/******************************************************************************
 * IEEE 802.15.4 PPDU format
 * [4B Preamble][1B SFD][7b Framelength, 1b Reserved][nB PSDU/Payload]
 * 
 * IEEE 802.15.4 MAC DATA format (the payload of PHY frame)
 * Beacon Frame
 * [2B Frame Control] [1B Sequence Number][4 or 10 Address][2 Superframe Specification]
 * 		[k GTS fields][m Padding address fields] [n Beacon payload][2 FCS]
 * 
 * Data Frame
 * [2B Frame Control] [1B Sequence Number][4 or 20 Address][n Data Payload][2 FCS]
 * 
 * ACK Frame
 * [2B Frame Control] [1B Sequence Number][2 FCS]
 * 
 * MAC Control Frame
 * [2B Frame Control] [1B Sequence Number][4 or 20 ADdress][1 Command Type][n Command Payload][2 FCS]
 *
 * Frame Control
 * b2b1b0  	frame type 000 beacon, 001 data 010 ACK 011 command 100-111 reserved
 * b12b13 	reserved.
 * 
 *****************************************************************************/

// command	Ӧ�ò��Լ�����������PHY/MAC�������õ�command�޹أ�
typedef struct{
	char	command;
}TOpenPacket;

/* you cannot change following defination. they are defined according to 
 * IEEE 802.15.4 specification. RTS and CTS are exception!
 */  
#define OPF_TYPE_BEACON 0x00
#define OPF_TYPE_DATA 	0x01
#define OPF_TYPE_ACK	0x02
#define OPF_TYPE_MACCMD 0x03
#define OPF_TYPE_RTS	0x04
#define OPF_TYPE_CTS	0x05
#define OPF_TYPE_RESERVED 0x06	

#define OPF_FRAME_SIZE 0x7E
#define OPF_PAYLOAD_SIZE (OPF_FRAME_SIZE-8)


// ��Frame����libsink��GUI application��������
/* @attention
 * due to the __packed keyword, the source code can only be compiled with 
 * ARM compiler (provided in ADS)
 *
 * _packed is a ARMCC keyword. it indicates the compile arrange the member variables
 * in the following structure byte by byte. you should attention this if you 
 * want to port OpenMAC to other platform and compiles.
 *
 * payload	���ÿ�������һ��TOpenPacket��
 * ����дһ��С�Ĺ��ߺ������������⼸���ṹ����Ҫע�ⲻҪ���⼸���ṹ�����࿴�����
 * ���ӳ�Ա����Ϊ���ǵĳ�Ա�����ڲ�����ܹؼ���
 */

//typedef __packed struct{
typedef struct{ 
	uint8 	length;
	uint16 	control;
	uint8  	seqid;
	uint16 	panid;
	uint16 	nodeid;
	uint8  	payload[OPF_PAYLOAD_SIZE];
}TOpenFrame;

// THdlFrame��UartDriver��libsink������ģ�齻�����ݵĸ�ʽ�����磬uart_rawread / uart_rawwrite
// ����buf�����ݾ����ǰ����¸�ʽ��š�ע�⣬������ǲ�ͬ��ʵ������Ӳ���ϴ����frame��ʵ�ʴ����
// ʱ��uart_driver�����������в���ת���ַ�������ȷ����frame�Ŀ�ʼ��
//
// seqid	���к� ������ͨ���Ƿ���Ҫ���кţ������Űɣ�
// payload	���ÿ��Է���һ��TOpenFrame
//
//typedef __packed struct{
typedef struct{
	uint8	length;
	uint8	control; 
	uint8	seqid;
	char	payload[OPF_FRAME_SIZE]; 
}THdlFrame;

TOpenPacket * GetOpenFramePayload( char * buf );
TOpenFrame * GetHdlFramePayload( char * buf );

#endif
