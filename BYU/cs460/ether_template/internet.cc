
// This is just a template for you to use in your coding

#include "internet.h"

// This is the maximum packet size that will be sent.  It is also declared in main.cc
#define MAXLEN 40

int popit =0;

int  internet::init (char *name, protocol *higher, protocol *lower, 
                        char *addr, int port, int client)
{
    // curlen keeps track of the length we currently expect, it is only 
    // used for testing
    curlen = 2;
    cout << "This is the internet init routine for " << name << endl;
    print_addr("Internet Address", (unsigned char*)addr, IP_ADDRESS_LEN);
    strcpy(prot_name , name);
    prot_higher = higher;
    prot_lower = lower;
    popit = client;

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
    
    return 0;
}

int whichtest = 0;

// Pop the data from the next lower layer
int internet::pop (unsigned char *buf, int len)
{
    int i;
    if(popit)
    { 
        unsigned long *beef;
        cout << "This is the internet pop routine" << endl;
        print_chars(prot_name, buf, len);
        if(whichtest>=4)
        {
            cout<< "If the sentence went through with no errors you passed!!!"<<endl;
            cin.getline((char*)buf, 900);

            exit(0);
        }
        beef = (unsigned long *)buf;
        if(*beef == 0xefbeadde)
        {
            cout << "test #" << whichtest << " worked" << endl;
            whichtest++;
        }
        else
        {
            printf("expected 0xdeadbeef, got %X\n", *beef);
            exit(0);
        }
        if(whichtest >= 4)
        { 
            cout << "IT WORKED !!!!!!!!!!!!! You passed off the receive portion of the lab!!" << endl;
            cout<<"GOTO the Server and type in a string to send."<<endl;
        }
    }
    else
    {  
        cout << "This is the internet pop routine" << endl;
        print_chars(prot_name, buf, len);
        if(len != curlen)       // We keep track of what we expect next
        {
            cout << "ERROR!!!!!!!!!! expected packet of length " << curlen << "Got packet of length " << len; 
            exit(0);
        }
        for(i = 0; i < curlen; i++)
        {
            if((unsigned char)buf[i] != (unsigned char)(i ^ 0xff))
            {
                printf("Error, expected %x at buf[%d] but got %x\n",(i ^ 0xff),i,buf[i]);
                exit(0);
            }
        }
        curlen+=2;
        if(curlen == MAXLEN)
        {
            cout << "IT WORKED !!!!!!!!!!!!! You passed off the send portion of lab 4!!" << endl;
            cout << "Now type the ENTER key to run the receive tests." << endl;
            cout << "If you havent done so already, you will need to compile your code with" << endl;
            cout << "the new version of internet.cc that you can find on the lab 4 web page. " << endl;
            cout << "It will tell you if you passed off everything. " << endl;      
        }
    }
    
    return 0;
}
