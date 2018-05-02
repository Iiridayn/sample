
// This is just a template for you to use in your coding

#include "tcp.h"

#define SYN 0x02
#define ACK 0x10
#define PSH 0x08
#define BASE_SEQ 32000
#define MAX_PACKET 30

int  tcp::init (char *name, protocol *higher, protocol *lower,
 char *addr, int wsize, int client)
{

  cout << "This is the tcp init routine for " << name << endl;
  memcpy(my_tcpaddr,addr,TCP_ADDRESS_LEN);
  memcpy(my_ipaddr,&addr[2],IP_ADDRESS_LEN);
  print_addr("tcp port Address", my_tcpaddr, TCP_ADDRESS_LEN);
  print_addr("tcp IP Address", my_ipaddr, IP_ADDRESS_LEN);
  strcpy(prot_name , name);
	prot_higher = higher;
  	prot_lower = lower;
	//tcpwindow = wsize;
	tcpwindow = MAX_PACKET;
	seqnum = BASE_SEQ;
	acknum = 0;
	lastack = 0;
	connection = (sem_t *) malloc(sizeof(sem_t));
	sem_init(connection, 0, 0);
  return 0;
}

/*
int tcp::checksum(unsigned char *locbuf, int len) {
	register int checksum = 0;
	short *tempbuf = (short *) locbuf;
	for(int i = 0; i < len/2; i++) {
		checksum += tempbuf[i];
		if(checksum & 0x10000) {
			checksum &= 0xFFFF;
			checksum++;
		}
	}
	return ~(checksum & 0xFFFF);
}
//*/

unsigned short tcp::checksum(unsigned char *locbuf, int len) {
	register unsigned long checksum = 0;
	register int tempcount = len/2;
	unsigned short *tempbuf = (unsigned short *) locbuf;
	while(tempcount--) {
		checksum += *tempbuf++;
		if(checksum & 0xFFFF0000) {
			checksum &= 0xFFFF;
			checksum++;
		}
	}
	if(len%2) { // if odd
		checksum += (*tempbuf & 0xFF00); //ignore the last byte
		if(checksum & 0xFFFF0000) {
			checksum &= 0xFFFF;
			checksum++;
		}
	}
	return ~(checksum & 0xFFFF);
}

// Push the data to the next lower layer
int tcp::push (unsigned char *buf, int len, char *addr)
{
  //cout << endl << "This is the tcp push routine" << endl;
  //print_chars(prot_name, buf, len);
	
	unsigned char pseudo[12];
	memset(pseudo, 0, 12);
	memcpy(pseudo, my_ipaddr, IP_ADDRESS_LEN);
	memcpy(pseudo+4, addr+2, IP_ADDRESS_LEN); // correct?
	pseudo[9] = 6;
	*(((short *) pseudo)+5) = htons(20+len); // correct?
	//short sum = checksum(pseudo, 12);
	
	unsigned char *message = (unsigned char *) calloc((20+len), sizeof(char));
	memcpy(message+20, buf, len);
	
	memcpy(message, my_tcpaddr, TCP_ADDRESS_LEN);
	memcpy(message+TCP_ADDRESS_LEN, addr, TCP_ADDRESS_LEN);
	*(((int *) message)+1) = htonl(seqnum); // correct?
	*(((int *) message)+2) = htonl(acknum);
	message[12] = 5<<4;
	message[13] = (acknum)?PSH|ACK:PSH;
	*(((short *) message)+7) = htons(tcpwindow);
	
	acknum = 0;
	seqnum += len;
	
	//int totsum = sum + checksum(message, 20+len);
	//sum = (totsum & 0x10000)?(totsum&0xffff)+1:totsum;
	unsigned char *sumbuf = (unsigned char *) calloc(12+20+len, sizeof(unsigned char));
	memcpy(sumbuf, pseudo, 12);
	memcpy(sumbuf+12, message, 20+len);
	short sum = checksum(sumbuf, 12+20+len);
	
	*(((short *) message)+8) = sum;

	bool recieved = false;
	char lowerAddr[4];
	memcpy(lowerAddr, addr+2, 4);
	while(!recieved) {
		prot_lower->push(message, 20+len, lowerAddr);
		sleep(5);
		if(lastack == seqnum)
			recieved = true;
		else
			cout << "Retransmit" << endl;
	}
	
	free(message);
	free(sumbuf);
	
  return 1;
}

