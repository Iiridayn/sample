#include <qstring.h>
#include <qimage.h>
#include <qscrollview.h>

#include <qpainter.h>
#include <qrect.h>

#include <iostream.h>

#include "image_window.h"

ImageWindow::ImageWindow(QString filename, QWidget *parent, const char *name, int wFlags) : QMainWindow(parent, name, wFlags) {
	image = new Image(filename, this);
	setCaption(filename);
	setCentralWidget(image);
	image->show();
	file = filename;
}

ImageWindow::ImageWindow(QImage *inImage, QWidget *parent, const char *name, int wFlags) : QMainWindow(parent, name, wFlags) {
	image = new Image(inImage, this);
	//setCaption(filename);
	setCentralWidget(image);
	image->show();
	file = "";
}

void ImageWindow::plotGrayHist(int* hist) {
	plot(hist, 256);
}

void ImageWindow::plot(int *vals, int numvals, const char *type, const char *title) {
	double *tmp = new double[numvals];
	for(int i = 0; i < numvals; i++)
		tmp[i] = vals[i];
	plot(tmp, numvals, type, title);
}

void ImageWindow::plot(double *vals, int numvals, const char *type, const char *title) {
	// plot the histogram with gnuplot
	char buf[1024];
	int i;
	FILE *out_file = fopen("gnuplot.data", "wb");
	
	if (!out_file)
	{
		fprintf(stderr, "\nFile %s could not be written\n\n", "gnuplot.data");
		
	}  // if
	
	for (i = 0; i < numvals; i++)
		fprintf(out_file, "%lf\n", vals[i]);
	
	fclose(out_file);
	out_file = fopen("gnuplot.command", "wb");
	
	if (!out_file)
	{
		fprintf(stderr, "\nFile %s could not be written\n\n", "gnuplot.command");
	
	}  // if
	
	fprintf(out_file, "set title \"%s\"\n", title);
	fprintf(out_file, "set xrange [0:%d]\n", numvals-1);
	fprintf(out_file, "set xtics 16\n");
	fprintf(out_file, "plot \"%s\" with %s\n", "gnuplot.data", type);
	fclose(out_file);

	snprintf(buf, 1024, "/usr/bin/gnuplot -persist \"%s\"", "gnuplot.command");
	system(buf);
}

/////////////////////////////////////////////////////////////////////////////////
//								Image										   //
/////////////////////////////////////////////////////////////////////////////////

Image::Image(QString filename, QWidget *parent, const char *name, int wFlags) : QWidget(parent, name, wFlags) {
	image = new QImage(filename);
	setMinimumSize(image->width(), image->height());
	setMaximumSize(image->width(), image->height());
	parent->setMinimumSize(image->width(), image->height());
	parent->setMaximumSize(image->width(), image->height());
	this->parent = parent;
	grayImage = image->allGray();
}

Image::Image(QImage *inImage, QWidget *parent, const char *name, int wFlags) : QWidget(parent, name, wFlags) {
	image = inImage;
	setMinimumSize(image->width(), image->height());
	setMaximumSize(image->width(), image->height());
	parent->setMinimumSize(image->width(), image->height());
	parent->setMaximumSize(image->width(), image->height());
	this->parent = parent;
	grayImage = image->allGray();
}

void Image::setPixel(int x, int y, int r, int g, int b) {
	if(grayImage)
		image->setPixel(x, y, r);
	else {
		QRgb val = qRgb(r, g, b);
		image->setPixel(x, y, val);
	}
}

void Image::changesize(int w, int h) {
	setMinimumSize(w, h);
	setMaximumSize(w, h);
	parent->setMinimumSize(w, h);
	parent->setMaximumSize(w, h);
}

void Image::make_gray() {
	int x = image->width();
	int y = image->height();

	for(int i = 0; i < x; i++)
		for(int j = 0; j < y; j++) {
			QRgb pix = image->pixel(i, j);
			int gray = (int)(0.299*qRed(pix) + 0.587*qGreen(pix) + 0.114*qBlue(pix));
			setPixel(i, j, gray);
		}
	if(!image->allGray())
		cerr << "Image not fully converted to Grayscale!" << endl;
	grayImage = true;
}

int* Image::hist_eq() {
	int x = image->width();
	int y = image->height();
	int size = x*y;

	calcGrayHist();

	int sum = 0;
	for(int k = 0; k < 256; k++) {
		sum += grayHist[k];
		grayHist[k] = (int)(((double)sum/size)*255);
	}

	for(int i = 0; i < x; i++)
		for(int j = 0; j < y; j++) {
			QRgb pix = image->pixel(i, j);
			setPixel(i, j, grayHist[qRed(pix)]);
		}
	return grayHist;
}

void Image::calcGrayHist() {
	int x = image->width();
	int y = image->height();

	for(int i = 0; i < 256; i++)
		grayHist[i] = 0;

	for(int i = 0; i < x; i++)
		for(int j = 0; j < y; j++)
			grayHist[(qRed(image->pixel(i, j)))]++;
}

void Image::grad_mag() {
	int width = image->width();
	int height = image->height();
	int bound = 2;
	int start = -1;

	int flippedSobelX[3][3] = { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };
	int flippedSobelY[3][3] = { {1, 0, -1}, {2, 0, -2}, {1, 0, -1} };

	QImage *result = new QImage(width, height, image->depth(), image->numColors());
	for(int i = 0; i < result->numColors(); i++)
		result->setColor(i, image->color(i));

	int sumx, sumy;
	for(int y = 0; y < height; y++)
		for(int x = 0; x < width; x++) {
			sumx = sumy = 0;
			for(int j = start; j < bound; j++)
				if((y+j >= 0) && (y+j < height))
					for(int i = start; i < bound; i++)
						if((x+i >= 0) && (x+i < width)) {
							sumx += flippedSobelX[i-start][j-start] * qRed(image->pixel(x+i, y+j));
							sumy += flippedSobelY[i-start][j-start] * qRed(image->pixel(x+i, y+j));
						}
			int comp = (int) sqrt(sumx*sumx + sumy*sumy);
			if(comp >= result->numColors()) comp = result->numColors()-1;
			if(comp < 0) comp = 0;
			result->setPixel(x, y, comp);
		}
	
	delete image;
	image = result;
}

