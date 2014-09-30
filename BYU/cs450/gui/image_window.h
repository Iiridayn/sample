#ifndef IMAGE_WINDOW_H
#define IMAGE_WINDOW_H

#define PI 3.14159265

#include <qmainwindow.h>
#include <qwidget.h>
class QString;

class Image : public QWidget {
Q_OBJECT
public:
	Image(QString filename, QWidget *parent=0, const char *name=0, int wFlags=0);
	Image(QImage *inImage, QWidget *parent=0, const char *name=0, int wFlags=0);
	void saveFile(QString filename);
	QImage *img() { return image; }

	// color ops
	void make_gray();

	// gray ops
	int *getGrayHist() { calcGrayHist(); return grayHist; }
	void calcGrayHist();
	int *hist_eq();
	void median_filter(int size);
	void average_filter(int size);
	void convolute(int *window[], int size);
	void grad_mag(); //compute the gradient magnitude of it

	// FFT
	static void FFT(int N, QImage *input, bool inverse = false);
	//static void fft(int N, double *real, double *imag, bool inverse = false);
	static void fft(int m,double *x,double *y,int dir);
	static void fft_split(QImage *one, QImage *two, int size);
	void fftmult(QImage *multimage); // multiplies by the passed image in the fourier domain
	static void removeSomeInterference(QImage *input);

	// Geometric
	void magnify(int factor);
	void shrink(int factor);
	void rotate(int degrees, bool doub = false);

protected:
	void paintEvent(QPaintEvent *e);

private:
	void setPixel(int x, int y, int gray) { setPixel(x, y, gray, gray, gray); }
	void setPixel(int x, int y, int r, int g, int b);
	void changesize(int w, int h);

	QImage *image;
	QWidget *parent;
	int grayHist[256];
	bool grayImage;
};

class ImageWindow : public QMainWindow {
//Q_OBJECT
public:
	ImageWindow(QString filename, QWidget *parent=0, const char *name=0, int wFlags=0);
	ImageWindow(QImage *inImage, QWidget *parent=0, const char *name=0, int wFlags=0);
	void saveFile(QString filename) { file = filename; image->saveFile(filename); }
	void update() { image->hide(); image->show(); } // force a paint event
	void save() { saveFile(file); }
	QString getFileName() { return file; }
	Image *img() { return image; }
	void plotGrayHist(int* hist);
	static void plot(int *vals, int numvals, const char *type="boxes", const char *title="Gray Level Histogram");
	static void plot(double *vals, int numvals, const char *type="boxes", const char *title="Gray Level Histogram");

private:
	QString file;
	Image *image;
	//ImageWindow(const ImageWindow&);
	//ImageWindow& operator=(const ImageWindow&);
};

#endif