// Pop the data from the next lower layer
int tcp::pop (unsigned char *buf, int len)
{
  //cout << endl << "This is the tcp pop routine" << endl;
  //print_chars(prot_name, buf, len);
	
	unsigned char *locbuf = (unsigned char *) calloc(len, sizeof(char));
	memcpy(locbuf, buf, len);
	
	// checksum the packet
	unsigned char pseudo[12];
	memset(pseudo, 0, 12);
	memcpy(pseudo, my_ipaddr, IP_ADDRESS_LEN);
	memcpy(pseudo+4, communicating+2, IP_ADDRESS_LEN);
	pseudo[9] = 6;
	*(((short *) pseudo)+5) = htons(len); // correct?
	
	short theirSum = *(((short *) locbuf)+8);
	*(((short *) locbuf)+8) = 0;
	
	unsigned char *sumbuf = (unsigned char *) calloc(len+12, sizeof(unsigned char));
	memcpy(sumbuf, pseudo, 12);
	memcpy(sumbuf+12, locbuf, len);
	short mySum = checksum(sumbuf, len+12);
	
	if(locbuf[13] & 1) // FIN
		goto nosum;
	if(mySum != theirSum) {
		printf("Bad checksum.  Expected %08X, got %08X.\n", mySum, theirSum);
		return 0;
	}
	nosum:
	
	if(locbuf[13] == (SYN|ACK)) { // got a syn/ack here
		cout << "SYN/ACK" << endl;
		
		if(ntohl(*(((int *) locbuf)+2)) == seqnum) {
			acknum = htonl(*(((int *) locbuf)+1))+1;
			cout << "Proper numbers too" << endl;
		}
		
		// Send an ACK
		unsigned char pseudo[12];
		memset(pseudo, 0, 12);
		memcpy(pseudo, my_ipaddr, IP_ADDRESS_LEN);
		memcpy(pseudo+4, communicating+2, IP_ADDRESS_LEN);
		pseudo[9] = 6;
		*(((short *) pseudo)+5) = htons(20);	
		
		unsigned char message[20];
		memset(message, 0, 20);
		
		memset(message, 0, 20);
		memcpy(message, my_tcpaddr, TCP_ADDRESS_LEN);
		memcpy(message+TCP_ADDRESS_LEN, communicating, TCP_ADDRESS_LEN);
		*(((int *) message)+1) = htonl(seqnum); // don't increment seqnum; no data
		*(((int *) message)+2) = htonl(acknum);
		message[12] = 5<<4;
		message[13] = ACK;
		*(((short *) message)+7) = htons(tcpwindow);
	
		unsigned char sumbuf[32];
		memcpy(sumbuf, pseudo, 12);
		memcpy(sumbuf+12, message, 20);
		short sum = checksum(sumbuf, 32);
		*(((short *) message)+8) = sum;
		
		cout << "Sending ACK!" << endl;
		prot_lower->push(message, 20, (char *)communicating+2);
		
		acknum = 0;
		//seqnum++;
		
		sem_post(connection);
		
		return 1;
	}
	
	if(locbuf[13]&ACK) { // an ack component at least
		lastack = ntohl(*(((int *) locbuf)+2));
		//cout << "Got ack: " << (lastack-BASE_SEQ) << endl;
		//cout << "Seqnum: " << (seqnum-BASE_SEQ) << endl;
	}
	if(locbuf[13]&PSH) { // some data
		//cout << "Normal packet, popping up." << endl;
		acknum = htonl(*(((int *) locbuf)+1))+(len-20);
		
		// Ack here and now
		unsigned char pseudo[12];
		memset(pseudo, 0, 12);
		memcpy(pseudo, my_ipaddr, IP_ADDRESS_LEN);
		memcpy(pseudo+4, communicating+2, IP_ADDRESS_LEN);
		pseudo[9] = 6;
		*(((short *) pseudo)+5) = htons(20);	
		
		unsigned char message[20];
		memset(message, 0, 20);
		
		memset(message, 0, 20);
		memcpy(message, my_tcpaddr, TCP_ADDRESS_LEN);
		memcpy(message+TCP_ADDRESS_LEN, communicating, TCP_ADDRESS_LEN);
		*(((int *) message)+1) = htonl(seqnum); // don't increment seqnum; no data
		*(((int *) message)+2) = htonl(acknum);
		message[12] = 5<<4;
		message[13] = ACK;
		*(((short *) message)+7) = htons(tcpwindow);
	
		unsigned char sumbuf[32];
		memcpy(sumbuf, pseudo, 12);
		memcpy(sumbuf+12, message, 20);
		short sum = checksum(sumbuf, 32);
		*(((short *) message)+8) = sum;
		
		//cout << "Sending ACK!" << endl;
		prot_lower->push(message, 20, (char *)communicating+2);
		
		acknum = 0;
		
		prot_higher->pop(locbuf+20, len-20);
	}	
	
	free(locbuf);
	free(sumbuf);
  return 0;
}

