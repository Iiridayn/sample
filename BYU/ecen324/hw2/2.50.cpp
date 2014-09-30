#include <iostream>
using namespace std;

void kMult(int x) {
	int result = 0;
	int k = 0;
	
	/* A */
	k = 5;
	result = ( ( x << 2 ) + x );
	printf("%d*%d=%d\n",x,k,result);
	// x*2^2 + x = 4x + x = 5x 
	
	/* B */
	k = 9;
	result = ( (x << 3) + x);
	printf("%d*%d=%d\n",x,k,result);
	// x*2^3 + x = 8x + x = 9x
	
	/* C */
	k = 14;
	result = ( (x << 4) - (x << 1) );
	printf("%d*%d=%d\n",x,k,result);
	// x*2^4 - x*2^1 = 16x - 2x = 14x
	
	/* D */
	k = -56;
	result = ( (x << 3) - (x << 6) );
	printf("%d*%d=%d\n",x,k,result);
	// x*2^3 - x*2^6 = 8x - 64x = -56x
}

int main() {
	kMult(3);
	kMult(7);
	kMult(10);
	kMult(13);
}

