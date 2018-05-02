// rpc.h
#include <ctype.h>
#include "protocol.h"
#include "internet.h"
 
#ifndef RPC_H
#define RPC_H

#include <fcntl.h>
#include <semaphore.h>

#define TCP_ADDRESS_LEN 2 // Holds the port number

#define REQ 1
#define REP 2

#define OPEN 1
#define READ 2
#define WRITE 3

class rpc: public protocol {
private:
	sem_t *blocked;
	int remoteFD;
	unsigned char databuf[100];
	int bytenum;
public:
  int amclient;
  unsigned char dest_ipaddr[IP_ADDRESS_LEN];
  unsigned char dest_tcpaddr[TCP_ADDRESS_LEN];

  int init (char *name, protocol *higher, protocol *lower, 
   char *unused1, int unused2, int unused3);
  int push(unsigned char *buf, int len, char *addr);
  int pop(unsigned char *buf, int len);
  int open(char *filename);
  int read(int handle, unsigned char *buf, int len);
  int write(int handle, unsigned char *buf, int len);
};

#endif
