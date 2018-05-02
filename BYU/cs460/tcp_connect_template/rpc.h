// rpc.h
#include <ctype.h>
//#include "protocol.h"
#include "internet.h"
 
#ifndef RPC_H
#define RPC_H

#define TCP_ADDRESS_LEN 2 // Holds the port number

class rpc: public protocol {
private:

public:
  int packet_count, remaining;
  int amclient;
  unsigned char my_ipaddr[IP_ADDRESS_LEN];
  unsigned char my_tcpaddr[TCP_ADDRESS_LEN];
  int errors;

  int init (char *name, protocol *higher, protocol *lower, 
   char *addr, int wsize, int client);
  int push (unsigned char *buf, int len, char *addr);
  int pop (unsigned char *buf, int len);
};

#endif
