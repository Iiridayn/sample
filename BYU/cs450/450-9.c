#include <stdio.h>
#include <math.h>

void Plot(double *vals, int numvals, char *title);
void fft(int m,double *x,double *y,int dir);

int main() {
	const char *filename1 = "v1.ub";
	const char *filename2 = "v2.ub";
	const char *filename3 = "v3.ub";
	const int file1s = 27012;
	const int file2s = 37382;
	const int file3s = 96086;
	
	const int file1p = 32768;
	const int file2p = 65536;
	const int file3p = 131072;
	
	const int file1e = 15;
	const int file2e = 16;
	const int file3e = 17;
	
	double *fr;
	double *fi;
	double *plot;
	double *butterworth;
	
	double *tr;
	double *ti;
	
	FILE* openfile;
	
	int i;
	
	int highest = 0;
	double rate = 0;

	double n = 1;
	int uc = 0;
	
	char buf[100];
	
	//goto lbl2;
	
	// 1: no idea whatsoever; says "holy schnikies" or somesuch though
	
	// 2:
	openfile = fopen(filename1, "r");
	fr = (double *) calloc(file1p, sizeof(double));
	fi = (double *) calloc(file1p, sizeof(double));
	for(i = 0; i < file1s; i++) {
		fr[i] = (getc(openfile)-128);
	}
	close(openfile);
	
	fft(file1e, fr, fi, 0);
	
	for(i = 1; i < file1s; i++) // non-0 mean index?
		if((fr[i] > highest && fr[i] != 0) || highest == 0)
			highest = fr[i];
	
	printf("Highest non-zero value of V1: %lf Hz\n", highest/2.45);
	
	//"compute sampling rate"...  how? - 2.45 seconds long - samples/seconds
	rate = file1s/2.45;
	printf("Sampling rate is %lf, expect 11025...\n", rate);
	
	// power spectrum
	plot = (double *) calloc(file1s, sizeof(double));
	for(i = 0; i < file1s; i++)
		plot[i] = fr[i]*fr[i] + fi[i]*fi[i];
	Plot(plot, file1s, "V1 Power Spectrum");
	free(plot);
	
	// plot .5 secs of vowel?  how?
	plot = (double *) calloc(5512,sizeof(double));
	openfile = fopen(filename1, "r");
	for(i = 0; i < file1s; i++) {
		char tmp = getc(openfile)-128;
		if(i >= 2205 && i < 7717)
			plot[i-2205] = tmp;
	}
	close(openfile);
	Plot(plot, 5512, ".5 secs of vowel: \\\"O\\\" in V1");
	free(plot);
	
	// 3: uc=30, n = .25
	//uc = highest;
	uc = 30;
	n = .25;
		
	//while(1) {
	
	tr = (double *) calloc(file1p, sizeof(double));
	ti = (double *) calloc(file1p, sizeof(double));
	for(i = 0; i < file1p; i++) {
		tr[i] = fr[i];
		ti[i] = fi[i];
	}
	
	// butterworth
	butterworth = (double *) calloc(file1p, sizeof(double));
	for(i = 0; i < file1p; i++) {
		butterworth[i] = (1/(1+pow(( ((double) (i*i)) / (uc * uc) ), n)));
		tr[i] = tr[i]*butterworth[i];
		ti[i] = ti[i]*butterworth[i];
	}
	sprintf(buf, "Butterworth, n = %lf, uc = %d, s = %d", n, uc, file1s);
	Plot(butterworth, file1s, buf);
	
	// power spectrum
	plot = (double *) calloc(file1s, sizeof(double));
	for(i = 0; i < file1s; i++)
		plot[i] = tr[i]*tr[i] + ti[i]*ti[i];
	Plot(plot, file1s, "V1 Smoothed Power Spectrum");
	free(plot);
	
	fft(file1e, tr, ti, 1);
	
	openfile = fopen("tmp.ub", "w");
	for(i = 0; i < file1s; i++)
		putc(tr[i]+128, openfile);
	close(openfile);

	free(tr);
	free(ti);
	
	//test
	#ifndef NOPLAY
	printf("Playing file, uc=%d\n", uc);
	system("play tmp.ub -r 11025 -d /dev/dsp");
	unlink("tmp.ub");
	usleep(500000); //half a second between clips
	//sleep(3);
	#endif
	
	//uc-=5;
	//}
	
	free(fr);
	free(fi);
	free(butterworth);
	
	//return;
	
	

lbl2:
	// Part 4:
	// 1: no idea whatsoever; says "and I live in a van down by the river"
	
	// 2:
	openfile = fopen(filename2, "r");
	fr = (double *) calloc(file2p, sizeof(double));
	fi = (double *) calloc(file2p, sizeof(double));
	for(i = 0; i < file2s; i++) {
		fr[i] = (getc(openfile)-128);
	}
	close(openfile);
	
	fft(file2e, fr, fi, 0);
	
	for(i = 1; i < file2s; i++) // non-0 mean index?
		if((fr[i] > highest && fr[i] != 0) || highest == 0)
			highest = fr[i];
	
	printf("Highest non-zero value of V2: %lf Hz\n", highest/3.39057);
	
	//"compute sampling rate"...  3.39057 seconds long - samples/seconds
	rate = file2s/3.39057;
	printf("Sampling rate is %lf, expect 11025...\n", rate);
	
	// power spectrum
	plot = (double *) calloc(file2s, sizeof(double));
	for(i = 0; i < file2s; i++)
		plot[i] = fr[i]*fr[i] + fi[i]*fi[i];
	Plot(plot, file2s, "V2 Power Spectrum");
	free(plot);
	
	// plot .5 secs of vowel?  how?
	plot = (double *) calloc(2205,sizeof(double));
	openfile = fopen(filename2, "r");
	for(i = 0; i < file2s; i++) {
		char tmp = getc(openfile)-128;
		if(i >= 1653 && i < 3858)
			plot[i-1653] = tmp;
	}
	close(openfile);
	Plot(plot, 2205, ".2 secs of vowel: \\\"I\\\" in V2");
	free(plot);
	
	// 3: uc=4, n = .25
	//uc = highest;
	uc = 4;
	n = .25;
		
	//while(1) {	
	
	tr = (double *) calloc(file2p, sizeof(double));
	ti = (double *) calloc(file2p, sizeof(double));
	for(i = 0; i < file2p; i++) {
		tr[i] = fr[i];
		ti[i] = fi[i];
	}
	
	// butterworth
	butterworth = (double *) calloc(file2p, sizeof(double));
	for(i = 0; i < file2p; i++) {
		butterworth[i] = (1/(1+pow( ((((double) i )* i) / (uc * uc) ), n)));
		//butterworth[i] = 0;
		tr[i] = tr[i]*butterworth[i];
		//if(tr[i] != 0) printf("ERROR!!!\n");
		ti[i] = ti[i]*butterworth[i];
		//if(ti[i] != 0) printf("ERROR!!!\n");
	}
	sprintf(buf, "Butterworth, n = %lf, uc = %d, s = %d", n, uc, file2s);
	Plot(butterworth, file2s, buf);
	
	// power spectrum
	plot = (double *) calloc(file2s, sizeof(double));
	for(i = 0; i < file2s; i++)
		plot[i] = tr[i]*tr[i] + ti[i]*ti[i];
	Plot(plot, file2s, "V2 Smoothed Power Spectrum");
	free(plot);
	
	fft(file2e, tr, ti, 1);
	
	openfile = fopen("tmp.ub", "w");
	for(i = 0; i < file2s; i++)
		putc(tr[i]+128, openfile);
	close(openfile);

	free(tr);
	free(ti);
	
	//test
	#ifndef NOPLAY
	printf("Playing file, uc=%d\n", uc);
	system("play tmp.ub -r 11025 -d /dev/dsp");
	unlink("tmp.ub");
	usleep(500000); //half a second between clips
	//sleep(3);
	#endif
	
	//uc-=1;
	//}
	
	free(fr);
	free(fi);
	free(butterworth);
	
	//return;


	
	// Part 5: uc=12, n = .25
	// "I am eagle lover x 4"
	// "I am evil homer x 4"
	uc = 12;
	n = .25;
	
	openfile = fopen(filename3, "r");
	fr = (double *) calloc(file3p, sizeof(double));
	fi = (double *) calloc(file3p, sizeof(double));
	for(i = 0; i < file3s; i++) {
		fr[i] = (getc(openfile)-128);
	}
	close(openfile);
	
	fft(file3e, fr, fi, 0);
	
	butterworth = (double *) calloc(file3p, sizeof(double));
	for(i = 0; i < file3p; i++) {
		butterworth[i] = (1/(1+pow(( (((double) i)*i) / (uc * uc) ), n)));
		//if(butterworth[i] != butterworth[i]) {
		//	printf("nan, i = %d, uc = %d, n = %lf,\ni*i = %lf, uc*uc = %d, #1/#2 = %lf\n", 
		//						i, uc, n, (((double) i) * i), uc*uc, ((double) i*i)/(uc*uc));
		//	exit(1);
		//}
		fr[i] = fr[i]*butterworth[i];
		fi[i] = fi[i]*butterworth[i];
	}
	sprintf(buf, "Butterworth, n = %lf, uc = %d, s = %d", n, uc, file3s);
	Plot(butterworth, file3s, buf);
	
	fft(file3e, fr, fi, 1);
	
	openfile = fopen("tmp.ub", "w");
	for(i = 0; i < file3s; i++)
		putc(fr[i]+128, openfile);
	close(openfile);

	free(fr);
	free(fi);
	free(butterworth);
	
	//test
	#ifndef NOPLAY
	printf("Playing file, uc=%d\n", uc);
	system("play tmp.ub -r 11025 -d /dev/dsp");
	unlink("tmp.ub");
	#endif
}

