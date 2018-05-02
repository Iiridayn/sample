// ethernet.h
#include <ctype.h>
#include "protocol.h"
#include "message.h"
#include <unistd.h>
#include <netinet/in.h>

#ifndef ETHERNET_H
#define ETHERNET_H

#define ETH_ADDRESS_LEN 6

#define BASIC_FRAME_SIZE 	18   //bytes in a frame w/o message or sentinels
#define MAXBUF 100
#define MAX_MSG 56
//#define AMBLE 0b01111110
#define AMBLE 0x7E
typedef unsigned char BYTE;


class ethernet: public protocol {
private:
	//The following are parts of the ethernet frame
	char MAC[ETH_ADDRESS_LEN];
	//vars for stuffing

	//FUNCTIONS
	int bitstuff(char *buf, int len);
	int unstuff(char *buf, int len, int start, int *length);
	int genCRC(char *buf, int len);
public:
	
	int init (char *name, protocol *higher, protocol *lower, char *addr, int port, int client);
  	int push (const unsigned char *buf, int len, char *addr);
  	int pop (unsigned char *buf, int len);
};

#endif
