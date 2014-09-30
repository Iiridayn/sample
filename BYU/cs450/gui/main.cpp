#include <qapplication.h>

#include "main_window.h"


int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	
	MainWindow *mw = new MainWindow();
	mw->setCaption("Ged's 450 GUI");

	a.setMainWidget(mw);

	mw->show();
	return a.exec();
}
