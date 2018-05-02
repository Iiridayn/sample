// This is the test configuration for protocols


#include "protocol.h"
#include "physical.h"
#include "ethernet.h"
#include "arp.h"
#include "internet.h"
#include "tcp.h"
#include "rpc.h"
#include "nat.h"
#include <ctype.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

physical *phys;
ethernet *eth;
arp *arpp;
internet *ip;
tcp *tcpp;
rpc *rpcp;

int blabby;
int gerrors;
int process_args(char **argv,char *my_ip,char *my_ethaddr,char *dest_ip,char *dest_ethaddr);


#define MAXLEN 40
#define MY_TCP {0x08, 0xff}
#define DEST_TCP {0x09, 0xee}

// Set up the NAT table for proxy translation
#define PROXY_IP1 {192,168,102,66}
// The following is the real machine that should be the destination of the ping
#define NEW_DST_IP1 {192,168,102,246} // 460six
#define PROXY_IP2 {192,168,102,67}
// The following is the real machine that you are running the ping on
#define NEW_DST_IP2 {192,168,102,245} // 460five
struct nat nat_table[]={{PROXY_IP1,NEW_DST_IP1,PROXY_IP2},
                       {PROXY_IP2,NEW_DST_IP2,PROXY_IP1}};


int main (int argc, char* argv[]) {
  
	system("cp testfile.bak testfile");
	
  int len, i, port, i_am_client;
  unsigned char buff[1000], inchars[10];
  // Set up the ethernet address
  char my_ethaddr[ETH_ADDRESS_LEN];
  char my_ipaddr[IP_ADDRESS_LEN];
  char dest_ipaddr[IP_ADDRESS_LEN];
  char dest_ethaddr[ETH_ADDRESS_LEN];
  char arpaddr[ETH_ADDRESS_LEN+IP_ADDRESS_LEN];
  char tcpaddr[TCP_ADDRESS_LEN+IP_ADDRESS_LEN];
  char tcpaddr1[TCP_ADDRESS_LEN] = MY_TCP;
  char tcpaddr2[TCP_ADDRESS_LEN] = DEST_TCP;
  char *my_tcpaddr;
  char *dest_tcpaddr;

  // Declare protocols on the heap so that we can use threads
  // You should change the declaration of phys to be of class physical
  //   once you get your code written.  It should inherit from the class
  //   protocol so this code shouldnt know the difference.


  phys = new physical;
  eth = new ethernet;
  arpp = new arp;
  ip = new internet;
  tcpp = new tcp;
  rpcp = new rpc;
  char *sendaddr = new char[11];

  gerrors = 0;
  if (argc < 8) {
    cerr << "Usage:" << argv[0] << " hostname, port, src_ip, src_eth, dst_ip, dst_eth <-s or -c> [-v 0xff]" << endl;
    exit (1);
  }
  if(strcmp(argv[7],"-s") == 0) { // This is the server
    process_args(argv,dest_ipaddr,dest_ethaddr,my_ipaddr,my_ethaddr);
    i_am_client = 0;
    my_tcpaddr = tcpaddr2;
    dest_tcpaddr = tcpaddr1;
  } else {
    process_args(argv,my_ipaddr,my_ethaddr,dest_ipaddr,dest_ethaddr);
    i_am_client = 1;
    my_tcpaddr = tcpaddr1;
    dest_tcpaddr = tcpaddr2;
  }

  if((argc >= 9) && (strcmp(argv[8],"-v") == 0))
    {
      blabby = strtol(argv[9],NULL,0);
      printf("verbose mode %X\n",blabby);
    }
  else
    {
      blabby = 0;
    }

  port = atoi(argv[2]);
  memset(sendaddr,0,sizeof(sendaddr));
  memcpy(sendaddr,&my_ipaddr,IP_ADDRESS_LEN);
  memcpy(sendaddr+4,&my_ethaddr,ETH_ADDRESS_LEN);
  print_addr("src ip address",(unsigned char *)my_ipaddr,IP_ADDRESS_LEN);
  print_addr("dst ip address",(unsigned char *)dest_ipaddr,IP_ADDRESS_LEN);
// Initialize the protocols

  eth->init ("ethernet", arpp, phys, my_ethaddr, 0, i_am_client);
  arpp->init ("ARP", ip, eth, sendaddr, 0, i_am_client);

  //ip->init ("internet", tcpp, arpp, my_ipaddr, 1, i_am_client);
  ip->init ("internet", rpcp, arpp, my_ipaddr, 1, i_am_client);

  /*
  memcpy(&tcpaddr[0],my_tcpaddr,TCP_ADDRESS_LEN);
  memcpy(&tcpaddr[TCP_ADDRESS_LEN],my_ipaddr,IP_ADDRESS_LEN);
  tcpp->init ("tcp", rpcp, ip, tcpaddr, 4, i_am_client);
  */

  phys->init ("physical", eth, NULL, argv[1], port, i_am_client);
  memcpy(&tcpaddr[0],dest_tcpaddr,TCP_ADDRESS_LEN);
  memcpy(&tcpaddr[TCP_ADDRESS_LEN],dest_ipaddr,IP_ADDRESS_LEN);

  //rpcp->init ("rpc", NULL, tcpp, tcpaddr, 0, i_am_client);
  rpcp->init ("rpc", NULL, ip, tcpaddr, 0, i_am_client);

  if(i_am_client)  { // This is the client
    //Remove this connect, and the following accept, portions of code if you plan to run RPC on top of IP
    int connect_ret_val = 0; //tcpp->connect( (unsigned char *) dest_ipaddr, (unsigned char *) dest_tcpaddr );

        //
        //  Wait for the connection to establish.
        //

        if ( connect_ret_val == 0 )
        {
        cout << "Connection established! " << endl;
        }
        else
        {
        cerr << "Failed to connect.  Return value: " << connect_ret_val << endl;
        exit( 0 );
        }
        //END Portion to remove

    buff[0] = 0xde;
    buff[1] = 0xad;
    buff[2] = 0xbe;
    buff[3] = 0xef;
    len = 4;
  } else  { // This is the server
    //Remove this accept portion if you plan to run this on ip
    int connect_ret_val = tcpp->accept( (unsigned char *) dest_ipaddr, (unsigned char *) dest_tcpaddr );

        //
        //  Wait for the connection to establish.
        //

        if ( connect_ret_val == 0 )
        {
        cout << "Connection established! " << endl;
        }
        else
        {
        cerr << "Failed to accept.  Return value: " << connect_ret_val << endl;
        exit( 0 );
        }
        //END of portion to remove

    printf("When you see a message indicating that you have passed off the send part of lab, you passed it off\n");
    getchar();
    exit(0);
  }
  // Initialize the protocols


  if(strcmp (argv[7], "-s")) { //this is the client
    int fd1, rval;
    int fd;
    unsigned char buff[100];

    fd = rpcp->open("testfile");
    cout<<"\ngot it "<<fd<<endl;
    rval = rpcp->read(fd, buff, 3);
    cout << "Read returned " << rval << endl;
    print_chars("Read data", buff, rval);

    // Now check the data
    if((rval == 3) &&
       ((buff[0] == 'b') &&
        (buff[1] == 'y') &&
        (buff[2] == 'u')) ) {
           cout << "Initial data was read correctly!!!!!!!!!!!!" << endl;
    } else {
      cout << "Error, incorrect data received, expected 'byu' " << endl;
      gerrors++;
    }
    
    cout<<"now going to short stuff.."<<endl;
    // This read should only return 2 since it should reach the end of the file
    rval = rpcp->read(fd, buff, 20);
    cout << "Read returned " << rval << endl;

    print_chars("Read data", buff, rval);

    // Now check the data
    if((rval == 2) &&
       ((buff[0] == '#') &&
        (buff[1] == '1')) ) {
           cout << "short data was read correctly!!!!!!!!!!!!" << endl;
    } else {
      cout << "Error, incorrect data received, expected '#1' " << endl;
      gerrors++;
    }
    
    // This write should leave the correct data in the file
    for(i = 0; i < 20; i++) {
      buff[i] = i;
    }
    rval = rpcp->write(fd, buff, 20);
    cout << "Write returned " << rval << endl;
    if(rval == 20) {
      cout << "Write appeared to work !!!!!!!!!!!!" << endl;
    } else {
      cout << "Error, incorrect write length, expected 20 " << endl;
      gerrors++;
    }

    // This open should return a new file handle pointing to the beginning of the file
    int fd3 = rpcp->open("testfile");

    // Read all of the initial data, the stuff we wrote should still be left
    rval = rpcp->read(fd3, buff, 5);
    cout << "Read returned " << rval << endl;
    print_chars("Read data", buff, rval);

    // Now check the data
    if((rval == 5) &&
       ((buff[0] == 'b') &&
        (buff[1] == 'y') &&
        (buff[2] == 'u') &&
        (buff[3] == '#') &&
        (buff[4] == '1')) ) {
           cout << "Initial data was read correctly!!!!!!!!!!!!" << endl;
          
    } else {
      cout << "Error, incorrect data received, expected 'byu#1' " << endl;
      gerrors++;
    }

    // Now read the data that we wrote, the read should only return the number we wrote
    rval = rpcp->read(fd3, buff, 30);
    cout << "Read returned " << rval << endl;
    print_chars("Read data", buff, rval);
    // Now check the data
    if(rval == 20) {
       for(i = 0; i < 20; i++) {
	 if(buff[i] != i) {
	   printf("Bad Data Read, expected 0x%X, got 0x%X\n",i,buff[i]);
	   gerrors++;
	 }
       }
    } else {
      cout << "Error, incorrect data received, expected 20 bytes " << endl;
      gerrors++;
    }
    
    
    if(gerrors == 0)
      {
	cout << "You passed off the RPC Lab!!!!!" << endl;
	getchar();
	exit(0);
      }
    else
      {
	cout << "You didnt pass off the RPC Lab, you had " << gerrors << " errors :^( " << endl;
	getchar();
	exit(0);
      }
  } else {
    cout << "Nothing to do on the server side, hit CR when Complete" << endl;
    cin.getline((char*)inchars, 10);
  }



  cin.getline((char*)inchars, 10);
  return 0;
}