// Start the connect sequence
int tcp::connect (unsigned char *dest_ipaddr, unsigned char *dest_port)
{
  cout << "This is the tcp connect routine" << endl;
  print_addr(prot_name, dest_ipaddr, 4);
  print_addr(prot_name, dest_port, 2);
	
	memcpy(communicating, dest_port, 2);
	memcpy(communicating+2, dest_ipaddr, 4);
	
	unsigned char pseudo[12];
	memset(pseudo, 0, 12);
	memcpy(pseudo, my_ipaddr, IP_ADDRESS_LEN);
	memcpy(pseudo+4, dest_ipaddr, IP_ADDRESS_LEN);
	pseudo[9] = 6;
	*(((short *) pseudo)+5) = htons(20);
	//pseudo[11] = 20;
	//short psum = checksum(pseudo, 12);
	
	
	// Send SYN
	unsigned char message[20];
	memset(message, 0, 20);
	
	memcpy(message, my_tcpaddr, TCP_ADDRESS_LEN);
	memcpy(message+TCP_ADDRESS_LEN, dest_port, TCP_ADDRESS_LEN);
	*(((int *) message)+1) = htonl(seqnum++); // correct?
	message[12] = 5<<4;
	message[13] = SYN;
	*(((short *) message)+7) = htons(tcpwindow);
	
	//short hsum = checksum(message, 20);
	//int totsum = psum + hsum;
	//short sum = (totsum & 0x10000)?(totsum&0xffff)+1:totsum;
	unsigned char sumbuf[32];
	memcpy(sumbuf, pseudo, 12);
	memcpy(sumbuf+12, message, 20);
	short sum = checksum(sumbuf, 32);
	
	*(((short *) message)+8) = sum;
	prot_lower->push(message, 20, (char *)dest_ipaddr);
	
	sem_wait(connection);
	cout << "Connected" << endl;

  return 0;
}

int tcp::accept (unsigned char *dest_ipaddr, unsigned char *dest_port)
{
  cout << "This is the tcp accept routine" << endl;
  print_addr(prot_name, dest_ipaddr, 4);
  print_addr(prot_name, dest_port, 2);
	
	memcpy(communicating, dest_port, 2);
	memcpy(communicating+2, dest_ipaddr, 4);
	
  return 0;
}

