#include "virtmemdefines.h"

typedef struct {
	unsigned int data[NUMWORDSPERPAGE];
	int PID; //belongs to page, I thinks
	int type;
	int swapID; // used for output purposes
	int VMPage;
} mempage;

typedef struct {
	bool pinned;
	bool used;
	bool dirty;
	mempage *page;
} frame;

typedef struct {
	mempage* page;
} swap;

frame **memory;
int frames = 0;
swap* secondary[NUMFRAMESSWAP];
int swapnext = 1;
//int *rptIndex; is in swap[0]
int processMax = 256;
int hits = 0, misses = 0;
int hand = 0;
//bool pageFaults = true;

// adds an RPT's frame to the database
void rptAdd(int index, int PID) {
	/*
	if(processMax <= PID) {
		int *temp = (int *) malloc(PID*sizeof(int));
		memcpy(temp, rptIndex, processMax);
		free(rptIndex);
		rptIndex = temp;
		processMax = PID;
	}
	*/
	if(processMax <= PID) exit(1);
	if(index >= frames) exit(1);
	//rptIndex[PID] = index;
	secondary[0]->page->data[PID] = 0x10000000 | ((index & 0xFFF) << 12);
}

// retrieves the frame a processes RPT is in
int rptGet(int PID) {
	if(processMax <= PID) exit(1);
	//return rptIndex[PID];
	return (secondary[0]->page->data[PID] & 0x00FFF000) >> 12;
}

// swaps a page out of main memory
void swappage(unsigned int frameloc) {
	if(memory[frameloc]->page == 0) return;
	if(memory[frameloc]->pinned) return;
	// if page, check UPT for others, unpin if no others
	memory[frameloc]->used = false;
	memory[frameloc]->dirty = false; //not need pinned
	
	//fprintf(stderr, "swapping out physical page %d\n", frameloc);
	
	// inform higher up of the change
	switch(memory[frameloc]->page->type) {
		case 1: // UPT, inform RPT, clear memory thing
			//fprintf(stderr, "Page is a UPT, cleaning RPT\n");
			for(int i = 0; i < 64; i++) {
				if((memory[rptGet(memory[frameloc]->page->PID)]->page->data[i] & 0x00000FFF) == memory[frameloc]->page->swapID)
					memory[rptGet(memory[frameloc]->page->PID)]->page->data[i] &= 0x0F000FFF;
			}
			break;
		case 0: // page, inform UPT (will be locked in memory)
			{
			//fprintf(stderr, "Page is a page, UPT needs to know\n");
			// this will be much easier if I store the VMPage value ;P
			int rptdat = memory[rptGet(memory[frameloc]->page->PID)]->page->data[((memory[frameloc]->page->VMPage)>>8)&0x0000003F];
			//fprintf(stderr, "RPT data = %08X\n", rptdat);
			int uptloc = (rptdat & 0x00FFF000) >> 12;
			if(!uptloc) {
				fprintf(stderr, "UPT not in memory\n");
				exit(1);
			}
			//fprintf(stderr, "UPT @ %d\n", uptloc);
			bool uptlock = false;
			for(int i = 0; i < 256; i++) { // need to check them all anyway
				if(memory[uptloc]->page->data[i] & 0x10000000) {
					//fprintf(stderr, "UPT has page in mem.  entry: %08X\n", memory[uptloc]->page->data[i]);
					if(((memory[uptloc]->page->data[i] & 0x00FFF000) >> 12) == frameloc) {
						//fprintf(stderr, "Clearing page out of memory\n");
						memory[uptloc]->page->data[i] &= 0x0F000FFF;
						//fprintf(stderr, "new entry: %08X\n", memory[uptloc]->page->data[i]);
					}
					else
						uptlock = true;
				}
			}
			memory[uptloc]->pinned = uptlock;
			}
		case 2: return;
		default:
			fprintf(stderr, "Very bad error in swappage\n");
			exit(1);
	}
	memory[frameloc]->page = 0; //clear it out of memory
}

// swaps out a page, using the advanced clock algorythm
// returns freed index, no swap if free frames (ideally)
int runclock() {
	//printf("runclock, %d frames\n", frames);
	int frameno = -1;
	int starting = hand;
	int pass = 0; // to == normal set to 1;
	while(frameno == -1) {
		switch(pass) {
			case 0: //altered a little to fill up memory first
				//printf("Hand: %d\nFrames: %d\nmemory[hand]: %d\n", hand, frames, memory[hand]);
				if(memory[hand]->page == 0)
					frameno = hand;
     			break;					
			case 1:
				if(memory[hand]->pinned) break;
				if(memory[hand]->used == 0 && memory[hand]->dirty == 0)
					frameno = hand;
					break;
			case 2:
				if(memory[hand]->pinned) break;
				if(memory[hand]->used == 0 && memory[hand]->dirty == 1)
					frameno = hand;
				memory[hand]->used = false;
				break;
			case 3:
				if(memory[hand]->pinned) break;
				if(memory[hand]->used == 0 && memory[hand]->dirty == 0)
					frameno = hand;
					break;
			case 4:
				if(memory[hand]->pinned) break;
				if(memory[hand]->used == 0 && memory[hand]->dirty == 1)
					frameno = hand;
				break;
			default:
				//cerr << "Very bad error in adv. clock" << endl;
				fprintf(stderr, "Very bad error in adv. clock\n");
				exit(1);
		}
		
		hand++;
		if(hand >= frames) hand = 0;
		if(hand == starting) pass++;
	}
	swappage(frameno);
	return frameno;
}

/*
void updateT(mempage *page) {
	switch(page->type) {
		case 0: // page
			
		case 1: // UPT
			for(int i = 0; i < 64; i++) {
				if((memory[rptGet(page->PID)]->page->data[i] & 0x00000FFF) == page->swapID)
					memory[rptGet(page->PID)]->page->data[i] &= 0x0F000FFF;
			}
		case 2:
			return;
		default:
			fprintf(stderr, "Big error in updateT\n");
			exit(1);
	}
}
//*/

// swaps in a page from swap, returns its location in memory
int loadpage(int swappage) {
	int clock = runclock();
	memory[clock]->page = secondary[swappage]->page;
	//memory[clock]->used = true;
	//secondary[swappage]->page = 0; leave it in swap
	return clock;
}

// loads a page into swap
int makepage(mempage *inpage) {
	/*
	int clock = runclock();
	memory[clock]->page = inpage;
	return clock;
	*/
	if(swapnext > NUMFRAMESSWAP) {
		printf("CRITICAL ERORR: out of swap space\n");
		exit(1);
	}
	//printf("Makepage at %d\n", swapnext);
	secondary[swapnext]->page = inpage;
	inpage->swapID = swapnext;
	return swapnext++;
}