void Image::convolute(int *window[], int size) {
	if(!(size&1)) { 
		cerr << "Invalid convolution window size (not odd)" << endl;
		return;
	}
	int width = image->width();
	int height = image->height();
	int bound = ((size/2)+1);
	int start = -(size/2);

	QImage *result = new QImage(width, height, image->depth(), image->numColors());
	for(int i = 0; i < result->numColors(); i++)
		result->setColor(i, image->color(i));
	
	///*
	int max = size-1;
	for(int i = 0; i < size; i++) // flip vertically
		for(int j = 0; j < bound; j++) {
			int temp = window[i][j];
			window[i][j] = window[i][max-j];
			window[i][max-j] = temp;
		}
	for(int i = 0; i < size; i++) // flip horiontally
		for(int j = 0; j < bound; j++) {
			int temp = window[j][i];
			window[j][i] = window[max-j][i];
			window[max-j][i] = temp;
		}
	//*/

	// convolute
	int sum;
	for(int y = 0; y < height; y++)
		for(int x = 0; x < width; x++) {
			sum = 0;
			for(int j = start; j < bound; j++)
				if((y+j >= 0) && (y+j < height))
					for(int i = start; i < bound; i++)
						if((x+i >= 0) && (x+i < width))
							sum += window[i-start][j-start] * qRed(image->pixel(x+i, y+j));
			sum = abs(sum);
			if(sum >= result->numColors()) sum = result->numColors()-1;
			if(sum < 0) sum = 0;
			result->setPixel(x, y, sum);
		}

	delete image;
	image = result;
}

void Image::average_filter(int size) {
	if(size > 9 || size < 3 || !(size&1)) {
		cerr << "Invalid size specifications to average_filter: " << size << endl;
		return;
	}

	int width = image->width();
	int height = image->height();
	QImage *filtered = new QImage(width, height, image->depth(), image->numColors());
	//cout << "Image depth: " << image->depth() << " " << filtered->depth() << endl;
	//cout << "Numcolors: " << image->numColors() << " " << filtered->numColors() << endl;
	//cout << "MAX(x, y) = (" << width << ", " << height << ")" << endl;
	for(int i = 0; i < filtered->numColors(); i++)
		//cout << "Color[" << i << "] = " << filtered->color(i) << endl;
		filtered->setColor(i, image->color(i));
	//getchar();
	
	//cout << "Ready to filter!" << endl;

	int sum;
	int bound = ((size/2)+1);
	int start = -(size/2);
	int size2 = size*size;
	for(int y = 0; y < height; y++)
		for(int x = 0; x < width; x++) {
			//cout << "for(" << x << ", " << y << ")" << endl;
			sum = 0;
			for(int j = start; j < bound; j++)
				if((y+j >= 0) && (y+j < height))
					for(int i = start; i < bound; i++)
						if((x+i >= 0) && (x+i < width)) //{
							//if(x+i == width || y+j == height ) cout << x << " " << y << endl;
							sum += qRed(image->pixel(x+i, y+j));
						//}
			sum /= size2;
			//if(sum == 0 || sum == 256) cout << sum << endl;
			//QRgb val = qRgb(sum, sum, sum);
			filtered->setPixel(x, y, sum);
			//cout << "Set pixel (" << x << ", " << y << ", " << sum << ")" << endl;
		}
	
	delete image;
	image = filtered;
	//cout << "Done: " << size << endl;
}

void Image::median_filter(int size) {
	if(size > 7 || size < 3 || !(size&1)) {
		cerr << "Invalid size specifications to median_filter: " << size << endl;
		return;
	}

	int width = image->width();
	int height = image->height();
	QImage *filtered = new QImage(width, height, image->depth(), image->numColors());
	//cout << "Image depth: " << image->depth() << " " << filtered->depth() << endl;
	//cout << "Numcolors: " << image->numColors() << " " << filtered->numColors() << endl;
	for(int i = 0; i < filtered->numColors(); i++)
		filtered->setColor(i, image->color(i));

	int median[49]; // max size
	int bound = ((size/2)+1);
	int start = -(size/2);
	int bound2 = (size*size);
	int midpoint = (bound2/2)+1;
	for(int y = 0; y < height; y++)
		for(int x = 0; x < width; x++) {
			for(int i = 0; i < 49; i++) median[i] = 0;
			for(int j = start; j < bound; j++)
				if((y+j >= 0) && (y+j < height))
					for(int i = start; i < bound; i++)
						if((x+i >= 0) && (x+i < width))
							median[(j-start)+size*(i-start)] = qRed(image->pixel(x+i, y+j));
			int med = 0;
			for(int i = 0; i < bound2; i++) { // selection sort to find the median
				int smallest = median[i], smallindex = i;
				for(int j = i+1; j < bound2; j++) {
					if(median[j] < smallest) {
						smallest = median[j];
						smallindex = j;
					}
				}
				int temp = median[i];
				median[i] = smallest;
				median[smallindex] = temp;
			}
			med = median[midpoint];
			//QRgb val = qRgb(med, med, med);
			filtered->setPixel(x, y, med);
		}
	
	delete image;
	image = filtered;
	//cout << "Done: " << size << endl;
}

