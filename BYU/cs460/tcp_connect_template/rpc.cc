
// This is just a template for you to use in your coding

#include "rpc.h"

//
// This is the maximum packet size that will be sent.  It is also declared in main.cc
//
#define MAXLEN 40
#define CHECKSIZE 500

extern int gerrors;

//
//	Initialize the RPC object.  This si not used in the TCP connect lab.
//
int rpc::init (char *name, protocol *higher, protocol *lower, char *addr, int port, int client)
{
  errors = 0;
  packet_count = 0;
  remaining = CHECKSIZE;
  cout << "This is the rpc init routine for " << name << endl;
  memcpy(my_tcpaddr,addr,TCP_ADDRESS_LEN);
  memcpy(my_ipaddr,addr+2,IP_ADDRESS_LEN);
  print_addr( "rpc port Address", my_tcpaddr, TCP_ADDRESS_LEN);
  print_addr( "rpc IP Address", my_ipaddr, IP_ADDRESS_LEN);
  strcpy(prot_name , name);
  if(client == 1)
    amclient = 1;
  else
    amclient = 0;
  prot_higher = higher;
  prot_lower = lower;
  return 0;
}

//
// Push the data to the next lower layer.  This simply takes whatever it 
//	was handed and passes it down the stack.
//
int rpc::push (unsigned char *buf, int len, char *addr)
{
  cout << "This is the rpc push routine" << endl;
  print_chars(prot_name, buf, len);


  if( prot_lower )
    prot_lower->push (buf, len, addr);
  return 0;
}

extern int crcerrors;

//
// Pop the data from the next lower layer
//
int rpc::pop (unsigned char *buf, int len)
{
	//
	//	For the TCP connect lab, we simply output to the screen whatever the buffer contents
	//	were that was handed to us.  When the lab works correctly, you should see your webserver's
	//	HTTP response to the GET command that was submitted in 'main.cc'.
	//
  buf[ len ] = '\0';
  cout << buf << flush;

  return 0;
}









