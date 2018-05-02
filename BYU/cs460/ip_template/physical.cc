// This is just a template for you to use in your coding

#include "physical.h"
extern int blabby;

// Start the thread
void *startthread (void *arg) {
  physical *myps = (physical*)arg;
  myps->readloop();
  return(0);
}

int  physical::init (char *name, protocol *higher, protocol *lower, 
 char *address, int port, int client)
{
  int hServerSocket;  /* handle to socket */
  struct hostent* pHostInfo;   /* holds info about a machine */
  struct sockaddr_in Address; /* Internet socket address stuct */
  int nAddressSize=sizeof(struct sockaddr_in);
  long nHostAddress;
  int err;

  cout << "This is the init routine for " << name << endl;
  strcpy(prot_name , name);
  prot_higher = higher;
  prot_lower = lower;
  
// We always behave like a client
   /* make a socket */
    hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(hSocket == SOCKET_ERROR)
    {
        printf("\nCould not make a socket\n");
        exit(1);
    }

    /* get IP address from name */
    pHostInfo=gethostbyname(address);

    /* copy address into long */
    memcpy(&nHostAddress,pHostInfo->h_addr,pHostInfo->h_length);

    /* fill address struct */
    Address.sin_addr.s_addr=nHostAddress;
    Address.sin_port=htons(port);
    Address.sin_family=AF_INET;

    printf("\nConnecting to %s on port %d\n",address,port);

    /* connect to host */
    if(connect(hSocket,(struct sockaddr*)&Address,sizeof(Address)) 
       == SOCKET_ERROR)
    {
        printf("\nCould not connect to host\n");
        exit(1);
    } else {
        printf("Server\n\
              sin_family        = %d\n\
              sin_addr.s_addr   = %x\n\
              sin_port          = %d\n"
              , Address.sin_family
              , Address.sin_addr.s_addr
              , ntohs(Address.sin_port)
            );
    }


  cout << endl << endl;
  cout << "creating thread from init" << endl;

   // BLD  if(err = pthread_create(NULL, NULL, startthread, this)) {  
  pthread_t threadId;                                             // BLD
  if(err = pthread_create(&threadId, NULL, startthread, this)) {  // BLD
    printf("thread create failed %d\n",err);
    exit(1);
  }
  return 1;
}


// Push the data to the next lower layer
int physical::push (unsigned char *buf, int len, char *addr)
{
  int rval, total; 
  unsigned char sentinel=SENTINEL;
  int length;

  if(blabby & 0x1) {
    cout << "This is the physical push routine" << endl;
    print_chars(prot_name, buf, len);
  }
  memset(outbuffer, PACKETSIZE, 0);
  memcpy(outbuffer, buf, len);

  // You should put code to send the data over the network here
    total = 0;
    rval = write(hSocket, &sentinel, sizeof(sentinel));
    length = len;
    rval = write(hSocket, &length, sizeof(length));
    while(total < len) {
      rval = write(hSocket, outbuffer+total, len-total);
      //printf("write returned\n",rval);
      if(rval > 0) {
  total += rval;
      } else {
        cout << "read from socket failed" << endl;
  exit(-1);
      }
    }

    return(0);
}


int physical::readloop () {
  int rval, total, len; 
  unsigned char sentinel;
  int length;
  unsigned char *inbuff = (unsigned char *)malloc(2000);

  for(;;) {
    total=0;
    sentinel=~SENTINEL;
    while(sentinel != SENTINEL) {
      rval = read(hSocket, &sentinel, sizeof(sentinel));
      //printf("physical sentinel %x\n",sentinel);
      if(rval != sizeof(sentinel)) {
        printf("readloop: read failed got %d\n",rval);
        exit(0);
      }
    }
    rval = read(hSocket, &length, sizeof(length));
    //printf("readloop: length received rval %d, length %d\n",rval,length);
    if((rval != sizeof(length)) || (length > MAXBUFLEN)) {
      printf("readloop: bad length received rval %d, length %d\n",rval,length);
      continue;
    }
    while(total < length) {
      rval = read(hSocket, inbuff+total, length-total);
      //printf("physical read %d characters\n",rval);
      if(rval > 0) {
        total += rval;
      } else {
        cout << "read from socket failed" << endl;
      }
    }
    if(blabby & 0x1) {
      cout << "This is the physical readloop routine" << endl;
      print_chars(prot_name, inbuff, total);
    }
    if(prot_higher){
 	pthread_t threadId;
  	pthread_create(&threadId, NULL, startthread, this);
      	prot_higher->pop (inbuff, total);
	free(inbuff);
	return 0;
   }

  }
  return(0);
}

int physical::close()
{



  /*  if (libnet_close_link_interface(network) == -1)
    {
        libnet_error(LN_ERR_WARNING, "libnet_close_link_interface couldn't close the interface");
	}*/

    return 1;
}

