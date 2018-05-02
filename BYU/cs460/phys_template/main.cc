// This is the test configuration for protocols


#include "protocol.h"
#include "physical.h"
#include "ethernet.h"

physical *phys;
ethernet *eth;

int blabby;

#define MAXLEN 90

int main (int argc, char* argv[]) {
  
  int len, i, port;
  unsigned char buff[1000];

  // Declare protocols on the heap so that we can use threads
  // You should change the declaration of phys to be of class physical
  //   once you get your code written.  It should inherit from the class
  //   protocol so this code shouldnt know the difference.


  phys = new physical;
  eth = new ethernet;


  if (argc != 4) {
    cerr << "Usage:" << argv[0] << " hostname, port, <-s or -c> " << endl;
    exit (1);
  }

  port = atoi(argv[2]);

  // Initialize the protocols
  phys->init ("physical", eth, NULL, argv[1], port, strcmp (argv[3], "-s"));
  eth->init ("ethernet", NULL, phys, argv[1], port, strcmp (argv[3], "-s"));

  for (i = 0; i < MAXLEN; i++)
    buff[i]=i;

  if((argc >= 5) && (strcmp(argv[4],"-v") == 0)) 
    {
      blabby = strtol(argv[5],NULL,0);
      printf("verbose mode %X\n",blabby);
    } 
  else
    {
      blabby = 0;
    }
	
	blabby = 1;

  if(strcmp(argv[3], "-s")) { // send all of the stuf for the client
    for(len = 2; len < MAXLEN; len+=2) {
      phys->push (buff, len, NULL);
    }
  } 
  else 
    {
      int ch=0;
      unsigned char buffer[90] ={0};
      cout<<"Type a string to send"<<endl;
      for( i = 0; (i < 90) &&  ((ch = getchar()) != EOF) && (ch != '\n'); i++ )
		buffer[i] = (char)ch;
      phys->push (buffer, 90, NULL);
      //cin.getline(buff, 900);
    }
 
  cout << "You should have seen the message you typed appear in the Client side." << endl;
  cout << "Test Complete; hit ENTER to quit." << endl;
  cin.getline((char*)buff, 900);
	//phys->destruct();
  return 0;
}



