/******************************************************************************
 * This file is part of OpenWSN, the Open Wireless Sensor Network System.
 *
 * Copyright (C) 2005,2006,2007 zhangwei (openwsn@gmail.com)
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
 *****************************************************************************/ 

#ifndef _HAL_CC2420_H_1278_
#define _HAL_CC2420_H_1278_

/******************************************************************************
 * @author zhangwei on 2006-07-20
 * TCc2420
 * This is an software abstraction of the cc2420 transceiver hardware. you can 
 * fully manipulate the cc2420 hardware through this object. 
 * 
 * @attention
 * 	- TCc2420 is different to TSpiDriver. TSpiDriver is only
 * the encapsulation of SPI communication. It shouldn't has relationships with 
 * cc2420. 
 *  - this module is a software abstraction and encapsulation of cc2420 hardware. 
 * it should be as simple as possible.
 * 	- some one may regards this module as the PHY layer of the whole system. 
 * however, it may contain some MAC functions. This is due to the reason that 
 * the chip "cc2420" is 802.15.4 MAC supported.
 * 
 * @TODO
 * 	try to implement standard PHY functions based on this module in the future.
 * 
 * @modified by huanghuan on 2006-08-10
 * - modified the interface function. 
 * - changes a lot data type from uint16 to uint8, because cc2420 support maximum
 * 	frame length of 256.
 * - add interface function: cc2420_configure, cc2420_sleep, cc2420_wakeup
 * 
 * @modified by huanghuan on 2006-08-01
 * porting the old source code successfully
 * testing passed. 
 * 
 * @modified by zhangwei on 2006-09-06
 * update the comment. add a lot of descriptions.
 * revise code logic and the interface. 
 * ��ƻ����ձ��ļ��ṩ�Ľӿڶ�ԭ������е�������ע����������
 * 
 * @modified by zhangwei on 2006-09-11
 * add support to sleep in the state machine
 * revised the state machine and other source code.
 * 
 * @modified by zhangwei on 2006-10-14
 * integrated with huanghuan's code. and do much more format revisions.
 *
 * @modified by zhangwei on 2006-11-19
 * formulate the I/O interface of cc2420 drvier
 *
 * uint8 cc2420_rawread( TCc2420 * cc, char * buf, uint8 size, uint8 opt );
 * ��2420 driver�ڲ�buffer�ж���һ��frame, size��ʾ����buf��ָ�������Ĵ�С
 * return the byte count moved to parameter "frame" buffer.
 * ����ʵ��move���ֽ���
 * ���sizeƫС�Ų��£���ôֻ���size���ַ���������Ϊ�����������ο����㹻���buffer������rawread
 * ����buf��ָ��buffer�н�����һ��������PHY frame�����������ޱ䶯
 * ���û�����ݰ��ɶ����򷵻�0
 * opt��������
 * 
 * uint8 cc2420_rawwrite( TCc2420 * cc, char * buf, uint8 len, uint8 opt );
 * ��char * buf�е�frame���ͳ�ȥ
 * �������buf��ָ���ڴ����Ѿ�������һ��������cc2420 phy frame���������ݶ��Ѿ����ڴ����ź���
 * ����ʵ�ʷ��ͳɹ����ֽ�����һ�������Ӧ�õ���len����ʾȫ�����ͳɹ�
 * ������һ��������frame��Ϊ���ͺͽ��յ�λ����֧�ַ��ͻ��߽��հ��frame
 *
 * int8 cc2420_sendframe( TCc2420 * cc, TCc2420Frame * frame, uint8 opt );
 * ������cc2420_rawwrite ����frame��Ϣ��ͨ��һ���ṹ��TCc2420Frame���ݽ����ģ��������ڴ����ź��ٴ�������
 * 
 * int8 cc2420_recvframe( TCc2420 * cc, TCc2420Frame * frame, uint8 opt );
 * ������cc2420_rawread, ����һ��frame�����ǽ��յ������ݲ����������ڴ��У����Ƿ���һ���ṹ����
 * opt�е�ĳһλָ���Ƿ���broadcast������ͨ�ķ��ͣ��Ƿ���ҪACK
 * opt�е����ý����ǲ���frame�е�����
 * 
 * #define cc2420_rawbroadcast(cc,frame,len,opt) cc2420_rawwrite(cc,frame,len,opt|0x01)
 * #define cc2420_broadcast(cc,frame,opt) cc2420_sendframe(cc,frame,opt|0x01)
 * �㲥һ��frame��ȥ����ʵ���ǵ���rawwrite����sendframe
 * broadcast frame�ǲ���ҪACK��
 * 
 * ɾ��read_stream��write_stream
 * ���Բ�֧��cc2420_read()��cc2420_write()
 *
 * @modified by zhangwei on 20070510
 * add function cc2420_open() and cc2420_close(). 
 * 
 * cc2420_init() is deprecated. you needn't call it in your source code. it will
 * called automatically in cc2420_configure(). 
 * 
 * @modified by makun on 20070511
 * add support to disable address recognition for sniffer applications.
 * 
 *****************************************************************************/

