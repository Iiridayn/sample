// internet.h
#include <ctype.h>
#include "protocol.h"
 
#ifndef INTERNET_H
#define INTERNET_H

#define IP_ADDRESS_LEN 4
#define MAX_FRAGMENT_SIZE 1024 // 1k
//#define MAX_ACTIVE_FRAGMENT 3
#define MAX_ACTIVE_FRAGMENT 100 //figure this is a good timeout/wraparound

class internet: public protocol {
private:
	typedef struct {
		unsigned char data[MAX_FRAGMENT_SIZE];
		int len;
		int total;
	} fragpack;
	short ident;
	
	fragpack fragdata[MAX_ACTIVE_FRAGMENT]; // 100k array
	
	char ipaddr[IP_ADDRESS_LEN];
	
	void checksum(unsigned char *buf, int len);
	void packetHeader(unsigned char *locbuf, int len);
	
public:
  int init (char *name, protocol *higher, protocol *lower, 
	    char *addr, int port, int client);
  int push (unsigned char *buf, int len, char *addr);
  int pop (unsigned char *buf, int len);
};

#endif
