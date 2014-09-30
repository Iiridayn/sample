#include "forstudents.h"
#include "virtmemwindow.h"
#include <qmessagebox.h>
#include <qfile.h>
#include <qstringlist.h>

#include "pages.h"

void forstudents::studentinit()
{
	//QMessageBox::information(NULL, "Initializing", "Please insert your own code in forstudents::studentinit");
	//This function is called by the constructor of the gui.
	//initialize what you see fit here, or just comment out that annoying messagebox
	for(int i = 0; i < NUMFRAMESSWAP; i++) {
		secondary[i] = (swap *) malloc(sizeof(swap));
		memset(secondary[i], 0, sizeof(swap));
	}
	secondary[0]->page = (mempage *) malloc(sizeof(mempage));
	memset(secondary[0]->page, 0, sizeof(mempage));
}

void forstudents::studentresetpushed()
{
	//QMessageBox::information(NULL, "Reset Pushed", "Please insert your own code in forstudents::studentresetpushed");
	//This function expects you to reinitialize all of your variables
	//Basically, it should be equivalent to deleting them all and reinitializing them
	hits = 0;
	misses = 0;
	swapnext = 1;
	for(int i = 0; i < NUMFRAMESSWAP; i++) {
		free(secondary[i]->page);
		free(secondary[i]);
	}
	for(int i = 0; i < frames; i++) {
		//if(memory[i]->page)
			//free(memory[i]->page);
		free(memory[i]);
	}
	free(memory);
	//free(rptIndex);
	//processMax = 0;
	frames = 0;
	//memset(secondary, 0, NUMFRAMESSWAP*sizeof(swap));
	for(int i = 0; i < NUMFRAMESSWAP; i++) {
		secondary[i] = (swap *) malloc(sizeof(swap));
		memset(secondary[i], 0, sizeof(swap));
	}
	secondary[0]->page = (mempage *) malloc(sizeof(mempage));
	memset(secondary[0]->page, 0, sizeof(mempage));
	hand = 0;
	//pageFaults = true;
}

unsigned int * forstudents::studentviewmainmemorypushed(int pageno)
{
	//QMessageBox::information(NULL, "View mainmemory Pushed", "Please insert your own code in forstudents::studentviewmainmemorypushed");
	//This should return a full page of your memory. NOT SWAP SPACE, memory.
	//It should be organized as an int pointer to 256 ints.
	//such an int could be initialized like this:
	//int * bob = new int[256];
	if(pageno >= frames) { // 0 is a page, x isn't
		QMessageBox::information(NULL, "Error: Frame number too high",
					"Value entered exceeds current maximum number of frames");
		return 0;
	}
	if(memory[pageno]->page == 0) return 0;
	//printf("view main returning: %d", memory[pageno]->page->data);
	return memory[pageno]->page->data;
	//return NULL;
}

unsigned int * forstudents::studentviewswapspacepushed(int pageno)
{
	//QMessageBox::information(NULL, "View swapspace Pushed", "Please insert your own code in forstudents::studentviewswapspacepushed");
	//This should return a full page of your swap space. NOT MEMORY, swap space.
	//It should be organized as an int pointer to 256 ints.
	//such an int could be initialized like this:
	//int * bob = new int[256];
	if(pageno >= 1024) {
		QMessageBox::information(NULL, "Error: Page number too high",
					"Value entered exceeds maximum number of pages");
		return 0;
	}
	if(secondary[pageno]->page == 0) return 0;
	return secondary[pageno]->page->data;
	//return NULL;
}

// not used in this version
void forstudents::studentverbosepushed(bool pressed)
{
	//This method provided for convinience.  You may change the way
	//your code prints things depending on if you want to be in debug mode
	if (pressed)
	{
		QMessageBox::information(NULL, "Verbose Mode ON", "Please insert your own code in forstudents::studentverbosepushed");
	}
	else
	{
		QMessageBox::information(NULL, "Verbose Mode OFF", "Please insert your own code in forstudents::studentverbosepushed");
	}
}

