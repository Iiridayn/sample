// This is just a template for you to use in your coding

#include "internet.h"
#include "arp.h"
#include "nat.h"
#include <netinet/in.h>

#define HLEN 5
#define TOS 0
#define TTL 1
#define IPT_TCP 6
#define FRAGMENT 0x2000
#define VERSION 4

extern int blabby;

int  internet::init (char *name, protocol *higher, protocol *lower, 
 char *addr, int port, int client)
{
  cout << "This is the internet init routine for " << name << endl;
  print_addr("Internet Address", (unsigned char *)addr, IP_ADDRESS_LEN);
  strcpy(prot_name , name);
  prot_higher = higher;
  prot_lower = lower;
	
	memcpy(ipaddr, addr, IP_ADDRESS_LEN);
	ident = 0;

  return(0);
}

void internet::packetHeader(unsigned char *locbuf, int len) {
	locbuf[0] = HLEN | (VERSION << 4);
	locbuf[1] = TOS;
	locbuf[2] = ((20+len) >> 8) & 0xFF;
	locbuf[3] = (20+len) & 0xFF; // gotta be an easier way
	locbuf[4] = (ident >> 8) & 0xFF;
	locbuf[5] = ident & 0xFF;
	// 6+7 are 0
	locbuf[8] = TTL;
	locbuf[9] = IPT_TCP;
}

void internet::checksum(unsigned char *locbuf, int len) {
	int checksum = 0;
	short *tempbuf = (short *) locbuf;
	for(int i = 0; i < len/2; i++) {
		checksum += tempbuf[i];
		if(checksum & 0x10000) {
			checksum &= 0xFFFF;
			checksum++;			
		}
	}
	//return ~(checksum & 0xFFFF);
	tempbuf[5] = ~(checksum & 0xFFFF);
}

// Push the data to the next lower layer
int internet::push (unsigned char *buf, int len, char *addr)
{
	if(blabby & 0x10) {
		cout << "This is the internet push routine" << endl;
		print_chars(prot_name, buf, len);
	}
	
	if(len > 30) { // need to fragment into 24 byte chunks
		int i;
		for(i = 0; i < (len/24); i++) {
			unsigned char locbuf[44];
			memset(locbuf, 0, 44);
			memcpy(locbuf+20, buf+(i*24), 24);
			packetHeader(locbuf, 24);
			locbuf[6] = (FRAGMENT | (i*3)) >> 8 & 0xFF;
			locbuf[7] = (i*3) & 0xFF;
			memcpy(locbuf+12, ipaddr, IP_ADDRESS_LEN);
			memcpy(locbuf+16, addr, IP_ADDRESS_LEN);
			checksum(locbuf, 20);
			if(blabby & 0x10) {
				cout << "Internet Push: fragment " << i << ":" << endl;
				print_chars("", locbuf, 44);
			}
			prot_lower->push(locbuf, 44, addr);
		}
		unsigned char locbuf[44];
		memset(locbuf, 0, 44);
		memcpy(locbuf+20, buf+(i*24), len%24);
		packetHeader(locbuf, len%24);
		locbuf[6] = (i*3) >> 8 & 0xFF;
		locbuf[7] = (i*3) & 0xFF;
		memcpy(locbuf+12, ipaddr, IP_ADDRESS_LEN);
		memcpy(locbuf+16, addr, IP_ADDRESS_LEN);
		checksum(locbuf, 20);
		if(blabby & 0x10) {
			cout << "Internet Push: fragment " << i << ":" << endl;
			print_chars("", locbuf, 20+(len%24));
		}
		prot_lower->push(locbuf, 20+(len%24), addr);
	}
	else {
		unsigned char locbuf[50];
		memset(locbuf, 0, 50);
		memcpy(locbuf+20, buf, len);
		packetHeader(locbuf, len);
		// 10 and 11 are checksum
		memcpy(locbuf+12, ipaddr, IP_ADDRESS_LEN);
		memcpy(locbuf+16, addr, IP_ADDRESS_LEN);
		// now checksum
		checksum(locbuf, 20); // checksum on header only
		if(blabby & 0x10) {
			print_chars("Internet Push:", locbuf, len+20);
		}
		prot_lower->push(locbuf, len+20, addr);
	}
	ident++;
	ident %= MAX_ACTIVE_FRAGMENT; // keep within table
	// TA code !likes > 100
}

// Pop the data from the next lower layer
int internet::pop (unsigned char *buf, int len)
{
	if(blabby & 0x10) {
		print_chars("Popping in the internet Layer.",buf,len);
	}
	
	if((buf[0] >> 4) & 0xF != VERSION) {
		cout << "Packet with bad version number." << endl;
		return 0;
	}
	if(!memcmp(buf+15, ipaddr, 4)) {
		cout << "Packet not to this IP address." << endl;
		return 0;
	}
	if(len < (((short)(buf[2]) << 8) | buf[3])) {
		cout << "Actual data less than claimed packet length." << endl;
		return 0;
	}
	
	unsigned char locbuf[50]; // max size
	memset(locbuf, 0, 50);
	memcpy(locbuf, buf, len);
	
	int HLen = ((buf[0]&0xF)*4);
	
	int csum = (((short)(locbuf[10]) << 8) | locbuf[11]);
	locbuf[10] = 0;
	locbuf[11] = 0;
	checksum(locbuf, HLen);
	if((((short)(locbuf[10]) << 8) | locbuf[11]) != csum) {
		printf("Invalid checksum.  Is %04x, should be %04x.", csum, (((short)(locbuf[10]) << 8) | locbuf[11]));
		return 0;
	}
	
	int length = (((short)(locbuf[2]) << 8) | locbuf[3]);
	
	if(!locbuf[6] && !locbuf[7]) { // not a fragment, send it up; neither flags nor offset
		prot_higher->pop(locbuf+((buf[0]&0xF)*4), length-HLen);
	}
	else { // is a fragment
		int ident = (((short)(locbuf[4]) << 8) | locbuf[5]);
		int fragindex = ident%MAX_ACTIVE_FRAGMENT;
		int offset = (((short)(locbuf[6] & 0x1F) << 8) | locbuf[7]);
		
		fragdata[fragindex].len += length-HLen;
		memcpy((fragdata[fragindex].data)+(offset*8), locbuf+HLen, length-HLen);
		if(!(locbuf[6] & 0x20)) // is an ending packet
			fragdata[fragindex].total = offset*8 + length-HLen;
		
		if(fragdata[fragindex].len == fragdata[fragindex].total) {
			prot_higher->pop(fragdata[fragindex].data, fragdata[fragindex].len);
		}
	}
	
	return(0);
}
