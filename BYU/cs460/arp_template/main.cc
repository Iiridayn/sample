/***************************************************************************
                          main.cc  -  description
                             -------------------
    begin                : Wed May 17 2000
    copyright            : (C) 2000 by Trevor Clifton
    email                : Trevor_Clifton@bigfoot.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
// This is the test configuration for protocols


#include "protocol.h"
#include "physical.h"
#include "ethernet.h"
#include "internet.h"
#include "arp.h"

physical *phys;
ethernet *eth;
internet *ip;
arp *arpsend;

// Used to turn on debug messages
int blabby;
int process_args(char **,char *,char *,char *,char *);

#define MAXLEN 1000

#define OTHER1_IP {192,168,102,245}
#define OTHER2_IP {192,168,102,249}

int main (int argc, char* argv[]) 
{
  int len, i, port, rval;
  unsigned char buff[1000], inbuff[1000], inchars[10];
  // Set up the ethernet and IP addresses
  char my_ethaddr[ETH_ADDRESS_LEN];
  char my_ip[IP_ADDRESS_LEN];
  char dest_ethaddr[ETH_ADDRESS_LEN];
  char dest_ip[IP_ADDRESS_LEN];
  char other1_ipaddr[IP_ADDRESS_LEN]=OTHER1_IP; 
  char other2_ipaddr[IP_ADDRESS_LEN]=OTHER2_IP; 
  char sendaddr[ETH_ADDRESS_LEN+IP_ADDRESS_LEN];

  phys = new physical;
  eth = new ethernet;
  ip = new internet;
  arpsend = new arp;

  if (argc < 8) {
    cerr << "Usage:" << argv[0] << " hostname, port, src_ip, src_eth, dst_ip, dst_eth <-s or -c> [-v 0xff]" << endl;
    exit (1);
  }
  process_args(argv,my_ip,my_ethaddr,dest_ip,dest_ethaddr);
  if((argc >= 9) && (strcmp(argv[8],"-v") == 0)) 
    {
      blabby = strtol(argv[9],NULL,0);
      printf("verbose mode %X\n",blabby);
    } 
  else 
    {
      blabby = 0;
    }

  port = atoi(argv[2]);

 

  if(strcmp(argv[7],"-s") == 0) { // This is the server
  // Copy the ethernet address and IP address into the parameter for ARP init
    memset(sendaddr,0,sizeof(sendaddr));
    memcpy(sendaddr,&dest_ip,IP_ADDRESS_LEN);
    memcpy(sendaddr+4,&dest_ethaddr,ETH_ADDRESS_LEN);

  // Initialize the protocols
    phys->init ("physical", eth, NULL, argv[1], port, strcmp (argv[7], "-s"));
    eth->init ("ethernet", arpsend, phys, dest_ethaddr, 0, 0);
    arpsend->init ("ARP", ip, eth, sendaddr, 0, 0);
    ip->init ("internet", NULL, arpsend, my_ip, 0, 0);

    printf("When you see a message indicating that you have passed off the send part of lab, hit a CR to continue\n");
    getchar();
  } else { // Initialize the client
  // Copy the ethernet address and IP address into the parameter for ARP init
    memset(sendaddr,0,sizeof(sendaddr));
    memcpy(sendaddr,&my_ip,IP_ADDRESS_LEN);
    memcpy(sendaddr+4,&my_ethaddr,ETH_ADDRESS_LEN);

    printf("src ip address %d.%d.%d.%d\n",(unsigned char)my_ip[0],
            (unsigned char)my_ip[1],(unsigned char)my_ip[2],
            (unsigned char)my_ip[3]);
  // Initialize the protocols
    phys->init ("physical", eth, NULL, argv[1], port, strcmp (argv[7], "-s"));
    eth->init ("ethernet", arpsend, phys, my_ethaddr, 0, 0);
    arpsend->init ("ARP", ip, eth, sendaddr, 0, 0);
    printf("src ip address %d.%d.%d.%d\n",(unsigned char)my_ip[0],
            (unsigned char)my_ip[1],(unsigned char)my_ip[2],
            (unsigned char)my_ip[3]);
    ip->init ("internet", NULL, arpsend, my_ip, 0, 0);
  }
  // The server waits for the client to complete, then he sends 
  // the same thing back


  // Initialize the test pattern
  buff[0]=0xfe;
  buff[1]=0xed;
  buff[2]=0xbe;
  buff[3]=0xef;
  // Now send the test pattern to the correct place

  if(strcmp(argv[7],"-s") == 0) { // This is the server
    arpsend->push (buff, 4, my_ip);
  } else {
    int ch;
    // Send a string to make sure everything is working
    cout<<"Type a string to send"<<endl;
    memset(inbuff,0,MAXLEN);
    for( i = 0; (i < MAXLEN) &&  ((ch = getchar()) != '\n'); i++ )
      inbuff[i] = (char)ch;
    arpsend->push (inbuff, i, dest_ip);
    // Now send the test pattern
    arpsend->push (buff, 4, dest_ip);
    arpsend->push (buff, 4, other1_ipaddr);
    arpsend->push (buff, 4, other2_ipaddr);
    // There should be no arp before this send
    arpsend->push (buff, 4, dest_ip);
  }
      
  cout << "Send Test Complete, wait for an indication that receive worked before exiting" << endl;
  cin.getline((char*)buff, 10);

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
    printf("src ip address malformed rval %d, %d.%d.%d.%d\n",
            rval,tmp_addr[0],tmp_addr[1], tmp_addr[2],tmp_addr[3]);
    exit(0);
  } else {
    // Assign to the real variable
    for(i = 0; i < IP_ADDRESS_LEN; i++)
      my_ip[i]=tmp_addr[i];
    printf("src ip address %d.%d.%d.%d\n",(unsigned char)my_ip[0],
            (unsigned char)my_ip[1],(unsigned char)my_ip[2],
            (unsigned char)my_ip[3]);
  }

  // Second the src eth address
  rval = sscanf(argv[4],"%x:%x:%x:%x:%x:%x",&tmp_addr[0],&tmp_addr[1],
          &tmp_addr[2],&tmp_addr[3],&tmp_addr[4],&tmp_addr[5]);
  if(rval != 6) {
    printf("src eth address malformed rval %d, %x:%x:%x:%x:%x:%x\n",
            rval,tmp_addr[0],tmp_addr[1], tmp_addr[2],tmp_addr[3],
            tmp_addr[4],tmp_addr[5]);
    exit(0);
  } else {
    for(i = 0; i < ETH_ADDRESS_LEN; i++)
      my_ethaddr[i]=tmp_addr[i];
    printf("src eth address %x:%x:%x:%x:%x:%x\n",
            (unsigned char)my_ethaddr[0],(unsigned char)my_ethaddr[1],
            (unsigned char)my_ethaddr[2],(unsigned char)my_ethaddr[3],
            (unsigned char)my_ethaddr[4],(unsigned char)my_ethaddr[5]);
  }
  // Third the dst ip address
  rval = sscanf(argv[5],"%d.%d.%d.%d",&tmp_addr[0],&tmp_addr[1],
                &tmp_addr[2],&tmp_addr[3]);
  if(rval != 4) {
    printf("dst ip address malformed rval %d, %d.%d.%d.%d\n",
            rval,tmp_addr[0],tmp_addr[1], tmp_addr[2],tmp_addr[3]);
    exit(0);
  } else {
    for(i = 0; i < IP_ADDRESS_LEN; i++)
      dest_ip[i]=tmp_addr[i];
    printf("dst ip address %d.%d.%d.%d\n",(unsigned char)dest_ip[0],(unsigned char)dest_ip[1],(unsigned char)dest_ip[2],(unsigned char)dest_ip[3]);
  }

  // Forth the dst eth address
  rval = sscanf(argv[6],"%x:%x:%x:%x:%x:%x",&tmp_addr[0],&tmp_addr[1],
          &tmp_addr[2],&tmp_addr[3],&tmp_addr[4],&tmp_addr[5]);
  if(rval != 6) {
    printf("dst eth address malformed rval %d, %x:%x:%x:%x:%x:%x\n",
            rval,tmp_addr[0],tmp_addr[1], tmp_addr[2],tmp_addr[3],
            tmp_addr[4],tmp_addr[5]);
    exit(0);
  } else {
    for(i = 0; i < ETH_ADDRESS_LEN; i++)
      dest_ethaddr[i]=tmp_addr[i];
    printf("dst eth address %x:%x:%x:%x:%x:%x\n",
            (unsigned char)dest_ethaddr[0],(unsigned char)dest_ethaddr[1],
            (unsigned char)dest_ethaddr[2],(unsigned char)dest_ethaddr[3],
            (unsigned char)dest_ethaddr[4],(unsigned char)dest_ethaddr[5]);
  }
  return 1;
}