// not done
QString forstudents::studentviewframetablepushed()
{
	//This function should return a frame table that looks incredibly similar
	//to the ta example
	//QMessageBox::information(NULL, "View Frame Table", "Please insert your own code in forstudents::studentviewframetablepushed");
	//return "";
	QString retval;
	//return ("Frame\tPinned\tPID\tVMPage\tSwapPg\tUsed\tMod\n"); //+ //header
	retval += "Frame\tPinned\tPID\tVMPage\tSwapPg\tUsed\tMod\n";
	for(int i = 0; i < frames; i++) {
		char buf[100];
		if(memory[i]->page) {
			sprintf(buf, "%d\t%c\t%d\t%d\t%d\t%c\t%c\n", i, (memory[i]->pinned)?'1':'0',
				memory[i]->page->PID, memory[i]->page->VMPage, memory[i]->page->swapID, 
				(memory[i]->used)?'1':'0', (memory[i]->dirty)?'1':'0');
		}
		else
			sprintf(buf, "%d\t0\t--\t0\t--\t0\t0\n", i);
		retval += buf;
	}
	return retval;
}

void forstudents::studentsetnummainmempages(int numpages)
{
	//This function hands you the number of pages in main memory that the user
	//has requested
	//QMessageBox::information(NULL, "New Number of Pages", "Please insert your own code in forstudents::studentsetnummainmempages");
	memory = (frame **) malloc(numpages*sizeof(frame *));
	for(int i = 0; i < numpages; i++) {
		memory[i] = (frame *) malloc(sizeof(frame));
		memset(memory[i], 0, sizeof(frame));
	}
	frames = numpages;
}

void forstudents::studentloadprocess(QString filename, int processno)
{
	//This function hands you the name of a process to be loaded into main memory
	//as per the instructions on the webpage
	//QMessageBox::information(NULL, "New file to be loaded", "Please insert your own code in forstudents::studentloadprocess");
	//the following code here to show how to use ta provided functions
	if (taloadprocess(filename))
	{
		//my code
		// make RPT for process
		mempage *RPT = (mempage *) malloc(sizeof(mempage));
		memset(RPT, 0, sizeof(mempage));
		RPT->PID = processno;
		RPT->type = 2;
		// set it up, all 0 is the correct value to start
		//memset(RPT->data, 0, NUMWORDSPERPAGE);
		//pin it into a frame
		//int RPTindex = makepage(RPT);
		// need to manually load into memory
		int RPTindex = runclock();
		memory[RPTindex]->page = RPT;
		memory[RPTindex]->pinned = true;
		//secondary[0]->page->data[RPTindex] |= 0x10000000 | (RPTindex << 12) & 0x00FFF000;
		rptAdd(RPTindex, processno);
		
		// now declare and initialize each of the UPTs (64)
		// in swap
		for(int i = 0; i < 64; i++) {
			RPT->data[i] = 0x01000000 | (swapnext & 0x00000FFF);
			mempage *UPT = (mempage *) malloc(sizeof(mempage));
			memset(UPT, 0, sizeof(mempage));
			UPT->PID = processno;
			UPT->type = 1;
			makepage(UPT);
		}
		
		//my code end
		for (int i = 0; i < totalinfile; i++)
		{
			fileinfo temp = tagetline();
			
			if (temp.addr == 0 && temp.value == 0)
			{
				QMessageBox::information(NULL, "TA Load Process", "Fileinfo not initialized correctly.  ERROR.");
				break;
			}
			//printf("%d %d\n", temp.addr, temp.value);
			// My code
			// doing this for modularity, code reuse, and convinience
			//studentwritemem(temp.addr, temp.value, processno);
			// don't want to do it prolly, too much to back up/restore
			// would: hold hits/misses at same level; and ensure memory had exact
			//			same data (painful)
			// modify writemem: use temp hits/misses var; never load from swap
			int upt = RPT->data[(temp.addr>>18)&0x3F] & 0x00000FFF;
			int page = secondary[upt]->page->data[(temp.addr>>10)&0xFF] & 0x00000FFF;
			//printf("Loading memory, page %d is at %d, UPT is %08X\n", (temp.addr>>10)&0xFF, 
				//secondary[upt]->page->data[(temp.addr>>10)&0xFF] & 0x00000FFF, secondary[upt]->page->data[(temp.addr>>10)&0xFF]);
			if(!page) {
				mempage *tempp = (mempage *) malloc(sizeof(mempage));
				memset(tempp, 0, sizeof(mempage));
				tempp->PID = processno;
				tempp->VMPage = temp.addr >> 10;
				page = makepage(tempp);
				secondary[upt]->page->data[(temp.addr>>10)&0xFF] |= 0x01000000 | page & 0x00000FFF;
			}
			secondary[page]->page->data[(temp.addr>>2) & 0xFF] = temp.value;
			// my code end
		}
	}
	else
	{
		QMessageBox::information(NULL, "Error 404: File not Found", filename);
	}
}

