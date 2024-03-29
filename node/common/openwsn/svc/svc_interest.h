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

#ifndef _SVC_INTEREST_4768_
#define _SVC_INTEREST_4768_

/**
 * This module implements the "interest" object required by the Directed Diffusion 
 * protocol (in module "svc_dd").
 */

#define DD_CTRL_REQUEST 0x01
#define DD_CTRL_RESPONSE 0x02

typedef struct{
  uint8 ctrl;
  uint32 address;
  uint8 iid;
  uint8 seq;
  uint32 timestamp;
  void * payload;
}TiDdInterest;

typedef struct{
  TDdInterest[20] interests;
  uint8[20] age; 
}TiDdCache;

dd_cache_construct
dd_cache_destroy
dd_cache_insert
dd_cache_remove
dd_cache_find

#endif
