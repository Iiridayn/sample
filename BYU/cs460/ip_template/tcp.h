// tcp.h
#include <ctype.h>
#include "protocol.h"
#include "internet.h"
 
#ifndef TCP_H
#define TCP_H

#define TCP_ADDRESS_LEN 2 // Holds the port number

class tcp: public protocol {
private:

public:
  int packet_count;
  int amclient;
  unsigned char my_ipaddr[IP_ADDRESS_LEN];
  unsigned char my_tcpaddr[TCP_ADDRESS_LEN];

  int init (char *name, protocol *higher, protocol *lower, 
   char *addr, int port, int client);
  int push (unsigned char *buf, int len, char *addr);
  int pop (unsigned char *buf, int len);
};

#endif
