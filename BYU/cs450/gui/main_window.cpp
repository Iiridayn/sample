#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qfiledialog.h>
#include <qvbox.h>
#include <qworkspace.h>
#include <qwidget.h>
#include <qstring.h>
#include <qevent.h>
#include <qdialog.h>
#include <qregexp.h>
#include <qimage.h>

#include "main_window.h"
#include "image_window.h"

MainWindow::MainWindow(QWidget *parent, const char *name) : QMainWindow(parent, name) {
	setupMenus();
	statusBar()->message("Ready.");
	
	QVBox* viewBox = new QVBox( this );
	viewBox->setFrameStyle(QFrame:: StyledPanel | QFrame::Sunken);
	
	workspace = new QWorkspace(viewBox);

	setCentralWidget(viewBox);
}

void MainWindow::setupMenus() {
	fileMenu = new QPopupMenu(this);
	fileMenu->insertItem("&Open", this, SLOT(slotOpen()), CTRL+Key_O);
	fileMenu->insertSeparator();
	fileMenu->insertItem("&Save", this, SLOT(slotSave()), CTRL+Key_S);
	fileMenu->insertItem("Save &As", this, SLOT(slotSaveAs()), CTRL+Key_A);
	fileMenu->insertSeparator();
	fileMenu->insertItem("Dump All", this, SLOT(slotDumpAll()));
	fileMenu->insertSeparator();
	fileMenu->insertItem("&Quit", qApp, SLOT(quit()), CTRL+Key_Q);

	functionMenu = new QPopupMenu(this);
	functionMenu->insertItem("Grayscale &Histogram", this, SLOT(displayGrayHistogram()), CTRL+Key_H);

	projectMenu = new QPopupMenu(this);
	projectMenu->insertItem("Project &1", this, SLOT(project1()), Key_F1);
	projectMenu->insertItem("Project &2", this, SLOT(project2()), Key_F2);
	
	project3menu = new QPopupMenu(this);
	project3menu->insertItem("&Arithmetic", this, SLOT(project3a()));
	project3menu->insertItem("&Spatial", this, SLOT(project3b()));
	projectMenu->insertItem("Project &3", project3menu);

	projectMenu->insertItem("Project &4", this, SLOT(project4()), Key_F4);
	projectMenu->insertItem("Project &6", this, SLOT(project6()), Key_F6);
	projectMenu->insertItem("Project &7", this, SLOT(project7()), Key_F7);
	projectMenu->insertItem("Project &8", this, SLOT(project8()), Key_F8);

	windowMenu = new QPopupMenu(this);
	windowMenu->setCheckable(true);
	connect(windowMenu, SIGNAL(aboutToShow()), SLOT(showWindowMenu()));

	menuBar()->insertItem("&File", fileMenu);
	menuBar()->insertItem("F&unctions", functionMenu);
	menuBar()->insertItem("&Projects", projectMenu);
	menuBar()->insertItem("&Window", windowMenu);
}

/////////////////////////////////////////////////////////////////////////////
//		File operations
/////////////////////////////////////////////////////////////////////////////
ImageWindow* MainWindow::open() {
	QString filename = QFileDialog::getOpenFileName();
	if(!filename.isEmpty() || !filename.isNull())
		return openFile(filename); // or something like this
	return 0;
}

void MainWindow::save() {
	ImageWindow *tmp = (ImageWindow *)workspace->activeWindow();
	if(tmp)
		tmp->save();
}

void MainWindow::saveas() {
	QString filename = QFileDialog::getSaveFileName();
	if(!filename.isEmpty() || !filename.isNull())
		saveFile(filename);
}

ImageWindow* MainWindow::openFile(QString filename) {
	ImageWindow *tmp = new ImageWindow(filename, workspace, "", WDestructiveClose);
	tmp->show();
	return tmp;
}

void MainWindow::saveFile(QString filename) {
	ImageWindow *tmp = (ImageWindow *)workspace->activeWindow();
	if(tmp)
		tmp->saveFile(filename);
}

