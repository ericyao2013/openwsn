/*************************************************************
 * @author zhangwei on 2005-07-19
 * @note: Interface file CRC check module.
 *	thanks Chen Ruijie to give the original source code (first version). 
 * 
 * @history
 * 2005-07-19 first created by zhangwei
 * @modified by zhangwei on 20061030
 * revised and released as part of RTL
 ************************************************************/

#ifndef _RTL_CRC_H_4278_
#define _RTL_CRC_H_4278_

/*  CRCProduce
*********************************************************************************************************
* Description: ����CRCУ���ֶ�.
* Arguments  : buf,  ֡����������ָ�룬ָ���һ����У����ַ�
*                     number, ��У��֡���ַ�����
* Returns    :   fcsOut, CRCУ����
*                    ���⣬������������buf��������ĩβ�����CRCУ����  
* Complete date  : 2005.5.20
                   Modified by xxx at xxxx.xx.xx:
*********************************************************************************************************
*/
extern unsigned short crc_produce(unsigned char *buf, unsigned short number);

/*  CRCCheck
*********************************************************************************************************
* Description: CRCУ�麯��.
* Arguments  : buf,  ֡����������ָ�룬ָ���һ����У����ַ�
*                     number, ��У��֡���ַ����������԰���CRCУ����
* Returns    :   fcsOut, CRCУ����
*                    ���⣬������֡����CRCУ����ʱ��CRCУ����ȷʱ���fcsOutΪ0  
* Complete date  : 2005.5.20
                   Modified by xxx at xxxx.xx.xx:
*********************************************************************************************************
*/
extern unsigned short crc_check(unsigned char *buf, unsigned short number);

#endif