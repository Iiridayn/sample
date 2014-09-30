#include <stdio.h>
#include <math.h>
#include <unistd.h>

#define PI 3.14159265

typedef struct {
	double real;
	double imaginary;
} cnumber;

void Plot(double *vals, int numvals, char *title);
cnumber *FFT(int N, cnumber *f, int reverse);
void partD(int *data, int size, int n);
cnumber cmult(cnumber a, cnumber b) {
	cnumber res;
	res.real = a.real*b.real - a.imaginary*b.imaginary;
	res.imaginary = a.imaginary*b.real + a.real*b.imaginary;
	return res;
}
cnumber cdiv(cnumber a, cnumber b) {
	cnumber res;
	res.real = a.real*b.real + a.imaginary*b.imaginary;
	res.imaginary = a.imaginary*b.real - a.real*b.imaginary;
	double div = b.real*b.real + b.imaginary*b.imaginary;
	///*
	if(abs(div) < 0.000000001) { // approx 0
		res.real = 0;
		res.imaginary = 0;
	} else {//*/
		res.real /= div;
		res.imaginary /= div;
	}
	return res;
}

int main(int argc, char* argv[]) {
	char *file1 = "1D_Rect128.dat";
	char *file2 = "1D_Gauss128.dat";
	char *file3 = "1D_Signal.dat";
	char *file4 = "1D_Output128.dat";
	const int size = 128;
	int i, loc = 1;
	double plot[size];
	cnumber *res;
	int idata[size];
	double ddata[size];
	
	cnumber sine[size];
	cnumber cose[size];
	cnumber sum[size];
	for(i = 0; i < size; i++) {
		sine[i].real = sin((2*PI*8*i)/size);
		sine[i].imaginary = 0;
		cose[i].real = cos((2*PI*8*i)/size);
		cose[i].imaginary = 0;
		sum[i].real = sine[i].real + cose[i].real;
		sum[i].imaginary = 0;
		//printf("%f - %d\n", (2*PI*8*i)/size, (2*PI*8*i)/size);
		//printf("%f + %f i\n", sine[i].real, sine[i].imaginary);
	}

	// read file 1
	cnumber data1[size];
	FILE *f1 = fopen(file1, "r");
	for(i = 0; i < size; i++) {
		fscanf(f1, "%lf", &(ddata[i]));
		data1[i].real = ddata[i];
		data1[i].imaginary = 0;
		//printf("File1  read: %f\n", data1[i].real);
	}
	fclose(f1);

	// read file 2
	cnumber data2[size];
	FILE *f2 = fopen(file2, "r");
	for(i = 0; i < size; i++) {
		fscanf(f2, "%lf", &(ddata[i]));
		data2[i].real = ddata[i];
		data2[i].imaginary = 0;
	}
	fclose(f2);

	// read file 3
	int data3[size];
	FILE *f3 = fopen(file3, "r");
	for(i = 0; i < size; i++) {
		fscanf(f3, "%d", &(idata[i]));
		data3[i] = idata[i];
	}
	fclose(f3);
	
	// read file 4
	cnumber data4[size];
	FILE *f4 = fopen(file4, "r");
	for(i = 0; i < size; i++) {
		fscanf(f4, "%lf", &(ddata[i]));
		data4[i].real = ddata[i];
		data4[i].imaginary = 0;
	}
	fclose(f4);

	while(loc < argc) {
	switch(argv[loc][0]) {
	case 'A':
	// Part A:

	if(argv[loc][1] == '\0' || argv[loc][1] == '1') {	
	// Part F1
	res = (cnumber *) malloc(size*sizeof(cnumber));
	for(i = 0; i < size; i++)
		res[i] = sine[i];
	FFT(size, res, 0);
	for(i = 0; i < size; i++)
		plot[i] = sine[i].real;
	Plot(plot, size, "Sine");
	for(i = 0; i < size; i++)
		plot[i] = res[i].real;
	Plot(plot, size, "FFT Sine Real Part");
	for(i = 0; i < size; i++)
		plot[i] = res[i].imaginary;
	Plot(plot, size, "FFT Sine Imaginary Part");
	for(i = 0; i < size; i++)
		plot[i] = hypot(res[i].real, res[i].imaginary);
	Plot(plot, size, "FFT Sine Magnitude");
	for(i = 0; i < size; i++)
		plot[i] = atan(res[i].imaginary/res[i].real);
	Plot(plot, size, "FFT Sine Phase");
	free(res);
	}

	if(argv[loc][1] == '\0' || argv[loc][1] == '2') {
	// Part 2
	res = (cnumber *) malloc(size*sizeof(cnumber));
	for(i = 0; i < size; i++)
		res[i] = cose[i];
	FFT(size, res, 0);
	for(i = 0; i < size; i++)
		plot[i] = cose[i].real;
	Plot(plot, size, "Cosine");
	for(i = 0; i < size; i++)
		plot[i] = res[i].real;
	Plot(plot, size, "FFT Cosine Real Part");
	for(i = 0; i < size; i++)
		plot[i] = res[i].imaginary;
	Plot(plot, size, "FFT Cosine Imaginary Part");
	for(i = 0; i < size; i++)
		plot[i] = hypot(res[i].real, res[i].imaginary);
	Plot(plot, size, "FFT Cosine Magnitude");
	for(i = 0; i < size; i++)
		plot[i] = atan(res[i].imaginary/res[i].real);
	Plot(plot, size, "FFT Cosine Phase");
	free(res);
	}
	
	if(argv[loc][1] == '\0' || argv[loc][1] == '3') {
	// Part 3
	res = (cnumber *) malloc(size*sizeof(cnumber));
	for(i = 0; i < size; i++)
		res[i] = sum[i];
	FFT(size, res, 0);
	for(i = 0; i < size; i++)
		plot[i] = sum[i].real;
	Plot(plot, size, "Sum");
	for(i = 0; i < size; i++)
		plot[i] = res[i].real;
	Plot(plot, size, "FFT Sum Real Part");
	for(i = 0; i < size; i++)
		plot[i] = res[i].imaginary;
	Plot(plot, size, "FFT Sum Imaginary Part");
	for(i = 0; i < size; i++)
		plot[i] = hypot(res[i].real, res[i].imaginary);
	Plot(plot, size, "FFT Sum Magnitude");
	for(i = 0; i < size; i++)
		plot[i] = atan(res[i].imaginary/res[i].real);
	Plot(plot, size, "FFT Sum Phase");
	free(res);
	}

	if(argv[loc][1] == '\0' || argv[loc][1] == '4') {
	// Part 4
	// mess with weightings
	}
	break;
	
	case 'B':
	// Part B:
	res = (cnumber *) malloc(size*sizeof(cnumber));
	for(i = 0; i < size; i++)
		res[i] = data1[i];
	FFT(size, res, 0);
	/*
	for(i = 0; i < size; i++)
		plot[i] = data1[i].real;
	Plot(plot, size, "Rectangular Pulse");
	*/
	for(i = 0; i < size; i++)
		plot[i] = hypot(res[i].real, res[i].imaginary);
	Plot(plot, size, "Rectangular Pulse Magnitude");
	for(i = 0; i < size; i++)
		plot[i] = (res[i].real*res[i].real)+(res[i].imaginary*res[i].imaginary);
	Plot(plot, size, "Rectangular Pulse Power Spectrum");
	free(res);
	break;
	
	case 'C':
	// Part C:
	res = (cnumber *) malloc(size*sizeof(cnumber));
	for(i = 0; i < size; i++)
		res[i] = data2[i];
	FFT(size, res, 0);
	for(i = 0; i < size; i++)
		plot[i] = hypot(res[i].real, res[i].imaginary);
	Plot(plot, size, "Gaussian Magnitude");
	for(i = 0; i < size; i++)
		plot[i] = (res[i].real*res[i].real)+(res[i].imaginary*res[i].imaginary);
	Plot(plot, size, "Gaussian Power Spectrum");
	free(res);
	break;

	case 'D':
	// Part D: works for sure
	for(i = 0; i < size; i++)
		idata[i] = data3[i];
	partD(idata, size, 3);
	break;

	case 'E':
	// Part E: //out/in
	FFT(size, data1, 0); // in
	FFT(size, data4, 0); // out
	cnumber data5[size];
	for(i = 0; i < size; i++) {
		data5[i] = cdiv(data4[i], data1[i]);
		plot[i] = hypot(data5[i].real, data5[i].imaginary);
	}
	Plot(plot, size, "H(u) Magnitude");
	FFT(size, data5, 1);
	for(i = 0; i < size; i++)
		plot[i] = hypot(data5[i].real, data5[i].imaginary);
	Plot(plot, size, "h(u) Magnitude");
	break;
	
	default: printf("Error: Invalid arguments\n"); exit(1);
	} loc++;}
}

