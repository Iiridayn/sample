// This is just a template for you to use in your coding

#include "physical.h"

extern int blabby;

// Start the thread

int  physical::init (char *name, protocol *higher, protocol *lower, 
 char *address, int port, int client) {
  int hServerSocket;  /* handle to socket */
  struct hostent* pHostInfo;   /* holds info about a machine */
  struct sockaddr_in Address; /* Internet socket address stuct */
  int nAddressSize=sizeof(struct sockaddr_in);
  long nHostAddress;
  int err;

  if(blabby & 0x1) cout << "This is the physical init routine for " << name << endl;
  strcpy(prot_name , name);
  prot_higher = higher;
  prot_lower = lower;
  // You should put code to set up the socket here  
	/* make a socket */
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
  	if(sock == -1) cerr << "Physical: error creating socket." << endl, exit(0);
  
	if(client) {
		pHostInfo=gethostbyname(address);
		memcpy(&nHostAddress,pHostInfo->h_addr,pHostInfo->h_length);
		Address.sin_addr.s_addr=nHostAddress;
		//inet_aton(address, &Address.sin_addr);
    	Address.sin_port=htons(port);
    	Address.sin_family=AF_INET;
		
		if(blabby & 0x1) cout << "Physical client: connecting to " << address << " on port " << port << "." << endl;
		
		if(connect(sock, (struct sockaddr*)&Address, nAddressSize))
			cerr << "Physical: error connecting to server." << endl, exit(0);
		
		hSocket = sock;
		
		//physical::readloop(this);
	}
	else {
		int one = 1;
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
		
		Address.sin_addr.s_addr=INADDR_ANY;
    	Address.sin_port=htons(port);
    	Address.sin_family=AF_INET;
		
		if(bind(sock, (struct sockaddr*)&Address, nAddressSize))
			cerr << "Physical: server binding error." << endl, exit(0);
		
		if(listen(sock, 1))
			cerr << "Physical: listen error." << endl, exit(0);
		
		//single accept, thread readloop()
		hSocket = accept(sock, (struct sockaddr*)&Address, (socklen_t *)&nAddressSize);
		
		//pthread_create(&threadId, 0, physical::readloop, this);
	}
	
	pthread_create(&threadId, 0, physical::readloop, this);  
 
	return(0);
}

// Push the data to the next lower layer
int physical::push (unsigned char *buf, int len, char *addr) {
  int rval, total; 

  if(blabby & 0x1) {
    cout << "Push: ";
    print_chars(prot_name, buf, len);
  }
  
	// My code
  	memset(outbuffer, 0, BUFFERSIZE);
  	if(len > BUFFERSIZE) cerr << "Physical: critical error: push buf too long." << endl, exit(0);
  	memcpy(outbuffer, buf, len);
  	write(hSocket, outbuffer, PACKETSIZE);
  	// no more should be needed

	return(0);
}

void *physical::readloop(void* arg) {
	if(blabby & 0x1) cout << "Physical: in readloop" << endl;
	
	while(1) {
		int bytes = 0;	
		memset(((physical*)arg)->inbuffer, 0, BUFFERSIZE);
		
		while(bytes < PACKETSIZE) {
			//if(blabby) cout << "readloop has " << bytes << " bytes." << endl;
			bytes += read(((physical*)arg)->hSocket, ((physical*)arg)->inbuffer, PACKETSIZE-bytes);
		}
		if(blabby & 0x1) {
			cout << "Pop: ";
			print_chars(((physical*)arg)->prot_name, ((physical*)arg)->inbuffer, PACKETSIZE);
		}
		usleep(100000);
		((physical*)arg)->prot_higher->pop(((physical*)arg)->inbuffer, PACKETSIZE);
	}
}




