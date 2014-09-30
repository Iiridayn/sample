#include <iostream>
using namespace std;

void bPattern(int j, int k) {
	int pattern = 0;
	
	/* A */
	pattern = (-1 << k);
	// Is 1^(w-k)0^k, or k zeros on the end.
	printf("A(%d)   0x%08X\n", k, pattern);
	
	/* B */
	pattern = ( ((unsigned)-1 >> ((8*sizeof(unsigned))-k)) << j); 
	// Is 0^(w-(j+k))1^k0^j       ^^^ the 8 is so is bits, not bytes
	// It shifts far enough to kill all but k 1's.  ( >> (w-k))
	// It shifts back for the j 0's on the end (<< j)
	printf("B(%d,%d) 0x%08X\n", j, k, pattern);
}

int main() {
	bPattern(8,8);
	bPattern(24,8);
	bPattern(16,4);
	bPattern(4,20);
}

