#include "../configall.h"
#include "../foundation.h"
#include <stdlib.h>
#include "../hal/hal.h"
#include "../service/svc_location.h"
#include "../global.h"
#include "location_test.h"

#define ANCHOR_NODE_1
//#define ANCHOR_NODE_2
//#define ANCHOR_NODE_3
//#define LOCATION_REQUEST_NODE

static TLocationService m_locservice;
static TLocation  m_location;

void location_test()
{  
    target_init();
    global_construct();
    spi_configure( g_spi );
    uart_configure( g_uart, CONFIG_UART_BAUDRATE, 0, 0, 0, 0 );
    cc2420_configure( g_cc2420, CC2420_BASIC_INIT, 0, 0);
    cc2420_configure( g_cc2420, CC2420_CONFIG_PANID, g_cc2420->panid, 0);    
    //initialization

/*  
    g_loservice 	= lcs_construct( (char*)(&m_locservice), sizeof(TLocationService));

    #ifdef LOCATION_REQUEST_NODE
    g_loservice->state = 0x00;
    #endif
    
    #ifdef ANCHOR_NODE_1
    g_loservice->state = 0x80;
    g_loservice->nodeid = 1;
    #endif
    
    #ifdef ANCHOR_NODE_2
    g_loservice->state = 0x80;
    g_loservice->nodeid = 2;
    #endif
    
    #ifdef ANCHOR_NODE_3
    g_loservice->state = 0x80;
    g_loservice->nodeid = 3;
    #endif

    lcs_init(g_loservice,g_cc2420);
    
    while(1) 
    {
      lcs_evolve(g_loservice,&m_location,g_cc2420);   //enter location service
                                                      //return x,y,z coordination 
                                                      //in the structure of m_location
    }*/
}