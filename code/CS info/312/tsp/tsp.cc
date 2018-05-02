// write your code in this file.  

#include </usr/include/sys/timeb.h>
#include <stdlib.h>
#include <stdio.h>

#define INF 2500000

void usage() {
  fprintf(stderr, "USAGE: tsp <filename>\n");
  exit(0);
}

int** tsp(int **matrix, int n);
void pickEdge(int **matrix, int n, int *i, int *j);
int solution(int **matrix, int n);
int** include(int **matrix, int n, int i, int j);
int** exclude(int **matrix, int n, int i, int j);
void reduce(int **matrix, int n);
void printMatrix(int **matrix, int n, char *msg);
int cycle(int **matrix, int n, int x, int y, int end);

typedef struct linked_list {
  int **data;
  struct linked_list* next;
} List;
List *head = 0;
int** pop();
void push(int** matrix);

/*declarations*/
struct timeb    *pTimerStart, *pTimerStop;
double          DiffTime = 0, maxtime = 0;
int main(int argc, char* argv[]){
  short millipart;
  int i, j, n, timebound, **matrix;
  FILE *infile;
  
  pTimerStart = (struct timeb *) malloc(sizeof(struct timeb));
  pTimerStop = (struct timeb *) malloc(sizeof(struct timeb));

  // you should read the input file before starting the timer
  if(argc > 2 || argc < 2) {
  	usage();
  }
  
  infile = fopen(argv[1], "r");
  if(!infile) {
    fprintf(stderr, "File didn't open right: %s\n", argv[1]);
	exit(0);
  }
  
  fscanf(infile, "%d %d", &n, &timebound);
  maxtime = timebound/1000;
  // make n+1*n+1 matrix, load in
  matrix = (int **)calloc(n+1,sizeof(int));
  for(i = 0; i <= n; i++)
    matrix[i] = (int *)calloc(n+1,sizeof(int));
  for(i = 1; i <= n; i++)
    for(j = 1; j <= n; j++)
      fscanf(infile, " %d ", &(matrix[i][j]));
  
  ftime(pTimerStart); 
  /* the timer is now running.  You should start solving the 
     problem, be sure to exit when time is up. */
  matrix = tsp(matrix, n);
  ftime(pTimerStop);

  // call your output function to print either the optimal solution, 
  // or the soultion you have. 
  millipart = (pTimerStop->millitm - pTimerStart->millitm);
  DiffTime = (pTimerStop->time - pTimerStart->time) + ((float)millipart)/1000;
  printf("File: %s\nLimit: %.2f\nTime: %.2f\n%s\nDistance: %d\nTour: ", argv[1], 
    maxtime, DiffTime, (maxtime > DiffTime)?"OPTIMAL":"NOT Optimal", matrix[0][0]);
  //printMatrix(matrix, n, "Afterwards:");
  putchar('a');
  if(matrix[0][1] == 0 && matrix[1][0] == 0) {
    for(i = 2; i <= n; i++) {
      if(i < 27)
        putchar('a'+(i-1));
      else
        putchar('A'+(i-27));
    }
  }
  else {
    for(i = matrix[1][0]; i != 1; i = matrix[i][0]) {
      if(i < 27)
        putchar('a'+(i-1));
      else
        putchar('A'+(i-27));
    }
  }
  putchar('a');
  putchar('\n');
  // to code below doesn't print output in the right format, but 
  // is an example of how to print the time. 
  /*
  printf("starttime: %ld.%3ld sec.\n", pTimerStart->time,
                                                    pTimerStart->millitm);
  printf("stoptime:  %ld.%3ld sec.\n", pTimerStop->time,
                                                    pTimerStop->millitm);
  millipart = (pTimerStop->millitm - pTimerStart->millitm);

  DiffTime = (pTimerStop->time - pTimerStart->time) + ((float)millipart)/1000;
  printf("Total Time= %.2f sec\n", DiffTime);
  */
  return 0;
}

