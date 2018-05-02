/***************************************************************************
                          ethernet.cc  -  description
                             -------------------
    begin                : Wed May 17 2000
    copyright            : (C) 2000 by Trevor Clifton
    email                : Trevor_Clifton@bigfoot.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "ethernet.h"
#include <netinet/in.h>
#define OFFSET     14        // 6 (destAddr) + 6 (srcAddr) + 2 (type) = 14
#define BITMASK	   128
#define ARPTYPE	   0x0806
#define IPTYPE	   0x0800

extern int blabby;


int  ethernet::init (char *name, protocol *higher, protocol *lower, 
 char *addr, int port, int client)
{
  if(blabby & 0x2) {
    cout << "This is the ethernet init routine for " << name << endl;
    print_addr("Ethernet Address", (unsigned char *)addr, ETH_ADDRESS_LEN);
  }
  memcpy ((void*)&myAddr[0], addr , 6);
  strcpy(prot_name , name);
  prot_higher = higher;
  prot_lower = lower;

   return 1;

}

// Push the data to the next lower layer
int ethernet::push (unsigned char *buf, int len, char *addr)
{ 
  int ethtype, msglength;
// Grab the type out of the top of the length parameter and send it
  ethtype = len >> 16;
  msglength = len & 0xffff;

  if(blabby & 0x2) {
    cout << "This is the ethernet push routine len = " << msglength << endl;

    print_addr("Push Ethernet SRC Address", myAddr, ETH_ADDRESS_LEN);
    print_addr("Push Ethernet DST Address", (unsigned char *)addr, ETH_ADDRESS_LEN);
    print_chars(prot_name, buf, msglength );
  }

  //put together the pieces for the ethernet frame along with the message
  memcpy ((void*)&ethmessage[0], addr, ETH_ADDRESS_LEN);
  memcpy ((void*)&ethmessage[6], myAddr, ETH_ADDRESS_LEN);
  if(blabby & 0x2) {
    printf("ETHTYPE is %x\n",ethtype);
  }

// Byte swap, we need to change this to a htons
  ethmessage[13] = (ethtype & 0xff);
  ethmessage[12] = ((ethtype >> 8) & 0xff);

  memcpy ((void*)&ethmessage[14], buf, msglength );
  

  if(prot_lower)
    prot_lower->push ((unsigned char *)ethmessage, msglength+14, 0);
    
   return 1;

}


// Pop the data from the next lower layer
int ethernet::pop (unsigned char *buf, int len)
{
  int i;
  int combined;
  int ethtype;
  int notbcast; // Flag indicating this is not a bcast address
  if(blabby & 0x2) {
    cout << "\nThis is the ethernet pop routine" << endl;
    print_chars(prot_name, buf, len);
  }

  memcpy (destAddr,&buf[0], ETH_ADDRESS_LEN);
  memcpy (srcAddr, &buf[ETH_ADDRESS_LEN], ETH_ADDRESS_LEN);
  memcpy (&ethtype, &buf[ETH_ADDRESS_LEN*2], 2);
  if(blabby & 0x2) {
    print_addr("Pop Ethernet SRC Address", srcAddr, ETH_ADDRESS_LEN);
    print_addr("Pop Ethernet DST Address", destAddr, ETH_ADDRESS_LEN);
  }

  // Check to make sure that we should accept this packet
  // We accept it if it is for the broadcast address or our address
  notbcast = 0;
  for(i = 0; i < ETH_ADDRESS_LEN; i++) {
    if(destAddr[i] != 0xff) {
      notbcast = 1;
      break;
    }
  }
  // If this is not the broadcast address, check to make sure it matches ours
  if(notbcast) 
    for(i = 0; i < ETH_ADDRESS_LEN; i++) {
      if(destAddr[i] != myAddr[i]) 
      {
      if(blabby & 0x4) {
	cout << "ethernet::pop dst address didnt match my address " << endl;
	print_addr("MY Address", myAddr, ETH_ADDRESS_LEN);
	print_addr("Dest Address", destAddr, ETH_ADDRESS_LEN);
      }
      return 0;
      }
    }
  // Append type to length and pop
  combined = (len-14) | htons(ethtype)<<16;
  if(blabby & 0x2) {
    printf("COMBINED is %x\n", combined);
  }
  if(prot_higher)
    prot_higher->pop ((unsigned char *)&buf[14], combined);
  return(1);
}


 
