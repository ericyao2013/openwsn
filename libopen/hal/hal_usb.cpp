#include "hal_usb.h"

//----------------------------------------------------------------------------
// refer to: C++ Builder ���� USB �ڵķ��� (Victor Chen)
// ���뻷��: Borland C++ Builder 6.0, Windows Server 2003 DDK
// ���л���: Win98/2000/XP/2003
//----------------------------------------------------------------------------

#define CONFIG_USE_WINDDK
#undef CONFIG_USE_WINDDK

#ifdef CONFIG_USE_WINDDK

#include <vcl.h>
#include <dir.h>
#include <setupapi.h>
#include "C:/WINDDK/3790/inc/ddk/w2k/usbdi.h"
#include "C:/WINDDK/3790/inc/ddk/w2k/devioctl.h"
#include <initguid.h>


//----------------------------------------------------------------------------
// USB �豸��USB ������USB Ӧ�ó���
//
// 1.USB �豸Ӳ������
//  a.���Ӳ���ı�ʶ���õ� Vender ID �� Product ID, �������ұ�ʶ���͡���Ʒ��ʶ��
//  b.���Ӳ���涨�˸��� End Point (�˵�) ������, ��/д �� ���� (Control/Interrupt/Bulk/Isochronous)
//  c.���Ӳ���Ĺ̼������� DeviceIoControl ��ʵ�ֲ���, �涨����������ľ�������Ͷ���
//
// 2.USB �豸����
// ��Ӳ���ӿ�
//  a.��Ҫʶ�� Vender ID �� Product ID
//  b.��ÿ�� EndPoint ��ÿ�� I/O ����һ�� Pipe, ������һ��������Ϊ����ӿ�
//  c.�� DeviceIoControl �Ľӿ�
// ������ӿ�
//  a.GUID, ��������ı�ʶ, ÿ����������ʹ�ò�ͬ�� GUID, GUID ��ʶ��������, ��Ӳ���޹� (�������������汾 GUID �����޸�)
//  b.Ӳ���ӿ������ b: Pipe ����������ӿ�, ��� Pipe ���ִ��������������, ��Ӳ���޹�, �����������ܸ� Pipe ������
//  c.Ӳ���ӿ������ c �ĸ�������Ҳ������Ľӿ�, ��Щ��������Ӳ��������, ���������涨��, ��ȻҲ��������������ת��, �����豸����ʵ���
// ����������������� WinDDK �����, �������ı��༭���������������ߵı༭����������, Ȼ����� WinDDK ����
//
// 3.��д USB �ڵĳ���
// ���������Ľӿ�
//  a.����������������� GUID �ҳ��豸���ļ���, �� CreateFile �������豸����ǰ��ĳ�������� OpenUsbDevice �����������
//  b.ͨ�� a.�õ����豸�ļ������������������ Pipe ���� Pipe, ������� Pipe ��Ӧ�� USB �˵� (��д����)
//  c.ʹ�� a.�� CreateFile �õ��ľ��, ͨ�� DeviceIoControl ʵ���豸�涨�Ķ���
// ���й���Ҫ������
//  a.Vender ID, Product ID �� GUID һ������������� .inf �ļ������ܿ���, ����Ҳ�������Ҫ�ͳ�����ϵ
//  b.Pipe ����������������涨��, ��Ҫ��������������ϲ���֪��
//  c.DeviceIoControl �Ĳ�����Ҫ��������������ϻ���Ӳ�����ϲ���֪��
// ���������һ���� C/C++ ֱ�ӱ�д, ���ʹ����������(VB/PB��)��Ҫ���� C/C++ ��� DLL
//
//
// �����������:
//
// USB ����������Ե�ע��������ҵ���
// "HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Enum\\USB\\Vid_���ұ�ʶ&Pid_��Ʒ��ʶ\\��������"
//
// ����� ClassGUID ������������� GUID ��ʶ, ���� {36FC9E60-C465-11CF-8056-444553540000}
// �൱�ڳ����: DEFINE_GUID(USB_DRIVER_GUID, 0x36FC9E60,0xC465,0x11CF,0x80,0x56,0x44,0x45,0x53,0x54,0x00,0x00);
// ���������ע�������滹���ҵ��й��豸����������, ���� DeviceDesc = "USB Mass Storage Device" ��
//
// ��ο���վ: �й� BCB ���� USB �ڵ�����, ���滹�в�����ص����ݡ�
// 	��ַ: http://www.cppfans.com/
//
//----------------------------------------------------------------------------



