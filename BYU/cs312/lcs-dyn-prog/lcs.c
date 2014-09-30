#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STRING 20000
#define MAX(a, b) (((a) > (b))?(a):(b))

void usage();
int lengthLCS();

char bufa[MAX_STRING];
char bufb[MAX_STRING];

int main(int argc, char* argv[]) {
	FILE* infile;

  if(argc > 2 || argc < 2) {
  	usage();
  }
  
  infile = fopen(argv[1], "r");
	if(!infile) {
		fprintf(stderr, "File didn't open right: %s", argv[1]);
		exit(0);
  }
  
  fgets(bufa, MAX_STRING, infile);
  fgets(bufb, MAX_STRING, infile);
  fclose(infile);
  
  printf("The LCS is %d.\n", lengthLCS());
}

int lengthLCS() {
	int* result[2];
	int a = strlen(bufa)-1; // fgets sticks in the newline
  int b = strlen(bufb)-1;
  int i,j,ans,*temp;
  
  result[0] = (int*)calloc(b,sizeof(int));
  result[1] = (int*)calloc(b,sizeof(int));
  
  // Going for bottom up, as all cases will be considered anyway
  for(i = 0; i < a; i++) {
  	for(j = 0; j < b; j++) {
    	if(i == 0 || j == 0)
      	result[1][j] = 0;
      else if(bufa[i] == bufb[j])
      	result[1][j] = result[0][j-1] + 1;
      else {
				result[1][j] = MAX(result[1][j-1], result[0][j]);
       	//printf("MAX(%d,%d) = %d\n",result[1][j-1],result[0][j],result[1][j]);
      }
	    //printf("Result[%d][%d] = %d\n",1,j,result[1][j]);
    } // j
    
		/* Changed my mind here after handin */
    // Ended up saving about 2 secs out of the 20 left.
    temp = result[0];
    result[0] = result[1];
    result[1] = temp; // just overwrites, so why recreate?
    /*
    // test3 was 7 seconds faster w/heap.  Still 20 secs though...
    free(result[0]);
    result[0] = result[1];
    result[1] = (int*)calloc(b,sizeof(int));
    */
    
    /*
    // shift up row, don't bother clearing, will overwrite
    // No longer MAX_STRING, only need the first b
    for(j = 0; j < b; j++)
    	result[0][j] = result[1][j];
    */
  } // i
  ans = result[0][b-1];
  free(result[0]);
  free(result[1]);
  //printf("a = %d\nb = %d\nresult[1][b-1] = %d\n",a,b,result[1][b-1]);
  return ans;
} // funct

void usage() {
	fprintf(stderr, "USAGE: lcs <filename>\n");
  exit(0);
}

