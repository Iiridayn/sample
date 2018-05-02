// Base class methods: These should be overridden by the derived classes

#include "protocol.h"

int  protocol::init (char *name, protocol *higher, protocol *lower, 
 char *addr, int port, int client)
{
   cout << "This is the protocol init routine for" << name << endl;
   strcpy(prot_name , name);
   prot_higher = higher;
   prot_lower = lower;
   return 1;
}

// Push the data to the next lower layer
int protocol::push (unsigned char *buf, int len, char *addr)
{
   cout << "This is the protocol push routine" << endl;
   print_chars(prot_name, buf, len);

   // The default behavior will be to echo the same data back through 
   // pop routine if there is no lower layer protocol

   if(!prot_lower)
     pop (buf, len);
   else
     prot_lower->push (buf, len, addr);

   return 1;
}

// Pop the data from the next lower layer
int protocol::pop (unsigned char *buf, int len)
{
   cout << "This is the protocol pop routine" << endl;
   print_chars(prot_name, buf, len);
   if(prot_higher)
      prot_higher->pop (buf, len);

   return 1;
}

