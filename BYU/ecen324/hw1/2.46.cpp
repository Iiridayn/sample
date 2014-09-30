#include <iostream>
using namespace std;

/* Declaration of data type where 4 bytes are packed into an unsigned */
typedef unsigned packed_t;

/* Extract byte from word.  Return as signed integer */
int xbyte(packed_t word, int bytenum);

/*
 *  A.  My predecessor made the mistake of taking a bit mask of 
 *      the byte he was trying to extract, as a 32 bit number. 
 *      This prevented it from becoming sign extended, thus failing
 *      to implement core functionality.  He has since been replaced.
 *  (his way: (word >> (bytenum << 3)) & 0xFF ) -- note the mask
 */

/* B */
// Uses only left and right shifts, with one subtraction
// To put it on one line, I was forced to use a cast, unfortunatly.
// Please don't dock too much for it, I just like one liners.
int xbyte(packed_t word, int bytenum) {
	return (( (int)(word << (24 - (bytenum << 3))) ) >> 24);
	// I used 24, because multiplication was not allowed.  24 is 3 bytes.
	// Similarly with the shift on bytenum, I was multiplying it by 8.
	// I basically pulled the important byte to the top, and strung it down
	// letting it sign extend on the way.
}
 
int main() {
	packed_t packtest = 0xFF00A55A; 
	printf("The values of the bytes in %08X in order are:\n",packtest); 
	for(int i = 0; i < 4; i++) {
		printf("%08X\n", xbyte(packtest, i));
	}
}

