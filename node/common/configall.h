#ifndef _DEVX_CONFIGALL_H_4237_
#define _DEVX_CONFIGALL_H_4237_
/*******************************************************************************
 * This file is part of OpenWSN, the Open Wireless Sensor Network Platform.
 *
 * Copyright (C) 2005-2010 zhangwei(TongJi University)
 * 
 * OpenWSN is a free software; you can redistribute it and/or modify it under 
 * the terms of the GNU General Public License as published by the Free Software 
 * Foundation; either version 2 or (at your option) any later version.
 * 
 * OpenWSN is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple 
 * Place, Suite 330, Boston, MA 02111-1307 USA.
 * 
 * For non-opensource or commercial applications, please choose commercial license.
 * Refer to OpenWSN site http://code.google.com/p/openwsn/ for more detail.
 * 
 * For other questions, you can contact the author through email openwsn#gmail.com
 * or the mailing address: Dr. Wei Zhang, Dept. of Control, Dianxin Hall, TongJi 
 * University, 4800 Caoan Road, Shanghai, China. Zip: 201804
 * 
 ******************************************************************************/ 

/******************************************************************************
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
 * @modified by zhangwei on 20090523
 *	- replace CONFIG_TARGET_GAINS with CONFIG_TARGET_GAINZ
 *	- enable CONFIG_COMPILER_GNU by default
 *  - enable #define CONFIG_OS_OSX and disable CONFIG_OS_TINYOS at the same time
 * @modified by zhangwei on 20090708
 *  - add fundamental data types definition 
 *****************************************************************************/


/******************************************************************************
 * @attention
 *                  IMPORTANT ISSUE ON HOW TO USE THIS FILE
 *
 * OPENWSN_XXX are constant macro definitions. you should NOT change them. while,
 * CONFIG_XXX are configure setting macros. you can change them to reflect your own
 * settings
 *
 * if you want to cancel some macro, pls use #undef macro. you can just place
 * the #undef after the previous macro. if you want it to take effective, you
 * can place the #undef XXX macro before the configuration macro.
 *
 * NOT to use "//" to comment these macros. because future developes may think
 * they are really comments and deleted them!
 *****************************************************************************/

 /* attention
  * in the future, this file will be generated by some utility to help customize openwsn */
  
#define CONFIG_DEBUG

/******************************************************************************
 * User Changable Configurations
 *****************************************************************************/

#define OPENWSN_MAJOR_VERSION 1
#define OPENWSN_MINOR_VERSION 0
#define OPENWSN_MICRO_VERSION 0
#define OPENWSN_BUILD_VERSION 365

/* a macro to enable the debug source code
 * for release version, you should undef this macro
 */
#ifdef CONFIG_DEBUG
  #define GDEBUG
#endif

/* Hardware Platform Choosing Configuration
 * now we have four hardware platforms:
 * - CONFIG_TARGET_OPENNODE_10  for OpenNODE version 1.0
 * - CONFIG_TARGET_OPENNODE_20  for OpenNODE version 2.0
 * - CONFIG_TARGET_OPENNODE_30  for OpenNODE version 3.0
 * - CONFIG_TARGET_WLSMODEM_11 for WlsModem version 1.1
 * - CONFIG_TARGET_GAINZ for ICT's GAINZ hardware
 *
 * @attention: there're only one above macro allowed in the system!
 * currently, openwsn only support OPENNODE_10, 20, 30
 */
#define CONFIG_TARGET_OPENNODE_10
#undef  CONFIG_TARGET_OPENNODE_10

#define CONFIG_TARGET_OPENNODE_20
#undef  CONFIG_TARGET_OPENNODE_20

#define CONFIG_TARGET_OPENNODE_30
#undef  CONFIG_TARGET_OPENNODE_30

#define CONFIG_TARGET_WLSMODEM_11
#undef  CONFIG_TARGET_WLSMODEM_11

#undef  CONFIG_TARGET_GAINZ
#define CONFIG_TARGET_GAINZ