void MainWindow::slotDumpAll() { // only works for very special cases (dir exists, not moved, etc)
	QWidgetList windows = workspace->windowList();
	QString filebase = "/users/ugrad/g/ged/450/output/";
	
	for (int i = 0; i < int(windows.count()); ++i) {
		ImageWindow *tmp = (ImageWindow *)windows.at(i);
		QString filename = filebase + QString::number(i) + ".png";
		tmp->saveFile(filename);
	}
}

/////////////////////////////////////////////////////////////////////////////
//		Projects
/////////////////////////////////////////////////////////////////////////////
void MainWindow::project1() {
	ImageWindow *tmp = (ImageWindow *)workspace->activeWindow();
	if(!tmp)
		tmp = open();
	if(!tmp) {
		statusBar()->message("Error!  No image loaded!", 2000);
		return;
	}

	statusBar()->message("Converting to grayscale...");
	tmp->img()->make_gray();
	tmp->update();
	statusBar()->message("Saving as PGM...");
	saveFile(tmp->getFileName().replace(1+tmp->getFileName().findRev('.'), 3, "pgm"));
	displayGrayHistogram();
	statusBar()->message(tr("Ready."));
}

void MainWindow::project2() {
	ImageWindow *tmp = (ImageWindow *)workspace->activeWindow();
	if(!tmp)
		tmp = open();
	if(!tmp) {
		statusBar()->message("Error!  No image loaded!", 2000);
		return;
	}

	// alg on it
	statusBar()->message("Equalizing Image Histogram...");
	tmp->plot(tmp->img()->hist_eq(), 256, "lines");
	tmp->update();

	// plot hist
	displayGrayHistogram();
	// open another copy, now so the hist is for the changed one
	statusBar()->message("Opening Original...");
	openFile(tmp->getFileName());
	statusBar()->message("Ready.");
}

//void MainWindow::project3() { }

void MainWindow::project3a() {
	ImageWindow *tmp = (ImageWindow *)workspace->activeWindow();
	if(!tmp)
		tmp = open();
	if(!tmp) {
		statusBar()->message("Error!  No image loaded!", 2000);
		return;
	}
	
	// using part 2
	QString filename = tmp->getFileName();
	QString filestart = filename.left(filename.find(QRegExp("[0-9]"), filename.findRev('/')));
	QString fileend = filename.right(filename.length() - filename.findRev(QRegExp("[0-9]")) - 1);
	//statusBar()->message((filestart + QString::number(9999) + fileend));

	tmp->setCaption("1 image");
	
	QImage* images[40];
	int numbers[5] = {2, 5, 10, 20, 40};
	for(int i = 0; i < 40; i++) {
		//printf("Opening file: %s\n", (filestart+QString::number(i)+fileend).latin1());
		//images[i] = new QImage();
		images[i] = new QImage((filestart+QString::number(i)+fileend));
		//printf("Image depth: %d\n", images[i]->depth());
		//printf("Numcolors: %d\n", images[i]->numColors());
		//printf("MAX(x, y) = (%d, %d)\n", images[i]->width(), images[i]->height());
		//printf("Null: %d, valid(0,0): %d\n", images[i]->isNull(), images[i]->valid(0,0));
	}
	//getchar();

	for(int i = 0; i < 5; i++) {
		/*
		for(int num = 0; num < numbers[i]; num++) {
			printf("Opening file: %s\n", (filestart+QString::number(num)+fileend).latin1());
			//images[i] = new QImage((filestart+QString::number(num)+fileend));
			images[i]->load((filestart+QString::number(num)+fileend));
			printf("Image depth: %d\n", images[i]->depth());
			printf("Numcolors: %d\n", images[i]->numColors());
			printf("MAX(x, y) = (%d, %d)\n", images[i]->width(), images[i]->height());
		}
		*/
		int height = images[0]->height();
		int width = images[0]->width();
		QImage *result = new QImage(width, height, images[0]->depth(), images[0]->numColors());
		for(int x = 0; x < result->numColors(); x++)
			result->setColor(x, images[0]->color(x));
		for(int y = 0; y < height; y++)
			for(int x = 0; x < width; x++) {
				int avg = 0;
				for(int num = 0; num < numbers[i]; num++) {
					//printf("Image %d, (%d, %d): (%d)\n", num, x, y, images[num]->valid(x, y));
					avg += qRed(images[num]->pixel(x, y));
				}
				avg /= numbers[i];
				//QRgb val = qRgb(avg, avg, avg);
				result->setPixel(x, y, avg);
			}
		ImageWindow *tmp = new ImageWindow(result, workspace, "", WDestructiveClose);
		tmp->show();
		tmp->setCaption(QString::number(numbers[i])+" images averaged");
	}
	statusBar()->message("Ready.");
}

