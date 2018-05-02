// This is the test configuration for protocols


#include "protocol.h"
#include "physical.h"
#include "ethernet.h"
#include "internet.h"
#include <stdio.h>

physical *phys;
ethernet *eth;
internet *ip;

int blabby;

#define MAXLEN 40

#define MY_ETHADDR {0x7e, 0x34, 0x22, 0x11, 0x75, 0x45}
#define DEST_ETHADDR {0x24, 0x69, 0x12, 0x67, 0x35, 0x49}

int main (int argc, char* argv[]) {
  
  int len, i, port;
  unsigned char buff[1000], inchars[10];
  // Set up the ethernet address
  char my_ethaddr[6]=MY_ETHADDR;
  char dest_ethaddr[6]=DEST_ETHADDR;

  // Declare protocols on the heap so that we can use threads
  // You should change the declaration of phys to be of class physical
  //   once you get your code written.  It should inherit from the class
  //   protocol so this code shouldnt know the difference.


  phys = new physical;
  eth = new ethernet;
  ip = new internet;


  if (argc < 4) {
    cerr << "Usage:" << argv[0] << "hostname, port, <-s or -c> " << endl;
    exit (1);
  }

  port = atoi(argv[2]);

  // Initialize the protocols
  phys->init ("physical", eth, NULL, argv[1], port, strcmp (argv[3], "-s"));
  if(strcmp (argv[3], "-s")) { //this is the client
    eth->init ("ethernet", ip, phys, my_ethaddr, 0, 0);
  } else {
    eth->init ("ethernet", ip, phys, dest_ethaddr, 0,0);
  }
    
  ip->init ("internet", NULL, eth, argv[1], 0,  strcmp (argv[3], "-s"));

  if((argc >= 5) && (strcmp(argv[4],"-v") == 0)) 
    {
      blabby = strtol(argv[5],NULL,0);
      printf("verbose mode %X\n",blabby);
    } 
  else
    {
      blabby = 0;
    }

  if(strcmp (argv[3], "-s")) 
    { //this is the client
      for (i = 0; i < MAXLEN; i++)
	buff[i]=i ^ 0xff;
      
      for(len = 2; len < MAXLEN; len+=2) 
	{
	  print_chars("main sending ", buff, len);
	  eth->push (buff, len, dest_ethaddr);
	}
      cout<<"Test Complete."<<endl;
    } 
  else
    {
      cout << "Send test: hit the enter key when you see the <Send Test Complete> message" <<endl;
      cin.getline((char*)inchars, 10);
      buff[0] = 0xde;
      buff[1] = 0xad;
      buff[2] = 0xbe;
      buff[3] = 0xef;
//      eth->test (buff, 4, my_ethaddr, NULL);
      cout << "You should have received a good frame with contents <0xdeadbeef>" << endl;
      cout << "followed by a frame with CRC error"  << endl;
      cout << "followed by a frame with bad destination error"  << endl;
      cout << "followed by a frame with bad bit stuffing"  << endl;
      cout << "followed by a frame with one bad frame and one good frame"  << endl;
      cout << "followed by a packet with two good frames in the same 90 byte packet"  << endl;
      cout << "The second one was bit shifted so the beginning sentinal was not on a byte boundary"  << endl;
      int ch=0;
      unsigned char buffer[90] ={0};
      cout<<"Type a string to send"<<endl;
      for( i = 0; (i < 90) &&  ((ch = getchar()) != EOF) && (ch != '\n'); i++ )
	buffer[i] = (char)ch;
      eth->push (buffer, i,  my_ethaddr);
      cout << "You should have seen the message you typed appear in the Client side." << endl;
      cout << "If so \nTest Complete hit ENTER in Client to quit." << endl;
    }
  
  cin.getline((char*)buff, 10);
  return 0;
}