void Image::paintEvent(QPaintEvent *e) {
	QPainter p;
    QRect r;
    
    QWidget::paintEvent( e ); // call the super class paintEvent
    //    r = e->rect().intersect( contentsRect() );
    r = e->rect();
    p.begin( this );
    p.drawImage( r.x(), r.y(), *image, r.x(), 
		 r.y(), r.width(), r.height() );
    p.end();
}

void Image::saveFile(QString filename) {
	int index = filename.findRev('.');
	QString extn(filename.mid(index+1)); // index plus 1 gets me past '.'
	
	if( extn == "png" ){
		image->save(filename, "PNG" );
	} else if( extn == "ppm" ){
		image->save(filename, "PPMRAW" );
	} else if(extn == "pgm") {
		image->save(filename, "PGMRAW");
	} else {
		cerr << "Only images in ppm, pgm, or png format are supported. Writing in PPM" << endl;
		image->save(filename, "PPMRAW" );
	}
}

///*
//#define EXPECT(x, y, in, n) ((((x-1)>0)?in[x-1][y]:0 + ((x+1)<n)?in[x+1][y]:0 + ((y+1)<n)?in[x][y+1]:0 + ((y-1)>0)?in[x][y-1]:0)/4)
double expect(int x, int y, double **in, int n) {
	//printf("x: %d y: %d n: %d (x-1): %d (x+1) %d (y-1) %d (y+1) %d\n", x, y, n, x-1, x+1, y-1, y+1);
	int c = 0;
	double x1 = ((x-1)>0)?c++,in[x-1][y]:0;
	double x2 = ((x+1)<n)?c++,in[x+1][y]:0;
	double y1 = ((y+1)<n)?c++,in[x][y+1]:0;
	double y2 = ((y-1)>0)?c++,in[x][y-1]:0;
	double expected = ((x1 + x2 + y1 + y2)/c);
	//printf("x: %d, y: %d, c: %d, expected:%lf\n", x, y, c, expected);
	return expected;
}
//*/

// Removes the two points with the greatest difference from their expected value
void Image::removeSomeInterference(QImage *input) {
	int N = input->width();
	int temp = N;
	int n = N;
	int count = 0;
	while(1) {
		temp /= 2;
		count++;
		if(temp == 1) break; // is good
		if(!(temp%2)) {
			temp += 1;
		}
	}
	temp = 1;
	for(int i = count; i != 0; i--)
		temp *= 2;
	N = temp; // now is padded
	
	//printf("N: %d, n: %d, count: %d\n", N, n, count);

	// create the double arrays from the image, pad as needed
	double **imager = new double*[N];
	for(int i = 0; i < N; i++)
		imager[i] = new double[N];
	double **imagei = new double*[N];
	for(int i = 0; i < N; i++)
		imagei[i] = new double[N];
	
	for(int j = 0; j < n; j++) {
		for(int i = 0; i < n; i++) {
			imager[i][j] = qRed(input->pixel(i, j));
			imagei[i][j] = 0;
		}
		for(int i = n; i < N; i++) {
			imager[i][j] = 0;
			imagei[i][j] = 0;
		}
	}
	for(int j = n; j < N; j++) {
		for(int i = 0; i < N; i++) {
			imager[i][j] = 0;
			imagei[i][j] = 0;
		}
	}
	
	for(int i = 0; i < N; i++) {
		double *real = new double[N];
		double *imag = new double[N];
		for(int j = 0; j < N; j++) {
			real[j] = imager[i][j];
			imag[j] = imagei[i][j];
		}
		fft(count, real, imag, 0);
		for(int j = 0; j < N; j++) {
			imager[i][j] = real[j];
			imagei[i][j] = imag[j];
		}
		delete[] real;
		delete[] imag;
	}
	// now other direction
	for(int j = 0; j < N; j++) {
		double *real = new double[N];
		double *imag = new double[N];
		for(int i = 0; i < N; i++) {
			real[i] = imager[i][j];
			imag[i] = imagei[i][j];
		}
		fft(count, real, imag, 0);
		for(int i = 0; i < N; i++) {
			imager[i][j] = real[i];
			imagei[i][j] = imag[i];
		}
		delete[] real;
		delete[] imag;
	}

	double **hyp = new double*[n];
	for(int i = 0; i < n; i++)
		hyp[i] = new double[n];
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			hyp[i][j] = hypot(imager[i][j], imagei[i][j]);

	// oop = out of place, should always be > 0
	double oop1 = 0, oop2 = 0;
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	//hyp[0][0] = 0; // So it stops reading this as the large (won't change the image)

	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++) {
			//printf("P(%d,%d)=%lf\n", i, j, hyp[0][0]);
			if(i == 0 || j == 0) continue;
			if(i == (n-1) && j == (n-1)) printf("We got to the end too...\n");
			double expected = expect(i, j, hyp, n);
			double diff = fabs(hyp[i][j] - expected);
			//if(diff > 0) printf("val %lf @ (%d,%d)\n", diff, i, j);
			//else printf("(%d,%d) = %lf\n", i, j, diff);
			//if(expected < 10 && diff > 10) printf("Should be < 10 here... (%d,%d)\n", i, j);
			if(diff > oop1) {
				printf("diff is largest @ %lf! (%d,%d)\n", diff, i, j);
				oop2 = oop1;
				x2 = x1;
				y2 = y1;
				oop1 = diff;
				x1 = i;
				y1 = j;
			}
			else if(diff > oop2) {
				printf("diff is 2nd largest @ %lf! (%d,%d)\n", diff, i, j);
				oop2 = diff;
				x2 = i;
				y2 = j;
			}
			/*
			if(hyp[i][j] > large) {
				large2 = large;
				x2 = x;
				y2 = y;
				large = hyp[i][j];
				x = i;
				y = j;
			}
			else if(hyp[i][j] > large2) {
				large2 = hyp[i][j];
				x2 = i;
				y2 = j;
			}
			*/
		}
	
	printf("2 largest at: (%d, %d) and (%d, %d)\n", x1, y1, x2, y2);

	// Set to the average of its surroundings
	imager[x1][y1] = expect(x1, y1, imager, n);
	imagei[x1][y1] = expect(x1, y1, imagei, n);
	imager[x2][y2] = expect(x1, y1, imager, n);
	imagei[x2][y2] = expect(x1, y1, imagei, n);

	// Reverse the FFT
	for(int i = 0; i < N; i++) {
		double *real = new double[N];
		double *imag = new double[N];
		for(int j = 0; j < N; j++) {
			real[j] = imager[i][j];
			imag[j] = imagei[i][j];
		}
		fft(count, real, imag, 1);
		for(int j = 0; j < N; j++) {
			imager[i][j] = real[j];
			imagei[i][j] = imag[j];
		}
		delete[] real;
		delete[] imag;
	}
	// now other direction
	for(int j = 0; j < N; j++) {
		double *real = new double[N];
		double *imag = new double[N];
		for(int i = 0; i < N; i++) {
			real[i] = imager[i][j];
			imag[i] = imagei[i][j];
		}
		fft(count, real, imag, 1);
		for(int i = 0; i < N; i++) {
			imager[i][j] = real[i];
			imagei[i][j] = imag[i];
		}
		delete[] real;
		delete[] imag;
	}

	// put them back into the image
	//scale first...
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			hyp[i][j] = hypot(imager[i][j], imagei[i][j]);
	double small = hyp[0][0], large = hyp[0][0];
	//find largest and smallest
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++) {
			if(hyp[i][j] > large)
				large = hyp[i][j];
			if(hyp[i][j] < small)
				small = hyp[i][j];
		}

	//printf("small = %f, large = %f\n", small, large);
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			double tmp = hyp[i][j];
			tmp = ((tmp-small)/(large-small));
			tmp *= 255;
			if(tmp < 0 || tmp > 255) printf("-");
			int val = (int) tmp;
			if((unsigned int)val == 0x80000000) { printf("*"); continue; } // the float was nan
			input->setPixel(i, j, val);
		}
	}
	//printf("\n");
	
	for(int i = 0; i < n; i++)
		delete[] hyp[i];
	delete hyp;
	for(int i = 0; i < N; i++)
		delete[] imager[i];
	delete[] imager;
	for(int i = 0; i < N; i++)
		delete[] imagei[i];
	delete[] imagei;
}