void MainWindow::project3b() {
	ImageWindow *tmp = (ImageWindow *)workspace->activeWindow();
	if(!tmp)
		tmp = open();
	if(!tmp) {
		statusBar()->message("Error!  No image loaded!", 2000);
		return;
	}

	QString filename = tmp->getFileName();
	tmp->setCaption("original");
	
	tmp = openFile(filename);
	statusBar()->message("Averaging...");
	tmp->img()->average_filter(3);
	tmp->setCaption("3x3 average");
	tmp->update();
	tmp = openFile(filename);
	//statusBar()->message("Averaging...");
	tmp->img()->average_filter(5);
	tmp->setCaption("5x5 average");
	tmp = openFile(filename);
	//statusBar()->message("Averaging...");
	tmp->img()->average_filter(7);
	tmp->setCaption("7x7 average");
	tmp = openFile(filename);
	//statusBar()->message("Averaging...");
	tmp->img()->average_filter(9);
	tmp->setCaption("9x9 average");
	tmp = openFile(filename);
	statusBar()->message("Median filtering...");
	tmp->img()->median_filter(3);
	tmp->setCaption("3x3 median");
	tmp = openFile(filename);
	//statusBar()->message("Median filtering...");
	tmp->img()->median_filter(5);
	tmp->setCaption("5x5 median");
	tmp = openFile(filename);
	//statusBar()->message("Median filtering...");
	tmp->img()->median_filter(7);
	tmp->setCaption("7x7 median");
	statusBar()->message("Ready.");
}

