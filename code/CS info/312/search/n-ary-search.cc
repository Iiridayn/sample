// SearchProj.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "fcyc.h"

#define MAXNUMBERS 1000000
#define TRIALS 10000

typedef void (*test_funct_v)(void *);

extern void binary_search(int find, int count, int* numbers, int* result);
extern void trinary_search(int find, int count, int* numbers, int* result);

FILE* process_command_line(int argc, char* argv[]) {
	FILE* fp;
	if (argc == 2) {
		fp = NULL;
		return fp;
	} else if (argc == 4) {
		if ((fp = fopen(argv[2], "r")) == NULL) {
			printf("ERROR: file %s did not open!\n", argv[1]);
			exit(-1);
		}
		return fp;
	} else {
		printf("usage: n-ary-search -g\nor\n       n-ary-search -t <filename> <number>\n");
		exit(0);
	}

	return fp;
}

int stringToInt(char *arg) {
	int i = 0;
	int j;

	for (j = 0; j < (int)strlen(arg); j++) {
		i *= 10;
		i = i + (int)(arg[j]) - (int)('0');
	}
	return i;
}

int get_input(FILE *fp) {
	int input = -1;

	if (fscanf(fp, "%d", &input) != 1) {
		printf("ERROR: bad input file\n");
		exit(-1);
	}

	return input;
}

void get_list(FILE *fp, int count, int* numbers) {
	int i;

	for (i = 0; i < count; i++) {
		numbers[i] = get_input(fp);
	}
}

void buildlist(int *numbers) {
	int i;
	for (i = 0; i < MAXNUMBERS; i++) {
		numbers[i] = i+1;
	}
}

void printResults_g(double cycles, char *method) {
	int i;

	printf("%s:\n", method);
	for (i = 0; i < (int)strlen(method); i++) {
		printf("-");
	}
	printf("\nTotal Cycles:\t%d\nTotal Searches:\t%d\nAvg Cycles per Search\t%f\n\n",
		(int)cycles, (int)TRIALS, (float)(cycles)/(float)(TRIALS));
}

void printResults_t(int find, int location, char *method) {
	int i;

	printf("%s:\n", method);
	for (i = 0; i < (int)strlen(method); i++) {
		printf("-");
	}
	if (location >= 0) {
		printf("\nFound %d at location %d\n\n", find, location);
	} else {
		printf("\nDid not find %d\n\n", find);
	}
}

typedef void (*lab_test_func)(int, int, int *, int *);
typedef void (*test_funct_v)(void *);

void func_wrapper(void *arglist[]) {
	lab_test_func f = (lab_test_func)arglist[0];
	int *p1, *p2, *p3, *p4;

	p1 = (int *)arglist[1];
	p2 = (int *)arglist[2];
	p3 = (int *)arglist[3];
	p4 = (int *)arglist[4];

	(*f)(*p1, *p2, p3, p4);

	return;
}

int main(int argc, char* argv[]) {

	int numbers[MAXNUMBERS];
	FILE *fp = process_command_line(argc, argv);
	int count;
	int find;
	int location;
	int trialcount;
	int i;

	double num_cycles;
	double bin_total_cycles = 0.0;
	double tri_total_cycles = 0.0;

	set_fcyc_cache_size(1 << 14); /* 16 KB cache size */
	set_fcyc_clear_cache(1);      /* Clear cache before each measurement */
	set_fcyc_compensate(1);       /* Try to compensate for timer overhead */

	srand(1);

	if (fp != NULL) {
		trialcount = 1;
		count = get_input(fp);
		find = stringToInt(argv[3]);
		get_list(fp, count, numbers);
	} else {
		count = MAXNUMBERS;
		buildlist(numbers);
		trialcount = TRIALS;
	}

	for (i = 0; i < trialcount; i++) {

		if (fp == NULL) {
			find = (int)(1.1*MAXNUMBERS*drand48());
		}

		void *arglist[5];

		arglist[0] = (void *)binary_search;
		arglist[1] = (void *)&find;
		arglist[2] = (void *)&count;
		arglist[3] = (void *)numbers;
		arglist[4] = (void *)&location;

		num_cycles = fcyc_v((test_funct_v)&func_wrapper, arglist);
		bin_total_cycles += num_cycles;

		if (fp != NULL) {
			printResults_t(find, location, "Binary Search");
		}

		arglist[0] = (void *)trinary_search;

		num_cycles = fcyc_v((test_funct_v)&func_wrapper, arglist);
		tri_total_cycles += num_cycles;

		if (fp != NULL) {
			printResults_t(find, location, "Trinary Search");
		}
	}

	if (fp == NULL) {
		printResults_g(bin_total_cycles, "Binary Search");
		printResults_g(tri_total_cycles, "Trinary Search");
	} 

	return 0;
}

