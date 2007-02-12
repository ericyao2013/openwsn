/*****************************************************************************
 * This file is part of OpenWSN, the Open Wireless Sensor Network System.
 *
 * Copyright (C) 2005,2006,2007,2008 zhangwei (openwsn@gmail.com)
 * 
 * OpenWSN is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 or (at your option) any later version.
 * 
 * OpenWSN is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with eCos; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 * 
 * As a special exception, if other files instantiate templates or use macros
 * or inline functions from this file, or you compile this file and link it
 * with other works to produce a work based on this file, this file does not
 * by itself cause the resulting work to be covered by the GNU General Public
 * License. However the source code for this file must still be made available
 * in accordance with section (3) of the GNU General Public License.
 * 
 * This exception does not invalidate any other reasons why a work based on
 * this file might be covered by the GNU General Public License.
 * 
 ****************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Collections;
using System.Threading;

//using System.ComponentModel;
//using System.Data;
//using System.Drawing;
//using System.Windows.Forms;


// @modified by zhangwei on 20070123
// for Zhou Songli
//
// - pls add a separate module as LocalService. this is a encapsulation of the DLL functions
// all the GUI should call the functions in LocalService rather call the DLL directly
//
// - namespace changed to WorldView
// - i also changed your local settings to adopt relative directory rather than absolute ones
// so others can easyliy use your project file.
//
namespace WorldView
{
    /* 
     public struct RouteAddr
     {
         private ushort srcAddr;
         private ushort dstAddr;//destionateion node address;   
         private ushort [] leapStep;        
         private byte steptotal;
        
         //public interface function;
         public void setSrcAddr(ushort source) { srcAddr = source;}
         public void setDestAddr(ushort destination) { dstAddr = destination; }
         public ushort getSrcAddr() {return (srcAddr);}
         public ushort getDestAddr() {return (dstAddr);}
        
         public ushort getIndexAddr(byte index) 
         {
             ushort result = 0;
             if (index > totaleap) return result;
             return (leapStep[index]);
         }
         public void addleap(ushort leap)
         {
             leapStep[steptotal++] = leap;
         }
     }
     */
    public enum DataType
    {
        DATA_TYPE_GET_NODE_ID_REQUEST = 0x00,
        DATA_TYPE_ROUTE_REQUEST,
        DATA_TYPE_TEMPSENSOR_QUERY_REQUEST ,
        DATA_TYPE_VIBSENSOR_QUERY_REQUEST,
        DATA_TYPE_STRAINSENSOR_QUERY_REQUEST,
        DATA_TYPE_LIGHTSENSOR_QUERY_REQUEST,
        DATA_TYPE_GET_NODE_ID_ACK = 0x08,
        DATA_TYPE_ROUTE_ACK,        
        DATA_TYPE_TEMPSENSOR_QUERY_ACK,        
        DATA_TYPE_VIBSENSOR_QUERY_ACK,        
        DATA_TYPE_STRAINSENSOR_QUERY_ACK,        
        DATA_TYPE_LIGHTSENSOR_QUERY_ACK,
        DataStream
    }

    public struct PacketFrame
    {
        /* The FCF(frame control field) occupies one byte.It is an unsigned integer.The structure of the FCF  is defined as follow:

      D3~D0 bits indicate the data type in the application. the range of the data value is between 0 and 15.

      D7~D4 bits show the number of the hop in the route trace in this frame.The maximum value is 15 and the minimum value is 1.*/

        public byte PacketControl;
        public byte seqNumber;
        //public ushort srcNodeid;
        //public ushort destNodeid;
        //public ushort[] leapStep;
        public byte[] pData;//[100-sizeof(RouteAddr)];//里面的常数不能修改；
    }
      
    class LocalService
    {
        [DllImport("libsink", EntryPoint = "svc_read", CharSet = CharSet.Auto)]
        public unsafe static extern byte svc_read(void* svc, [In, Out] byte* buf, byte capacity, ushort opt);

        [DllImport("libsink", EntryPoint = "svc_write")]
        public unsafe static extern byte svc_write(void* svc, char[] buf, byte len, ushort opt);

        [DllImport("libsink", EntryPoint = "svc_create")]
        public unsafe static extern void* svc_create(ushort id, ushort opt);                                                        

        [DllImport("libsink", EntryPoint = "svc_start")]
        public unsafe static extern void* svc_start(void* svc);

        [DllImport("libsink", EntryPoint = "svc_stop")]
        public unsafe static extern void* svc_stop(void* svc);

        [DllImport("libsink", EntryPoint = "svc_uart_open")]
        public unsafe static extern sbyte svc_uart_open(void* svc, byte[] name);

        [DllImport("libsink", EntryPoint = "svc_uart_close")]
        public unsafe static extern sbyte svc_uart_close(void* svc);

        [DllImport("libsink", EntryPoint = "svc_uart_configure")]
        public unsafe static extern sbyte svc_uart_configure(void* svc, uint baudrate, byte databits, byte stopbits, byte parity, byte optflag);


        private  unsafe void* svc = null;
        //private  ushort id, opt;

        private const byte MAX_DATA_REV_NUMBER = 0x0f;

        private const byte MAX_ROUTE_PATH_NUMBER = 200;

        private const byte MAX_ROUTE_PATH_OPTIMAL_NUMBER = 50;

        private const byte DATA_TYPE_MASK = 0x0F;
        private const byte DATA_TYPE_BM = 0x0;

        private const byte ROUTE_ADDRLIST_MASK = 0x0F;
        private const byte ROUTE_ADDRLIST_BM = 0x04;

        private const byte ADDRLIST_BM = 0xF0;

        private const byte MAX_LEAP_NUMBER = 0x0a;
        private const byte MAX_ROUTE_LIST_NUMBER = 0x04;

        private static ushort sinknode;

        public  byte maxhop;
        public  UInt32 updateperiod;

        private  TRoutePathCache pathCache;
        private  TRoutePathItem routpath;
        private  dataRevCache revCache;
       
        private  byte seqNum;
        public   byte[] payload;
        
        public ushort getSink() { return (sinknode);}
        public UInt32 getUpdatePeriod() {return (updateperiod);}
        public void setUpdatePeriod(UInt32 period) { updateperiod = period; }
        public void setSink(ushort node) { sinknode = node; }
       
        public TRoutePathCache getPathCache() { return (pathCache);}
        public TRoutePathItem getpathItem() { return (routpath);}
        public dataRevCache getRevDataCache() { return (revCache);}
        
        public byte getSequenceNumber(){ return (seqNum);}      

        public unsafe void* Start(ushort id,ushort opt)
        {
            unsafe
            {
                svc = svc_create(id, opt);
                if (svc != null)
                svc_start(svc);

            }

            maxhop = MAX_ROUTE_PATH_NUMBER;
            pathCache = new TRoutePathCache();
            pathCache.construct(MAX_ROUTE_PATH_NUMBER);
            revCache = new dataRevCache();
            revCache.construct(10);           
            routpath = new TRoutePathItem();            
            seqNum = 1;
            return svc;
        }


        public void Stop()
        {
            unsafe
            {
                svc_stop(svc);
            }

        }

        /* The command at the upper layer in this application occupies two bytes.It is 
       an unsigned short in c shape. The structure of the command is defined as follow:

       D3~D0 bits denote the data type in the application. the range of the data value is between 0 and 15.

       D7~D4 bits denote the number of the hop in the route trace in this frame. The maximum value is 15 and 
       the minimum value is 1.

       D8 bit denotes whether optimum route trace is selected.It means the operations of read,write,update and
       delete only work on the optimum route trace if the bit is set to 1. otherwise,these operations will work 
       on all route trace.

       D9 bit denotes whether the operations of read,write,update and delete only work on the appointed single node.

       It means these operations only work on the appointed single node if the value of the bit is 1.otherwise, these
       operations will work on all nodes.

       D13~D10 bits are not used now and reserved for later version.
       D15~D14 denotes the type of the command.
       That the value of D15~D14 is 00 means the type of the command is read;
        01 means the type is add (insert);
        10 means the type is update;
        11 means the type is delete.
       */

        public int Read(ref byte[] buf, byte size, ushort opt)
        {
            byte cnt = 0;
            unsafe
            {

                lock (this)
                {
                    //cnt = svc_read(svc,out DataReved.Text.ToCharArray(),(byte)DataReved.Text.Length, 0);
                    fixed (byte* p = buf)
                    {
                        cnt = svc_read(svc, p, size, 0);
                    }

                }
            }

            /*  if (cnt > 0)
              {
                Encoding.UTF8.GetString(revData, 0, revData.Length);
                //  revNumber.Text = "已接收字符：" + cnt.ToString();
                //  revNumber.Visible = true;
              }
            */
            return cnt;
        }
        
        public int Write(byte[] buf, byte size, ushort opt)
        {
            byte cnt =10;
            lock (this)
            {
                unsafe
                {
                   // cnt = svc_write(svc, buf, size, opt);
                }
            }

            return cnt;
        }


       // public int ReadDataPacket(ref byte[] packet, ushort size, ushort opt)
        public void phaseDataRev()
        {
            byte[] tempdata = new byte[128];
            int len = Read(ref tempdata, 128, 0);
            ushort srcNode, dstNode;
            ushort nextleap = 0;
            int RouteleapNumber = 0;
            int i = 0;//, j = 0;
            if (len < 1) return;
            //check the datatype.        
            DataType datatype = (DataType)(tempdata[i] & DATA_TYPE_MASK >> DATA_TYPE_BM);
            RouteleapNumber = tempdata[i++] & ROUTE_ADDRLIST_MASK >> ROUTE_ADDRLIST_BM;
                       
            PacketFrame pframe = new PacketFrame();
            pframe.seqNumber = tempdata[i++];
            srcNode = tempdata[i++];
            srcNode += (ushort)(tempdata[i++] << 8);
            dstNode = tempdata[i++];
            dstNode += (ushort)(tempdata[i++] << 8);
/*   public struct PacketFrame
    {
        public byte PacketControl;
        public byte seqNumber;    
        public byte[] pData;//[100-sizeof(RouteAddr)];//里面的常数不能修改；
    }  
  */ 
            /*
            for (j = 0; j < RouteleapNumber; j++)
            {
                nextleap = tempdata[i++];
                nextleap += (ushort)(tempdata[i++] << 8);
                pframe.leapStep[j] = nextleap;
            }
            */
            i++;//the second datum is the  sequence number of the data received;
            len = tempdata[i++];//the third one is the length of the data received.

            switch (datatype)
            {
                case DataType.DATA_TYPE_GET_NODE_ID_ACK:
                    srcNode = tempdata[i++];
                    srcNode +=(ushort)(tempdata[i++]<<8);
                    sinknode = srcNode;
                    break;
                case DataType.DATA_TYPE_ROUTE_ACK://路由反馈包                    
                    TRoutePathItem pathitem = new TRoutePathItem();
                    pathitem.construct(srcNode, dstNode, RouteleapNumber, false);

                    for (i = 0; i < RouteleapNumber; i++)
                    {
                       // nextleap = pframe.leapStep[i];
                        pathitem.addleap(nextleap);
                    }
                    pathCache.appendRoutePath(pathitem);
                    
                    //更新节点序列，需要检查一下是否已经存在，避免重复
                    return;

                case DataType.DATA_TYPE_LIGHTSENSOR_QUERY_ACK:
                    //pframe.pData.CopyTo(packet, 0);
                    dataRevItem item = new dataRevItem();
                    item.construct(srcNode, 128);
                    item.Write(tempdata, (ushort)len, 0);
                    revCache.appendataItem(item);
                    break;

                case DataType.DataStream:
                    break;
                /*
                   case DataType.QueryFeekback:
                       break;
                */
            }
    
           return;
        }
        public int WriteDataPacket(byte[] packet, ushort size, ushort opt) { return 0; }
        public int ReadRoutingPacket(byte[] packet, ushort size, ushort opt)
        {
            return 0;
        }
        public int GetSinkState() { return 0; }
        public int GetNodeData() { return 0; }

        public byte generatePacketFrame(ref byte[] packet, byte[] payload,TRoutePathItem routePath, DataType datatype)
        {
            byte i = 0;
            int index;
            ushort nextleap;
            byte PacketControl = (byte)(((byte)datatype) << DATA_TYPE_BM);

            if (datatype != DataType.DATA_TYPE_GET_NODE_ID_REQUEST && datatype != DataType.DATA_TYPE_ROUTE_REQUEST)
            {
                PacketControl &= (byte)(routePath.getleaptotal() << ROUTE_ADDRLIST_BM);
            }
            packet[i++] = PacketControl;
            packet[i++] = seqNum++;

            if (datatype != DataType.DATA_TYPE_GET_NODE_ID_REQUEST && datatype != DataType.DATA_TYPE_ROUTE_REQUEST)
            {
                //有路由表信息或者信息不是发给sink节点的；
                ushort nodeid = routePath.getSrcNode();
                packet[i++] = (byte)nodeid;
                packet[i++] = (byte)(nodeid >> 8);
                nodeid = routePath.getDestNode();
               
                packet[i++] = (byte)nodeid;
                packet[i++] = (byte)(nodeid >> 8);

                for (index = 0; index < routePath.getleaptotal(); index++)
                {
                    nextleap = routePath.getLeapStep(index);
                    packet[i++] = (byte)nextleap;
                    packet[i++] = (byte)(nextleap >> 8);
                }
                
            }                 

            if (datatype == DataType.DataStream)
            {
                packet[i++] = (byte)(payload.Length);
                for (index = 0; index < payload.Length; index++)
                    packet[i++] = payload[index];
            }
            else
            {
                packet[i++] = 0;
            }
            return (i);
        }
      
     /*     public int phasePacket( byte[] packet, ushort size, ushort opt)
           {
               byte[] tempdata = new byte[128];
               int len = Read(ref tempdata, 128, 0);
               ushort nextleap = 0;
               int RouteleapNumber = 0;
               int i = 0, j = 0;
               if (len < 1) return 0;
               //check the datatype.        
               DataType datatype = (DataType)(tempdata[i] & DATA_TYPE_MASK >> DATA_TYPE_BM);
               RouteleapNumber = tempdata[i++] & ROUTE_ADDRLIST_MASK >> ROUTE_ADDRLIST_BM;

               PacketFrame pframe = new PacketFrame();
               pframe.seqNumber = tempdata[i++];
               pframe.srcNodeid = tempdata[i++];
               pframe.srcNodeid += (ushort)(tempdata[i++] << 8);
               pframe.destNodeid = tempdata[i++];
               pframe.destNodeid += (ushort)(tempdata[i++] << 8);

               for (j = 0; j < RouteleapNumber; j++)
               {
                   nextleap = tempdata[i++];
                   nextleap += (ushort)(tempdata[i++] << 8);
                   pframe.leapStep[j] = nextleap;
               }

               len = tempdata[i++];

               for (j = 0; j < len; j++)
               {

                   pframe.pData[j] = tempdata[i++];
               }


               switch (datatype)
               {
        

                   case DataType.RouteFeedback://路由反馈包                    
                       TRoutePathItem pathitem = new TRoutePathItem();
                       pathitem.construct(pframe.srcNodeid, pframe.destNodeid, RouteleapNumber, false);

                       for (i = 0; i < RouteleapNumber; i++)
                       {
                           nextleap = pframe.leapStep[i];
                           pathitem.addleap(nextleap);
                       }

                       pathCache.appendRoutePath(pathitem);
                       return 0;
                   case DataType.QueryData:
                       pframe.pData.CopyTo(packet, 0);
                       break;                
               }

               return len;
           }
        */
    }
}