// not done, need to update hits/misses
unsigned int forstudents::accessMem(unsigned int addr, int processno, bool write, unsigned int writedata) {
	int hit = 0, miss = 0;	
	hit++;
	int rptdata = memory[rptGet(processno)]->page->data[(addr>>18)&0x3F];
	memory[rptGet(processno)]->used = true;
	int uptmemoffset;
	if(rptdata & 0x10000000) { // UPT in mem
		hit++;
		uptmemoffset = (rptdata & 0x00FFF000) >> 12;
		//if(uptmemoffset >= frames) return -1;
	}
	else if(rptdata & 0x01000000) { // UPT in swap
		miss++;
		int swapoffset = (rptdata & 0x00000FFF);
		//if(secondary[swapoffset]->page == 0) return -1;
		uptmemoffset = loadpage(swapoffset);
		memory[rptGet(processno)]->page->data[(addr>>18)&0x3F] |= 0x10000000 | ((uptmemoffset << 12) & 0x00FFF000);
	}
	else { // make new UPT - better never happen
		/*
		miss++;
		mempage *temp = (mempage *) malloc(sizeof(mempage));
		memset(temp, 0, sizeof(mempage));
		uptmemoffset = makepage(temp);
		// update RPT to the UPT
		memory[rptGet(processno)]->page->data[(addr>>18)&0x3F] = 0x10000000 + (uptmemoffset << 12) & 0x00FFF000;
		memory[rptGet(processno)]->dirty = true;
		*/
		fprintf(stderr, "CRITICAL ERORR: make new upt\n");
		fflush(stderr);
		exit(1);
	}
	
	int uptdata = memory[uptmemoffset]->page->data[(addr>>10)&0xFF];
	memory[uptmemoffset]->used = true;
	int pagememoffset;
	if(uptdata & 0x10000000) { // page in mem
		hit++;
		pagememoffset = (uptdata & 0x00FFF000) >> 12;
		//if(pagememoffset >= frames) return -1;
	}
	else if(uptdata & 0x01000000) { // page in swap
		miss++;
		int swapoffset = (uptdata & 0x00000FFF);
		//if(secondary[swapoffset]->page == 0) return -1;
		pagememoffset = loadpage(swapoffset);
		memory[uptmemoffset]->pinned = true; //even if isn't will be now
		memory[uptmemoffset]->used = true;
		memory[uptmemoffset]->page->data[(addr>>10)&0xFF] |= 0x10000000 | ((pagememoffset << 12) & 0x00FFF000);
	}
	else { // make new page
		miss++;
		mempage *temp = (mempage *) malloc(sizeof(mempage));
		memset(temp, 0, sizeof(mempage));
		temp->PID = processno;
		temp->VMPage = addr >> 10;
		
		int pageswapoffset = makepage(temp); //load into swap
		memory[uptmemoffset]->page->data[(addr>>10)&0xFF] |= 0x01000000 | (pageswapoffset & 0x00000FFF);
		pagememoffset = loadpage(pageswapoffset);
		memory[uptmemoffset]->pinned = true; //even if isn't will be now
		memory[uptmemoffset]->used = true;
		memory[uptmemoffset]->page->data[(addr>>10)&0xFF] |= 0x10000000 | ((pagememoffset << 12) & 0x00FFF000);
		//memory[uptmemoffset]->dirty = true;
		// UPTs are never dirty ;P
	}
	
	// now page is in memory, read/write from/to it
	memory[pagememoffset]->used = true;
	int pagedata = 1; // so write returns true on success
	if(write) {
		memory[pagememoffset]->page->data[(addr>>2)&0xFF] = writedata;
		memory[pagememoffset]->dirty = true;
	}
	else
		pagedata = memory[pagememoffset]->page->data[(addr>>2)&0xFF];
	
	//may want to update hits/misses here (in the GUI that is)
	hits += hit;
	misses += miss;
	emitnewinfo(hits, misses, 0);
	if(pagedata == 268453682) {
		fprintf(stderr, "Page in memory %d, swap %d, at location %d, has value %d.\n", pagememoffset, (uptdata & 0x00000FFF), (addr>>2)&0xFF, 268453682);
	}
	return pagedata;
}

