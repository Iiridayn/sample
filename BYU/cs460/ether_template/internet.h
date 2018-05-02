// internet.h
#include <ctype.h>
#include "protocol.h"
 
#ifndef INTERNET_H
#define INTERNET_H

#define IP_ADDRESS_LEN 4

class internet: public protocol
{
private:

public:
  int curlen;
  int poplen; 

  int init (char *name, protocol *higher, protocol *lower,
   char *addr, int port, int client);
  int push (unsigned char *buf, int len, char *addr);
  int pop (unsigned char *buf, int len);
};

#endif
