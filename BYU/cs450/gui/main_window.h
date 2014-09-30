#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <qmainwindow.h>
#include <qapplication.h>
class QPopupMenu;
class QWorkspace;
class ImageWindow;

class MainWindow : public QMainWindow {
Q_OBJECT
public:
	MainWindow(QWidget *parent=0, const char *name=0);
	ImageWindow* open();
	void save();
	void saveas();

private slots:
	void slotOpen() { open(); }
	void slotSave() { save(); }
	void slotSaveAs() { saveas(); }
	void slotDumpAll();
	
	void project1();
	void project2();
	//void project3();
	void project3a();
	void project3b();
	void project4();
	//void project5();
	void project6();
	void project7();
	void project8();

	void displayGrayHistogram();
	
	void showWindowMenu();
	void windowMenuActivated(int id);
	void closewindows();

//protected:
	//void keyPressEvent(QKeyEvent *e);

private:
	void setupMenus();
	void saveFile(QString filename);
	ImageWindow* openFile(QString filename);

	QPopupMenu *fileMenu;
	QPopupMenu *functionMenu;
	QPopupMenu *projectMenu;
	QPopupMenu *project3menu;
	QPopupMenu *windowMenu;
	QWorkspace *workspace;
};

#endif
