#include <iostream>
using namespace std;


/* {~, &, ^, |, !, &&, ||} */
void testFunction(int x) {
	int calc = 0;
	
	/* A */
	calc = (x & -1)&&1; //-1 is all ones, for any word size.  Really, I could
	// just have sent it straight out, but I wanted to show _something_
	//cout << "Any bit of " << x << " is 1 is: " << (calc)?"True":"False" << endl;
	printf("%s0x%08X%s%s\n","Any bit of ", x, " is 1 is: ", (calc)?"True":"False");
	
	/* B */
	calc = (x ^ -1)&&1; //If is a 0, will go high, if 1, will go low.  So, if there
	// are any ones, it will be non-zero, and thus true.
	//cout << "Any bit of " << x << " is 0 is: " << (calc)?"True":"False" << endl;
	printf("%s0x%08X%s%s\n","Any bit of ", x, " is 0 is: ", (calc)?"True":"False");
	
	/* C */
	calc = (x & 0xFF)&&1; //Bit mask the lower byte (or 8 bits).
	//cout << "Any bit in the LSB of " << x << " is 1 is: " << (calc)?"True":"False" << endl;
	printf("%s0x%08X%s%s\n","Any bit in the LSB of ", x, " is 1 is: ", (calc)?"True":"False");
	
	/* D */
	calc = ((x | ~0xFF) ^ -1)&&1; //force all but the lower byte (inverted mask) 1,
	// then check if any are 0 (xor 1, is only a 1 anywhere if there was a 0)
	//cout << "Any bit in the LSB of " << x << " is 0 is: " << (calc)?"True":"False" << endl;
	printf("%s0x%08X%s%s\n","Any bit in the LSB of ", x, " is 0 is: ", (calc)?"True":"False");
}

int main() {
	testFunction(-1);
	testFunction(0);
	testFunction(255);
	testFunction(0xFFFFFF00);
	testFunction(0xF0F0F0F0);
}
