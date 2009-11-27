/****************************************Copyright (c)**************************************************
**                               Guangzou ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.zlgmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name: config.h
** Last modified Date:  2004-09-17
** Last Version: 1.0
** Descriptions: User Configurable File
**
**------------------------------------------------------------------------------------------------------
** Created By: Chenmingji
** Created date:   2004-09-17
** Version: 1.0
** Descriptions: First version
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
********************************************************************************************************/
#ifndef __CONFIG_H 
#define __CONFIG_H

#include "../configall.h"

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	�޷���8λ���ͱ���  */
//typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		�з���8λ���ͱ���  */
//typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	�޷���16λ���ͱ��� */
//typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		�з���16λ���ͱ��� */
//typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	�޷���32λ���ͱ��� */
//typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		�з���32λ���ͱ��� */
//typedef float          fp32;                    /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
//typedef double         fp64;                    /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */

/********************************/
/*      uC/OS-II specital code  */
/*      uC/OS-II���������      */
/********************************/

#define     USER_USING_MODE    0x10                    /*  User mode ,ARM 32BITS CODE �û�ģʽ,ARM����                  */
// 
                                                     /*  Chosen one from 0x10,0x30,0x1f,0x3f.ֻ����0x10,0x30,0x1f,0x3f֮һ       */
#include "Includes.h"


/********************************/
/*      ARM���������           */
/*      ARM specital code       */
/********************************/
//��һ������Ķ�
//This segment should not be modify

#include    "LPC2294.h"


/********************************/
/*     Ӧ�ó�������             */
/*Application Program Configurations*/
/********************************/
//���¸�����Ҫ�Ķ�
//This segment could be modified as needed.
#include    <stdio.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <setjmp.h>
#include    <rt_misc.h>

// @modified by zhangwei on 20070629
// zhangwei moved the following to "../configall.h"
// 
/********************************/
/*     �����ӵ�����             */
/*Configuration of the example */
/********************************/

/* System configuration .Fosc��Fcclk��Fcco��Fpclk must be defined 
 * ϵͳ����, Fosc��Fcclk��Fcco��Fpclk���붨�� */

#ifndef Fosc 
//Crystal frequence,10MHz~25MHz��should be the same as actual status. 
//Ӧ����ʵ��һ������Ƶ��,10MHz~25MHz��Ӧ����ʵ��һ��
#define Fosc            11059200                 
//System frequence,should be (1~32)multiples of Fosc,and should be equal or less  than 60MHz. 
//ϵͳƵ�ʣ�����ΪFosc��������(1~32)����<=60MHZ
#define Fcclk           (Fosc * 4)                  
//CCO frequence,should be 2��4��8��16 multiples of Fcclk, ranged from 156MHz to 320MHz. 
//CCOƵ�ʣ�����ΪFcclk��2��4��8��16������ΧΪ156MHz~320MHz
#define Fcco            (Fcclk * 4)                 
//VPB clock frequence , must be 1��2��4 multiples of (Fcclk / 4).
//VPBʱ��Ƶ�ʣ�ֻ��Ϊ(Fcclk / 4)��1��2��4��
#define Fpclk           (Fcclk / 4) * 1             
#endif

#include    "target.h"              //This line may not be deleted ��һ�䲻��ɾ��

#endif 
/*********************************************************************************************************
 **                            End Of File
 ********************************************************************************************************/