// tcp.h
#include <ctype.h>
//#include <synch.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "internet.h"
#include <netinet/in.h>




#ifndef TCP_H
#define TCP_H

#define TCP_ADDRESS_LEN 2 // Holds the port number

class tcp: public protocol {
private:
	unsigned char communicating[6]; // IP, port, packed
	unsigned int seqnum;
	short tcpwindow;
	unsigned short checksum(unsigned char *locbuf, int len);
	unsigned int acknum;
	unsigned int lastack;
	sem_t *connection;
public:
  unsigned char my_ipaddr[IP_ADDRESS_LEN];
  unsigned char my_tcpaddr[TCP_ADDRESS_LEN];
  int init (char *name, protocol *higher, protocol *lower, 
	    char *addr, int wsize, int client);
  int push (unsigned char *buf, int len, char *addr);
  int pop (unsigned char *buf, int len);
  int connect (unsigned char *dest_ipaddr, unsigned char *dest_port);
  int accept (unsigned char *dest_ipaddr, unsigned char *dest_port);
};

#endif
