/***************************************************************************
                          internet.cc  -  description
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


#include "internet.h"


int  internet::init (char *name, protocol *higher, protocol *lower, 
 char *addr, int port, int client)
{
  // curlen keeps track of the length we currently expect, it is only 
  // used for testing

  curlen = 2;
  cout << "This is the internet init routine for " << name << endl;
  print_addr("Internet Address", (unsigned char *)addr, IP_ADDRESS_LEN);
  strcpy(prot_name , name);
  prot_higher = higher;
  prot_lower = lower;

  return 1;
}

// Push the data to the next lower layer
int internet::push (unsigned char *buf, int len, char *addr)
{
  cout << "This is the internet push routine" << endl;
  print_chars(prot_name, buf, len);

  // The default behavior will be to echo the same data back through 
  // pop routine if there is no lower layer protocol

  // You should put code to send the data over the network here

  if(prot_lower)
    prot_lower->push (buf, len, addr);

  return 1;
}

// Pop the data from the next lower layer
int internet::pop (unsigned char *buf, int len)
{
  int i;

  cout << "This is the internet pop routine" << endl;
  print_chars(prot_name, buf, len);
  // We expect 0xfeedbeef
  if((buf[0] == 0xfe) &&(buf[1] == 0xed) &&
     (buf[2] == 0xbe) &&(buf[3] == 0xef)) {
    cout << "IT WORKED !!!!!!!!!!!!! You passed off this portion of lab 4!!" << endl;
  }

  return 1;
}

