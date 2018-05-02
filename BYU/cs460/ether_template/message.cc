#include "message.h"

#define PRINTING


void print_addr(char *pre, unsigned char *addr, int len)
{
int i;
#ifdef PRINTING
	printf("%s ", pre);
	for( i=0; i<len; i++ )
	  printf("%x.", addr[i]);
	//cout << (int)((unsigned char *)%x addr)[i] << ".";
        printf("(");
	for( i=0; i<len; i++ )
	  printf("%d.", addr[i]);
	//cout << (int)((unsigned char *)%x addr)[i] << ".";
        printf(")\n");

#endif
}

void print_bits(char *head, char *source,int len){

  int x;
  int y;
  int num;

  printf("%s\n",head);

  //This works for little endian only!
  for (x=0;x<len;x++){
    num=source[x];
    for (y=0;y<8;y++){
      if(num & 128)
        printf("1");
      else
        printf("0");

      if (y==3) printf(" ");
      num <<= 1;
    }
    printf("  ");
    if(!((x+1)%4)) printf("\n");
  }

  printf("\n");
}
void print_chars(char *head, unsigned char *buff, int length)
{
  int i;
  int j;
#ifdef PRINTING
  
  printf("%s\n",head);
  for(i = 0; i < length; i+=16) 
    {
      for (j=0; j< 16 ; j++)
	{
	  if(j==8) printf(" ");
	  if(i+j < length) 
	    printf("%02x ",buff[i+j]);
	  else
	    printf("   ");
	}
      printf("  ");
      for (j=0; j< 16 ; j++)
	{
	  if(j==8) printf(" ");
	  if(i+j < length) 
	    {
	      if(isprint(buff[i+j]))
		printf("%c",buff[i+j]);
	      else
		printf(".");
	    }
	}
      printf("\n");
      
    }
#endif
}