void Image::fftmult(QImage *multimage) {
	int N = image->width();

	int temp = N;
	int n = N;
	int count = 1;
	while(1) {
		temp /= 2;
		count++;
		if(temp == 1) break; // is good
		if(!(temp%2)) {
			temp += 1;
		}
	}
	temp = 1;
	for(int i = count; i != 0; i--)
		temp *= 2;
	//if(temp != N) printf("Padded: n: %d N: %d count:%d\n", N, temp, count);
	N = temp; // now is padded

	double **image1r = new double*[N];
	for(int i = 0; i < N; i++)
		image1r[i] = new double[N];
	double **image1i = new double*[N];
	for(int i = 0; i < N; i++)
		image1i[i] = new double[N];
	
	double **image2r = new double*[N];
	for(int i = 0; i < N; i++)
		image2r[i] = new double[N];
	double **image2i = new double*[N];
	for(int i = 0; i < N; i++)
		image2i[i] = new double[N];


	for(int j = 0; j < n; j++) {
		for(int i = 0; i < n; i++) {
			image1r[i][j] = qRed(image->pixel(i, j));
			image1i[i][j] = 0;
			image2r[i][j] = qRed(multimage->pixel(i, j));
			image2i[i][j] = 0;
		}
		for(int i = n; i < N; i++) {
			image1r[i][j] = 0;
			image1i[i][j] = 0;
			image2r[i][j] = qRed(multimage->pixel(i, j));
			image2i[i][j] = 0;
		}
	}
	for(int j = n; j < N; j++) {
		for(int i = 0; i < N; i++) {
			image1r[i][j] = 0;
			image1i[i][j] = 0;
			image2r[i][j] = qRed(multimage->pixel(i, j));
			image2i[i][j] = 0;
		}
	}

	int rev = 0;
	for(int i = 0; i < N; i++) {
		double *real1 = new double[N];
		double *imag1 = new double[N];
		double *real2 = new double[N];
		double *imag2 = new double[N];
		for(int j = 0; j < N; j++) {
			real1[j] = image1r[i][j];
			imag1[j] = image1i[i][j];
			real2[j] = image2r[i][j];
			imag2[j] = image2i[i][j];
		}
		fft(count, real1, imag1, rev);
		fft(count, real2, imag2, rev);
		for(int j = 0; j < N; j++) {
			image1r[i][j] = real1[j];
			image1i[i][j] = imag1[j];
			image2r[i][j] = real2[j];
			image2i[i][j] = imag2[j];
		}
		delete[] real1;
		delete[] imag1;
		delete[] real2;
		delete[] imag2;
	}
	// now other direction
	for(int j = 0; j < N; j++) {
		double *real1 = new double[N];
		double *imag1 = new double[N];
		double *real2 = new double[N];
		double *imag2 = new double[N];
		for(int i = 0; i < N; i++) {
			real1[i] = image1r[i][j];
			imag1[i] = image1i[i][j];
			real2[i] = image2r[i][j];
			imag2[i] = image2i[i][j];
		}
		fft(count, real1, imag1, rev);
		fft(count, real2, imag2, rev);
		for(int i = 0; i < N; i++) {
			image1r[i][j] = real1[i];
			image1i[i][j] = imag1[i];
			image2r[i][j] = real2[i];
			image2i[i][j] = imag2[i];
		}
		delete[] real1;
		delete[] imag1;
		delete[] real2;
		delete[] imag2;
	}

	rev++;

	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			double temp = image1r[i][j];
			image1r[i][j] = image1r[i][j] * image2r[i][j] + image1i[i][j] * image2i[i][j];
			image1i[i][j] = image1i[i][j] * image2r[i][j] + temp * image2i[i][j];
		}
	}

	// now only care about image1
	for(int i = 0; i < N; i++) {
		double *real = new double[N];
		double *imag = new double[N];
		for(int j = 0; j < N; j++) {
			real[j] = image1r[i][j];
			imag[j] = image1i[i][j];
		}
		fft(count, real, imag, rev);
		for(int j = 0; j < N; j++) {
			image1r[i][j] = real[j];
			image1i[i][j] = imag[j];
		}
		delete[] real;
		delete[] imag;
	}
	// now other direction
	for(int j = 0; j < N; j++) {
		double *real = new double[N];
		double *imag = new double[N];
		for(int i = 0; i < N; i++) {
			real[i] = image1r[i][j];
			imag[i] = image1i[i][j];
		}
		fft(count, real, imag, rev);
		for(int i = 0; i < N; i++) {
			image1r[i][j] = real[i];
			image1i[i][j] = imag[i];
		}
		delete[] real;
		delete[] imag;
	}

    //scale first
	double **hyp = new double*[n];
	for(int i = 0; i < n; i++)
		hyp[i] = new double[n];
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			hyp[i][j] = hypot(image1r[i][j], image1i[i][j]);
	double small = hyp[0][0], large = hyp[0][0];
	//find largest and smallest
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++) {
			if(hyp[i][j] > large)
				large = hyp[i][j];
			if(hyp[i][j] < small)
				small = hyp[i][j];
		}
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			double tmp = hyp[i][j];
			tmp = ((tmp-small)/(large-small));
			tmp *= 255;
			if(tmp < 0 || tmp > 255) printf("-");
			int val = (int) tmp;
			if((unsigned int)val == 0x80000000) { printf("*"); continue; } // the float was nan
			multimage->setPixel(i, j, val);
		}
	}
	
	for(int i = 0; i < n; i++)
		delete[] hyp[i];
	delete hyp;
	for(int i = 0; i < N; i++)
		delete[] image1r[i];
	delete[] image1r;
	for(int i = 0; i < N; i++)
		delete[] image1i[i];
	delete[] image1i;
	for(int i = 0; i < N; i++)
		delete[] image2r[i];
	delete[] image2r;
	for(int i = 0; i < N; i++)
		delete[] image2i[i];
	delete[] image2i;
}

