/***************************************************************************
                          internet.h  -  description
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
#ifdef WIN32

#else
  #include <ctype.h>
#endif

#include "protocol.h"
 
#ifndef INTERNET_H
#define INTERNET_H

#define IP_ADDRESS_LEN 4

class internet: public protocol {
private:

public:
  int curlen;

  int init (char *name, protocol *higher, protocol *lower, 
   char *addr, int port, int client);
  int push (unsigned char *buf, int len, char *addr);
  int pop (unsigned char *buf, int len);
};

#endif