void Plot(double *vals, int numvals, char *title) {
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
		fprintf(outfile, "%lf\n", vals[i]);
		#ifdef PLOTFILE
		fprintf(tmpfile, "%lf\n", vals[i]);
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
	fprintf(outfile, "plot \"%s\" with lines\n", "gnuplot.data");
	fclose(outfile);

	snprintf(buf, 1024, "/usr/bin/gnuplot -persist \"%s\"", "gnuplot.command");

	system(buf);
	
	sleep(1);

	unlink("gnuplot.data");
	unlink("gnuplot.command");
}

void fft(int m,double *x,double *y,int dir) {
   long nn,i,i1,j,k,i2,l,l1,l2;
   double c1,c2,tx,ty,t1,t2,u1,u2,z;

   /* Calculate the number of points */
   nn = 1;
   for (i=0;i<m;i++)
      nn *= 2;

   /* Do the bit reversal */
   i2 = nn >> 1;
   j = 0;
   for (i=0;i<nn-1;i++) {
      if (i < j) {
         tx = x[i];
         ty = y[i];
         x[i] = x[j];
         y[i] = y[j];
         x[j] = tx;
         y[j] = ty;
      }
      k = i2;
      while (k <= j) {
         j -= k;
         k >>= 1;
      }
      j += k;
   }

   /* Compute the FFT */
   c1 = -1.0;
   c2 = 0.0;
   l2 = 1;
   for (l=0;l<m;l++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0;
      u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<nn;i+=l2) {
            i1 = i + l1;
            t1 = u1 * x[i1] - u2 * y[i1];
            t2 = u1 * y[i1] + u2 * x[i1];
            x[i1] = x[i] - t1;
            y[i1] = y[i] - t2;
            x[i] += t1;
            y[i] += t2;
         }
         z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      if (dir == 1)
         c2 = -c2;
      c1 = sqrt((1.0 + c1) / 2.0);
   }

   /* Scaling for forward transform */
   if (dir == 1) {
      for (i=0;i<nn;i++) {
         x[i] /= (double)nn;
         y[i] /= (double)nn;
      }
   }

   //return(TRUE);
}