void Image::fft_split(QImage *one, QImage *two, int size) {
	int N = size; // for code reuse
	int n = size; // again
	
	double **image1r = new double*[N];
	for(int i = 0; i < N; i++)
		image1r[i] = new double[N];
	double **image1i = new double*[N];
	for(int i = 0; i < N; i++)
		image1i[i] = new double[N];
	
	double **image2r = new double*[N];
	for(int i = 0; i < N; i++)
		image2r[i] = new double[N];
	double **image2i = new double*[N];
	for(int i = 0; i < N; i++)
		image2i[i] = new double[N];
	
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++) {
			image1r[i][j] = qRed(one->pixel(i, j));
			image1i[i][j] = 0;
		}
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++) {
			image2r[i][j] = qRed(two->pixel(i, j));
			image2i[i][j] = 0;
		}
	
	for(int i = 0; i < N; i++) {
		double *real1 = new double[N];
		double *imag1 = new double[N];
		double *real2 = new double[N];
		double *imag2 = new double[N];
		for(int j = 0; j < N; j++) {
			real1[j] = image1r[i][j];
			imag1[j] = image1i[i][j];
			real2[j] = image2r[i][j];
			imag2[j] = image2i[i][j];
		}
		//printf("row %d:\n", i);
		//fft(N, real1, imag1);
		//fft(N, real2, imag2);
		fft(8, real1, imag1, 0);
		fft(8, real2, imag2, 0);
		// put them back into the image array
		for(int j = 0; j < N; j++) {
			image1r[i][j] = real1[j];
			image1i[i][j] = imag1[j];
			image2r[i][j] = real2[j];
			image2i[i][j] = imag2[j];
		}
		delete[] real1;
		delete[] imag1;
		delete[] real2;
		delete[] imag2;
	}
	// now other direction
	for(int j = 0; j < N; j++) {
		double *real1 = new double[N];
		double *imag1 = new double[N];
		double *real2 = new double[N];
		double *imag2 = new double[N];
		for(int i = 0; i < N; i++) {
			real1[i] = image1r[i][j];
			imag1[i] = image1i[i][j];
			real2[i] = image2r[i][j];
			imag2[i] = image2i[i][j];
		}
		//printf("column %d:\n", i);
		//fft(N, real1, imag1);
		//fft(N, real2, imag2);
		fft(8, real1, imag1, 0);
		fft(8, real2, imag2, 0);
		// put them back into the image array
		for(int i = 0; i < N; i++) {
			image1r[i][j] = real1[i];
			image1i[i][j] = imag1[i];
			image2r[i][j] = real2[i];
			image2i[i][j] = imag2[i];
		}
		delete[] real1;
		delete[] imag1;
		delete[] real2;
		delete[] imag2;
	}
	
	// Compute phase/mag
	double **hyp1 = new double*[n];
	for(int i = 0; i < n; i++)
		hyp1[i] = new double[n];
	double **hyp2 = new double*[n];
	for(int i = 0; i < n; i++)
		hyp2[i] = new double[n];
	
	double **phase1 = new double*[n];
	for(int i = 0; i < n; i++)
		phase1[i] = new double[n];
	double **phase2 = new double*[n];
	for(int i = 0; i < n; i++)
		phase2[i] = new double[n];
	
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++) {
			hyp1[i][j] = hypot(image1r[i][j], image1i[i][j]);
			hyp2[i][j] = hypot(image2r[i][j], image2i[i][j]);
			phase1[i][j] = atan2(image1i[i][j],image1r[i][j]);
			phase2[i][j] = atan2(image2i[i][j],image2r[i][j]);
		}
		
	// Mix and match and remake the normal stuff
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++) {
			image1r[i][j] = hyp1[i][j] * cos(phase2[i][j]);
			image1i[i][j] = hyp1[i][j] * sin(phase2[i][j]);
			image2r[i][j] = hyp2[i][j] * cos(phase1[i][j]);
			image2i[i][j] = hyp2[i][j] * sin(phase1[i][j]);
			//image1r[i][j] = log(hyp1[i][j]);
			//image1i[i][j] = 0;
			//image2r[i][j] = log(hyp2[i][j]);
			//image2i[i][j] = 0;
		}
		
	///*
	// Now reverse FFT the thing
	for(int i = 0; i < N; i++) {
		double *real1 = new double[N];
		double *imag1 = new double[N];
		double *real2 = new double[N];
		double *imag2 = new double[N];
		for(int j = 0; j < N; j++) {
			real1[j] = image1r[i][j];
			imag1[j] = image1i[i][j];
			real2[j] = image2r[i][j];
			imag2[j] = image2i[i][j];
		}
		//printf("row %d:\n", i);
		//fft(N, real1, imag1, true);
		//fft(N, real2, imag2, true);
		fft(8, real1, imag1, 1);
		fft(8, real2, imag2, 1);
		// put them back into the image array
		for(int j = 0; j < N; j++) {
			image1r[i][j] = real1[j];
			image1i[i][j] = imag1[j];
			image2r[i][j] = real2[j];
			image2i[i][j] = imag2[j];
		}
		delete[] real1;
		delete[] imag1;
		delete[] real2;
		delete[] imag2;
	}
	// now other direction
	for(int j = 0; j < N; j++) {
		double *real1 = new double[N];
		double *imag1 = new double[N];
		double *real2 = new double[N];
		double *imag2 = new double[N];
		for(int i = 0; i < N; i++) {
			real1[i] = image1r[i][j];
			imag1[i] = image1i[i][j];
			real2[i] = image2r[i][j];
			imag2[i] = image2i[i][j];
		}
		//printf("column %d:\n", i);
		//fft(N, real1, imag1, true);
		//fft(N, real2, imag2, true);
		fft(8, real1, imag1, 1);
		fft(8, real2, imag2, 1);
		// put them back into the image array
		for(int i = 0; i < N; i++) {
			image1r[i][j] = real1[i];
			image1i[i][j] = imag1[i];
			image2r[i][j] = real2[i];
			image2i[i][j] = imag2[i];
		}
		delete[] real1;
		delete[] imag1;
		delete[] real2;
		delete[] imag2;
	}
	//*/
	
	// Writeback
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++) {
			hyp1[i][j] = hypot(image1r[i][j], image1i[i][j]);
			hyp2[i][j] = hypot(image2r[i][j], image2i[i][j]);
		}
	double small1 = hyp1[0][0], large1 = hyp1[0][0];
	double small2 = hyp2[0][0], large2 = hyp2[0][0];
	//find largest and smallest
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++) {
			if(hyp1[i][j] > large1)
				large1 = hyp1[i][j];
			if(hyp1[i][j] < small1)
				small1 = hyp1[i][j];
			if(hyp2[i][j] > large2)
				large2 = hyp2[i][j];
			if(hyp2[i][j] < small2)
				small2 = hyp2[i][j];
		}
	// copyback
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++) {
			double tmp1 = hyp1[i][j];
			double tmp2 = hyp2[i][j];

			tmp1 = ((tmp1-small1)/(large1-small1));
			tmp1 *= 255;
			int val1 = (int) tmp1;
			
			tmp2 = ((tmp2-small2)/(large2-small2));
			tmp2 *= 255;
			int val2 = (int) tmp2;
			
			if((unsigned int)val1 == 0x80000000) { printf("&"); continue; } // the float was nan
			if((unsigned int)val2 == 0x80000000) { printf("&"); continue; } // the float was nan
			
			one->setPixel(i, j, val1);
			two->setPixel(i, j, val2);
		}
		
	// Delete memory - wow, a lot of it
	for(int i = 0; i < n; i++)
		delete[] hyp1[i];
	delete hyp1;
	for(int i = 0; i < n; i++)
		delete[] hyp2[i];
	delete hyp2;
	for(int i = 0; i < n; i++)
		delete[] phase1[i];
	delete phase1;
	for(int i = 0; i < n; i++)
		delete[] phase2[i];
	delete phase2;
	for(int i = 0; i < N; i++)
		delete[] image1r[i];
	delete[] image1r;
	for(int i = 0; i < N; i++)
		delete[] image1i[i];
	delete[] image1i;
	for(int i = 0; i < N; i++)
		delete[] image2r[i];
	delete[] image2r;
	for(int i = 0; i < N; i++)
		delete[] image2i[i];
	delete[] image2i;
}


