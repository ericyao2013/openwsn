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


#ifndef _RTL_CONFIGALL_H_
#define _RTL_CONFIGALL_H_

/*****************************************************************************
 * @name rtl_configall.h
 * @author zhangwei on 20070331
 * 
 * configure file for runtime library(RTL)
 * 
 * @history
 * @modified by xxx on 200704
  * @modified by zhangwei on 20090708
  *	- eliminate RTL_DEBUG, USER_DEBUG. they should be replaced with CONFIG_DEBUG
 * 
 ****************************************************************************/ 

#include "../configall.h"

/* If you want to build a Dynamic Link Library(DLL). please define
 * compiler macro BUILD_DLL as the following before including this
 * interface file:
 *		#define BUILD_DLL
 *
 * By default, the whole source code will be compiled as DLL
 */
/*
#if (!defined(BUILD_STANDALONE) && !defined(BUILD_APPLICATION))
  #define BUILD_DLL
#endif

#ifdef DLLFUNC
  #undef DLLFUNC
#endif

#ifdef BUILD_DLL
  #define DLLFUNC __declspec(dllexport)
#else
  #define DLLFUNC __declspec(dllimport)
#endif

#if defined( BUILD_APPLICATION )
  #define MODULE_INTERFACE __declspec(dllimport)
#elif defined( BUILD_DLL )
  #define MODULE_INTERFACE __declspec(dllexport)
#elif defined( BUILD_STANDALONE )
  #define MODULE_INTERFACE extern
#endif

#define MODU_INTF MODULE_INTERFACE
*/
/******************************************************************/
#endif