#include "hal_foundation.h"
#include "hal_configall.h"
#include "hal_spi.h"
#include "hal_cc2420chip.h"
#include "hal_openframe.h"
 
/*******************************************************************************
 * IEEE 802.15.4 PPDU format
 * [4B Preamble][1B SFD][7b Framelength, 1b Reserved][nB PSDU/Payload]
 * 
 * IEEE 802.15.4 MAC DATA format (the payload of PHY frame)
 * Beacon Frame
 * [2B Frame Control] [1B Sequence Number][4 or 10 Address][2 Superframe Specification]
 * 		[k GTS fields][m Padding address fields] [n Beacon payload][2 FCS]
 * 
 * Data Frame
 * [2B Frame Control] [1B Sequence Number][4 or 20 Address][n Data Payload][2 FCS]
 * 
 * ACK Frame
 * [2B Frame Control] [1B Sequence Number][2 FCS]
 * 
 * MAC Control Frame
 * [2B Frame Control] [1B Sequence Number][4 or 20 ADdress][1 Command Type][n Command Payload][2 FCS]
 * 
 ******************************************************************************/

#define TCc2420Driver TCc2420
#define CC2420_BUF_CAPACITY 1 

#ifdef CONFIG_TARGET_OPENNODE_10
#define FIFO            8  // P0.8  - Input: FIFO from CC2420
#define FIFOP           9  // P0.9  - Input: FIFOP from CC2420
#define CCA            10  // p0.10 - Input:  CCA from CC2420
#define RESET_N        12  // P0.12 - Output: RESET_N to CC2420
#define VREG_EN        13  // P0.13 - Output: VREG_EN to CC2420
#define SFD            16  // P0.16 - Input:  SFD from CC2420
#define CSN            21  // P1.21 - Output: SPI Chip Select (CS_N)  

#define SFD_PORT       0
#define CCA_PORT       0     
#define FIFO_PORT      0    
#define FIFOP_PORT     0     
#define VREG_EN_PORT   0       
#define RESET_N_PORT   0
#define CSN_PORT       1  
#endif

#ifdef CONFIG_TARGET_OPENNODE_20
#define FIFO           22  
#define FIFOP          15  
#define CCA            13  
#define RESET_N        12 
#define VREG_EN        10  
#define SFD            16 
#define CSN            21  

#define SFD_PORT       0
#define CCA_PORT       0     
#define FIFO_PORT      1    
#define FIFOP_PORT     0     
#define VREG_EN_PORT   0       
#define RESET_N_PORT   0
#define CSN_PORT       1  
#endif

#ifdef CONFIG_TARGET_WLSMODEM_11
#define FIFO           16  // P0.16  - Input: FIFO from CC2420
#define FIFOP          15  // P0.15  - Input: FIFOP from CC2420
#define CCA            12  // p0.12 - Input:  CCA from CC2420
#define RESET_N        23  // P1.23 - Output: RESET_N to CC2420
#define VREG_EN        10  // P0.10 - Output: VREG_EN to CC2420
#define SFD            11  // P0.11 - Input:  SFD from CC2420
#define CSN            17  // P0.17 - Output: SPI Chip Select (CS_N)  

#define SFD_PORT       0
#define CCA_PORT       0     
#define FIFO_PORT      0    
#define FIFOP_PORT     0     
#define VREG_EN_PORT   0       
#define RESET_N_PORT   1
#define CSN_PORT       0  
#endif

// the default settings when the cc2420 transceiver first started
#define CC2420_DEFAULT_PANID 1
#define CC2420_DEFAULT_ADDRESS 1
#define CC2420_DEFAULT_CHANNEL  19         //channel is 11 - 26

/* cc2420 state constants */
enum { CC_STATE_IDLE=0, CC_STATE_SLEEP, CC_STATE_POWERDOWN };
    
/* config control code */    
#define CC2420_CONFIG_PANID 			0x01 
#define CC2420_CONFIG_LOCALADDRESS		0x02 
#define CC2420_CONFIG_TUNNING_POWER 	0x03
#define CC2420_CONFIG_CHANNEL			0x04
#define CC2420_BASIC_INIT               0x05
#define CC2420_XTAL_SWITCH              0x06
#define CC2420_CONFIG_APPLY				0x07
#define CC2420_SET_ACKREQUEST           0x08
#define CC2420_CONFIG_SNIFFER_MODE      0x09