//---------------------------------------------------------------------------
// �������Ϊ��������� GUID ֵ, ��������д����
DEFINE_GUID(USB_DRIVER_GUID, 0x12345678,0xabcd,0x1122,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0x00);
//---------------------------------------------------------------------------
HANDLE OpenOneDevice(HDEVINFO hDvcInfo, PSP_INTERFACE_DEVICE_DATA DvcInfoData, char *sDevNameBuf)
{
  HANDLE hOut = INVALID_HANDLE_VALUE;

  ULONG  iReqLen = 0;
  SetupDiGetInterfaceDeviceDetail(hDvcInfo, DvcInfoData, NULL, 0, &iReqLen, NULL);

  ULONG iDevDataLen = iReqLen; //sizeof(SP_FNCLASS_DEVICE_DATA) + 512;
  PSP_INTERFACE_DEVICE_DETAIL_DATA pDevData = (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc(iDevDataLen);

  pDevData->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
  if(SetupDiGetInterfaceDeviceDetail(hDvcInfo, DvcInfoData, pDevData, iDevDataLen, &iReqLen, NULL))
   {
     strcpy(sDevNameBuf, pDevData->DevicePath);
     hOut = CreateFile(pDevData->DevicePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
   }

  free(pDevData);
  return hOut;
}
//---------------------------------------------------------------------------
HANDLE OpenUsbDevice(const GUID *pGuid, char *sDevNameBuf)
{
  HANDLE hOut = INVALID_HANDLE_VALUE;

  HDEVINFO hDevInfo = SetupDiGetClassDevs(pGuid, NULL, NULL, DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);

  SP_INTERFACE_DEVICE_DATA deviceInfoData;
  deviceInfoData.cbSize = sizeof (SP_INTERFACE_DEVICE_DATA);

  ULONG nGuessCount = MAXLONG;
  for(ULONG iDevIndex=0; iDevIndex<nGuessCount; iDevIndex++)
   {
     if(SetupDiEnumDeviceInterfaces(hDevInfo, 0, pGuid, iDevIndex, &deviceInfoData))
      {
        if((hOut=OpenOneDevice(hDevInfo, &deviceInfoData, sDevNameBuf)) != INVALID_HANDLE_VALUE)
          break;
      }
     else if(GetLastError() == ERROR_NO_MORE_ITEMS) //No more items
      {
        break;
      }
   }
  SetupDiDestroyDeviceInfoList(hDevInfo);
  return hOut;
}
//---------------------------------------------------------------------------
bool GetUsbDeviceFileName(const GUID *pGuid, char *sDevNameBuf)
{
  HANDLE hDev = OpenUsbDevice(pGuid, sDevNameBuf);
  if(hDev != INVALID_HANDLE_VALUE)
   {
     CloseHandle(hDev);
     return true;
   }
  return false;
}
//---------------------------------------------------------------------------
HANDLE OpenMyDevice()
{
  char DeviceName[MAXPATH] = "";
  return OpenUsbDevice(&USB_DRIVER_GUID, DeviceName);
}
//---------------------------------------------------------------------------
HANDLE OpenMyDevPipe(const char *PipeName)
{
  char DeviceName[MAXPATH] = "";
  if(GetUsbDeviceFileName(&USB_DRIVER_GUID, DeviceName))
   {
     strcat(DeviceName,"\\");
     strcat(DeviceName,PipeName);
     return CreateFile(DeviceName, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
   }
  return INVALID_HANDLE_VALUE;
}
//---------------------------------------------------------------------------

��������ĺ����Ϳ��Է��� USB ����:
//�� USB �ڶ�д, ����������� Pipe ��ȷ��

HANDLE hPipe = OpenMyDevPipe("MyPipe1"); //������������� Pipe ��, ��Ӧ����ĳ���˵�� I/O, ��������д��, ��Ҫ������һ��
if(hPipe != INVALID_HANDLE_VALUE) //�� Pipe �ɹ�
 {
   ReadFile(hPipe, Buffer, BufSize, &nBytesRead, NULL); //�� hPipe ���ȡ���ݵ� Buffer ��
   //WriteFile(hPipe, Buffer, BytesToWrite, &nBytesWritten, NULL); //�� Buffer ����� BytesToWrite �ֽ�д�� hPipe
   CloseHandle(hPipe);
 }

//ʹ�� DeviceIoControl ���� USB �豸

HANDLE hDevice = OpenMyDevice();
if(hDevice != INVALID_HANDLE_VALUE) //���豸�ɹ�
 {
   //��Щ DeviceIoControl ���ܶ������豸�����, ���忴�豸������������
   if(DeviceIoControl(hDevice, IOCTL_READ_xxxx, &IOBlock, sizeof(IOBLOCK), &c, 1, &nBytes, NULL))
    {
      //�ɹ�
    }
   CloseHandle(hDevice);
}

#endif // CONFIG_USE_WINDDK