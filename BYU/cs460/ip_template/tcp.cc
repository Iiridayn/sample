
// This is just a template for you to use in your coding

#include "tcp.h"

// This is the maximum packet size that will be sent.  It is also declared in main.cc
#define MAXLEN 40


int  tcp::init (char *name, protocol *higher, protocol *lower, 
 char *addr, int port, int client)
{

  packet_count = 0;
  cout << "This is the tcp init routine for " << name << endl;
  memcpy(my_tcpaddr,addr,TCP_ADDRESS_LEN);
  memcpy(my_ipaddr,addr,IP_ADDRESS_LEN);
  print_addr("tcp port Address", my_tcpaddr, TCP_ADDRESS_LEN);
  print_addr("tcp IP Address", my_ipaddr, IP_ADDRESS_LEN);
  strcpy(prot_name , name);
  amclient = client;
  printf("tcp init client %d\n",amclient);
  prot_higher = higher;
  prot_lower = lower;
  return(0);
}

// Push the data to the next lower layer
int tcp::push (unsigned char *buf, int len, char *addr)
{
  cout << "This is the tcp push routine" << endl;
  print_chars(prot_name, buf, len);


  if(prot_lower)
    prot_lower->push (buf, len, addr);
  return(0);
}

extern int crcerrors;

// Pop the data from the next lower layer
int tcp::pop (unsigned char *buf, int len)
{
  int i;

  printf("This is the tcp pop routine len %d\n",len);
  print_chars(prot_name, buf, len);

    if(packet_count == 0) {
      if(len != 4) {
	  printf("Error, incorrect length, got %d expected 4\n",len);
	  exit(0);
      }
      if(amclient) {
	   packet_count++;
	if( (buf[0] == 0xfe) &&
	  (buf[1] == 0xed) &&
	  (buf[2] == 0xbe) &&
	  (buf[3] == 0xef) ) {
	   cout << "Received packet  " << packet_count << "!!!!!!!!!!!!" << endl;
	   return(0);
	} else {
	  cout << "Error, incorrect data received, expected 0xfeedbeef" << endl;
	  exit(0);
	}
      } else {
	if( (buf[0] == 0xde) &&
	  (buf[1] == 0xad) &&
	  (buf[2] == 0xbe) &&
	  (buf[3] == 0xef) ) {
	   packet_count++;
	   cout << "Received packet  " << packet_count << "!!!!!!!!!!!!" << endl;
	   return(0);
	} else {
	  cout << "Error, incorrect data received, expected 0xdeadbeef" << endl;
	  exit(0);
	}
      }
    }
    if(packet_count == 1) {
      if(len != 31) {
	  printf("Error, incorrect length, got %d expected 31\n",len);
	  exit(0);
      }
      for(i = 0; i < 31; i++) {
	if(buf[i] != i) {
	  printf("Error, incorrect data received got %x, expected %x",buf[i],i);
	  exit(0);
	}
      }
      packet_count++;
      cout << "Received packet  " << packet_count << "!!!!!!!!!!!!" << endl;
      return(0);
    }
    if(packet_count == 2) {
      if(len != 256) {
	  printf("Error, incorrect length, got %d expected 256\n",len);
	  exit(0);
      }
      for(i = 0; i < 256; i++) {
	if(buf[i] != i) {
	  printf("Error, incorrect data received got %x, expected %x",buf[i],i);
	  exit(0);
	}
      }
      packet_count++;
      cout << "You passed off part of the IP lab, hit CR !!!!!!!!!!!!!!" << endl;
      return(0);
    }
}

