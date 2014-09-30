// tsp-ga.c by Ged Crimsonclaw.
// based off of a stub file prolly owned by BYU.
// Created about 4-9-2004.

#include </usr/include/sys/timeb.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#define POPULATION 20

/*declarations*/
void usage() {
  fprintf(stderr, "USAGE: tsp-ga <filename>\n");
  exit(0);
}
int* tsp_ga(int **matrix, int n);
int length(int **matrix, int* path, int n);
void mutate(int *path, int n);
int range(int start, int end);
void crossover(int *father, int *mother, int n, int **brother, int **sister);

struct timeb    *pTimerStart, *pTimerStop;
double          DiffTime = 0, maxtime = 0;

/* A curious idea
typedef struct {
	short venerable;
	short length;
	int quality;
	int* cities;
} solution;
*/

int main(int argc, char* argv[]){
  short millipart;
  int i, j, n, timebound, **matrix, *solution;
  FILE *infile;

  srand(time(NULL));

  pTimerStart = (struct timeb *) malloc(sizeof(struct timeb));
  pTimerStop = (struct timeb *) malloc(sizeof(struct timeb));

  // you should read the input file before starting the timer
  if(argc > 2 || argc < 2) {
    	usage();
    }

    infile = fopen(argv[1], "r");
    if(!infile) {
      fprintf(stderr, "%s didn't open right.\n", argv[1]);
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
  solution = tsp_ga(matrix, n);
  ftime(pTimerStop);

  // call your output function to print either the optimal solution,
  // or the soultion you have.
  millipart = (pTimerStop->millitm - pTimerStart->millitm);
  DiffTime = (pTimerStop->time - pTimerStart->time) + ((float)millipart)/1000;
  printf("File: %s\nLimit: %.2f\nTime: %.2f\n%s\nDistance: %d\nTour: ", argv[1],
      maxtime, DiffTime, (maxtime > DiffTime)?"OPTIMAL":"NOT Optimal", length(matrix, solution, n));
    //printMatrix(matrix, n, "Afterwards:");
  for(i = 0; i < n; i++) {
  	if(solution[i] < 27)
      putchar('a'+(solution[i]-1));
    else
      putchar('A'+(solution[i]-27));
  }
  putchar((solution[i] < 27)?'a'+solution[i]-1:'A'+solution[i]-27);
  putchar('\n');

  return 0;
}

int* tsp_ga(int **matrix, int n) {
  int i, venerable, tempi = 0, j, random;
  int bssf = 0, *gen, divisor = 1<<30 /* need some +inf */, *brother = 0, *sister = 0;
  int *BSSF = 0;
  //int* used = calloc(n, sizeof(int));
  // these mallocs may well not work
  int **temppop;
  int **population = (int **) malloc(POPULATION*sizeof(int *));
  float total = 0, tempf;
  int millipart;

  // initialize random population
  for(i = 0; i < POPULATION; i++) {
		gen = (int *) malloc(n*sizeof(int));
		for(j = 0; j < n; j++) gen[j] = j;
		for(j = 0; j < n; j++) { // now randomize them
			random = range(j, n-1); // pick a random from the rest
			tempi = gen[j];
			gen[j] = gen[random];
		  gen[random] = tempi; // the number at that index is the next number
		}
  	population[i] = gen;
  }



	// inefficient, will fix later                                    <--------  FIX
  for(i = 0; i < POPULATION; i++) {
		if(length(matrix, population[i], n) < divisor) {
			divisor = length(matrix, population[i], n);
			bssf = divisor;
			if(BSSF) free(BSSF);
			BSSF = (int *) malloc(n*sizeof(int));
			for(j = 0; j < n; j++)
				BSSF[j] = population[i][j];
		}
	}
	for(i = 0; i < POPULATION; i++)
		total += divisor/length(matrix, population[i], n);

  /*
  	Selection function idea: Get first bssf number.
  		divide all others buy that, and add to 1.  This is
  		the fitness number.
  		Proceed otherwise as stated.
  */

  while(1) { // while time left (see bottom of loop)
  	temppop = (int **) malloc(POPULATION*sizeof(int *));
    // new generation
    venerable = range(1, n-1); // oldsters to stay

    //pick which old guys stay
		for(i = 0; i < venerable; i++) {
			temppop[i] = (int *) malloc(n*sizeof(int));
			tempf = total * (float) rand() / (float) 0x7fffffff; //atm, allowing duplicates
			for(j = 0; j < POPULATION; j++) {
				tempf-=divisor/length(matrix, population[j], n);
				if(tempf <= 0) tempi = j;
			}
			for(j = 0; j < n; j++)
				temppop[i][j] = population[tempi][j];
		}

    // rest are kids, choose (same way) two to procreate (replaces, change it)
		for(i = venerable; i < POPULATION; i++) {
			tempf = total * (float) rand() / (float) 0x7fffffff; //atm, allowing duplicates
			for(j = 0; j < POPULATION; j++) {
				tempf-=divisor/length(matrix, population[j], n);
				if(tempf <= 0) tempi = j;
			}
			tempf = total * (float) rand() / (float) 0x7fffffff; //atm, allowing duplicates
			for(j = 0; j < POPULATION; j++) {
				tempf-=divisor/length(matrix, population[j], n);
				if(tempf <= 0) break; //use j
			}
			crossover(population[tempi], population[j], n, &brother, &sister);
			temppop[i] = brother;
			if(++i < POPULATION)
				temppop[i] = sister;
		}

    // mutate some random amount of the population
    random = range(1, n-1); // not all, but definatly some
    for(i = 0; i < random; i++) // allow double mutations?  sure, why not?
  		mutate(temppop[range(0, n-1)], n);

    // new bssf?
    for(i = 0; i < POPULATION; i++) {
			if(length(matrix, population[i], n) < bssf) {
				bssf = length(matrix, population[i], n);
				free(BSSF);
				BSSF = (int *) malloc(n*sizeof(int));
				for(j = 0; j < n; j++)
					BSSF[j] = population[i][j];
			}
		}

		//clean up after self
		for(i = 0; i < POPULATION; i++)
			free(population[i]);
		free(population);
		population = temppop;
		temppop = 0; // just in case

    // out of time?
    ftime(pTimerStop);
    millipart = (pTimerStop->millitm - pTimerStart->millitm);
    DiffTime = (pTimerStop->time - pTimerStart->time) + ((float)millipart)/1000;
    if(DiffTime >= maxtime) break;
  } //while

  //free the lot
  for(i = 0; i < POPULATION; i++)
  	free(population[i]);
  free(population);

  return BSSF;
}

int length(int **matrix, int* path, int n) {
	int i, last = path[0], length = matrix[path[n-1]][last];
	for(i = 1; i < n; i++) {
		length += matrix[last][path[i]];
	}
	if(length) return length;
	else return 1; // is divided by, 1 is close enough
}

void mutate(int *path, int n) {
	int start, end, i, temp;
	// reverse order of sub-sequence randomly
	start = range(0, n-1);
	end = range(0, n-1);
	if(start > end) {
		temp = start;
		start = end;
		end = start;
	}

	// swap locations
	for(i = 0; i < (end-start-1); i++) {
		temp = path[start+i];
		path[start+i] = path[end-i];
		path[end-i] = temp;
	}
}

int range(int start, int end) {
	return rand()%(end-start+1)+start;
}

// produces two children, replacing their parents
void crossover(int *father, int *mother, int n, int **bro, int **sis) {
	int i, j;
	int *brother = (int *) malloc(n*sizeof(int));
	int *sister = (int *) malloc(n*sizeof(int));

	brother[0] = father[0];
	sister[0] = mother[0];

	for(i = 1; i < n; i++) {
		switch(i%2) {
			case 0:
				for(j = 0; j < n; j++)
					if(father[j] == brother[i]) break;
				brother[i] = father[++j]; // the next one
				for(j = 0; j < n; j++)
					if(mother[j] == sister[i]) break;
				sister[i] = mother[++j];
				break;
			case 1:
				for(j = 0; j < n; j++)
					if(mother[j] == brother[i]) break;
				brother[i] = mother[++j];
				for(j = 0; j < n; j++)
					if(father[j] == sister[i]) break;
				sister[i] = father[++j];
				break;
		}
	} // for

	/*
	free(father);
	free(mother); // old age.  Sad
	father = brother;
	mother = sister; // they grew up! ;P
	*/
	*bro = brother;
	*sis = sister;
	/*
		While this code may appear to be pro-incest, I (the author) do not
		support nor condone incest in any way shape or form.  Furthermore,
		I (the author still) do not support or endorse any form of deviant
		sexual nor marriage practices.  Thank you for your time.
	*/
}