/* power level
 * POWER_1 is the highest, while POWER_8 is the lowest
 * the default settings is @TODO 
 */
#define CC2420_POWER_1  0x01       //  0dBm   17.4mA
#define CC2420_POWER_2  0x02       // -1dBm   16.5mA
#define CC2420_POWER_3  0x03       // -3dBm   15.2mA
#define CC2420_POWER_4  0x04       // -5dBm   13.9mA
#define CC2420_POWER_5  0x05       // -7dBm   12.5mA
#define CC2420_POWER_6  0x06       //-10dBm   11.2mA
#define CC2420_POWER_7  0x07       //-15dBm    8.9mA
#define CC2420_POWER_8  0x08       //-25dBm    8.5mA

#define cc2420_broadcast(cc,frame,len,opt) cc_2420_rawwrite(cc,frame,len,opt|0x01)

/* TCc2420Frame/TOpenFrame is define is hal_openframe.h. 
typedef struct{
  uint8 	length;
  uint16 	control;
  uint8  	seqid;
  uint16 	panid;
  uint16  nodeto;
  uint16 	nodefrom;
  uint8  	payload[OPF_PAYLOAD_SIZE];
  uint16  footer;
}TOpenFrame;
*/
#define TCc2420Frame TOpenFrame

typedef struct {
  TCc2420Frame pRxInfo;
  uint8 payload_length;
  UINT8 seqid;
  //volatile BOOL ackReceived;
  WORD panid;
  WORD myAddr;
  BOOL receiveOn;
  uint8 rssi;
}BASIC_RF_SETTINGS;

/* TCc2420 object
 * this structure represent a cc2420 object in the system. you can interact with 
 * the transceiver cc2420(by TI.com/chipcon.com) with this object. this object 
 * is designed to hide all the hardware details and provide an abstract interface
 * for upper layer application.
 * 
 * attention that cc2420 is a 802.15.4 MAC compatible transceiver. however, the 
 * TCc2420 is a MAC object. i think the TCc2420 is below the MAC layer and higher 
 * than the PHY layer.
 *  
 * 	mode			general mode and sniffer mode
 * 	spi				used for the core MCU to communicate with the transceiver
 * 	channel			communication channel setting. represents the frequency.
 * 	                the valid channel value varies from 11 to 26 . 
 * 	                while, the frequvency f = 2405 + 5*(channel - 11) MHz
 * 	rxbuf			used to hold the frame just received
 * 	rxlen			the data length in the rxbuf. if "rxlen == 0", then means 
 * 					their's no frame received.
 * 	txbuf			used to hold the frame to be sent
 * 	txlen			the data length of the frame to be sent. if "txlen == 0",
 * 	                then means the TCc2420 is avaiable for sending new frames
 *
 * @TODO: whether txbuf and rxbuf should be volatile or not? 
 */ 
typedef struct{
  uint8 state;
  uint8 nextstate;
  TSpiDriver * spi; 
  uint16 panid;
  uint16 address;
  uint8 channel; 
  volatile uint8 txlen;
  volatile uint8 rxlen;
  TCc2420Frame txbuffer[CC2420_BUF_CAPACITY];
  TCc2420Frame rxbuffer[CC2420_BUF_CAPACITY];
  char txbuf[CC2420_BUF_CAPACITY];
  char rxbuf[CC2420_BUF_CAPACITY];
  volatile BASIC_RF_SETTINGS rfSettings;
  uint8  sleeprequest;
  uint8  power;
  uint8  ackrequest;    
  volatile bool received_ack;
  uint8  rssi;                   //���һ�ν��յ�����Ϣ���ź�ǿ��
  uint8  receivepacket_len;      //���һ�ν��յ��İ����ܳ���
  uint8  receivepayload_len;     //���һ�ν��յ��İ���payload�ĳ���
  uint8  sendpayload_len;        //���һ�η��͵İ���payload����
}TCc2420;



/* The following variable is declared usually in "global.c". However, it is used
 * in the interrupt service routine of this module. Be sure the variable name 
 * cannot be modified! it must be "g_cc2420"!
 */
extern TCc2420 * g_cc2420;
 
TCc2420 * cc2420_construct( char * buf, uint16 size, TSpiDriver * spi );
void cc2420_destroy( TCc2420 * cc );

