#include "pnm.h"
#include <iostream>
#include <string>

void usage() {
	cout << "USAGE: 450-A -{c,d} filename" << endl;
	cout << "Where c is compress and d is decompress" << endl;
}

int main(int argv, char *argv[]) {
	if(argc != 3)
		usage();
		
	bool compress;
	if(argv[1][0] == '-')
		if(argv[1][1] == 'c' || argv[1][1] == 'd')
			compress = (argv[1] == 'c');
		else
			usage();
	else
		usage();
		
	string filename(argv[2]);
	// assume valid input
	
	if(compress) {
		Pnm image = new Pnm(filename.c_str());
		for(int i = 0; i < image.width; i++)
			for(int j = 0; j < image.height; j++) {
				int predicted = 0;
				int divisor = 0;
		 		if(i != 0) {
					predicted += image.data[(i-1)+image.width*j];
					divisor++;
					if(j != 0) {
						predicted += image.data[(i-1)+image.width*(j-1)];
						divisor++;
					}
				}
				if(j != 0) {
					predicted += image.data[i+image.width*(j-1)];
					divisor++;
					if((i+1) < image.width) {
						predicted += image.data[(i+1)+image.width*(j-1)];
						divisor++;
					}
				}
				
				predicted /= divisor;
				predicted -= image.data[i+image.width*j];
				data[i+image.width*j] = -predicted; // so the first at least is +
		}
		ofstream file(filename.append(.c), ios::out);
		file << image;
		cout << "Compressed file " << filename << endl;
	} else {
		//decompress
	}
}
