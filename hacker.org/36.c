#include <stdio.h>

union foo {
	int i;
	float f;
};

int main() {
	union foo u;
	u.i = 0b1000001000010001010001011000000;
	printf("%f\n", u.f);
}
