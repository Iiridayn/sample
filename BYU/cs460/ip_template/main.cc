// This is the test configuration for protocols


#include "protocol.h"
#include "physical.h"
#include "ethernet.h"
#include "arp.h"
#include "internet.h"
#include "tcp.h"
#include "nat.h"
#include <ctype.h>
#include <iostream>
using namespace std;
#include <stdlib.h>
#include <stdio.h>

physical *phys;
ethernet *eth;
arp *arpp;
internet *ip;
tcp *tcpp;

int blabby;
int process_args(char **,char *,char *,char *,char *);

#define MAXLEN 30

#define MY_TCP {0x08, 0xff}

#define DEST_TCP {0x09, 0xee}

// Set up the NAT table for proxy translation
#define PROXY_INPUT_SUBNET1 {192,168,102,16}
#define PROXY_INPUT_SUBNET2 {192,168,102,32}
#define PROXY_OUTPUT_SRC_SUBNET1 {192,168,102,32}
#define PROXY_OUTPUT_SRC_SUBNET2 {192,168,102,16}
#define PROXY_OUTPUT_DEST_SUBNET1 {192,168,102,240}
#define PROXY_OUTPUT_DEST_SUBNET2 {192,168,102,240}
#define PROXY_SUBNET_MASK {255,255,255,240}
struct nat nat_table[]={{PROXY_INPUT_SUBNET1,
                         PROXY_OUTPUT_SRC_SUBNET1,
                         PROXY_OUTPUT_DEST_SUBNET1,
                         PROXY_SUBNET_MASK},
                        {PROXY_INPUT_SUBNET2,
                         PROXY_OUTPUT_SRC_SUBNET2,
                         PROXY_OUTPUT_DEST_SUBNET2,
                         PROXY_SUBNET_MASK},
                       };
// End of NAT table

