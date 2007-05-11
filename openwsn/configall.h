#ifndef _CONFIGALL_H_4237_
#define _CONFIGALL_H_4237_

/*******************************************************************************
 * @author zhangwei on 2006-07-20
 * this is the configure file of the whole project
 * you can #define or #undef the macros in this file to enable/disable some 
 * software modules.
 * 
 * every module including re-configure functions SHOULD include this file as 
 * their first one. this is implemented by including "foundation.h". the "foundation.h"
 * has already included the "configall.h"
 * 
 * @modified by zhangwei on 2006-07-20
 * @modified by zhangwei on 20061015
 * add macro CONFIG_MAX_FRAME_LENGTH
 * 
 * @modified by zhangwei on 200601-26
 * add macros to support different hardware platforms
 * 
 * @modified by zhangwei on 20060510
 * replace TARGET_XXX with CONFIG_TARGET_XXX
 * 
 ******************************************************************************/

/*******************************************************************************
 * @attention
 *                  IMPORTANT ISSUE ON HOW TO USE THIS FILE
 *
 * if you want to cancel some macro, pls use #undef macro. you can just place
 * the #undef after the previous macro. if you want it to take effective, you 
 * can place the #undef XXX macro before the configuration macro.
 * 
 * NOT to use "//" to comment these macros. because future developes may think 
 * they are really comments and deleted them! 
 ******************************************************************************/

/*******************************************************************************
 * User Changable Configurations
 ******************************************************************************/
 
#define FOR_2420_TEST 
 
#define MAJOR_VERSION 1
#define MINOR_VERSION 2
 
/* a macro to enable the debug source code 
 * for release version, you should undef this macro
 */
#define CONFIG_DEBUG

/* Hardware Platform Choosing Configuration
 * now we have four hardware platforms:
 * - OpenNODE version 1.0
 * - OpenNODE version 2.0
 * - WlsModem version 1.1
 * - GAINS
 */
#undef CONFIG_TARGET_OPENNODE_10
#define CONFIG_TARGET_OPENNODE_20
#undef CONFIG_TARGET_WLSMODEM_11
#undef CONFIG_TARGET_GAINS

//#define TARGET_OPENNODE_10
//#undef TARGET_OPENNODE_10

//#ifdef CONFIG_TARGET_OPENNODE_20
//#define TARGET_OPENNODE_20
//#endif
//#undef TARGET_OPENNODE_20

//#define TARGET_WLSMODEM_11
//#undef TARGET_WLSMODEM_11



/* FlashStore�Ŀ�ʼ��ַ�ʹ�С
 * FlashStore����ϵͳ�����ڼ�洢�������ò���,����MCUоƬE2PROM����Flash��ַ�ռ��е�һƬ
 * ����. �������������ڳ�ʼ��TConfigure����(service\configure.*)
 * @warning
 * - ����ϵͳ����Ҳ��������Flash��,�������ڱ���ϵͳ������Ϣ��FlashStore�������ܿ�����
 * ��������.
 * - FlashStore��size�����ϵͳ��������ʵ��ռ�ÿռ��,���򽫷���Խ�����
 */
#define CONFIG_FLASHSTORE_ADDR 0x4215
#define CONFIG_FLASHSTORE_SIZE sizeof(TConfigStore)

/* �ú����ϵͳ�೤ʱ�����ι��һ��
 * ����ֵ��ν���,ȡ����watchdogӲ�������Ҫ��
 */
#define CONFIG_WATCHDOG_INTERVAL  50000

/* ����SIO Frame�Ŀ�ʼ��־�ֽ�
 * ����ʽModem���ԣ���ʹ�øñ�־
 */
#define CONFIG_START_OF_FRAME 27

/* UARTƥ�����RS232����485
 */
#undef CONFIG_UART_RS232
#define CONFIG_UART_RS232

#define CONFIG_UART_RS485
#undef CONFIG_UART_RS485

#ifdef CONFIG_UART_RS232
  #undef CONFIG_UART_RS485
#endif  

/* If you want the application to support RTOS like uCOS, you should define the following 
 * macro. the default setting is RTOS enabled */

#define CONFIG_USE_RTOS  
#undef CONFIG_USE_RTOS

/* maximum frame length. no more than 127*/
#define CONFIG_MAX_PHY_FRAME_LENGTH 0x7F
#define CONFIG_MAX_MAC_FRAME_LENGTH (CONFIG_MAX_PHY_FRAME_LENGTH-9)
#define CONFIG_MAX_UART_FRAME_LENGTH 0x7F

/* UART frame identification byte */
#define CONFIG_DEFAULT_ESCAPE_CHAR 27
#define CONFIG_DEFAULT_FRAME_PREFIX CONFIG_DEFAULT_ESCAPE_CHAR

/*******************************************************************************
 * User Un-Changable Configurations
 * these macros may induced from above macros, so you shouldn't modify them.
 ******************************************************************************/

#ifdef CONFIG_DEBUG
#define GDEBUG
#endif


#endif