int process_args(char **argv,char *my_ip,char *my_ethaddr,char *dest_ip,char *dest_ethaddr) {
  unsigned int tmp_addr[IP_ADDRESS_LEN+ETH_ADDRESS_LEN];
  int i, rval;

  // Pull out the src and dst addresses
  // First the src ip address (scanf needs an integer to read into
  rval = sscanf(argv[3],"%d.%d.%d.%d",&tmp_addr[0],&tmp_addr[1],
                &tmp_addr[2],&tmp_addr[3]);
  if(rval != 4) {
    printf("ip address1 malformed rval %d, %d.%d.%d.%d\n",
            rval,tmp_addr[0],tmp_addr[1], tmp_addr[2],tmp_addr[3]);
    exit(0);
  } else {
    // Assign to the real variable
    for(i = 0; i < IP_ADDRESS_LEN; i++)
      my_ip[i]=tmp_addr[i];
   }

  // Second the src eth address
  rval = sscanf(argv[4],"%x:%x:%x:%x:%x:%x",&tmp_addr[0],&tmp_addr[1],
          &tmp_addr[2],&tmp_addr[3],&tmp_addr[4],&tmp_addr[5]);
  if(rval != 6) {
    printf("eth address1 malformed rval %d, %x:%x:%x:%x:%x:%x\n",
            rval,tmp_addr[0],tmp_addr[1], tmp_addr[2],tmp_addr[3],
            tmp_addr[4],tmp_addr[5]);
    exit(0);
  } else {
    for(i = 0; i < ETH_ADDRESS_LEN; i++)
      my_ethaddr[i]=tmp_addr[i];
  }
  // Third the dst ip address
  rval = sscanf(argv[5],"%d.%d.%d.%d",&tmp_addr[0],&tmp_addr[1],
                &tmp_addr[2],&tmp_addr[3]);
  if(rval != 4) {
    printf("ip address2 malformed rval %d, %d.%d.%d.%d\n",
            rval,tmp_addr[0],tmp_addr[1], tmp_addr[2],tmp_addr[3]);
    exit(0);
  } else {
    for(i = 0; i < IP_ADDRESS_LEN; i++)
      dest_ip[i]=tmp_addr[i];
  }

  // Forth the dst eth address
  rval = sscanf(argv[6],"%x:%x:%x:%x:%x:%x",&tmp_addr[0],&tmp_addr[1],
          &tmp_addr[2],&tmp_addr[3],&tmp_addr[4],&tmp_addr[5]);
  if(rval != 6) {
    printf("eth address2 malformed rval %d, %x:%x:%x:%x:%x:%x\n",
            rval,tmp_addr[0],tmp_addr[1], tmp_addr[2],tmp_addr[3],
            tmp_addr[4],tmp_addr[5]);
    exit(0);
  } else {
    for(i = 0; i < ETH_ADDRESS_LEN; i++)
      dest_ethaddr[i]=tmp_addr[i];
  }
}