void MainWindow::project4() {
	ImageWindow *tmp = open();
	
	QString white = tmp->getFileName();
	tmp->setCaption("Original Box");
	
	// Part 1
	statusBar()->message("Part 1...");
	
	tmp = openFile(white);
	tmp->img()->average_filter(3);
	tmp->setCaption("3x3 average");
	tmp->update();

	tmp = openFile(white);
	tmp->img()->average_filter(5);
	tmp->setCaption("5x5 average");
	tmp->update();

	tmp = openFile(white);
	tmp->img()->average_filter(7);
	tmp->setCaption("7x7 average");
	tmp->update();

	tmp = openFile(white);
	tmp->img()->average_filter(9);
	tmp->setCaption("9x9 average");
	tmp->update();
	
	int sobelX1[3] = {-1, -2, -1};
	int sobelX2[3] = {0, 0, 0};
	int sobelX3[3] = {1, 2, 1};
	int *sobelX[3] = {sobelX1, sobelX2, sobelX3};
	int sobelY1[3] = {-1, 0, 1};
	int sobelY2[3] = {-2, 0, 2};
	int sobelY3[3] = {-1, 0, 1};
	int *sobelY[3] = {sobelY1, sobelY2, sobelY3};

	tmp = openFile(white);
	tmp->img()->convolute(sobelX, 3);
	tmp->setCaption("Sobel X");
	tmp->update();

	tmp = openFile(white);
	tmp->img()->convolute(sobelY, 3);
	tmp->setCaption("Sobel Y");
	tmp->update();

	int lap1[3] = {0, 1, 0};
	int lap2[3] = {1, -4, 1};
	int lap3[3] = {0, 1, 0};
	int *laplacian[3] = {lap1, lap2, lap3};
	tmp = openFile(white);
	tmp->img()->convolute(laplacian, 3);
	tmp->setCaption("Laplacian (-4 center)");
	tmp->update();

	// Part 2
	statusBar()->message("Part 2...");
	tmp = open();
	QString blocks = tmp->getFileName();
	tmp->setCaption("Original Blocks");

	tmp = openFile(white);
	tmp->img()->grad_mag();
	tmp->setCaption("Gradient Magnitude of Box");
	tmp->update();

	tmp = openFile(blocks);
	tmp->img()->grad_mag();
	tmp->setCaption("Gradient Magnitude of Blocks");
	tmp->update();

	// Part 3
	statusBar()->message("Part 3...");

	tmp = openFile(blocks);
	tmp->img()->average_filter(3);
	tmp->img()->grad_mag();
	tmp->setCaption("Grad Mag of 3x3 average");
	tmp->update();

	tmp = openFile(blocks);
	tmp->img()->average_filter(5);
	tmp->img()->grad_mag();
	tmp->setCaption("Grad Mag of 5x5 average");
	tmp->update();

	tmp = openFile(blocks);
	tmp->img()->average_filter(7);
	tmp->img()->grad_mag();
	tmp->setCaption("Grad Mag of 7x7 average");
	tmp->update();

	tmp = openFile(blocks);
	tmp->img()->average_filter(9);
	tmp->img()->grad_mag();
	tmp->setCaption("Grad Mag of 9x9 average");
	tmp->update();
	
	// Part 4
	statusBar()->message("Part 4...");
	int size = 3, strength = 2;
	
	//printf("Size and strength of the unsharp mask (0 0 to end): ");
	//scanf("%d %d", &size, &strength);
	//while(size != 0 || strength != 0) {

	int **mask = new int*[size];
	for(int i = 0; i < size; i++)
		mask[i] = new int[size];
	
	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++)
			mask[i][j] = -1;
	mask[size/2][size/2] = size*size + --strength; // + str, -1

	tmp = openFile(blocks);
	tmp->img()->convolute(mask, size);
	tmp->setCaption("Unsharp Mask: Size: " + QString::number(size) + " Strength: " + QString::number(++strength));
	tmp->update();

	for(int i = 0; i < size; i++)
		delete mask[i];
	delete mask;

	//printf("Size and strength of the unsharp mask (0 0 to end): ");
	//scanf("%d %d", &size, &strength);
	//}

	statusBar()->message("Ready.");
}

