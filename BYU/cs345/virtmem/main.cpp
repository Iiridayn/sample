#include <qapplication.h>
#include "virtmemwindow.h"

int main(int argc, char * argv[])
{
	QApplication a(argc, argv);
	virtmemwindow * window = new virtmemwindow();
	if (window==NULL) printf("Error Somehow");
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}