// Main function of the algorthym (body)
int** tsp(int **matrix, int n) {
  int i, j, bssf, tempi;
  int **incl, **excl, **current, **sol = 0;
  int millipart;
  
  //printMatrix(matrix, n, "Input Matrix:");
  
  for(i = 1; i <= n; i++)
    matrix[i][i] = INF;
    
  reduce(matrix, n);
  
  // initialize bssf, 1->2, 2->3, etc
  bssf = matrix[n][1];
  for(i = 1; i < n; i++)
    bssf += matrix[i][i+1];
  bssf+=matrix[0][0];
  
  //copy into sol
  sol = (int **)malloc((n+1)*sizeof(int));
  for(i = 0; i <= n; i++)
    sol[i] = (int *)malloc((n+1)*sizeof(int));
  for(i = 0; i <= n; i++)
    for(j = 0; j <= n; j++)
      sol[i][j] = matrix[i][j];
  sol[0][0] = bssf;
  
  push(matrix);
  
  //printf("bssf: %d\npushed: %d\n", bssf, matrix[0][0]);
  
  while((current = pop()) != 0 && current[0][0] < bssf) {
    //printMatrix(current, n, "Popped:");
    pickEdge(current, n, &i, &j);
    if(i == 0 && j == 0) {
      for(i = 0; i < n; i++)
        free(current[i]);
      free(current);
      continue;
    }
    
    incl = include(current, n, i, j);
    if(incl[0][0] < bssf) {
      tempi = solution(incl, n);
      if(tempi == 1) {
        bssf = incl[0][0];
        
        for(i = 0; i < n; i++)
          free(sol[i]);
        free(sol);
        
        sol = incl;
        
        continue; // go back to top
      }
      else if(tempi==0) push(incl);
    }
    excl = exclude(current, n, i, j);
    if(excl[0][0] < bssf) push(excl);
    
    // free the current matrix, not needed now
    for(i = 0; i < n; i++)
      free(current[i]);
    free(current);
    ///*
    ftime(pTimerStop);
    millipart = (pTimerStop->millitm - pTimerStart->millitm);
    DiffTime = (pTimerStop->time - pTimerStart->time) + ((float)millipart)/1000;
    if(DiffTime >= maxtime) break; //kick out of loop
    // Leaves Memory Leaks!  bad bad bad.
    //*/
  } //while
  
  //need to close loop first
  for(j=0;j<=n;j++)
    if(!sol[0][j]) break;
  for(i=0;i<=n;i++)
    if(!sol[i][0]) break;
  if(!(i == 1 && j == 1)) { // not first one found, which would be empty
    sol[0][0] += sol[i][j];
    sol[i][0] = j;
    sol[0][j] = i;
  }
  return sol;
}

// If is a solution, exactly one city should not have been left (the last picked)
int solution(int **matrix, int n) {
  int i, j, exit=-1;
  //char buf[80];
  
  for(i = 1; i <= n; i++)
    exit += !(matrix[0][i]);
  
  if(!exit) {
    for(i = 1; i <= n; i++)
      if(!matrix[i][0]) break;
    for(j = 1; j <= n; j++)
      if(!matrix[0][j]) break;
    if(cycle(matrix, n, i, j, 0) || cycle(matrix, n, i, j, 1)) exit=2;
    else exit=1;
  }
  else exit=0;
  //sprintf(buf, "Solution: %s", (exit==1)?"True":"False");
  //printMatrix(matrix, n, buf);
  return exit;
}

// The heart of the algorthym
void pickEdge(int **matrix, int n, int *i, int *j) {
  //Going with maximize exclude, for now
  int ii, jj, x, y, local, smallest, bound, gbound = -1;
  *i = 0; *j = 0;
  //char buf[80];
  //printf("Picking Edge\n");
  for(x = 1; x <= n; x++) {
    if(matrix[0][x]) {
      //printf("Column %d is out\n", x);
      continue; //is all inf if value in it
    }
    for(y = 1; y <= n; y++) {
      if(matrix[y][0]) {
        //printf("Row %d is out\n", y);
        continue;
      }
      if(!matrix[y][x] && !cycle(matrix, n, y, x, 0)) {
        //printf("Safe 0 located at %d,%d\n", y, x);
        smallest = INF;
        
        for(ii = 0; ii <= n; ii++) {
          local = matrix[y][ii];
          if(local < smallest)
            smallest = local;
        }
        bound = smallest;
        smallest = INF;
        
        for(jj = 0; jj <= n; jj++) {
          local = matrix[jj][x];
          if(local < smallest)
            smallest = local;
        }
        bound += smallest;
        
        if(bound > gbound) {
          gbound = bound;
          *i = y;
          *j = x;
        } // if bound
      } // if !matrix
    } // for y
  } // for x
  //sprintf(buf, "Picked edge (%d, %d) on:", *i, *j);
  //printMatrix(matrix, n, buf);
} // funxtion

// An important peice of the algorhym (though not quite the heart) (head?)
void reduce(int **matrix, int n) {
  int i, j, smallest, x, y, local;
  
  //printMatrix(matrix, n, "Reduce Start:");
  
  // Reduce Rows, don't need to reset x,y as they are overwritten
  for(i = 1, smallest = INF; i <= n; i++, smallest = INF) {
    if(matrix[i][0]) continue; // skip this iteration
    
    for(j = 1; j <= n; j++) {
      local = matrix[i][j];
      if(local < smallest) {
        smallest = local;
        x = i;
        y = j;
      }
    } //for j
    
    // Now smallest known, reduce
    matrix[0][0] += smallest;
    for(j = 1; j <= n; j++) {
      local = matrix[i][j];
      if(local != INF) 
        matrix[i][j] = local-smallest;
    } //for j
    
  } //for i
  
  // Reduce Columns, don't need to reset x,y as they are overwritten
  for(j = 1, smallest = INF; j <= n; j++, smallest = INF) {
    if(matrix[0][j]) continue; // skip this iteration
    
    for(i = 1; i <= n; i++) {
      local = matrix[i][j];
      if(local < smallest) {
        smallest = local;
        x = i;
        y = j;
      }
    } //for i
    
    // Now smallest known, reduce
    matrix[0][0] += smallest;
    for(i = 1; i <= n; i++) {
      local = matrix[i][j];
      if(local != INF) 
        matrix[i][j] = local-smallest;
    } //for i  
  } //for j
  //printMatrix(matrix, n, "Reduce End:");
} //reduce