void MainWindow::project6() {
	ImageWindow *tmp = open();
	int N = 256;

	QString infile = tmp->getFileName();
	tmp->setCaption("Ball");
	tmp = open();
	QString otherfile = tmp->getFileName();
	tmp->setCaption("Gull");
	
	// Test
	{
		QImage *temp = new QImage(infile);
		QImage *temp2 = new QImage(infile);
		Image::fft_split(temp, temp2, N);

		tmp = new ImageWindow(temp, workspace, "", WDestructiveClose);
		tmp->setCaption("Test of FFT->Reverse");
		tmp->show();
	}
	//return;

	// Part A
	int sa = 8; // 4-10 recommended
	//while(sa != 0) 
	{
		//printf("Const for Part A: ");
		//scanf("%d", &sa);
		//if(sa == 0) break;

		//QImage *temp = new QImage(N, N, 16);
		QImage *temp = new QImage(infile);
		for(int i = 0; i < N; i++) {
			int res = (int) ((((sin((2*PI*sa*i)/N))+1)/2)*255);
			if(res < 0) res = 0; // will the values have enough variance?
			//QRgb val = qRgb(res, res, res);
			int val = res;
			for(int j = 0; j < N; j++)
				temp->setPixel(i, j, val);
		}
		tmp = new ImageWindow(temp, workspace, "", WDestructiveClose);
		tmp->setCaption("Part A, s = " + QString::number(sa));
		tmp->show();

		QImage *temp2 = new QImage(*temp);
		temp2->detach();
		Image::FFT(N, temp2);

		tmp = new ImageWindow(temp2, workspace, "", WDestructiveClose);
		tmp->setCaption("Part A, FFT s = " + QString::number(sa));
		tmp->show();
	}
	//ImageWindow *A = tmp;

	// Part B
	int sb = 6;
	//while(sb != 0) 
	{
		/*
		do {
			if(sa == sb) printf("Can't have the same value as the other!\n");
			printf("Const for Part B: ");
			scanf("%d", &sb);
		} while(sa == sb);
		 */
		//if(sb == 0) break;

		//QImage *temp = new QImage(N, N, 16);
		QImage *temp = new QImage(infile);
		for(int j = 0; j < N; j++) {
			int res = (int) ((((sin((2*PI*sb*j)/N))+1)/2)*255);
			//if(res < 0) res = 0; // will the values have enough variance?
			//QRgb val = qRgb(res, res, res);
			int val = res;
			for(int i = 0; i < N; i++)
				temp->setPixel(i, j, val);
		}
		
		tmp = new ImageWindow(temp, workspace, "", WDestructiveClose);
		tmp->setCaption("Part B, s = " + QString::number(sb));
		tmp->show();

		QImage *temp2 = new QImage(*temp);
		temp2->detach();
		Image::FFT(N, temp2);

		tmp = new ImageWindow(temp2, workspace, "", WDestructiveClose);
		tmp->setCaption("Part B, FFT s = " + QString::number(sb));
		tmp->show();
	}
	//ImageWindow *B = tmp;

	// 3,4
	//QImage *sum = new QImage(N, N, 16);
	QImage *sum = new QImage(infile);
	{
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < N; j++) {
				int tval = (int) (((((sin((2*PI*sa*i)/N)+1)/2)*255) + (((sin((2*PI*sb*j)/N)+1)/2)*255))/2);
				//if(tval < 0) tval = 0;
				//QRgb val = qRgb(tval, tval, tval);
				int val = tval;
				sum->setPixel(i, j, val);
			}
		}
		tmp = new ImageWindow(sum, workspace, "", WDestructiveClose);
		tmp->setCaption("Part B, sum");
		tmp->show();

		QImage *temp = new QImage(*sum);
		temp->detach();
		
		Image::FFT(N, temp);

		tmp = new ImageWindow(temp, workspace, "", WDestructiveClose);
		tmp->setCaption("Part B, FFT sum");
		tmp->show();
	} // end 3,4
	
	// Part C
	{
		//QImage *temp = new QImage(N, N, 16);
		QImage *temp = new QImage(infile);
		for(int i = 0; i < N; i++)
			for(int j = 0; j < N; j++)
				temp->setPixel(i, j, qRed(sum->pixel(j,N-i-1))); // prolly a 90 deg rotate
		tmp = new ImageWindow(temp, workspace, "", WDestructiveClose);
		tmp->setCaption("Part C, Rotate sum");
		tmp->show();
		QImage *temp2 = new QImage(*temp);
		temp2->detach();
		Image::FFT(N, temp2);
		tmp = new ImageWindow(temp2, workspace, "", WDestructiveClose);
		tmp->setCaption("Part C, FFT Rotate sum");
		tmp->show();
	}

	// Part D
	{
		//QImage *temp = new QImage(N, N, 16);
		QImage *temp = new QImage(infile);
        for(int i = 0; i < N; i++) {
			for(int j = 0; j < N; j++) {
				int tval = (int) (((((sin((2*PI*sa*i)/N) * sin((2*PI*sb*j)/N)))+1)/2)*255);
				//if(tval < 0) tval = 0;
				//QRgb val = qRgb(tval, tval, tval);
				int val = tval;
				temp->setPixel(i, j, val);
			}
		}

		tmp = new ImageWindow(temp, workspace, "", WDestructiveClose);
		tmp->setCaption("Part D, Multiply");
		tmp->show();
		
		QImage *temp2 = new QImage(*temp);
		temp2->detach();
		Image::FFT(N, temp2);

		tmp = new ImageWindow(temp2, workspace, "", WDestructiveClose);
		tmp->setCaption("Part D, FFT Multiply");
		tmp->show();
	}

	// Part E
	QImage *one = new QImage(infile);
	QImage *two = new QImage(otherfile);
	Image::fft_split(one, two, N);
	tmp = new ImageWindow(one, workspace, "", WDestructiveClose);
	tmp->setCaption("Part E, Ball Mag, Gull Phase");
	tmp->show();
	tmp = new ImageWindow(two, workspace, "", WDestructiveClose);
	tmp->setCaption("Part E, Gull Mag, Ball Phase");
	tmp->show();
}

