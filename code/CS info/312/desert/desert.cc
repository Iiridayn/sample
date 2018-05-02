#include <stdlib.h>
#include <stdio.h>

#include "fcyc.h"

#define MAX_SIGNIFICANT_LOCATIONS  20
#define MAX_FOOD_UNITS             1E6

int capacity;
int locations;
int x[MAX_SIGNIFICANT_LOCATIONS];
int y[MAX_SIGNIFICANT_LOCATIONS];

void 
dump_graph() {
  int* ptr_x = x;
  int* ptr_y = y;
  int* end = ptr_x + locations;

  printf("%d %d\n", locations, capacity);

  while (ptr_x != end) {
    printf("%d %d\n", *ptr_x++, *ptr_y++);
  }
  
  printf("\n");
}

int 
get_input(FILE *fp) {
  int* ptr_x = x;
  int* ptr_y = y;
  int* end = NULL;

  if ( fscanf(fp, "%d %d", &locations, &capacity) != 2 ) {
    printf( "ERROR: bad input file\n" );
    exit(0);
  }

  if (locations == 0 && capacity == 0) {
    return 0;
  }

  end = ptr_x + locations;
  while (ptr_x != end) {
    if ( fscanf(fp, "%d %d", ptr_x++, ptr_y++) != 2 ) {
      printf( "ERROR: bad input file\n" );
      exit(0);
    }
  }

  return 1;
}

FILE * 
process_command_line(int argc, char* argv[]) {
  FILE *fp;
  if (argc == 1) {
    fp = stdin;
  }
  else if (argc == 2){
    if( (fp = fopen(argv[1], "r")) == NULL ) {
      printf("ERROR: failed to oopen %s for reading\n", argv[1]);
      exit(0);
    }
  }
  else {
    printf("usage: desert <filename> or cat <filename> | desert\n");
    exit(0);
  }
  return(fp);
}

void 
report_units(int units, double cycles) {
  static int trial = 1;
  if (units == -1 || units > MAX_FOOD_UNITS) {
    //printf("Trial %d: Impossible\n\n",trial);
    printf("Trial %d: Impossible (%0.f cycles)\n\n", trial, cycles); 
  }
  else {
    //printf("Trial %d: %d units of food\n\n",trial,units);
    printf("Trial %d: %d units of food (%.0f cycles)\n\n", 
	   trial, units, cycles);
  }
  ++trial;
}

extern int 
compute_food_units();

void
call_compute_food_units(int *units) {
  *units = compute_food_units();
}

typedef void (*lab_test_func)(int *);
typedef void (*test_funct_v)(void *);

void 
func_wrapper(void *arglist[]) {
  lab_test_func f;
  int *units;
  
  f = (lab_test_func) arglist[0];
  units = (int *) arglist[1];
  (*f)(units);
  
  return;
}

int 
main(int argc, char* argv[]) {
  FILE *fp = process_command_line(argc, argv);
  int units = 0;
  double num_cycles,cpe;
  double total_cycles = 0.0; 

  set_fcyc_cache_size(1 << 14); /* 16 KB cache size */
  set_fcyc_clear_cache(1);      /* Clear cache before each measurement */
  set_fcyc_compensate(1);       /* Try to compensate for timer overhead */

  while(get_input(fp)) {
    
    void *arglist[2];

#ifdef DEBUG
    dump_graph();
#endif

    arglist[0] = (void *) call_compute_food_units;
    arglist[1] = (void *) &units;

    num_cycles = fcyc_v((test_funct_v)&func_wrapper, arglist); 
    cpe = num_cycles/(double) locations;
total_cycles += num_cycles; 

	
    //    units = compute_food_units();
    report_units(units, num_cycles);
  }

  fclose(fp);
  printf ("%.0f cycles\n", total_cycles); 
  return 0;
}