// Returns a reduced matrix w/(x, y) included (right hand)
int** include(int **matrix, int n, int x, int y) {
  int i, j;
  int **temp;
  //char buf[80];
  //sprintf(buf, "Include (%d, %d) Start:", x, y);
  //printMatrix(matrix, n, buf);
  
  // first copy the matrix, so not overwriting
  temp = (int **)malloc((n+1)*sizeof(int));
  for(i = 0; i <= n; i++)
    temp[i] = (int *)malloc((n+1)*sizeof(int));
  for(i = 0; i <= n; i++)
    for(j = 0; j <= n; j++)
      temp[i][j] = matrix[i][j];
  
  temp[0][0] += temp[x][y];
  for(i = 1; i <= n; i++)
    temp[i][y] = INF;
  for(j = 1; j <= n; j++)
    temp[x][j] = INF;
  
  temp[x][0] = y;
  temp[0][y] = x;
  
  reduce(temp, n);
  //printMatrix(temp, n, "Include End:");
  return temp;
}

// Returns a reduced matrix w/(i, j) excluded (left hand)
int** exclude(int **matrix, int n, int x, int y) {
  int i, j;
  int **temp;
  //char buf[80];
  //sprintf(buf, "Exclude (%d, %d) Start:", x, y);
  //printMatrix(matrix, n, buf);
  
  // first copy the matrix, so not overwriting
  temp = (int **)malloc((n+1)*sizeof(int));
  for(i = 0; i <= n; i++)
    temp[i] = (int *)malloc((n+1)*sizeof(int));
  for(i = 0; i <= n; i++)
    for(j = 0; j <= n; j++)
      temp[i][j] = matrix[i][j];
  
  temp[x][y] = INF;
  reduce(temp, n);
  //printMatrix(temp, n, "Exclude End:");
  return temp;
}

int** pop() {
  int **tmp;
  List *temp;
  
  if(head == 0) return 0;
  
  tmp = head->data;
  temp = head->next;
  free(head);
  head = temp;
  return tmp;
}

void push(int** matrix) {
  List *temp, *pre;
  int bound = matrix[0][0];
  
  if(head == 0) {
    head = (List*)malloc(sizeof(List));
    head->data = matrix;
    head->next = 0;
  }
  else {
    for(temp = head, pre = 0; temp && temp->data[0][0] < bound; pre = temp, temp = temp->next);
    if(pre == 0) {
      head = (List*)malloc(sizeof(List));
      head->data = matrix;
      head->next = temp;
    }
    else {
      pre->next = (List*)malloc(sizeof(List));
      pre->next->next = temp;
      pre->next->data = matrix;
    }
  }
}

void printMatrix(int **matrix, int n, char *msg) {
  int i, j;
  
  printf("%s\n", msg);
  for(i = 0; i <= n; i++) {
    for(j = 0; j <= n; j++)
      if(matrix[i][j] != INF) printf("%-5d", matrix[i][j]);
      else printf("INF  ");
    printf("\n");
  }
  getchar();
}

int cycle(int **matrix, int n, int x, int y, int end) {
  int i, j;//, find = 0;
  //char buf[80];
  
  if(end) { //generic check
    //printMatrix(matrix, n, "Does this matrix cycle?");
    j = 1;
    for(i = matrix[1][0]; i != 1; i = matrix[i][0], i = (!i)?y:i) j++;
    if(j < n) return 1;
  }
  else { //cycle with edge?
    //sprintf(buf, "Does (%d, %d) on the matrix cycle?", x, y);
    //printMatrix(matrix, n, buf);
    for(i = 1; i <= n; i++) {
      /*if(find == 3) return 1;
      if(find != 1 && matrix[0][i] == y) find++;
      if(find != 2 && matrix[i][0] == x) find+=2;
      if(find == 3) return 1;*/
      if(matrix[y][0] == x && matrix[0][x] == y) return 1;
      if(matrix[0][i] == 0) continue; // 0 entries don't count
      for(j = i+1; j <= n; j++) 
        if(matrix[0][i] == matrix[0][j]) return 1;
    }
  }
  //printf("No, it doesn't.\n");
  //getchar();
  return 0; // safe still
}

