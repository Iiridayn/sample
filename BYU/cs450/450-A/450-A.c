#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUF 100

void usage() {
	printf("USAGE: 450-A -{c,d} filename\n");
	printf("Where c is compress and d is decompress\n");
	exit(1);
}

unsigned char* readpnm(char *filename);
void writepnm(char *filename, unsigned char* data);
unsigned char* readcmp(char *filename);
void writecmp(char *filename, unsigned char* data);
void Plot(int *vals, int numvals, char *title);
void plot(unsigned char *vals, int numvals, char *title);

int width, height;

int main(int argc, char *argv[]) {
	int i, j;
	char* filename;
	unsigned char *data;
	unsigned char *data2;
	
	if(argc != 3)
		usage();
		
	unsigned char compress;
	if(argv[1][0] == '-')
		if(argv[1][1] == 'c' || argv[1][1] == 'd')
			compress = (argv[1][1] == 'c');
		else
			usage();
	else
		usage();
	
	filename = (char *) calloc(strlen(argv[2])+4, sizeof(char));
	strcpy(filename, argv[2]);
	// assume valid input
	
	if(compress) {
		data = readpnm(filename);
		data2 = (unsigned char*) calloc(width*height, sizeof(unsigned char));
		plot(data, width*height, "Histogram of normal file");
	}	else {
		data2 = readcmp(filename);
		data = (unsigned char*) calloc(width*height, sizeof(unsigned char));
	}
	
	//plot((compress)?data:data2, width*height, (compress)?"Histogram of normal file":"Histogram of compressed file");
		
	for(i = 0; i < height; i++)
		for(j = 0; j < width; j++) {
			int predicted = 0;
			int divisor = 0;
			
			#ifdef SHAPE
			printf("\n");
			#endif
	 		if(i != 0) {
				if(j != 0) {
					predicted += data[(i-1)*width+(j-1)]; // NW
					divisor++;
					#ifdef SHAPE
					printf("%d ", data[(i-1)*width+(j-1)]);
					#endif
				}
				
				predicted += data[(i-1)*width+j]; //above (N)
				divisor++;
				#ifdef SHAPE
				printf("%d ", data[(i-1)*width+j]);
				#endif
				
				if((j+1) < width) {
					predicted += data[(i-1)*width+(j+1)]; // NE
					divisor++;
					#ifdef SHAPE
					printf("%d", data[(i-1)*width+(j+1)]);
					#endif
				}
				#ifdef SHAPE
				printf("\n");
				#endif
			}
			if(j != 0) {
				predicted += data[i*width+(j-1)]; // W
				divisor++;
				#ifdef SHAPE
				printf("%d ", data[i*width+(j-1)]);
				#endif
			}
			#ifdef SHAPE
			printf("x\n");
			#endif
			
			if(divisor > 1)
				predicted /= divisor;
				
			if(compress) {
				//printf("predicted: %d, actual: %d, correction: %d\n", predicted, data[i+width*j], -(predicted-data[i+width*j]));
				//if(j == 10) exit(0);
				predicted -= data[i*width+j];
				data2[i*width+j] = -predicted; // so the first at least is +
			} else {
				//printf("predicted: %d, correction: %d, actual: %d\n", predicted, data2[i+width*j], predicted+data2[i+width*j]);
				//if(j == 10) exit(0);
				predicted += (char) data2[i*width+j];
				data[i*width+j] = predicted;
			}
	}
	if(compress) {
		writecmp(filename, data2);
		printf("Compressed file %s\n", filename);
		plot(data2, width*height, "Histogram of compressed file");
	} else {
		writepnm(filename, data);
		printf("Decompressed file %s\n", filename);
	}
	
		//plot((!compress)?data:data2, width*height, (!compress)?"Histogram of normal file":"Histogram of compressed file");
}

