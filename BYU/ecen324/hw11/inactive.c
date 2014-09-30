#include <stdio.h>
#include <stdlib.h>
#include "clock.h"

int inactive_duration(int thresh); 

int main (int argc, char *argv[])
{
    int i;

    for (i = 0; i < 25; i++) 
    {
	int d = inactive_duration(1000);
	printf("%d cycles\n", d);
    }
    return 0;
}

// by Ged, 4-1-2004
int inactive_duration(int thresh) 
{
    double value;
    start_counter();
    while(value = get_counter())
      if(value > thresh) return (int) value;
      else start_counter(); //resets counter, so we check between reads 
}
