// arp.h
#include <ctype.h>
#include "protocol.h"
#include <string.h>
 
#ifndef ARP_H
#define ARP_H

#define ARP_TABLE_SIZE 10

class arp: public protocol {
private:
	typedef struct {
		unsigned char ip[4];
		unsigned char eth[6];
	} arptableentry; // not need timeouts for this lab
	arptableentry arptable[ARP_TABLE_SIZE];
	char arptableindex;
public:
  unsigned char ipaddr[4];
  unsigned char ethaddr[6];

  int init (char *name, protocol *higher, protocol *lower, 
   char *addr, int port, int client);
  int push (unsigned char *buf, int len, char *addr);
  int pop (unsigned char *buf, int len);

};

#endif