/*******************************************************************************
 * configure the cc2420 driver object.
 * you may add more parameters in the functions. 
 * different to the parameters in the construct function, you can use this function
 * to modify parameters without rebooting the system to make them take effect.
 * 
 * @param
 * idx 
 * 	1	���õ�ַ
 * 	2	�������书��
 * 	3	��������Ƶ��
 * pvalue	the buffer pointers to the configure data. it is also the buffer
 * 			to receive the results.
 * size		buffer size.
 ******************************************************************************/ 
 
 /* @modified by zhangwei on 20061108
  * �һ��Ǿ��ò�Ҫ��uint16, ����void * buf���� void * pvalue�ȽϺã����ͺ����
  * size����һ���ʾ�������ݡ���uint16���ƴ��뷽�㣬����ʹ���߻�û�й�������
  * ��������Ҫ���ȽϺ���,���Ҵ���ҲҪ��ǿ������ת����pvalue��ʽǿ���û�Ҫ����size. 
  * д�����ԭ���ǽӿ�ԭ��Ӧ������������δ�����󣬶�������ǰ��Ҫ��������ʵ�ֿ���
  * �ģ�������ԭ��һ��ȷ�����Ͳ������ױ䶯�������պ�ά�����׳���
  */
void cc2420_configure( TCc2420 * cc, uint8 ctrlcode, uint16 value, uint8 size );
void cc2420_init(TCc2420 * cc);
void cc2420_open( TCc2420 * cc );
void cc2420_close( TCc2420 * cc ); 

/* return the cc2420 driver state.
 * attention that state is used for other modules. it is not the same as "mode".
 */
uint8 cc2420_state( TCc2420 * cc );

void cc2420_set_power(TCc2420 * cc,uint8 power);
uint8 cc2420_ioresult( TCc2420 * cc );

/*******************************************************************************
 * read data from cc2420 driver
 * read data from the driver's internal buffer. the interrupt service routine 
 * is responsible to place the received data into the internal buffer.
 * you can always call this function to check whether there has data in the internal 
 * buffer no matter what state the wireless chip "cc2420" is. 
 * 
 * @attention
 * you may lost the input data when you cannot call this function fast enough.
 * 
 * @param
 * 	buf			the memory to receive the data
 * 	size		buffer size
 * 	opt			option settings.
 * @return
 * 	the character count copied successfully to the buffer
 ******************************************************************************/ 
uint8 cc2420_read( TCc2420 * cc,TCc2420Frame * frame,uint8 size,uint8 opt);

/*******************************************************************************
 * return the received frame entirely to the frame buffer. 
 * different to xxx_read() function, this function will not return when a frame
 * received entirely. For "cc2420" chip, the frame is a standard 802.15.4 frame.
 * 
 * @attention
 * 	- similar to cc_read(), you'll lost the received frame if you cannot call this 
 * function quickly enough.
 * 	- when this function is effective, you must guarantee the internal buffer is 
 * large enough to hold a frame.
 * 
 * @warning 
 * 	problems may be encountered when you switch between "cc2420_read()" and 
 * "cc2420_rawread()". rawread will always assume the second byte in the internal 
 * buffer as frame length and use it to check whether the whole frame received.
 * you'll encounter problems when there's still some data left in the buffer 
 * when you call cc2420_rawread(). you'd better identify the frame start position
 * using another member variable to improve reliability in the future.
 ******************************************************************************/ 
uint8 cc2420_rawread( TCc2420 * cc, char * frame, uint8 size, uint8 opt );
int8 cc2420_write( TCc2420 * cc, TCc2420Frame * frame, int8 len, uint8 opt);
int8 cc2420_rawwrite( TCc2420 * cc, char * frame, int8 len, uint8 opt );

/*******************************************************************************
 * this function is used mainly by the driver itself.
 * it will check whether there are some data to be sent. if there's data in the 
 * txbuf, then it starts the sending process. 
 * 
 * this function doesn't check whether there are some data received like some other
 * drivers, because the data receiving process is interrupt driven.
 * 
 * generally, you needn't to call this function in your source code.
 ******************************************************************************/ 
int8 cc2420_evolve( TCc2420 * cc );

/* ����cc2420Ӳ���豸
 * ���ڴ�Power Off / Power Down״̬�ָ���������������״̬
 */
void cc2420_startup( TCc2420 * cc );

/* �ر�cc2420Ӳ���豸
 * ���������շ�������״̬����Power off/Power down״̬
 * ע��ͬʱ��Ҫ����driver�ڲ���״̬��־
 */
//void cc2420_shutdown( TCc2420 * cc );

/* ���� */
//void cc2420_sleep( TCc2420 * cc );

/* �������л��� */
void cc2420_wakeup( TCc2420 * cc );

// @TODO: 20061026
// huanghuan: you should use the following setchannel
//void cc2420_setchannel( TCc2420 * cc, uint8 channel );
// not this one:
void cc2420_setchannel( TCc2420 * cc, uint8 channel );


void cc2420_receive_on(TCc2420 * cc);
void cc2420_receive_off(TCc2420 * cc);
void cc2420_waitfor_crystal_oscillator(TSpiDriver * spi);

//void cc2420_interrupt_init( void );
//void cc2420_event_handler(void);

#endif /* _HAL_CC2420_H_ */
