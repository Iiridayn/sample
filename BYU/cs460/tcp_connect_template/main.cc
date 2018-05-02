//
// This is the test configuration for protocols
//

#include "protocol.h"
#include "physical.h"
#include "ethernet.h"
#include "arp.h"
#include "internet.h"
#include "tcp.h"
#include "rpc.h"
#include "nat.h"
#include <ctype.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

physical *phys;
ethernet *eth;
arp *arpp;
internet *ip;
tcp *tcpp;
rpc *rpcp;

int blabby;
int gerrors;
int process_args(char **argv,char *my_ip,char *my_ethaddr,char *dest_ip,char *dest_ethaddr);

#define MAXLEN 4
#define MIN_ARGS 9//minimum required arguemtns
#define MY_TCP {0x28, 0xff}

//
//  Hard-coded TCP port for the lab.  In order to connect correctly to
//	your webserver, you may need to alter this value to match the port
//	that your webserver listen on for a connection.
//	
//  Here, its value is: 0x0929 = 2345
//
//#define DEST_TCP { 0x09, 0x29 }

int main ( int argc, char* argv[] )
{
  int len, i, port, i_am_client;
  unsigned char buff[1000], inchars[10];
  char my_ethaddr[ETH_ADDRESS_LEN];
  char my_ipaddr[IP_ADDRESS_LEN];
  char dest_ipaddr[IP_ADDRESS_LEN];
  char dest_ethaddr[ETH_ADDRESS_LEN];
  char arpaddr[ETH_ADDRESS_LEN+IP_ADDRESS_LEN];
  char tcpaddr[TCP_ADDRESS_LEN+IP_ADDRESS_LEN];
  char tcpaddr1[TCP_ADDRESS_LEN] = MY_TCP;
  //char tcpaddr2[TCP_ADDRESS_LEN] = DEST_TCP;//put further down
  char *my_tcpaddr;
  char *dest_tcpaddr;

  // Declare protocols on the heap so that we can use threads

  phys = new physical;
  eth = new ethernet;
  arpp = new arp;
  ip = new internet;
  tcpp = new tcp;
  rpcp = new rpc;
  char *sendaddr = new char[11];

  gerrors = 0;

  if ( argc < MIN_ARGS ) 
	{
    cerr << "Usage:" << argv[0] << " hostname, port, src_ip, src_eth, dst_ip, dst_eth <-s or -c> [-v 0xff]" << endl;
    exit ( 1 );
  }

  port = atoi( argv[2] );
  
  
  /*code to take the DEST_TCP port as a command line argument, rather than #define*/
  unsigned short DEST_TCP = (unsigned short) atoi(argv[MIN_ARGS-1]);
  DEST_TCP = htons(DEST_TCP);
  char tcpaddr2[TCP_ADDRESS_LEN];
  memcpy(tcpaddr2, &DEST_TCP, TCP_ADDRESS_LEN);  

  cout << "DEST TCP PORT::" << DEST_TCP << endl; 
  cout << "tcpaddr2::" << tcpaddr2 << endl;
  print_addr("tcpaddr2::",(unsigned char *)tcpaddr2,TCP_ADDRESS_LEN);
  
	//
  // Check to see if verbose mode was selected.
  //
  if( ( argc > MIN_ARGS ) && ( strcmp( argv[MIN_ARGS],"-v" ) == 0))
  {
    //
    //  In turn, the argv[ 5 ] must be change to argv[ 9 ].
    //
    blabby = strtol(argv[MIN_ARGS+1],NULL,0);
    printf( "verbose mode %X\n", blabby );
  }
    
  else
  {
    blabby = 0;
  }

	//
	//	The server section is not used for the TCP connect lab.
	//
  if(strcmp(argv[7],"-s") == 0)
  { 
    process_args(argv,dest_ipaddr,dest_ethaddr,my_ipaddr,my_ethaddr);
    i_am_client = 0;
    my_tcpaddr = tcpaddr2;
    dest_tcpaddr = tcpaddr1;
  }
  else
  {
    process_args( argv, my_ipaddr, my_ethaddr, dest_ipaddr, dest_ethaddr );
    i_am_client = 1;
    my_tcpaddr = tcpaddr1;
    dest_tcpaddr = tcpaddr2;
  }

  memset( sendaddr,0,sizeof(sendaddr) );
  memcpy( sendaddr,&my_ipaddr,IP_ADDRESS_LEN );
  memcpy( sendaddr+4,&my_ethaddr,ETH_ADDRESS_LEN );

  print_addr("src ip address",(unsigned char *)my_ipaddr,IP_ADDRESS_LEN);
  print_addr("dst ip address",(unsigned char *)dest_ipaddr,IP_ADDRESS_LEN);
	
	//
	// Initialize the protocols
	//
  eth->init ("ethernet", arpp, phys, my_ethaddr, 0, i_am_client);
  arpp->init ("ARP", ip, eth, sendaddr, 0, i_am_client);
  ip->init ("internet", tcpp, arpp, my_ipaddr, 1, i_am_client);
  //
  //  Copy my TCP address into the tcp addr.
  //
  memcpy( &tcpaddr[0], my_tcpaddr, TCP_ADDRESS_LEN );
  memcpy( &tcpaddr[TCP_ADDRESS_LEN], my_ipaddr, IP_ADDRESS_LEN );

  tcpp->init ("tcp", rpcp, ip, tcpaddr, MAXLEN, i_am_client);

  phys->init ("physical", eth, NULL, argv[1], port, i_am_client);
  rpcp->init ("rcp", NULL, tcpp, tcpaddr, 0, i_am_client);

	//
	//	Your 'connect' function chould return a value indicating the status of the connection.
	//	If a connection was properly established, return 0 (zero).  Otherwise return a non-zero value.
	//	(e.g., -1).
	//
  int connect_ret_val = tcpp->connect( (unsigned char *) dest_ipaddr, (unsigned char *) dest_tcpaddr );

  //
  //  Wait for the connection to establish.
  //

  if ( connect_ret_val == 0 )
  {
    cout << "Connection established! " << endl;
  }
  else
  {
    cerr << "Failed to connect.  Return value: " << connect_ret_val << endl;
    exit( 0 );
  }

	//
	//	We'll send the get command here.
	//	
  cout << "Sending get command:" << endl;

  char getbuff[] = "GET test1.html/ HTTP/1.0\r\n\r\n";

  /* Send get message */
  memcpy(&tcpaddr[0],dest_tcpaddr,TCP_ADDRESS_LEN);
  memcpy(&tcpaddr[TCP_ADDRESS_LEN],dest_ipaddr,IP_ADDRESS_LEN);
  print_chars( "main sending get message ", (unsigned char *)getbuff, strlen(getbuff) );
  print_addr( "to", (unsigned char *)tcpaddr, TCP_ADDRESS_LEN + IP_ADDRESS_LEN );

	//
	//	Use this as a visual separator between the get command and the response.
	//
  cout << "=============================================================================" << endl << endl;

	//
	//	Push the message directly onto the tcp connection.
	//
  tcpp->push( (unsigned char *) getbuff, (unsigned int) strlen(getbuff), (char *) tcpaddr );

  getchar();
  cout << "Test Complete" << endl;

	return 0;
}