void MainWindow::project7() {
	ImageWindow *Dbox = openFile("../450-7/2D_White_Box.pgm");
	Dbox->setCaption("2D Box");
	ImageWindow *inter = openFile("../450-7/interfere.pgm");
	inter->setCaption("Interfere.pgm");
	
	// Part A
	double *rdata = new double[128];
	double *idata = new double[128];
	FILE *f1 = fopen("../450-7/1D_Noise.dat", "r");
	for(int i = 0; i < 96; i++) {
		fscanf(f1, "%lf", &(rdata[i]));
		idata[i] = 0;
	}
	ImageWindow::plot(rdata, 96, "lines", "1D Noise");
	for(int i = 96; i < 128; i++) {
		rdata[i] = 0;
		idata[i] = 0;
	}
	fclose(f1);

	Image::fft(7, rdata, idata, 0);

	// mag/phase in r/i

	// Filter the thing!
	// for now, use an ideal filter (be fun to see the sinc effect too)
	// cutoff at 1/8?
	for(int i = 12; i < 128; i++) {
		rdata[i] = 0;
		idata[i] = 0;
	}

	// Convert back
	Image::fft(7, rdata, idata, 1); // reverse
	
	// idata should be all 0, but make the mag anyway
	for(int i = 0; i < 128; i++)
		rdata[i] = hypot(rdata[i], idata[i]);
	// scale? - maybe if I seem to need it
	// plot the first 96, the rest should again be 0
	ImageWindow::plot(rdata, 96, "lines", "Filtered 1D Noise");


	// Part B
	//while(1) {
	//QImage *mult = new QImage("../450-7/2D_White_Box*9.pgm");
	/*
	ImageWindow *tmp = open();
	tmp->setCaption("Filter test");
	QString infile = tmp->getFileName();
	//*/
	QImage *mult = new QImage("../450-7/filter4.pgm");
	
	// using Dbox as well

	Dbox->img()->fftmult(mult);	
	QImage *temp = new QImage("../450-7/2D_White_Box.pgm");
	
	for(int i = 0; i < 96; i++)
		for(int j = 0; j < 96; j++) {
			temp->setPixel(i, j, qRed(mult->pixel(i,j)));
		}
	
	ImageWindow *tmp = new ImageWindow(temp, workspace, "", WDestructiveClose);
	tmp->setCaption("Filtered Box");
	tmp->show();
	//}
	

	// Part C
	QString infile = inter->getFileName();
	temp = new QImage(infile);

	// Display for visual verfiying
	///*
	Image::FFT(temp->width(), temp);
	tmp = new ImageWindow(temp, workspace, "", WDestructiveClose);
	tmp->setCaption("FFT of Interference");
	tmp->show();
	temp = new QImage(infile);
	//*/

	///*
	Image::removeSomeInterference(temp);
	tmp = new ImageWindow(temp, workspace, "", WDestructiveClose);
	tmp->setCaption("Interference Removed");
	tmp->show();
	//*/
}