int main (int argc, char* argv[]) 
{
  int len, i, port, i_am_client;
  unsigned char buff[1000], inchars[10];
  // Set up the ethernet address
  char my_ethaddr[ETH_ADDRESS_LEN];
  char my_ipaddr[IP_ADDRESS_LEN];
  char dest_ipaddr[IP_ADDRESS_LEN];
  char dest_ethaddr[ETH_ADDRESS_LEN];
  char arpaddr[ETH_ADDRESS_LEN+IP_ADDRESS_LEN];
  char tcpaddr[TCP_ADDRESS_LEN+IP_ADDRESS_LEN];
  char tcpaddr1[TCP_ADDRESS_LEN] = MY_TCP;
  char tcpaddr2[TCP_ADDRESS_LEN] = DEST_TCP;
  char *my_tcpaddr;
  char *dest_tcpaddr;

  // Declare protocols on the heap so that we can use threads
  // You should change the declaration of phys to be of class physical
  //   once you get your code written.  It should inherit from the class
  //   protocol so this code shouldnt know the difference.


  phys = new physical;
  eth = new ethernet;
  arpp = new arp;
  ip = new internet;
  tcpp = new tcp;
  char *sendaddr = new char[11];

  if (argc < 8) {
    cerr << "Usage:" << argv[0] << " hostname, port, src_ip, src_eth, dst_ip, dst_eth <-s or -c> [-v 0xff]" << endl;
    exit (1);
  }

  // If we are the server, swap the source and dest addrs
  if(strcmp(argv[7],"-s") == 0) { // This is the server
    process_args(argv,dest_ipaddr,dest_ethaddr,my_ipaddr,my_ethaddr);
    i_am_client = 0;
    my_tcpaddr = tcpaddr2;
    dest_tcpaddr = tcpaddr1;
  } else {
    process_args(argv,my_ipaddr,my_ethaddr,dest_ipaddr,dest_ethaddr);
    i_am_client = 1;
    my_tcpaddr = tcpaddr1;
    dest_tcpaddr = tcpaddr2;
  }

  // Turn on verbose mode
  if((argc >= 9) && (strcmp(argv[8],"-v") == 0)) 
    {
      blabby = strtol(argv[9],NULL,0);
      printf("verbose mode %X\n",blabby);
    } 
  else 
    {
      blabby = 0;
    }

  memset(sendaddr,0,11);
  memcpy(sendaddr,&my_ipaddr,4);
  memcpy(sendaddr+4,&my_ethaddr,6);

 

  if (argc < 4) {
    cerr << "Usage: " << argv[0] << " hostname, port, <-s or -c> {-v level}" << endl;
    cerr << "the bits for level are 0x1 for physical, 0x2 for ethernet, 0x80 for CRC" << endl;
    exit (1);
  }

  port = atoi(argv[2]);

// Copy the ethernet address and IP address into the parameter for ARP init
  memset(sendaddr,0,sizeof(sendaddr));
  memcpy(sendaddr,&my_ipaddr,IP_ADDRESS_LEN);
  memcpy(sendaddr+4,&my_ethaddr,ETH_ADDRESS_LEN);

  print_addr("src ip address",(unsigned char *)my_ipaddr,IP_ADDRESS_LEN);
  print_addr("dst ip address",(unsigned char *)dest_ipaddr,IP_ADDRESS_LEN);
// Initialize the protocols
  eth->init ("ethernet", arpp, phys, my_ethaddr, 0, i_am_client);
  arpp->init ("ARP", ip, eth, sendaddr, 0, i_am_client);
  ip->init ("internet", tcpp, arpp, my_ipaddr, 1, i_am_client);
  memcpy(&tcpaddr[0],my_tcpaddr,TCP_ADDRESS_LEN);
  memcpy(&tcpaddr[TCP_ADDRESS_LEN],my_ipaddr,IP_ADDRESS_LEN);
  tcpp->init ("tcp", NULL, ip, tcpaddr, 1, i_am_client);
  phys->init ("physical", eth, NULL, argv[1], port, i_am_client);
  
  
  if(i_am_client)  { // This is the client
    buff[0] = 0xde;
    buff[1] = 0xad;
    buff[2] = 0xbe;
    buff[3] = 0xef;
    len = 4;
  } else  { // This is the server
    printf("When you see a message indicating that you have passed off the send part of lab, hit a CR to continue\n");
    getchar();

    buff[0] = 0xfe;
    buff[1] = 0xed;
    buff[2] = 0xbe;
    buff[3] = 0xef;
    len = 4;
  }

  // First send a 4 byte message
  print_chars("main sending small segment ", buff, len);
  print_addr("to", (unsigned char *)dest_ipaddr, IP_ADDRESS_LEN);
  ip->push (buff, len, dest_ipaddr);
  sleep(1);

  /* Send data consisting of incrementing bytes */
  for(i = 0; i < sizeof(buff); i++) {
    buff[i] = i;
  }
  print_chars("main sending 31 byte segment ", buff, 31);
  print_addr("to", (unsigned char *)dest_ipaddr, IP_ADDRESS_LEN);
  ip->push (buff, 31, dest_ipaddr);
  sleep(1);

  print_chars("main sending 256 byte segment ", buff, 256);
  print_chars("to", (unsigned char *)dest_ipaddr, IP_ADDRESS_LEN);
  ip->push (buff, 256, dest_ipaddr);
  sleep(1);

#define notdef
#ifdef notdef  // You can enable this section if you want to type in a string
  int ch;
  //cout<<"Type a string to send"<<endl;//changed for nubuf on 2004/5/25
  //memset(buff,0,MAXLEN);
  //for( i = 0; (i < MAXLEN) &&  ((ch = getchar()) != '\n'); i++ )
    //buff[i] = (char)ch;

  cout<<"Type a string to send (500 chars max)"<<endl;
  unsigned char nubuf[1024];
  memset(nubuf, 0, 1024);
  cin.getline((char*)nubuf, 500);
  int sendlen = strlen((char*)nubuf);
  cout << "You sent:" << sendlen << " bytes\n";
  ip->push (nubuf, sendlen, dest_ipaddr);
    
  //ip->push (buff, i, dest_ipaddr);//changed for nubuf on 2004/5/25
#endif

  if(i_am_client) { // This is the client
    printf("Hanging around to forward NAT\n");
    getchar();
  } else {
    cout << "Test Complete" << endl;
  }
  return 0;
}


int process_args(char **argv,char *my_ip,char *my_ethaddr,char *dest_ip,char *dest_ethaddr) {
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
    printf("ip address1 %d.%d.%d.%d\n",(unsigned char)my_ip[0],
            (unsigned char)my_ip[1],(unsigned char)my_ip[2],
            (unsigned char)my_ip[3]);
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
    printf("eth address1 %x:%x:%x:%x:%x:%x\n",
            (unsigned char)my_ethaddr[0],(unsigned char)my_ethaddr[1],
            (unsigned char)my_ethaddr[2],(unsigned char)my_ethaddr[3],
            (unsigned char)my_ethaddr[4],(unsigned char)my_ethaddr[5]);
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
    printf("ip address2 %d.%d.%d.%d\n",(unsigned char)dest_ip[0],(unsigned char)dest_ip[1],(unsigned char)dest_ip[2],(unsigned char)dest_ip[3]);
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
    printf("eth address2 %x:%x:%x:%x:%x:%x\n",
            (unsigned char)dest_ethaddr[0],(unsigned char)dest_ethaddr[1],
            (unsigned char)dest_ethaddr[2],(unsigned char)dest_ethaddr[3],
            (unsigned char)dest_ethaddr[4],(unsigned char)dest_ethaddr[5]);
  }
  return(0);
}
