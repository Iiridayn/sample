// ethernet.h
#include <ctype.h>
#include "protocol.h"
 
#ifndef ETHERNET_H
#define ETHERNET_H

class ethernet: public protocol {
private:

public:
  int curlen;
  int amclient;

  int init (char *name, protocol *higher, protocol *lower, 
   char *addr, int port, int client);
  int push (unsigned char *buf, int len, char *addr);
  int pop (unsigned char *buf, int len);
};

#endif
