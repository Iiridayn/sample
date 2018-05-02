// ethernet.h


#include <ctype.h>
#include <string.h>
#include "protocol.h"
 
#ifndef ETHERNET_H
#define ETHERNET_H

#define ETH_ADDRESS_LEN 6

#define BASIC_FRAME_SIZE 	18   //bytes in a frame w/o message or sentinels
typedef unsigned char BYTE;


class ethernet: public protocol {
private:
	//The following are parts of the ethernet frame
	unsigned char destAddr[ETH_ADDRESS_LEN];
        unsigned char srcAddr[ETH_ADDRESS_LEN], myAddr[ETH_ADDRESS_LEN];
	unsigned char ethmessage[1500], ethStuffed [2500];
	unsigned long crc;

public:
	
  	int init (char *name, protocol *higher, protocol *lower, 
   	  char *addr, int port, int client);
  	int push (unsigned char *buf, int len, char *addr);
  	//int test (unsigned char *buf, int len, char *addr, message *mb);
  	int pop (unsigned char *buf, int len);
};

#endif
