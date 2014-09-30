#include <stdio.h>
#include <stdlib.h>
#define BTMSK 34059

// Takes a number of integers from the user
// Preforms a random operation on them all
// (from the set {+,-,*,/}) with the previous
// result.  Outputs the result.

int main(int argc, char* argv[]) {
	srand(time(0)); // initialize randomness
	int result = 0;
	int i;
	for(i = 1; i < argc; i++) {
		int integer = atoi(argv[i]);
		int random = (rand()%4);
		switch (random) {
			   	case 0:
			   		result += integer;
					break;
				case 1:
					 result -= integer;
					 break;
				case 2:
					 result *= integer;
					 break;
				case 3:
					 result /= integer;
					 break;
		}
	}
	
    printf("Hello world\n"); // why not keep it...
	printf("The result of random operations on your data is: %d \n", result);
	printf("Your lucky numbers for today are: ");
	for(i = 0; i < 5; i++) {
		printf("%d ", (rand()%100));
	}
	printf("\nThank you for using our program and have a nice day.\n");
	
	// /* Obscurified code section
	printf("\nEnter a number: ");
	int usrinpt = 0;
	scanf("%d",&usrinpt);
	int rndm = rand();
	int tmpnmbr = ++usrinpt;
	tmpnmbr <<= 5, usrinpt &= (rndm & BTMSK | 1), tmpnmbr-= (usrinpt && 1);
	printf("%x %d %c", tmpnmbr, usrinpt, (-1 & 10));  
	// End Obscurified code section */
    exit(0); // why not "return 0;"?
}
