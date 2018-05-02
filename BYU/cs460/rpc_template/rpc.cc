
// This is just a template for you to use in your coding

#include "rpc.h"

extern int blabby;

int  rpc::init (char *name, protocol *higher, protocol *lower, 
 char *addr, int unused1, int unused2)
{
  cout << "This is the rpc init routine for " << name << endl;
  strcpy(prot_name , name);
  memcpy(dest_tcpaddr,addr,TCP_ADDRESS_LEN);
  memcpy(dest_ipaddr,addr+2,4);
  print_addr("rpc port Address", dest_tcpaddr, TCP_ADDRESS_LEN);
  print_addr("rpc IP Address", dest_ipaddr, IP_ADDRESS_LEN);
  prot_lower = lower;
  prot_higher = higher;
	
	blocked = (sem_t *) malloc(sizeof(sem_t));
	sem_init(blocked, 0, 0);
	remoteFD = -1;
	bytenum = -1;
	
	return 0;
}

// Push the data to the next lower layer
int rpc::push (unsigned char *buf, int len, char *addr)
{
  cout << "This is the rpc push routine" << endl;
  print_chars(prot_name, buf, len);

  cout << "This routine should never be called!!" << endl;
	return 0;
}


// Pop the data from the next lower layer and execute the given procedure
int rpc::pop (unsigned char *buf, int len)
{
  cout << "\nThis is the rpc pop routine " << endl;
  print_chars(prot_name, buf, len);
  //return 1;
	if(len < 4) return 0;
	
	switch(buf[0]) {
		case REP:
			switch(buf[1]) {
				case OPEN:
					remoteFD = buf[2];
					break;
				case READ:
					if(buf[3]+4 != len) {
						cerr << "Data read length other than claimed!" << endl;
						return 0;
					}
					memcpy(databuf, buf+4, buf[3]);
					//drop through for bytenum
				case WRITE:
					bytenum = buf[3];
					break;
				default: return 0;
			}
			sem_post(blocked);
			break;
		case REQ:
			// I am the server
			// FD = remoteFD
			if(buf[1] == OPEN) {
				if(remoteFD != -1) close(remoteFD);
				
				char *filename = (char *) calloc(buf[3]+1, sizeof(char));
				memcpy(filename, buf+4, buf[3]);
				filename[buf[3]] = 0;
				remoteFD = ::open(filename, O_RDWR | O_SYNC); //| O_APPEND);
				free(filename);
				
				char *packet = (char *) calloc(4, sizeof(char));
				packet[0] = REP;
				packet[1] = OPEN;
				packet[2] = remoteFD;
				packet[3] = 0;
				
				prot_lower->push((unsigned char *)packet, 4, (char *)dest_ipaddr);
				free(packet);
			}
			else if(buf[1] == READ) {
				if(buf[2] != remoteFD) {
					cerr << "Asked to open from an invalid file descriptor." << endl;
					return 0;
				}
				
				int readbytes = ::read(buf[2], databuf, buf[3]);
				
				char *packet = (char *) calloc(4+readbytes, sizeof(char));
				packet[0] = REP;
				packet[1] = READ;
				packet[2] = remoteFD;
				packet[3] = readbytes;
				memcpy(packet+4, databuf, readbytes);
				
				prot_lower->push((unsigned char *)packet, 4+readbytes, (char *)dest_ipaddr);
				free(packet);
			}
			else if(buf[1] == WRITE) {
				if(buf[2] != remoteFD) {
					cerr << "Asked to open from an invalid file descriptor." << endl;
					return 0;
				}
				
				int writebytes = ::write(buf[2], buf+4, buf[3]);
				
				char *packet = (char *) calloc(4, sizeof(char));
				packet[0] = REP;
				packet[1] = WRITE;
				packet[2] = remoteFD;
				packet[3] = writebytes;
				
				prot_lower->push((unsigned char *)packet, 4, (char *)dest_ipaddr);
				free(packet);
			}
			else return 0;
			break;
		default: return 0;
	}
	return 1;
}

// Open the remote file
int rpc::open (char *filename)
{
    cout << "\nThis is the rpc open routine" << endl;
    print_chars(prot_name, (unsigned char *)filename, strlen(filename));
	
	char *packet = (char *) calloc(strlen(filename)+4, sizeof(char));
	packet[0] = REQ;
	packet[1] = OPEN;
	packet[2] = remoteFD;
	packet[3] = strlen(filename);
	memcpy(packet+4, filename, strlen(filename));
	
	prot_lower->push((unsigned char *)packet, strlen(filename)+4, (char *)dest_ipaddr);
	free(packet);
	
	sem_wait(blocked);
	
	return remoteFD;
}

// Read the data from the remote file
int rpc::read (int handle, unsigned char *buf, int len)
{
    cout << "\nThis is the rpc read routine " << endl;
	
	char *packet = (char *) calloc(4, sizeof(char));
	packet[0] = REQ;
	packet[1] = READ;
	packet[2] = handle;
	packet[3] = len;
	
	prot_lower->push((unsigned char *)packet, 4, (char *)dest_ipaddr);
	free(packet);
	
	sem_wait(blocked);
	
	memcpy(buf, databuf, bytenum);
	return bytenum;
}

// Write the data to the remote file
int rpc::write (int handle, unsigned char *buf, int len)
{
    cout << "This is the rpc write routine" << endl;
    print_chars(prot_name, buf, len);
	
	char *packet = (char *) calloc(4+len, sizeof(char));
	packet[0] = REQ;
	packet[1] = WRITE;
	packet[2] = handle;
	packet[3] = len;
	memcpy(packet+4, buf, len);
	
	prot_lower->push((unsigned char *)packet, 4+len, (char *)dest_ipaddr);
	free(packet);
	
	sem_wait(blocked);
	
	return bytenum;
}