void partD(int *data, int size, int n) {
	int i,j,smallindex;
	double smallest,temp;
	
	cnumber *tmp = (cnumber *)malloc(size*sizeof(cnumber));
	for(i = 0; i < size; i++) {
		tmp[i].real = data[i];
		tmp[i].imaginary = 0;
	}
	FFT(size, tmp, 0);
	
	for(i = 1; i < size; i++) { // selection sort
		smallest = tmp[i].real;
		smallindex = i;
		for(j = i+1; j < size; j++)
			if(tmp[j].real < smallest) {
				smallest = tmp[j].real;
				smallindex = j;
			}
		temp = tmp[i].real;
		tmp[i].real = smallest;
		tmp[smallindex].real = temp;
		//printf("%f\t", tmp[i].real);
	}
	
	for(i = 0; i < n; i++)
		printf("The #%d in size is: %f\n", i+1, tmp[size-i-1].real);
}

void Plot(double *vals, int numvals, char *title) {
	char buf[1024];
	int i;

	#ifdef NOPLOT
	return;
	#endif

	FILE *outfile = fopen("gnuplot.data", "wb");
	FILE *tmpfile = fopen(title, "w");
	if(!outfile) fprintf(stderr, "Couldn't write plot data.\n");
	
	for(i = 0; i < numvals; i++) {
		fprintf(outfile, "%f\n", vals[i]);
		fprintf(tmpfile, "%f\n", vals[i]);
	}
	fclose(outfile);
	
	outfile = fopen("gnuplot.command", "wb");
	if(!outfile) fprintf(stderr, "Couldn't write command data.\n");

	fprintf(outfile, "set title \"%s\"\n", title);
	fprintf(outfile, "set xrange [0:%d]\n", numvals-1);
	fprintf(outfile, "set xtics 16\n");
	fprintf(outfile, "plot \"%s\" with lines\n", "gnuplot.data");
	fclose(outfile);

	snprintf(buf, 1024, "/usr/bin/gnuplot -persist \"%s\"", "gnuplot.command");

	system(buf);
	
	sleep(1);

	unlink("gnuplot.data");
	unlink("gnuplot.command");
}

