
// This is just a template for you to use in your coding

#include "ethernet.h"
#include <stdio.h>

#define CRC_POLY   0x04c11db7
#define OFFSET     14        // 6 (destAddr) + 6 (srcAddr) + 2 (type) = 14
#define BITMASK	   128
#define ARPTYPE	   0x0806
#define IPTYPE	   0x0800

extern int blabby;

int  ethernet::init (char *name, protocol *higher, protocol *lower, 
 char *addr, int port, int client)
{
  prot_higher = higher;
  prot_lower = lower;
	strcpy(prot_name , name);

  cout << "This is the ethernet init routine for " << name << endl;
  print_addr("Ethernet Address", (unsigned char *)addr, ETH_ADDRESS_LEN);
	//if(strlen(addr) > ETH_ADDRESS_LEN) cerr << "Ethernet init: address is " << strlen(addr) << ", too long." << endl, exit(0);
	memcpy(MAC, addr, ETH_ADDRESS_LEN);
  return(0);

}

// Push the data to the next lower layer
int ethernet::push (const unsigned char *buf, int len, char *addr) {
	usleep(100000);
	if(len > MAX_MSG) cerr << "Ethernet push: message too long" << endl, exit(0);
	//if(strlen(addr) > ETH_ADDRESS_LEN) cerr << "Ethernet push: address too long" << endl, exit(0);
	
	char message[MAXBUF];
	memset(message, 0, MAXBUF);
	//print_chars("empty", (unsigned char*) message, MAXBUF);
	memcpy(message+OFFSET+1, buf, len);
	//print_chars("message", (unsigned char*) message, MAXBUF);
	memcpy(message+1, addr, ETH_ADDRESS_LEN);
	//print_chars("dest", (unsigned char*) message, MAXBUF);
	memcpy(message+1+ETH_ADDRESS_LEN, MAC, ETH_ADDRESS_LEN);
	//print_chars("src", (unsigned char*) message, MAXBUF);
	// ignored type
	// CRC
	int CRC = genCRC(message+1, len+18);
	CRC = htonl(CRC);
	memcpy(message+OFFSET+1+len, &CRC, sizeof(int));
	//print_chars("CRC", (unsigned char*) message, MAXBUF);
	//if(blabby & 0x2) cout << "Ethernet Push: bitstuffing message: " << message << endl;
	int footerstart = bitstuff(message, len+19); // the leading 0's will be ignored
	//print_chars("stuffed", (unsigned char*) message, MAXBUF);
	//cout << "Footer start: " << footerstart << endl;
	//if(blabby & 0x2) cout << "Message stuffed, length " << footerstart << ", up from " << len << "." << endl;
	message[0] = AMBLE;
	//message[footerstart] = AMBLE;
	//print_chars("full", (unsigned char*) message, MAXBUF);
	
  if(blabby & 0x2)
    {
      //cout << "This is the ethernet push routine len = " << len << endl;
      print_addr("Ethernet Address", (unsigned char *)addr, ETH_ADDRESS_LEN);
		cout << "Push: ";
      print_chars(prot_name, (unsigned char*) buf, len);
    }
	
	if(prot_lower)
		prot_lower->push ((unsigned char *)message, footerstart+1, 0);
	//getchar();
 return(0);
}




// Pop the data from the next lower layer
int ethernet::pop (unsigned char *buf, int len)
{
	 if(len > MAXBUF) cerr << "Ethernet Pop: message too long" << endl, exit(0);
	 //cout << endl << endl << endl << endl;
	 char localBuf[MAXBUF];
	 memset(localBuf, 0, MAXBUF);
	 memcpy(localBuf, buf, len);
	 int end, start = 0, unlen;
	 while((end = unstuff(localBuf, len, start, &unlen)) != -1) { //while there are frames // This part is broken
		if(blabby & 0x2) {
			cout << "Pop: ";
			print_chars(prot_name, (unsigned char*)localBuf+start, unlen);
		}
		if(unlen < OFFSET) cerr << "Ethernet pop: Incomplete packet." << endl, exit(0);
		if(genCRC(localBuf+start, unlen)) {
			cerr << "Ethernet pop: Packet with a bad CRC." << endl;
			return 0;
		}
		if(strncmp(localBuf+start, MAC, 6)) {
			cerr << "Ethernet Pop: packet not to this address." << endl;
			return 0;
		}
			/*cout << "This address: " << MAC << endl;
			cout << "Packet address: " << localBuf[start] << localBuf[start+1] << localBuf[start+2]
				<< localBuf[start+3] << localBuf[start+4] << localBuf[start+4] << endl;
			cout << "They aren't equal." << endl;
		}*/
		//strip header, pass up
		char frameBuf[MAXBUF];
		memset(frameBuf, 0, MAXBUF);
		memcpy(frameBuf, localBuf+start+OFFSET, unlen-18); //?
		if(prot_higher)
     		prot_higher->pop ((unsigned char *)frameBuf, unlen-18); // baka magic numbers
		else
			cout << "Ethernet Pop: recieved: " << frameBuf << "." << endl;
		
		start = end;
	 } 
	 
 return 1;
}