void MainWindow::project8() {
	ImageWindow *tmp = open();
	tmp->setCaption("Original Parrots");

	QString parrots = tmp->getFileName();

	// Part 1
	tmp = new ImageWindow(parrots, workspace, "", WDestructiveClose);
	tmp->img()->magnify(2);
	tmp->setCaption("2x zoom");
	tmp->show();
	tmp = new ImageWindow(parrots, workspace, "", WDestructiveClose);
	tmp->img()->magnify(3);
	tmp->setCaption("3x zoom");
	tmp->show();

	// Part 2
	QString filename = parrots;
	for(int i = 0; i < 4; i++) {
		if(i > 0) {
			tmp = open();
			filename = tmp->getFileName();
			tmp->setCaption("Test File " + QString::number(i+1));
		}

		tmp = new ImageWindow(filename, workspace, "", WDestructiveClose);
		tmp->img()->shrink(4);
		tmp->setCaption("4x shrink File " + QString::number(i+1));
		tmp->show();
		tmp = new ImageWindow(filename, workspace, "", WDestructiveClose);
		tmp->img()->shrink(8);
		tmp->setCaption("8x shrink File " + QString::number(i+1));
		tmp->show();
	}

	// Part 3
	for(int i = 1; i <= 8; i++) {
		tmp = new ImageWindow(parrots, workspace, "", WDestructiveClose);
		for(int j = 1; j <= i; j++) {
			tmp->img()->rotate(15, (j == 1)?true:false);
		}
		tmp->setCaption("Rotated " + QString::number(15*i) + " degrees");
		tmp->show();
		printf("Rotated by %d degrees\n", 15*i);
	}
	tmp = new ImageWindow(parrots, workspace, "", WDestructiveClose);
	tmp->img()->rotate(120, true);
	tmp->setCaption("Rotated directly 120 degrees");
	tmp->show();
}

/////////////////////////////////////////////////////////////////////////////
//		SLOTs
/////////////////////////////////////////////////////////////////////////////
/*
void MainWindow::keyPressEvent(QKeyEvent *e) {
	if(e.key == Key_F3) 
		project3();
	else
		e.ignore();
}
*/


void MainWindow::closewindows() {
	//ImageWindow *tmp = (ImageWindow *)workspace->activeWindow();
	QWidgetList windows = workspace->windowList();
	for (int i = 0; i < int(windows.count()); ++i) {
		windows.at(i)->hide();
		delete windows.at(i);
	}
}


void MainWindow::displayGrayHistogram() {
	ImageWindow *tmp = (ImageWindow *)workspace->activeWindow();
	if(!tmp) {
		statusBar()->message("Error!  No image loaded!", 2000);
		return;
	}

	statusBar()->message("Displaying Histogram...");
	tmp->plotGrayHist(tmp->img()->getGrayHist());
	statusBar()->message("Ready.");
}

void MainWindow::showWindowMenu() {
	windowMenu->clear();
	
	windowMenu->insertItem("&Cascade", workspace, SLOT(cascade()), 0, 1000);
	windowMenu->insertItem("&Tile", workspace, SLOT(tile()), 0, 1001);
	windowMenu->insertItem("Close &All", this, SLOT(closewindows()), 0, 1002);
	
	if (workspace->windowList().isEmpty()) {
		menuBar()->setItemEnabled(1000, false);
		menuBar()->setItemEnabled(1002, false);
	}
	menuBar()->setItemEnabled(1001, false); // disable, does odd things to my pictures
	
	windowMenu->insertSeparator();
	
	QWidgetList windows = workspace->windowList();
	for (int i = 0; i < int(windows.count()); ++i) {
		int id = windowMenu->insertItem(QString("&%1 ").arg(i+1)+windows.at(i)->caption(), this, SLOT(windowMenuActivated(int)));
		windowMenu->setItemParameter(id, i);
		windowMenu->setItemChecked(id, workspace->activeWindow() == windows.at(i));
	}
}

void MainWindow::windowMenuActivated(int id) {
	QWidget* w = workspace->windowList().at(id);
	if(w) w->setFocus();
}
