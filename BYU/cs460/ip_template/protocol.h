// Base class for all protocols

#ifndef _PROTOCOl_H
#define _PROTOCOl_H

#include "message.h"

class protocol {
public:
  virtual ~ protocol ( ) { } ;
  char prot_name[80]; // This holds the name of the protocol "eth" etc.
  protocol *prot_higher, *prot_lower; // These are the adjacent protocols

  virtual int init (char *name, protocol *higher, protocol *lower, 
   char *addr, int port, int client);
  virtual int push (unsigned char *buf, int len, char *addr);
  virtual int pop (unsigned char *buf, int len);
};

#endif // _PROTOCOl_H