int ethernet::genCRC(char *buf, int len) {
	int CRC = 0;
	for(int i = 0; i < len; i++) {
		for(short readMask = 0x80; readMask; readMask >>= 1) {
			bool mask = false;
			if(CRC & 0x80000000) mask = true;
			CRC <<= 1;
			if(mask) CRC ^= CRC_POLY;
			if(buf[i] & readMask) CRC ^= 1;
		}
	}
	return CRC;
}

int ethernet::bitstuff(char *buf, int len) {
	char stuffed[MAXBUF];
	memset(stuffed, 0, MAXBUF);
	short stuffMask = 0x80, readMask;
	short ones, stuffIndex = 0;
	for(int i = 0; i < len; i++) {
		for(readMask = 0x80; readMask; readMask >>= 1) {
			//printf("Readmask: %02X\n", readMask);
			//sleep(1);
			if(buf[i]&readMask) {
				ones++;
				stuffed[stuffIndex] += stuffMask;
			}
			else
				ones = 0;
			
			stuffMask >>= 1;
			if(!stuffMask) {
				stuffMask = 0x80;
				stuffIndex++;
			}
			
			if(ones == 5) {
				stuffMask >>= 1; //insert a zero
				if(!stuffMask) {
					stuffMask = 0x80;
					stuffIndex++;
				}
				ones = 0;
			}
		}
	}
	
	//stick on the closing sentinal
	stuffMask >>= 1; // 0
	if(!stuffMask) {
		stuffMask = 0x80;
		stuffIndex++;
	}
	for(int i = 0; i < 6; i++) { // 6 1s
		stuffed[stuffIndex] |= stuffMask;
		stuffMask >>= 1;
		if(!stuffMask) {
			stuffMask = 0x80;
			stuffIndex++;
		}
	}
	stuffMask >>= 1; // 0
	if(!stuffMask) {
		stuffMask = 0x80;
		stuffIndex++;
	}
	
	stuffIndex++;
	for(int i = 0; i < stuffIndex; i++)
		buf[i] = stuffed[i];	
	return stuffIndex;
}

int ethernet::unstuff(char *buf, int len, int start, int *length) {
	char unstuffed[MAXBUF];
	memset(unstuffed, 0, MAXBUF);
	short ones = 0;
	short unstuffMask = 0x80, unstuffIndex = 0;
	bool done = false;
	int seq = -1;
	int index;
	
	/*
	if(blabby & 0x2) {
		cout << "Unstuffing:" << endl;
		for(index = start; index < len && !done; index++) 
			for(short readMask = 0x80; readMask && !done; readMask >>= 1) 
				putchar((buf[index]&readMask)?'1':'0');
		cout << endl << "Into:" << endl;
	}
	//*/
	
	for(index = start; index < len && !done; index++) {
		for(short readMask = 0x80; readMask && !done; readMask >>= 1) {
			if(buf[index]&readMask) {
				ones++;
				if(seq != -1) {
					//putchar('1');
					unstuffed[unstuffIndex] += unstuffMask;
				}
			}
			else {
				//if(seq != -1) putchar('0');
				ones = 0;
			}
			
			if(seq != -1) {
				unstuffMask >>= 1;
				if(!unstuffMask) {
					unstuffMask = 0x80;
					unstuffIndex++;
				}
			}
			
			if(ones == 5) {
				readMask >>= 1;
				if(!readMask) {
					readMask = 0x80;
					index++;
				}
				
				if(buf[index]&readMask) { // six ones
					readMask >>= 1;
					if(!readMask) {
						readMask = 0x80;
						index++;
					}
					
					if(buf[index]&readMask) {
						cerr << "Bad unstuff string." << endl;
						//return -1;
						// instead, throw away up to here, restart from here.
						memset(unstuffed, 0, MAXBUF);
						seq = -1;
						unstuffMask = 0x80;
						unstuffIndex = 0;
					}
					else {
						//cout << "seq = " << seq << endl;
						if(seq != -1) {
							if(seq == index-1) { //two in a row, not a good thing
								//clear up written, use this as the new start
								unstuffed[0] &= 0; //clear it
								seq = index;
								//cout << "Found two special chars in a row at byte " << index << endl;
							}
							else {
								done = true; //ending
								for(int i = 0; i < 6; i++) {
									unstuffed[unstuffIndex] &= ~unstuffMask; // zero the bit
									unstuffMask <<= 1;
									if(!unstuffMask) {
										unstuffMask = 0x01;
										unstuffIndex--;
									}
								}
								//cout << "Found closing terminator at byte " << index << endl;
							}
						}
						else {
							seq = index;
							//cout << "Starting signal at byte " << seq << "." << endl;
						}
					}
				}
				// else strip next, already incremented, stripped by not
				// putting on end of unstuffed buffer
				ones = 0;				
			}
		}
	}
	//cout << endl;
	if(!done) return -1;
	
	//unstuffIndex++;
	for(int i = 0; i < unstuffIndex+1; i++) {
		buf[start+i] = unstuffed[i];
	}
	//print_chars("unstuff", (unsigned char*)unstuffed, unstuffIndex);
	*length = unstuffIndex;
	return index;
}

