
// This is just a template for you to use in your coding

#include "arp.h"
extern int blabby;

int  arp::init (char *name, protocol *higher, protocol *lower, 
 char *addr, int port, int client)
{
  cout << "This is the init routine for " << name << endl;
  strcpy(prot_name , name);
  memcpy(ipaddr,addr,4);
  memcpy(ethaddr,addr+4,6);
  prot_higher = higher; 
  prot_lower = lower;
	arptableindex = 0;
  return 1;
}

int arp::push (unsigned char *buf, int len, char *addr)
{
  if(blabby & 0x8) {
    cout << "This is the ARP push routine" << endl;
    print_chars(prot_name, buf, len & 0xffff);
  }

	//My code start
  	unsigned char ethbroadcast[6] =  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	unsigned char mybuf[100];
	memset(mybuf, 0, 100);
	memcpy(mybuf, buf, len&0xffff);
	
	char arptablepos = -1;
	for(int i = 0; i < ARP_TABLE_SIZE; i++) {
		if(!memcmp(arptable[i].ip, addr, 4)) {
			arptablepos = i;
			break;
		}
	}
	
	if(arptablepos == -1) { //not already in table
		//send out an arp packet
		unsigned char arp[28];
		memset(arp, 0, 28);
		arp[1] = 1;
		arp[4] = 6;
		arp[5] = 4;
		arp[2] = 0x08;
		arp[7] = 1;
		memcpy(arp+8, ethaddr, 6);
		memcpy(arp+14, ipaddr, 4);
		memcpy(arp+18, ethbroadcast, 6);
		memcpy(arp+24, addr, 4);
		prot_lower->push(arp, 28 | 0x806 << 16, (char *)ethbroadcast);
		sleep(1);
		for(int i = 0; i < ARP_TABLE_SIZE; i++) {
			if(!memcmp(arptable[i].ip, addr, 4)) {
				arptablepos = i;
				break;
			}
		}
		if(arptablepos == -1) return 0;
	}
	prot_lower->push(mybuf, len&0xffff | 0x800 << 16, (char *)arptable[arptablepos].eth);	
	//My code end

  return 1;
}
// Pop the data from the next lower layer
int arp::pop (unsigned char *buf, int len)
{
  int dlen = len & 0xffff; // The data length
  int tlen = len>>16;      // The type

  if(blabby & 0x8) {
    cout << "This is the arp pop routine" << endl;
    print_chars(prot_name, buf, dlen);
    printf("len %x tlen %x and dlen %x\n",len,tlen, dlen);
  }
  	
	//cout << "Pop" << endl;
	if(tlen == 0x800) {
		//cout << "For IP" << endl;
		prot_higher->pop(buf, dlen);
	}
	else if(tlen == 0x806) { // ARP, deal with it
		if(blabby & 0x8) cout << "For ARP" << endl;
		if(dlen < 28) {
			cerr << "ARP packet length wrong." << endl;
			exit(1);
		}
		
		//printf("Pop packet IP: %02X.%02X.%02X.%02X\n", buf[24], buf[25], buf[26], buf[27]);
		//printf("My IP: %02X.%02X.%02X.%02X\n", ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3]);
		if(!memcmp(buf+24,ipaddr,4)) {
			//cout << "To my IP too!" << endl;
			unsigned char locbuf[100];
	  		memset(locbuf, 0, 100);
	  		memcpy(locbuf, buf, dlen);
			
			memcpy(arptable[arptableindex].eth, locbuf+8, 6);
			memcpy(arptable[arptableindex].ip, locbuf+14, 4);
			arptableindex++;
			
			//char swapbuf[10];
			//memcpy(swapbuf, locbuf+8, 10);
			memcpy(locbuf+18, locbuf+8, 10);
			memcpy(locbuf+8, ethaddr, 6);
			memcpy(locbuf+14, ipaddr, 4);
			locbuf[7] = 2;
			
			if(buf[7] != 2) {// good thing?  bad thing?  unknown
				//cout << "Sending my reply" << endl;
				prot_lower->push(locbuf, dlen | 0x806<<16, (char *)arptable[arptableindex-1].eth); // the last is the eth addr
			}
		} // else ignore it
		//else cout << "Not for my IP though" << endl;
	}
	else { // undefined
		cerr << "Unknown packet type; exiting." << endl;
		exit(1);
	}

  return 1;
}
