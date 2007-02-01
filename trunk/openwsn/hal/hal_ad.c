
#include "hal_foundation.h"
#include <stdlib.h>
#include "hal_ad.h"
#include "hal_uart.h"
#include "hal_global.h"


/* id�����һλ����ͨ�����ٸ�һλΪad��ѡ�ߣ���14 ��Ϊad1����ͨ�� */
TAdConversion * ad_construct( uint8 id, char * buf, uint8 size )
{  
	
	TAdConversion *ad;
	
	char* out_string = "ad consturct succesful!\n";
   	
   	if (sizeof(TAdConversion) > size)
		ad = NULL;
	else
		
		ad = (TAdConversion *)buf;
		
	if (ad != NULL)
	{
		memset( (char*)ad, 0x00, sizeof(TAdConversion) );
	 	
		ad->id = id;
		switch(id)
		{
		 case 0:  PINSEL1 = 0x00400000;break;
		 case 1:  PINSEL1 = 0x01000000;break;
		 case 2:  PINSEL1 = 0x04000000;break;
		 case 3:  PINSEL1 = 0x10000000;break;
		 case 4:  PINSEL1 = 0x00040000;break;
		 case 5:  PINSEL1 = 0x00100000;break;//?datasheet not consisitent
		 case 6:  PINSEL0 = 0x00000300;break;
		 case 7:  PINSEL0 = 0X0000C000;break;
		 default: break;
		}     
	     /*
����            switch(id)
                {
                case 0:  PINSEL1 = 0x00400000;break;
                case 1:  PINSEL1 = 0x01000000;break;         
����            case 2:  PINSEL1 = 0x04000000;break;
����            case 3:  PINSEL1 = 0x10000000;break;
����            case 4:  PINSEL1 = 0x00040000;break;
                case 5:  PINSEL1 = 0x00100000;break;//?datasheet not consisitent
����            case 6:  PINSEL0 = 0x00000300;break;
����            case 7:  PINSEL0 = 0X0000C000;break;
����            //case 10:  PINSEL0=0x00003000;break;
����            //case 11:  PINSEL0=0x00030000;break;
����            //case 12:  PINSEL0=0x00300000;break;
����            //case 13:  PINSEL0=0x03000000;break;
                //case 14:  PINSEL0=0x0c000000;break;
����            //case 15:  PINSEL0=0xc0000000;break;
����            //case 16:  PINSEL1=0x00000800;break;
����            //case 17:  PINSEL1=0x00001000;break;
                  default:  break;
��           }*/
		 
             uart_write(g_uart, out_string,24, 0);
	}

	return ad ; 
}

void ad_configutre(TAdConversion * ad)
{
	//ad->id = 0;
	AD0CR = (1 << 3)						|	// SEL=8,ѡ��ͨ��3
			((Fpclk / 1000000 - 1) << 8)	|	// CLKDIV=Fpclk/1000000-1,ת��ʱ��Ϊ1MHz
			(0 << 16)						|	// BURST=0,�������ת������
			(0 << 17)						|	// CLKS=0, ʹ��11clockת��
			(1 << 21)						|  	// PDN=1,��������ģʽ
			(0 << 22)						|  	// TEST1:0=00,��������ģʽ
			(1 << 24)						|	// START=1,ֱ������ADCת��
			(0 << 27);						 	// ֱ������ADCת��ʱ����λ��Ч
}

void ad_destroy( TAdConversion * ad )
{
	if (ad)
	{
		//ad->callback = NULL;
	}
}

// not used now.
void ad_start( TAdConversion * ad, TEventHandler callback, void * owner )
{
	//ad->callback = callback;
	//ad->callback_owner = owner;
}

uint16 ad_read( TAdConversion * ad, char * buf, uint8 size, uint8 opt )
{      
       
       uint32  temp;
       uint16  value;
       
       temp = AD0DR;		// ��ȡADC����������DONE��־λ
       
       AD0CR |= 1 << 24;					// ���е�һ��ת��
       while ((AD0DR & 0x80000000) == 0);	// �ȴ�ת������
       AD0CR |= 1 << 24;					// �ٴ�����ת��
       while ((AD0DR & 0x80000000) == 0);	// �ȴ�ת������
		
       
      
       temp=AD0DR;
       temp = (temp >> 6) & 0x3ff;
       
       value = (uint16) temp;
       
       return value;       		        		        
}