unsigned int forstudents::studentreadmem(unsigned int addr, int processno) {
	//emitnewinfo (myhits, mymisses, 0);
	//The function hands you the processnumber and address of memory to be read.
	//Please return the value at that location
	//QMessageBox::information(NULL, "Read Memory", "Please insert your own code in forstudents::studentreadmem");
	//return 0;
	return accessMem(addr, processno);
}

void forstudents::studentwritemem(unsigned int addr, unsigned int writeval, int processno)
{
    //emitnewinfo (myhits, mymisses, 0);
	//This function hands you not just the procesnumber and address in memory
	//but the value that you should write in memory at that location
	//QMessageBox::information(NULL, "Write Memory", "Please insert your own code in forstudents::studentwritemem");
	accessMem(addr, processno, true, writeval);
}

/*
void forstudents::emitnewinfo(int hits, int misses, int rptloc = 0) {
	//virtmemwindow::studentslink(hits, misses, rptloc);
	
}
*/

bool forstudents::taloadprocess(QString filename)
{
	QFile file(filename);
	if (file.open(IO_ReadOnly))
	{
		QTextStream stream(&file);
		QString temp;
		QStringList list;
		int counter = 0;
		while (!stream.eof())
		{
			temp = stream.readLine();
			list += temp;
			counter++;
		}
		if (currfile != NULL)
		{
			delete [] currfile;
			currfile = NULL;
		}
		currfile = new fileinfo[counter];
		locinfile = 0;
		totalinfile = 0;
		QString left;
		for (QStringList::Iterator it = list.begin(); it != list.end(); ++it)
		{
			temp = *it;
			left = temp.left(temp.find(SPACE));
			temp = temp.right(temp.length()-temp.find(SPACE)-1);
			currfile[totalinfile].addr = left.toUInt();
			currfile[totalinfile].value = temp.toUInt();
			totalinfile++;
		}
		return true;
	}
	return false;
}

fileinfo forstudents::tagetline()
{
	if (locinfile < totalinfile)
	{
		locinfile++;
		return currfile[locinfile-1];
	}
	fileinfo temp;
	temp.addr = 0;
	temp.value = 0;
	return temp;
}


forstudents::forstudents() : QObject()
{
	currfile = NULL;
}



forstudents::~forstudents()
{
	if (currfile != NULL)
	{
		delete [] currfile;
		currfile = NULL;
	}
}