/*
int ethernet::bitstuff(char *buf, int len) {
	char stuffed[MAXBUF];
	int sindex, index, snum, inum, sin;
	char comp; //search for 5 1's, insert a 0 after
	
	if(len < 1) cerr << "Message to stuff too small (no message)." << endl, exit(0);
	
	index = 1;
	comp = 0;
	sindex = 0;
	snum = 0;
	inum = 0;
	sin = buf[0];
	
	while(index < len) {
		if(sindex > 99) cerr << "Message exceeded maximum expected length." << endl, exit(0);
		if(snum == 8) { // 8 correct?
			stuffed[sindex++] = comp;
			snum = 0;
			comp = 0;
		}
		if(inum == 8) {
			sin = buf[index++];
			inum = 0;
		}
  		
   		comp <<= 1;
   		snum++;
   		comp += (0x80 & sin)?1:0;
   		sin <<= 1;
   		inum++;
     
       	if(!(~comp & 0x1F)) { // if bottom 5 bits are 1
       		comp <<= 1; //shift had better stick a 0 there.
       		snum++;
       	}
	}
	
	if(snum) { // for the last possible partial byte
		comp <<= 8-snum;
		stuffed[sindex++] = comp;
	}
	
	for(int i = 0; i < sindex; i++)
		buf[i] = stuffed[i];
	return sindex; // will buf be big enough?
}
*/

/*
int ethernet::unstuff(char *buf, int len, int start) {
	char unstuffed[MAXBUF];
	int sindex, index, snum, inum, sin;
	char comp;
	
	if(len < 1) cerr << "Message to unstuff too small (no message)." << endl, exit(0);
	if(len > MAXBUF) cerr << "Message exceeded maximum expected length." << endl, exit(0);
	
	index = start+1;
	comp = 0;
	sindex = 0;
	snum = 0;
	inum = 0;
	sin = buf[start];
	int seq = 0;
	bool frame = false;
	
	while((index < len) && !frame) {
		if(snum == 8) { // 8 correct?
			if(seq)
				unstuffed[sindex++] = comp;
			snum = 0;
			comp = 0;
		}
		if(inum == 8) {
			sin = buf[index++];
			inum = 0;
		}
  		
   		comp <<= 1;
   		snum++;
   		comp += (0x80 & sin)?1:0;
   		sin <<= 1;
   		inum++;
     
       	if(!(~comp & 0x1F)) { // if bottom 5 bits are 1
			if(0x80 & sin) { //had better be a control signal
				sin <<= 1;
				inum++;
				if(inum == 8) {
					sin = buf[index++];
					inum = 0;
				}
				
				if(0x80 & sin) cerr << "Bad unstuff string." << endl, exit(0);
				else { //control character
					if(!seq) { //found the start of the next frame
						seq = index-1;
						snum = 0;
						comp = 0; //don't want control in frame
					}
					else { // end of frame
						if(seq+1 == index-1) //two in a row
							seq = index-1;
						else {
							frame = true;
							unstuffed[sindex++] = (comp &0xC0); //dump the part of the control
						}
					}
				}
			}
			else { //if next is a 0, throw it away; simple incrememnt past it
				sin <<= 1;
				inum++;
			}
       	}
	}
	
	for(int i = 0; i < sindex; i++) //is okay, because will be same size or smaller
		buf[start+i] = unstuffed[i];
	
	if(frame) return index-1; //current char
	else return -1; //failure to find a frame
}
*/