unsigned char* readpnm(char *filename) {
  char str[400];
  char tmp;
  char type[400];
  unsigned char *data;
	int i;
  
  FILE* infile = fopen(filename, "r");

  /* read header */
  fgets(str, 400, infile);
  strcpy(type, str);
	//printf("read: %s\n", str);

  /* skip comments */
  tmp = '#';
  while (tmp == '#'){
    fgets(str, 400, infile);
		//printf("read: %s\n", str);
    tmp = str[0];
  } /* end while */
  
  /* read the width and height */
  sscanf(str, "%d %d", &width, &height);
	//printf("read: %d, %d\n", width, height);
  
  /* get the max value in the image data */
  fgets(str, 400, infile); // don't want it, just clear it
  //is.getline(str, 400);
  //istr.seekg(0);
  //istr >> pnm.max_val;
  
  if ( strcmp(type, "P5") ) {
    data = (unsigned char*) calloc(width*height, sizeof(unsigned char));
		//printf("data assigned %x\n", data);
		
		//fread(data, sizeof(unsigned char), width*height, infile);

		//printf("width: %d, height: %d, width*height: %d\n", width, height, width*height);
		for(i = 0; i < width*height; i++) {
			data[i] = (unsigned char) fgetc(infile);
			//printf("read in: %d\n", data[i]);
		}
  } else {
		printf("Not Supported yet\n");
  }
	
  fclose(infile);
  
  
  //printf("Returning data array, addr: %x\n", data);
  return data;
}

void writepnm(char *filename, unsigned char* data) {
	FILE* outfile = fopen(strcat(filename, ".dcp"), "w");
	
	fprintf(outfile, "P5\n%d %d\n%d\n", width, height, 255);
	
	fwrite(data, sizeof(unsigned char), width*height, outfile);
	
	fclose(outfile);
}

unsigned char* readcmp(char *filename) {
	unsigned char *data;
	FILE* infile = fopen(filename, "r");
	int i;
	
	fscanf(infile, "%d %d\n", &width, &height);
	
	data = (unsigned char*) calloc(width*height, sizeof(unsigned char));
	//fread(data, sizeof(unsigned char), width*height, infile);
	for(i = 0; i < width*height; i++)
		data[i] = (unsigned char) fgetc(infile);
		
	fclose(infile);
	
	return data;
}

void writecmp(char *filename, unsigned char* data) {
  FILE* outfile = fopen(strcat(filename, ".cmp"), "w");

  fprintf(outfile,"%d %d\n", width, height);
  
  fwrite(data, sizeof(unsigned char), width*height, outfile);
	
  fclose(outfile);
}

void plot(unsigned char *vals, int numvals, char *title) {
	int *hist = (int *) calloc(256, sizeof(int));
	int i;
	
	for(i = 0; i < numvals; i++)
		hist[vals[i]]++;
		
	Plot(hist, 256, title);
}

void Plot(int *vals, int numvals, char *title) {
	char buf[1024];
	int i;

	#ifdef NOPLOT
	return;
	#endif

	FILE *outfile = fopen("gnuplot.data", "wb");
	#ifdef PLOTFILE
	FILE *tmpfile = fopen(title, "w");
	#endif
	if(!outfile) fprintf(stderr, "Couldn't write plot data.\n");
	
	for(i = 0; i < numvals; i++) {
		fprintf(outfile, "%d\n", vals[i]);
		#ifdef PLOTFILE
		fprintf(tmpfile, "%d\n", vals[i]);
		#endif
	}
	fclose(outfile);
	#ifdef PLOTFILE
	fclose(tmpfile);
	#endif
	
	outfile = fopen("gnuplot.command", "wb");
	if(!outfile) fprintf(stderr, "Couldn't write command data.\n");

	fprintf(outfile, "set title \"%s\"\n", title);
	fprintf(outfile, "set xrange [0:%d]\n", numvals-1);
	fprintf(outfile, "set xtics 16\n");
	fprintf(outfile, "plot \"%s\" with boxes\n", "gnuplot.data");
	fclose(outfile);

	snprintf(buf, 1024, "/usr/bin/gnuplot -persist \"%s\"", "gnuplot.command");

	system(buf);
	
	sleep(1);

	unlink("gnuplot.data");
	unlink("gnuplot.command");
}