cnumber *FFT(int N, cnumber *f, int reverse) {
	int i;
	int halfN = N/2;
	if(N == 2) {
		//printf("in:f[0] = %f + %f i\nin:f[1] = %f + %f i\n", f[0].real, f[0].imaginary, f[1].real, f[1].imaginary);
		cnumber temp = f[0];
		f[0].real = f[0].real+f[1].real;
		f[0].imaginary = f[0].imaginary+f[1].imaginary;
		f[1].real = temp.real-f[1].real;
		f[1].imaginary = temp.imaginary-f[1].imaginary;
		//printf("2:f[0] = %f + %f i\n2:f[1] = %f + %f i\n", f[0].real, f[0].imaginary, f[1].real, f[1].imaginary);
	} else {
		cnumber *g = (cnumber *) malloc(halfN*sizeof(cnumber));
		cnumber *h = (cnumber *) malloc(halfN*sizeof(cnumber));
		for(i = 0; i < N; i++)
			if(i&1) {
				//printf("h[%d] = %f + %f i\n", i/2, f[i].real, f[i].imaginary);
				h[i/2].real = f[i].real;
				h[i/2].imaginary = f[i].imaginary;
			} else {
				//printf("g[%d] = %f + %f i\n", i/2, f[i].real, f[i].imaginary);
				g[i/2].real = f[i].real;
				g[i/2].imaginary = f[i].imaginary;
			}
		FFT(halfN,g,reverse);
		FFT(halfN,h,reverse);
		for(i = 0; i < N; i++) {
			cnumber tmp;
			tmp.real = cos((2*PI*i)/N);
			tmp.imaginary = -sin((2*PI*i)/N);
			tmp = cmult(tmp, h[i%halfN]);
			tmp.real += g[i%halfN].real;
			if(!reverse)
				tmp.imaginary += g[i%halfN].imaginary;
			else
				tmp.imaginary = g[i%halfN].imaginary - tmp.imaginary;
			f[i] = tmp;
			//printf("%d:f[%d] = %f + %f i\n", N, i, f[i].real, f[i].imaginary);
			/*
			//f[i] = g[i%halfN]+exp((-j*2*PI*i)/N)*h[i%halfN];
			f[i].real = g[i%halfN].real + cos((2*PI*i)/N)*h[i%halfN].real;
			if(reverse)
				f[i].imaginary = g[i%halfN].imaginary + sin((2*PI*i)/N)*h[i%halfN].imaginary;
			else
				f[i].imaginary = g[i%halfN].imaginary - sin((2*PI*i)/N)*h[i%halfN].imaginary;
			*/
		}
		free(g);
		free(h);
	}
	return f;
}