/* Compiler used to Compile the Source code
 * CONFIG_COMPILER_GNU
 * should be defined when using GNU c/c++ compiler. this includes:
 *	- WinARM (GNU compilers for ARM)
 *	- WinAVR for GAINZ platform
 *	- AVR Studio for GAINZ platform
 */
#define CONFIG_COMPILER_ADS
#undef  CONFIG_COMPILER_ADS

#undef  CONFIG_COMPILER_GNU
#define CONFIG_COMPILER_GNU


#ifdef __BORLANDC__
	#define CONFIG_COMPILER_BORLAND
	#if (__BORLANDC__ < 0x500)
		#error "Requires Borland C/C++ 5.0 or greater"
	#endif
	#define CONFIG_WINDOWS
#endif


/* "inline" and "_Bool" are standard C99 keywords. but not all the embedded compilers
 * support they two.
 *
 * Reference
 * [1] http://en.wikibooks.org/wiki/C_Programming/Reference_Tables
 * [2] http://publib.boulder.ibm.com/infocenter/lnxpcomp/v8v101/index.jsp?topic=/com.ibm.xlcpp8l.doc/language/ref/keyw.htm
 * [3] Supported Features of C99, http://docs.sun.com/source/819-3688/c99.app.html
 */
 
#ifdef CONFIG_COMPILER_GNU
  #define INLINE __inline__
#else
  #define INLINE inline
#endif



/* "openwsn" is designed to be integrated with existed mature OS. currently, it
 * only support uCOS-II. you can change the following macro to configure the OS
 * to be integrated.
 * currently, it only support uCOS. it's also the default settings.
 * in the future, the default setting will be OS_NONE
 */
#undef  CONFIG_OS_NONE
#undef  CONFIG_OS_TINYOS
#undef  CONFIG_OS_UCOSII
#undef  CONFIG_OS_EMBEDDEDLINUX
#undef  CONFIG_OS_FREERTOS
#define CONFIG_OS_OSX


/*******************************************************************************
 * Fundamental Data Types Used in All the Modules
 * attention the following code should be verified when the project porting to
 * a new hardware and compiling environment. 
 ******************************************************************************/

/* intx and uintx are related to CPU architecture
 * for most of the 8bit MCUs, intx/uintx are 8 bit, and for ARM, they're 32bit
 */

/* for atmega128 MCU and avr-gcc (WinAVR or AVR Studio)
 * ref: <stdint.h> in WinAVR
 */
#ifdef CONFIG_TARGET_GAINZ 
typedef signed char         int8;
typedef unsigned char       uint8;
typedef signed short int    int16;
typedef unsigned short int  uint16;
typedef signed long         int32;
typedef unsigned long       uint32;
typedef signed long long    int64;
typedef unsigned long long  uint64;
typedef float               fp32;             /* single precision floating point variable (32bits)  */
typedef double              fp64;             /* double precision floating point variable (64bits)  */
typedef signed int          intx;
typedef unsigned int        uintx;
#endif

#ifndef CONFIG_TARGET_GAINZ 
typedef signed char         int8;
typedef unsigned char 	    uint8;
typedef signed short        int16; // todo?
typedef unsigned short 	    uint16; // todo?
typedef signed long	        int32;
typedef unsigned long 	    uint32;
typedef long long 		    int64;
typedef unsigned long long  uint64;
typedef float          	    fp32;             /* single precision floating point variable (32bits)  */
typedef double         	    fp64;             /* double precision floating point variable (64bits)  */
typedef signed int          intx;
typedef unsigned int        uintx;
#endif

#ifndef CONFIG_WINDOWS
	#ifndef NULL
		#define NULL 0
	#endif
#endif

#ifndef __cplusplus
	#define bool char
    #define true 1
    #define false 0
#endif

#ifndef BOOL
	#define BOOL char
#endif

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#define BYTE uint8
#define WORD uint16
#define DWORD uint32
#define QWORD uint64

#endif