void Image::FFT(int N, QImage *input, bool inverse) {
	//printf("FFT:\n");

	int temp = N;
	int n = N;
	int count = 0;
	while(1) {
		temp /= 2;
		count++;
		if(temp == 1) break; // is good
		if(!(temp%2)) {
			temp += 1;
		}
	}
	temp = 1;
	for(; count != 0; count--)
		temp *= 2;
	if(temp != N) printf("Problem: this shouldn't happen: N: %d temp: %d\n", N, temp);
	N = temp; // now is padded
	//printf("N = %d, n = %d;\n", N, n);

	// create the double arrays from the image, pad as needed
	double **imager = new double*[N];
	for(int i = 0; i < N; i++)
		imager[i] = new double[N];
	double **imagei = new double*[N];
	for(int i = 0; i < N; i++)
		imagei[i] = new double[N];
	
	for(int j = 0; j < n; j++) {
		for(int i = 0; i < n; i++) {
			imager[i][j] = qRed(input->pixel(i, j));
			imagei[i][j] = 0;
		}
		for(int i = n; i < N; i++) {
			printf("ERROR: image was resized!\n");
			imager[i][j] = 0;
			imagei[i][j] = 0;
		}
	}
	for(int j = n; j < N; j++) {
		printf("ERROR: image was resized!\n");
		for(int i = 0; i < N; i++) {
			imager[i][j] = 0;
			imagei[i][j] = 0;
		}
	}
	
	for(int i = 0; i < N; i++) {
		double *real = new double[N];
		double *imag = new double[N];
		for(int j = 0; j < N; j++) {
			real[j] = imager[i][j];
			imag[j] = imagei[i][j];
		}
		//printf("row %d:\n", i);
		//fft(N, real, imag, inverse);
		fft(8, real, imag, (inverse)?1:0);
		// put them back into the image array
		for(int j = 0; j < N; j++) {
			imager[i][j] = real[j];
			imagei[i][j] = imag[j];
		}
		delete[] real;
		delete[] imag;
	}
	// now other direction
	for(int j = 0; j < N; j++) {
		double *real = new double[N];
		double *imag = new double[N];
		for(int i = 0; i < N; i++) {
			real[i] = imager[i][j];
			imag[i] = imagei[i][j];
		}
		//printf("column %d:\n", j);
		//fft(N, real, imag, inverse);
		fft(8, real, imag, (inverse)?1:0);
		// put them back into the image array
		for(int i = 0; i < N; i++) {
			imager[i][j] = real[i];
			imagei[i][j] = imag[i];
		}
		delete[] real;
		delete[] imag;
	}

	// put them back into the image
	//scale first...
	// hyp array
	double **hyp = new double*[n];
	for(int i = 0; i < n; i++)
		hyp[i] = new double[n];
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			hyp[i][j] = hypot(imager[i][j], imagei[i][j]);
	double small = hyp[0][0], large = hyp[0][0];
	//find largest and smallest
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++) {
			if(hyp[i][j] > large)
				large = hyp[i][j];
			if(hyp[i][j] < small)
				small = hyp[i][j];
		}

	//printf("small = %f, large = %f\n", small, large);
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			//double tmp = hypot(imager[i][j], imagei[i][j]);
			double tmp = hyp[i][j];

			tmp = ((tmp-small)/(large-small));//*256;
			//if(tmp < 0 || tmp >= 1) printf("|");
			tmp *= 255;
			if(tmp < 0 || tmp > 255) printf("-");
			//if(tmp > 255) tmp = 255;
			//else if(tmp < 0) tmp = 0;

			//QRgb val = qRgb(tmp, tmp, tmp);
			int val = (int) tmp; // incorrect conversion?
			//if(val < 0 || val > 255) printf("+");
			//if(val != 0) printf("!");
			//printf("val = %d, tmp = %f\n", val, tmp);
			//if(*(&val) == *(&tmp)) printf("P"); - lots of these
			if((unsigned int)val == 0x80000000) { printf("*"); continue; } // the float was nan
			input->setPixel(i, j, val);
		}
	}
	//printf("\n");
	
	for(int i = 0; i < n; i++)
		delete[] hyp[i];
	delete hyp;
	for(int i = 0; i < N; i++)
		delete[] imager[i];
	delete[] imager;
	for(int i = 0; i < N; i++)
		delete[] imagei[i];
	delete[] imagei;
}

