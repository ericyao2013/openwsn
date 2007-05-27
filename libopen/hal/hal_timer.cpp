/*******************************************************************************
 * @author zhangwei, tangwen on 2006-08-02
 * Timer hardware encapsulation
 * 
 * you can reference "test/timer.*" to see how to use the TTimer object.
 * 
 * @history
 * @modified by zhangwei on 20060802
 * integrate Tangwen's new Timer code today. but not test.
 * 
 * @modified by zhangwei on 20061021
 * add channel support. 
 * testing needed.
 * 
 ******************************************************************************/

#include "hal_foundation.h"
#include "hal_timer.h"
//#include <windows.h>
//#include <mmsystem.h>

TTimer * timer_construct( TTimer * timer, uint8 size )
{
	// @TODO
	return NULL;
}

void timer_destroy( TTimer * timer )
{
}


/*
����ȻWin95�¿��ӻ�����������VC��Delphi��C���� Builder�ȶ���ר�õĶ�ʱ���ؼ�Timer��
����ʹ�úܷ��㣬����ʵ��һ���Ķ�ʱ���ܣ�����С��ʱ���Ƚ�Ϊ55ms���Ҷ�ʱ����Ϣ��
���������ϵͳ�е����ȼ��ܵͣ����ܵõ���ʱ��Ӧ��������������ʵʱ���ƻ����µ�Ӧ�á�
����Microsoft��˾��Win32 API���������Ѿ�Ϊ�û��ṩ��һ�����ڸ߾��ȼ�ʱ�ĵײ㺯����
����û�ʹ�õõ�����ʱ���ȿɵ�1ms�������ʱ���ȡ�����һ���ʵʱϵͳ������ȫ����
����Ҫ���ֽ���C���� Builder 4.0�ṩ�����·�װ���һ����ʱ����ص���Ҫ�ӿں���
����������������������Win32 API������ͬ��˵�����£�
����DWORD timeGetTime(void)
�������ش�Windows������ʼ�����ĺ����������ֵΪ232��Լ49.71�졣
*/

uint32 timer_get_high_resolution()
{
    //return timeGetTime();
	return 0;
}
