/*
  msquare.cc by Ged Crimsonclaw (Michael Clark)
  Last modified: 3-5-2004 (written about then too)
  
  Solves a magic square given very specific limiting circumstances
  (for ease of programming).  Amazingly, worked after only two segfaults.
  (except the counter).  (And some missing library declarations.  Both segfaults
  were due to simple boundary cases in the linked list code.)
*/

/* Includes */
#include <stdio.h>
#include <stdlib.h>

/* Defines */
typedef struct linked_list {
  int value;
  struct linked_list* next;
} List;

/* Function Declarations */
void usage();
int msquare(int** square, List* candidate, int loc);
int feasible(int** square, int loc);
int solution(int** square);

/* Globals */
int* row;
int* col;
int n,n2,count;

int main(int argc, char* argv[]) {
  FILE* infile;
  int i, temp;
  int** square;
  List* candidate = 0;
  List* temp_l;

  if(argc > 2 || argc < 2) {
  	usage();
  }
  
  infile = fopen(argv[1], "r");
	if(!infile) {
		fprintf(stderr, "File didn't open right: %s", argv[1]);
		exit(0);
  }
  
  /* Read file data */
  fscanf(infile, "%d", &n);
  n2 = n*n;
  square = (int **)calloc(n,sizeof(int));
  row = (int *)calloc(n,sizeof(int));
  col = (int *)calloc(n,sizeof(int));
  for(i = 0; i < n; i++)
    square[i] = (int *)calloc(n,sizeof(int));
  for(i = 0; i < n2; i++) {
    fscanf(infile, "%d", &temp);
    temp_l = (List *)malloc(sizeof(List));
    temp_l->next = candidate;
    candidate = temp_l;
    candidate->value = temp;
  }
  for(i = 0; i < n; i++)
    fscanf(infile, "%d", &row[i]);
  for(i = 0; i < n; i++)
    fscanf(infile, "%d", &col[i]);

  fclose(infile);

  /* Core routine */
  if(msquare(square, candidate, 0)) {
    for(i = 0; i < n; i++) {
      for(temp = 0; temp < n; temp++)
        printf("%d\t", square[i][temp]);
      printf("= %d\n", row[i]);
    }
    for(i = 0; i < n; i++)
      printf("=\t");
    printf("\n");
    for(i = 0; i < n; i++)
      printf("%d\t", col[i]);
    printf("\n");
  }
  printf("Completed squares: %d\n", count);
  
  
  /* Close */
  for(i = 0; i < n; i++)
    free(square[i]);
  free(square);
  free(row);
  free(col);
  while(candidate != 0) {
    temp_l = candidate;
    candidate = candidate->next;
    free(temp_l);
  }
  exit(0);
}

void usage() {
  fprintf(stderr, "USAGE: msquare <filename>\n");
  exit(0);
}

int msquare(int** square, List* candidate, int loc) {
  List* index = candidate;
  List* pre = 0;
  int done = 0, a = loc/n, b = loc%n;
  
  count++;
  
  while(index != 0) {
    square[a][b] = index->value;
    if(pre == 0) candidate = index->next;
    else pre->next = index->next;
    
    if(feasible(square, loc)) // else, go on, else, ret 0 at bottom
      if(loc >= (n2-1) && solution(square)) done = 1;
      else done = msquare(square, candidate, loc+1);
    
    if(done) {
      if(pre == 0) candidate = index;
      else pre->next = index;
      return 1;
    }
    
    if(pre == 0) candidate = index;
    else pre->next = index; // index->next already correct
    pre = index;  
    index = index->next;
  }
  
  square[a][b] = 0; //clean up the square, so don't get stuff confused
  return 0;
}

// Can make it tighter, but figured this would be enough
int feasible(int** square, int loc) {
  int row_num = loc/3;  // only check what was changed
  int col_num = loc%3;
  int sum,i;
  
  sum = 0;
  for(i = 0; i < n; i++)
    sum += square[row_num][i];
  if(sum > row[row_num]) return 0; // All numbers are positive, so if greater,
  
  sum = 0;
  for(i = 0; i < n; i++)
    sum += square[i][col_num];
  if(sum > col[col_num]) return 0; // it's too big.
  
  return 1;
}

int solution(int** square) {
  int i,j,sum;
  
  //count++; // forgot this for some reason
  
  for(i = 0; i < n; i++) {
    sum = 0;
    for(j = 0; j < n; j++)
      sum += square[i][j];
    if(sum != row[i]) return 0;
  }
  
  for(i = 0; i < n; i++) {
    sum = 0;
    for(j = 0; j < n; j++)
      sum += square[j][i];
    if(sum != col[i]) return 0;
  }
  
  return 1;
}

