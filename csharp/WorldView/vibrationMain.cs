using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace WorldView
{
   
    public partial class vibrationMain : Form
    {
        LocalService service = new LocalService();
        private unsafe void* svc = null;
        private TRoutePathCache pathCache;
        private TRoutePathItem  pathitem;
        private byte pathItemCnt;
        
        private dataRevCache revDataCache;
        private byte dataRevItemCnt;
      
        public vibrationMain()
        {
            InitializeComponent();
            unsafe
            {
                svc = service.Start(100, 0);
            }
            pathCache = service.getPathCache();
            pathItemCnt = pathCache.itemCount();
            revDataCache = service.getRevDataCache();
            dataRevItemCnt = revDataCache.getcount();
        }
       
        private void queryTimer_Tick(object sender, EventArgs e)
        {
           service.phaseDataRev();
           ushort nodeid = service.getSink();
           if (nodeid != 0) MessageBox.Show(nodeid.ToString());
        }

        private void dispData_Click(object sender, EventArgs e)
        {
                   
           //ListView view = new ListView();
           ListViewItem nodeid = listViewDataRev.Items.Add("nodeid", 0);
           ListViewItem datatype = listViewDataRev.Items.Add("datatype", 1);
           ListViewItem contents = listViewDataRev.Items.Add("contents", 2);
           byte [] tempdata = new byte[128];
           int len = 0;
           dataRevItem dataitem = new dataRevItem();
           
           for (byte index = 0; index < dataRevItemCnt; index++)
           {
              dataitem = revDataCache.getDataItem(index);
              len = dataitem.Read(ref tempdata,128,0);
              if (len > 0)
              {
                  nodeid.SubItems.Add(dataitem.nodeid.ToString());
                  datatype.SubItems.Add(dataitem.datatype.ToString());
                  string text = Encoding.UTF8.GetString(tempdata,0,len);
                  contents.SubItems.Add(text);
              }
           }
        }

        private void dispRoute_Click(object sender, EventArgs e)
        {
                               
            //ListView view = new ListView();
            ListViewItem nodeid = listViewRouteInfo.Items.Add("nodeid", 0);
            ListViewItem contents = listViewRouteInfo.Items.Add("contents", 1);
          
            int len = 0;
            TRoutePathItem routeitem = new TRoutePathItem();

            for (byte index = 0; index < pathItemCnt; index++)
            {
                routeitem = pathCache.getPathItem(index);
                int j =0;
                byte[] tempdata = new byte[128];
                
                for (int i = 0; i < routeitem.getleaptotal();i++)
                {
                    tempdata[j++] = (byte) routeitem.getLeapStep(i);
                    tempdata[j++] = (byte)(routeitem.getLeapStep(i) >> 8);
                }

                if (len > 0)
                {
                    nodeid.SubItems.Add(routeitem.getSrcNode().ToString());
                    string text = Encoding.UTF8.GetString(tempdata, 0, j);
                    contents.SubItems.Add(text);
                }
            }
        }

        
        private void sendCmd_Click(object sender, EventArgs e)
        {
     
            DataType cmdtype = new DataType();
            switch(cmdList.Text.Trim())
            {
              ��case "��ȡSink�ڵ�":
                    cmdtype = DataType.DATA_TYPE_GET_NODE_ID_REQUEST;
                    break;
                case "·������":
                    cmdtype = DataType.DATA_TYPE_ROUTE_REQUEST;
                    break;
                case "�𶯲�ѯ":
                    cmdtype = DataType.DATA_TYPE_VIBSENSOR_QUERY_REQUEST;
                    break;
                case "��������":
                    cmdtype = DataType.DataStream;
                    break;
            }
            byte [] packet = new byte[60];
            
            byte len =  service.generatePacketFrame(ref packet,service.payload,pathitem,cmdtype);
            service.Write(packet,len,0);

            if (!queryTimer.Enabled) queryTimer.Enabled = true;
        }

        private void listViewRouteInfo_SelectedIndexChanged(object sender, EventArgs e)
        {
            //LocalService.routpath = listViewRouteInfo.Items[listViewRouteInfo.Items.IndexOf(this.)];
        }
    }
}