#include <iostream>
using namespace std;

bool is_little_endian() {
	short endian = 0x55AA;
	unsigned char* front = (unsigned char*)&endian;
	return (*(front) == 0xAA);
}

int main() {
	if(is_little_endian())
		cout << "This is a little endian machine." << endl;
	else
		cout << "This is a big endian machine." << endl;
}

