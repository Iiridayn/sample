
// This is just a template for you to use in your coding

#include "rpc.h"
#include <stdio.h>

extern int blabby;
int handle = 0;
//unsigned char GlobalBuf[100];

unsigned char *replyPack;
unsigned char *responsePack;
unsigned char *requestPack;


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
}

// Push the data to the next lower layer
int rpc::push (unsigned char *buf, int len, char *addr, message *mb)
{
  cout << "This is the rpc push routine" << endl;
  print_chars(prot_name, buf, len);

  cout << "This routine should never be called!!" << endl;
}


// Pop the data from the next lower layer and execute the given procedure
int rpc::pop (unsigned char *buf, int len, message *mb)
{
  cout << "\nThis is the rpc pop routine " <<response<< endl;
  print_chars(prot_name, buf, len);

  //if(REQUEST){  ->server
    //to open
      //open the file and make a REPLY pack with the handle  and tell it's open
    //call prot_lower->push and send REPLY pack

    //to read
    //unsigned char bufread[100];
    //get handle from the package, get the size from the package
    //read the file in bufread  in numBytesRead = ::read(handle,&bufread[0],size);
    //send a response pack with Operation = read, handle, numBytesRead,bufread
    //call push and send the Reply pack


    //write
    //will get the handle from the packege, get the dat from package, get the num of bytes to write from the package
    // numBytesWritten = ::write(handle,data,num of bytes to write);
    //make a Response with operation = wrote, handle, num of bytes written


  //}
  //else { it a RESPONSE ->client
  //it opend
    //set the global handle to the handle the came in the pack

  //it read
  //memcpy to GlobalBuf the data from the Reply pack
  //set GlobalLen = the numBytesRead from the package

  //it wrote
  //set GlobalLen = the numBytesWritten from the package


  //response =1;
  //}
  return 1;
}

// Open the remote file
int rpc::open (char *filename)
{
    cout << "\nThis is the rpc open routine" << endl;
    print_chars(prot_name, (unsigned char *)filename, strlen(filename));


    //should send a request to the server to open the file
    //make a Request pack with operation == Open
    //prot_lower->push send the request pack


    //while(!response);
    //response = 0;


    //return the globalhandle to main


}

// Read the data from the remote file
int rpc::read (int handle, unsigned char *buf, int len)
{
    cout << "\nThis is the rpc read routine " << endl;
    //make a REQUEST pack with operation = READ , handle, # of bytes to read
    //call prot_lower->push and sedn request pack

    //while(!response);
    //response = 0;

    //memcpy(buf,&globalBuf[0],GlobalLen);
        //return GlobalLen;
}

// Write the data to the remote file
int rpc::write (int handle, unsigned char *buf, int len)
{
    cout << "This is the rpc write routine" << endl;
    print_chars(prot_name, buf, len);
    //make a REQUEST pack with operation = WRITE, handle, numof bytes you wnat to write, the data that you want to write
    //call prot_lower->push and send the REQUEST pack

    //while(!response);
    //response = 0;

    //return GlobalLen;
}