/*
void Image::fft(int N, double *real, double *imag, bool inverse) {
	int halfN = N/2;
	//printf("N = %d\n", N);
	if(N == 2) {
		if(!(real[0] == real[0]) || !(real[1] == real[1]) ||
			!(imag[0] == imag[0]) || !(imag[1] == imag[1])) {
				printf("NAN!\n");
				exit(1);
			}
		double treal = real[0];
		double timag = imag[0];
		real[0] += real[1];
		imag[0] += imag[1];
		real[1] = treal-real[1];
		imag[1] = timag-imag[1];
	} else {
		//cnumber *g = (cnumber *) malloc(halfN*sizeof(cnumber));
		double *greal = new double[halfN];
		double *gimag = new double[halfN];
		//cnumber *h = (cnumber *) malloc(halfN*sizeof(cnumber));
		double *hreal = new double[halfN];
		double *himag = new double[halfN];
		for(int i = 0; i < N; i++)
			if(i&1) {
				hreal[i/2] = real[i];
				himag[i/2] = imag[i];
			} else {
				greal[i/2] = real[i];
				gimag[i/2] = imag[i];
			}
		fft(halfN, greal, gimag, inverse);
		fft(halfN, hreal, himag, inverse);
		for(int i = 0; i < N; i++) {
			double treal = cos((2*PI*i)/N);
			double timag = -sin((2*PI*i)/N);
			//tmp = cmult(tmp, h[i%halfN]);
			//tmp.real += g[i%halfN].real;
			real[i] = treal * hreal[i%halfN] + timag * himag[i%halfN];
			imag[i] = timag * hreal[i%halfN] + treal * himag[i%halfN];
			real[i] += greal[i%halfN];
			if(!inverse)
				imag[i] += gimag[i%halfN];
			else
				imag[i] = gimag[i%halfN] - imag[i];
		}
		delete[] greal;
		delete[] gimag;
		delete[] hreal;
		delete[] himag;
	}
}
*/

