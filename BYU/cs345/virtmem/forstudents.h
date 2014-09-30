#ifndef FORSTUDENTS_H
#define FORSTUDENTS_H

#include <qstring.h>
#include <qobject.h>

#define SPACE		' '

struct fileinfo
{
	unsigned int addr;
	unsigned int value;
};

class forstudents : public QObject
{
	Q_OBJECT
private:
	fileinfo * currfile;
	int locinfile;
	int totalinfile;
public:
	forstudents();
	~forstudents();
	void studentinit();
	void studentresetpushed();
	unsigned int * studentviewmainmemorypushed(int pageno);
	unsigned int * studentviewswapspacepushed(int pageno);
	void studentverbosepushed(bool pressed);
	QString studentviewframetablepushed();
	void studentsetnummainmempages(int numpages);
	void studentloadprocess(QString filename, int processno);
	unsigned int studentreadmem(unsigned int addr, int processno);
	void studentwritemem(unsigned int addr, unsigned int writeval, int processno);
	unsigned int accessMem(unsigned int addr, int processno, bool write = false, unsigned int writedata = 0);
	
	bool taloadprocess(QString filename);
	fileinfo tagetline();
signals:
	void emitnewinfo(int hits, int misses, int rptloc);
};

#endif

