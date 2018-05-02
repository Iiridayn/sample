
// This is just a template for you to use in your coding

#include "ethernet.h"
#define PACKETSIZE 90

int  ethernet::init (char *name, protocol *higher, protocol *lower, 
 char *addr, int port, int client)
{
  cout << "This is the ethernet init routine for " << name << endl;
  strcpy(prot_name , name);
  prot_higher = higher;
  prot_lower = lower;
  curlen = 2;
  amclient = client;
  // You should put code to set up the socket here
  return(0);
}

// Push the data to the next lower layer
int ethernet::push (unsigned char *buf, int len, char *addr)
{
  cout << "This is the ethernet push routine" << endl;
  print_chars(prot_name, buf, len);

  // The default behavior will be to echo the same data back through 
  // pop routine if there is no lower layer protocol

  // You should put code to send the data over the network here

  if(!prot_lower)
    pop (buf, len);
  else
    prot_lower->push (buf, len, addr);
  return(0);
}

// Pop the data from the next lower layer
int ethernet::pop (unsigned char *buf, int len)
{
  int i;

  cout << "This is the ethernet pop routine" << endl;
  print_chars(prot_name, buf, len);
  if(!amclient)
    {
      for(i = 0; i < curlen; i++)
	{
	  if(buf[i] != i)
	    {
	      printf("Error, expected 0x%x at buf[%d], but got 0x%x\n",i,i,buf[i]);
	      exit(0);
	    }
	}
      for(i = curlen; i < PACKETSIZE; i++) 
	{
	  if(buf[i] != 0)
	    {
	      printf("Error, expected 0x%x at buf[%d], but got 0x%x\n",0,i,buf[i]);
	      exit(0);
	    }
	}
      curlen+=2;
      if(curlen == PACKETSIZE) 
	{
	  strcpy((char *)buf, "You passed part of lab 1!!\n Look at Server side for final test");
	  prot_lower->push (buf, strlen((char *)buf), 0);
	  cout<<"Type a string to send"<<endl;
	  
	}
    }
  if(prot_higher)
    prot_higher->pop (buf, len);

  return(0);
}