void Image::fft(int m,double *x,double *y,int dir)
{
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

void Image::magnify(int factor) {
	int w = image->width(), h = image->height();
	int wn = w*factor, hn = h*factor;
	QImage temp = image->scale(wn, hn);
	QImage *output = new QImage(temp);
	output->detach();

	//printf("Input image: (%d,%d), Output image: (%d,%d), factor = %d\n", w, h, output->width(), output->height(), factor);

	for(int i = 0; i < wn; i++)
		for(int j = 0; j < hn; j++) {
			double x = i/factor;
			double y = j/factor;
			double wpercent = x - (int) x;
			double hpercent = y - (int) y;

			int x1 = (int) x;
			int x2 = (int) x+1;
			int y1 = (int) y;
			int y2 = (int) y+1;

			if(x2 == w) x2--;
			if(y2 == h) y2--; // both become the same as y1, so the "line" is the same "height"
			int p11 = qRed(image->pixel(x1, y1));
			int p21 = qRed(image->pixel(x2, y1));
			int p12 = qRed(image->pixel(x1, y2));
			int p22 = qRed(image->pixel(x2, y2));

			double top = p11 * (1 - wpercent) + p21 * wpercent;
			double bottom = p12 * (1 - wpercent) + p22 * wpercent;
			double val = top * (1 - hpercent) + bottom * hpercent;

			int index = (int) val;
			//printf("index: %d\n", index);
			output->setPixel(i, j, index);
		}
	delete image;
	image = output;
	changesize(wn, hn);
}

void Image::shrink(int factor) {
	int w = image->width(), h = image->height();
	int wn = w/factor, hn = h/factor; // better be a factor the way it is now...  ;P
	QImage temp = image->scale(wn, hn);
	QImage *output = new QImage(temp);
	output->detach();

	for(int i = 0; i < wn; i++)
		for(int j = 0; j < hn; j++) {
			// a factor*factor square represents each pixel in the output - not centered, 
			// but it*factor is the 0,0 of the square
			int sum = 0;
			for(int x = 0; x < factor; x++)
				for(int y = 0; y < factor; y++)
					sum += qRed(image->pixel(i*factor+x, j*factor+y));
			sum /= (factor * factor);
			output->setPixel(i, j, sum);
		}
	
	delete image;
	image = output;
	changesize(wn, hn);
}

//The transformation for rotation by angle t around point (xc, yc) is
// x' = (x - xc) cos t - (y - yc) sin t + xc
// y' = (x - xc) sin t + (y - yc) cos t + yc
void Image::rotate(int degrees, bool doub) {
	int w = image->width(), h = image->height();
	int wn = w, hn = h;
	if(doub) { wn *= 2; hn *= 2; }
	QImage temp = image->scale(wn, hn);
	QImage *output = new QImage(temp);
	output->detach();
	
	//printf("Input image: (%d,%d), Output image: (%d,%d)\n", w, h, output->width(), output->height());

	int xc = w/2;
	int yc = h/2;
	
	double rads = (degrees * PI) / 180;

	for(int i = 0; i < wn; i++)
		for(int j = 0; j < hn; j++) {						
			//double x = (i - xc) * cos(rads) - (j - yc) * sin(rads) + xc;
			//double y = (i - xc) * sin(rads) + (j - yc) * cos(rads) + yc;
			//double x = ((i + xc) + (j - yc) * sin(rads)) / cos(rads) - xc;
			//double y = ((i - xc) * sin(rads) + (j + yc)) / cos(rads) - yc;
			double x;
			double y;
			if(doub) {
				x = (i-w) * cos(rads) + (j-h) * sin(rads) + xc;
				y = (i-w) * (- sin(rads)) + (j-h) * cos(rads) + yc;
			}
			else {
				x = (i-xc) * cos(rads) + (j-yc) * sin(rads) + xc;
				y = (i-xc) * (- sin(rads)) + (j-yc) * cos(rads) + yc;
			}
			//printf("x: %lf, y: %lf\n", x, y);
			
			if(x < 0 || x >= w || y < 0 || y >= h) {
				output->setPixel(i, j, 0);
				continue;
			}

			double wpercent = x - (int) x;
			double hpercent = y - (int) y;

			int x1 = (int) x;
			int x2 = (int) x+1;
			int y1 = (int) y;
			int y2 = (int) y+1;

			if(x2 == w) x2--;
			if(y2 == h) y2--; // both become the same as y1, so the "line" is the same "height"

			int p11 = qRed(image->pixel(x1, y1));
			int p21 = qRed(image->pixel(x2, y1));
			int p12 = qRed(image->pixel(x1, y2));
			int p22 = qRed(image->pixel(x2, y2));

			double top = p11 * (1 - wpercent) + p21 * wpercent;
			double bottom = p12 * (1 - wpercent) + p22 * wpercent;
			double val = top * (1 - hpercent) + bottom * hpercent;

			int index = (int) val;
			output->setPixel(i, j, index);
		}
	//printf("Done with %d degree rotate.\n", degrees);

	delete image;
	image = output;
	changesize(wn, hn);
}