int process_args(char **argv,char *my_ip,char *my_ethaddr,char *dest_ip,char *dest_ethaddr) 
{
  unsigned int tmp_addr[IP_ADDRESS_LEN+ETH_ADDRESS_LEN];
  int i, rval;

  // Pull out the src and dst addresses
  // First the src ip address (scanf needs an integer to read into
  rval = sscanf(argv[3],"%d.%d.%d.%d",&tmp_addr[0],&tmp_addr[1],
                &tmp_addr[2],&tmp_addr[3]);
  if(rval != 4) {
    printf("ip address1 malformed rval %d, %d.%d.%d.%d\n",
            rval,tmp_addr[0],tmp_addr[1], tmp_addr[2],tmp_addr[3]);
    exit(0);
  } else {
    // Assign to the real variable
    for(i = 0; i < IP_ADDRESS_LEN; i++)
      my_ip[i]=tmp_addr[i];
    //printf("ip address1 %d.%d.%d.%d\n",(unsigned char)my_ip[0],
    //      (unsigned char)my_ip[1],(unsigned char)my_ip[2],
    //        (unsigned char)my_ip[3]);
  }

  // Second the src eth address
  rval = sscanf(argv[4],"%x:%x:%x:%x:%x:%x",&tmp_addr[0],&tmp_addr[1],
          &tmp_addr[2],&tmp_addr[3],&tmp_addr[4],&tmp_addr[5]);
  if(rval != 6) {
    printf("eth address1 malformed rval %d, %x:%x:%x:%x:%x:%x\n",
            rval,tmp_addr[0],tmp_addr[1], tmp_addr[2],tmp_addr[3],
            tmp_addr[4],tmp_addr[5]);
    exit(0);
  } else {
    for(i = 0; i < ETH_ADDRESS_LEN; i++)
      my_ethaddr[i]=tmp_addr[i];
    //printf("eth address1 %x:%x:%x:%x:%x:%x\n",
    //       (unsigned char)my_ethaddr[0],(unsigned char)my_ethaddr[1],
    //      (unsigned char)my_ethaddr[2],(unsigned char)my_ethaddr[3],
    //      (unsigned char)my_ethaddr[4],(unsigned char)my_ethaddr[5]);
  }
  // Third the dst ip address
  rval = sscanf(argv[5],"%d.%d.%d.%d",&tmp_addr[0],&tmp_addr[1],
                &tmp_addr[2],&tmp_addr[3]);
  if(rval != 4) {
    printf("ip address2 malformed rval %d, %d.%d.%d.%d\n",
            rval,tmp_addr[0],tmp_addr[1], tmp_addr[2],tmp_addr[3]);
    exit(0);
  } else {
    for(i = 0; i < IP_ADDRESS_LEN; i++)
      dest_ip[i]=tmp_addr[i];
    //printf("ip address2 %d.%d.%d.%d\n",(unsigned char)dest_ip[0],(unsigned char)dest_ip[1],(unsigned char)dest_ip[2],(unsigned char)dest_ip[3]);
  }

  // Forth the dst eth address
  rval = sscanf(argv[6],"%x:%x:%x:%x:%x:%x",&tmp_addr[0],&tmp_addr[1],
          &tmp_addr[2],&tmp_addr[3],&tmp_addr[4],&tmp_addr[5]);
  if(rval != 6) {
    printf("eth address2 malformed rval %d, %x:%x:%x:%x:%x:%x\n",
            rval,tmp_addr[0],tmp_addr[1], tmp_addr[2],tmp_addr[3],
            tmp_addr[4],tmp_addr[5]);
    exit(0);
  } else {
    for(i = 0; i < ETH_ADDRESS_LEN; i++)
      dest_ethaddr[i]=tmp_addr[i];
    //printf("eth address2 %x:%x:%x:%x:%x:%x\n",
    //      (unsigned char)dest_ethaddr[0],(unsigned char)dest_ethaddr[1],
    //      (unsigned char)dest_ethaddr[2],(unsigned char)dest_ethaddr[3],
    //      (unsigned char)dest_ethaddr[4],(unsigned char)dest_ethaddr[5]);
  }
  return 0;
}





