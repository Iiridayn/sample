#include <stdio.h>

#define BUFSIZE 20

// Ged wanted more control, so he wrote it this way
int good_echo() {
	char buf[BUFSIZE];
  int number = -1;
  
  char in = getchar();
  while(in != EOF && in != '\n') {
    if(++number < BUFSIZE-1)
	    buf[number] = in;
  	in = getchar();
  }
  if(number >= BUFSIZE-1)
  	return -1;
  
  buf[BUFSIZE-1] = '\0';
  printf("%s\n", buf);
  return 0;
}

// Test wrapper
int main() {
	if(good_echo())
  	printf("An error has occured.\n");
}

