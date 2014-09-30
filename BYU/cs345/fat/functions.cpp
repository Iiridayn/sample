#include <string>
#include <stdio.h>
//#include <fcntl.h>
//#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include <iostream>
using namespace std;

// Attributes
#define READ_ONLY 0x01
#define HIDDEN    0x02
#define SYSTEM    0x04
#define VOLUME    0x08 // this is the volume label entry
#define DIRECTORY 0x10
#define ARCHIVE   0x20 // same as file

#pragma pack(push, 1) // don't pad this struct
typedef struct {
	unsigned char  Name[8];      /* File name in capital letters. Padded with spaces, not NULL terminated. */
	unsigned char  Extension[3]; /* Extension in capital letters. Also padded with spaces. There is no '.' separator. 
									The '.' is added for readability when filenames with extensions are displayed */
	unsigned char  Attributes;   /* Holds the attributes code */
	unsigned char  Reserved[10]; /* Reserved for Windows NT. Set to zero when creating files/directories. */
	unsigned short Time;         /* Time of last write (file/dir creation is a write) */
	unsigned short Date;         /* Date of last write */
	unsigned short startCluster; /* Pointer to the first cluster of the file. */
	unsigned long  fileSize;     /* File size in bytes. Make sure this is updated when the file is modified */
} direntry;
#pragma pack(pop)

#pragma pack(push, 1) // don't pad this struct
typedef struct {
	unsigned char  jump[3];				/* Jump instruction to the boot code */
	unsigned char  SysName[8];			/* The name of the system that formatted the volume */
	unsigned short BytesPerSector;		/* How many bytes in a sector (should be 512) */
	unsigned char  SectorsPerCluster;	/* How many sectors are in a cluster (FAT-12 should be 1) */
	unsigned short ReservedSectorCount;	/* Number of sectors that are reserved (FAT-12 should be 1) */
	unsigned char  FATcount;			/* The number of FAT tables on the disk (should be 2) */
	unsigned short MaxRootEntries;		/* Maximum number of directory entries in the root directory */
	unsigned short TotalSectors1;		/* FAT-12 total number of sectors on the disk */
	unsigned char  MediaDescriptor;		/* Code for media type {fixed, removable, etc.} */
	unsigned short SectorsPerFAT;		/* FAT-12 number of sectors that each FAT table occupies (should be 9) */
	unsigned short SectorsPerTrack;		/* Number of sectors in one cylindrical track */
	unsigned short HeadCount;			/* Number of heads for this volume (2 heads on a 1.4Mb 3.5 inch floppy) */
	unsigned long  HiddenSectors;		/* Number of preceding hidden sectors (0 for non-partitioned media) */
	unsigned long  TotalSectors2;		/* FAT-32 number of sectors on the disk (0 for FAT-12) */
	unsigned char  DriveNumber;			/* A drive number for the media (OS specific) */
	unsigned char  Reserved1;			/* Reserved space for Windows NT (when formatting, set to 0) */
	unsigned char  ExtBootSignature;	/* Indicates that the following three fields are present (0x29) */
	unsigned long  VolumeSerial;		/* Volume serial number (for tracking this disk) */
	unsigned char  VolumeLabel[11];		/* Volume label (matches label in the root directory, or "NO NAME    ") */
	unsigned char  Reserved2[8];		/* Deceptive FAT type Label that may or may not indicate the FAT type */
} bootsector;
#pragma pack(pop)

/* Just in case I want them
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;
//*/

#define SECTOR 512
FILE *mountedFD = 0;
char *mountedFile;
int fileLength;
//char *cwd = ""; // current working directory, needs to be stored somewhere
char *cwd;
int cwdIndex;
char *rootDir; // is a constant for FAT-12
int rootLen;
int clusterStart;
extern "C"
{
// boot sector has sector size
// read in both FATs
int fdLoadMntPt(char *mountname) {
	if(mountedFD) { // unmount first
		fclose(mountedFD);
		//more to do to unmount?
	}
	mountedFD = fopen(mountname, "r+");
	if(mountedFD == 0) return 0;
	//more to do to mount? yes!
	bootsector bootdata;
	//read(mountedFile, bootdata, sizeof(bootsector));
	// length!
	// Adapted from code by "Michael H" at http://www.developerfusion.com/forums/topic-21316
	fseek(mountedFD, 0, SEEK_END);
	fileLength = ftell(mountedFD);
	fseek(mountedFD, 0, SEEK_SET);
	// End adapted code section
	
	mountedFile = (char *) mmap(0, fileLength, PROT_READ|PROT_WRITE, MAP_SHARED, fileno(mountedFD), 0);
	
	memcpy(&bootdata, mountedFile, sizeof(bootsector));
	if(bootdata.BytesPerSector != 512) {
		cerr << "Incorrect number of bytes per sector." << endl;
		return 0;
	}
	if(bootdata.SectorsPerCluster != 1) {
		cerr << "Incorrect number of sectors per cluster." << endl;
		return 0;
	}
	if(bootdata.ReservedSectorCount != 1) {
		cerr << "Incorrect number of reserved sectors." << endl;
		return 0;
	}
	if(bootdata.FATcount != 2) {
		cerr << "Incorrect number of FATs." << endl;
		return 0;
	}
	if(bootdata.SectorsPerFAT != 9) {
		cerr << "Incorrect number of sectors per FAT." << endl;
		return 0;
	}
	if(bootdata.TotalSectors2 != 0) {
		cerr << "Incorrect number of FAT-32 sectors (should be 0)." << endl;
		return 0;
	}
	
	rootDir = mountedFile+19*SECTOR;
	cwd = rootDir;
	cwdIndex = -1;
	//rootLen = bootdata.MaxRootEntries; WRONG, is directory entry count
	rootLen = (bootdata.MaxRootEntries / (SECTOR/sizeof(direntry)));
	clusterStart = 19+rootLen-2;
	
	return 1;
}

int fdDelMem(char *str) {
	//has to use delete to delete the memory?
	// Gonna use malloc and free
	free(str);
	return 1; //always returns a 1
}

/*
int getFAT(int index) {
	// FAT 1 offset is 1-9
	// entries are 12 bits, 2 per 3 bytes
	//cout << "getFAT(" << index << ")" << endl;
	//; entries are 12 30 = 0x312, 01 23 = 0x231
	int retval = 0;
	if(!(index % 2)) { // even
		retval = (mountedFile[SECTOR+((index/2)*3)+1])&0xF0;
		retval <<= 4;
		retval |= mountedFile[SECTOR+((index/2)*3)];
	}
	else {
		retval = mountedFile[SECTOR+((index/2)*3)+2];
		retval <<= 4;
		retval |= mountedFile[SECTOR+((index/2)*3)+1]&0xF;
	}
	//assert(retval); // if is 0, is unused
	// maybe check for reserved?
	//cout << "return " << retval << ";" << endl;
	return retval;
}
//*/

///*
unsigned short getFAT(int FATindex)
{
   unsigned short FATEntryCode;    // The return value
   int FatOffset = ((FATindex * 3) / 2);    // Calculate the offset of the unsigned short to get
   if (FATindex % 2 == 1)    // If the index is odd
   {
      FATEntryCode = *((unsigned short *) &mountedFile[SECTOR+FatOffset]);    // Pull out a unsigned short from a unsigned char array
      FATEntryCode >>= 4;   // Extract the high-order 12 bits
   }
   else    // If the index is even
   {
      FATEntryCode = *((unsigned short *) &mountedFile[SECTOR+FatOffset]);    // Pull out a unsigned short from a unsigned char array
      FATEntryCode &= 0x0fff;    // Extract the low-order 12 bits
   }
   return FATEntryCode;
}
//*/

///* It galls me to use the provided code like this
void setFAT(int FATindex, unsigned short FAT12ClusEntryVal)
{
   int FATOffset = ((FATindex * 3) / 2);  // Calculate the offset of the unsigned short to write
   if (FATindex % 2 == 0)    // If the index is even
   {
      FAT12ClusEntryVal &= 0x0FFF; // Certify that we passed in no extraneous high-order bits
      *((unsigned short *)&mountedFile[SECTOR+FATOffset]) = (*((unsigned short*)&mountedFile[SECTOR+FATOffset])) & 0xF000; // Clear the bits
   }
   else
   {
      FAT12ClusEntryVal <<= 4;    // Shift our given value into the high-order 12-bits
      *((unsigned short *)&mountedFile[SECTOR+FATOffset]) = (*((unsigned short*)&mountedFile[SECTOR+FATOffset])) & 0x000F; // Clear the bits
   }
   // Assign the FAT entry value into the unsigned char array...
   *((unsigned short *)&mountedFile[SECTOR+FATOffset]) = (*((unsigned short *)&mountedFile[SECTOR+FATOffset])) | FAT12ClusEntryVal;
}
//*/

char* fdListDir() {
	// Reads the contents of the current directory, formats the output, and then returns them through the char*
	// Allocate memory (using new) for the directory listing that will be returned.
	// Loop through the current directory adding valid directory entries to the return string.
	// Valid directory entries include all names, extensions, times, dates, attributes, start clusters, and file sizes.
	string output = "Filename\tTime(hh:mm:ss)\tDate(dd/mm/yyyy)\tAttributes\tStart Cluster\tSize\n";
	output.reserve(200); // some default large value
	
	if(cwd == rootDir && cwdIndex == -1) { // slightly different behavior
		for(int i = 0; i < rootLen; i++) {
			for(unsigned int j = 0; j < SECTOR/sizeof(direntry); j++) {
				direntry temp;
				memcpy(&temp, rootDir+(i*SECTOR)+(j*sizeof(direntry)), sizeof(direntry));
				if(temp.Name[0] == 0xe5) continue; // pass this entry
				if(!temp.Name[0]) goto breakout; // done with directory
				char tbuf[100];
				//sprintf(tbuf, "%s.%s\t", temp.Name, temp.Extension);
				memcpy(tbuf, temp.Name, 8);
				tbuf[8] = '.';
				memcpy(tbuf+9, temp.Extension, 3);
				tbuf[12] = '\t';
				tbuf[13] = 0;
				output += tbuf;
				sprintf(tbuf, "%02d:%02d:%02d\t", ((temp.Time & 0xF100) >> 11), ((temp.Time & 0x07E0) >> 5),
    					(temp.Time & 0x001F));
				output += tbuf;
				sprintf(tbuf, "%02d/%02d/%04d\t\t", (temp.Date & 0x001F), ((temp.Date & 0x01E0) >> 5),
    					(((temp.Date & 0xFE00) >> 9) + 1980));
				output += tbuf;
				sprintf(tbuf, "%c%c%c%c%c%c", (temp.Attributes & READ_ONLY)?'R':' ', (temp.Attributes & HIDDEN)?'H':' ',
							(temp.Attributes & SYSTEM)?'S':' ', (temp.Attributes & DIRECTORY)?'D':' ',
							(temp.Attributes & ARCHIVE)?'A':' ', (temp.Attributes & VOLUME)?'V':' ');
				output += tbuf;
				sprintf(tbuf, "\t\t%d\t\t%u\n", temp.startCluster, temp.fileSize);
				output += tbuf;
			}
		}
	} else {
		int index = cwdIndex;
		//cout << "Normal Dir, sector = " << index << endl;
		for(char *physpointer = cwd; (index&0xFF8) != 0xFF8; index = getFAT(index), physpointer = mountedFile+(index+clusterStart)*SECTOR) {
			//cout << "Got next sector, " << index << endl;
			for(unsigned int j = 0; j < SECTOR/sizeof(direntry); j++) {
				//cout << "Got next entry, " << j << endl;
				direntry temp;
				memcpy(&temp, physpointer+(j*sizeof(direntry)), sizeof(direntry));
				if(temp.Name[0] == 0xe5) continue; // pass this entry
				if(!temp.Name[0]) goto breakout; // done with directory
				char tbuf[100];
				//sprintf(tbuf, "%s.%s\t", temp.Name, temp.Extension);
				memcpy(tbuf, temp.Name, 8);
				tbuf[8] = '.';
				memcpy(tbuf+9, temp.Extension, 3);
				tbuf[12] = '\t';
				tbuf[13] = 0;
				output += tbuf;
				sprintf(tbuf, "%02d:%02d:%02d\t", ((temp.Time & 0xF100) >> 11), ((temp.Time & 0x07E0) >> 5),
    					(temp.Time & 0x001F));
				output += tbuf;
				sprintf(tbuf, "%02d/%02d/%04d\t\t", (temp.Date & 0x001F), ((temp.Date & 0x01E0) >> 5),
    					(((temp.Date & 0xFE00) >> 9) + 1980));
				output += tbuf;
				sprintf(tbuf, "%c%c%c%c%c%c", (temp.Attributes & READ_ONLY)?'R':' ', (temp.Attributes & HIDDEN)?'H':' ',
							(temp.Attributes & SYSTEM)?'S':' ', (temp.Attributes & DIRECTORY)?'D':' ',
							(temp.Attributes & ARCHIVE)?'A':' ', (temp.Attributes & VOLUME)?'V':' ');
				output += tbuf;
				sprintf(tbuf, "\t\t%d\t\t%u\n", temp.startCluster, temp.fileSize);
				output += tbuf;
			}
		}
	}
	breakout:
	output += "\n\0";
	char *retval = (char *) malloc(output.size()+1);
	memcpy(retval, output.c_str(), output.size()+1);
	return retval;
}

// change to root if cluster is 0
// changing from: root !has .. dir when created, so taken care of elsewhere
int fdChangeDir(char *dir) {
	char locdir[11];
	memcpy(locdir, dir, 11);
	// Everything will already be at 11 chars, as per help index
	
	//search for file in directory
	if(cwd == rootDir && cwdIndex == -1) { // in root dir
		for(int i = 0; i < rootLen; i++) {
			for(unsigned int j = 0; j < SECTOR/sizeof(direntry); j++) {
				direntry temp;
				memcpy(&temp, rootDir+(i*SECTOR)+(j*sizeof(direntry)), sizeof(direntry));
				if(temp.Name[0] == 0xe5) continue; // pass this entry
				if(!temp.Name[0]) goto breakout; // done with directory

				if(!memcmp(temp.Name, locdir, 11)) { // found it
					if(temp.Attributes & DIRECTORY) { // and it really is a directory
						cwdIndex = temp.startCluster;
						//cout << "CD: " << dir << " start sector = " << cwdIndex << endl;
						if(cwdIndex) {
							// -2 for the two reserved entries
							cwd = ((cwdIndex)+clusterStart)*SECTOR+mountedFile;
							//printf("CWD is %05X in the file.\n", ((cwdIndex-2)+clusterStart)*SECTOR);
							//printf("Cluster start = %d, rootLen = %d\n", clusterStart, rootLen);
							// 19 = start of root, + rootLen is start of clusters, + cwdIndex
							// is depth of clusters, * clusters in, + memory start offset
						}
						else { // root
							cwd = rootDir;
						}
						return 1; // Will set up parent DIRs as just another dir, so uniform change
					}
					else return 0;
				}
			}
		}
	} else { // in normal dir
		int index = cwdIndex;
		for(char *physpointer = cwd; (index&0xFF8) != 0xFF8; index = getFAT(index), physpointer = mountedFile+(index+clusterStart)*SECTOR) {
			for(unsigned int j = 0; j < SECTOR/sizeof(direntry); j++) {
				direntry temp;
				memcpy(&temp, physpointer+(j*sizeof(direntry)), sizeof(direntry));
				if(temp.Name[0] == 0xe5) continue; // pass this entry
				if(!temp.Name[0]) goto breakout; // done with directory
				
				if(!memcmp(temp.Name, locdir, 11)) { // found it
					if(temp.Attributes & DIRECTORY) { // and it really is a directory
						cwdIndex = temp.startCluster;
						//cout << "CD: " << dir << " start sector = " << cwdIndex << endl;
						if(cwdIndex) {
							// -2 for the two reserved entries
							cwd = ((cwdIndex)+clusterStart)*SECTOR+mountedFile; 
							//printf("CWD is %05X in the file.\n", ((cwdIndex-2)+clusterStart)*SECTOR);
							//printf("Cluster start = %d, rootLen = %d\n", clusterStart, rootLen);
	     					// 19 = start of root, + rootLen is start of clusters, + cwdIndex
	     					// is depth of clusters, * clusters in, + memory start offset
	 					}
	 					else { // root
	 						cwd = rootDir;
							cwdIndex = -1;
	 					}
	     				return 1; // Will set up parent DIRs as just another dir, so uniform change
					}
					else return 0;
				}
			}
		}
	}
	breakout:
	return 0;
}

int fdDelDir(char *dirname) {
	char locdir[11];
	memcpy(locdir, dirname, 11);
	
	// Need these two declared anyway
	char *reserved1 = "..         ";
	char *reserved2 = ".          ";
	
	// disallow deleting . and ..
	if(!memcmp(reserved1, locdir, 11) || !memcmp(reserved2, locdir, 11))
		return 0;
	
	//cout << "In Del Dir" << endl;
	if(cwd == rootDir && cwdIndex == -1) { // in root dir
		//cout << "Root" << endl;
		for(int i = 0; i < rootLen; i++) {
			for(unsigned int j = 0; j < SECTOR/sizeof(direntry); j++) {
				direntry temp;
				memcpy(&temp, rootDir+(i*SECTOR)+(j*sizeof(direntry)), sizeof(direntry));
				if(temp.Name[0] == 0xe5) continue; // pass this entry
				if(!temp.Name[0]) goto breakout; // done with directory

				if(!memcmp(temp.Name, locdir, 11)) { // found it
					if(temp.Attributes & DIRECTORY) { // and it really is a directory
						//cout << "Found a directory of that name" << endl;
						//cout << "Checking for files" << endl;
						int index = temp.startCluster;
						for(char *dirpointer = ((index)+clusterStart)*SECTOR+mountedFile; (index&0xFF8) != 0xFF8;
							index = getFAT(index), dirpointer = mountedFile+(index+clusterStart)*SECTOR) {
							for(unsigned int k = 0; k < SECTOR/sizeof(direntry); k++) {
								direntry temp1;
								memcpy(&temp1, dirpointer+(k*sizeof(direntry)), sizeof(direntry));
								if(temp1.Name[0] == 0xe5) continue; // pass this entry
								if(!temp1.Name[0]) goto topbreak; // done with directory
								
								//cout << "S" << index << " E" << k << ":" << endl;
								if(!memcmp(reserved1, temp1.Name, 11) || !memcmp(reserved2, temp1.Name, 11))
									continue; //they don't count
								else {
									//cout << "Is a file!" << endl;
									return 0; // file in dir, can't delete
								}
							}
						}
						topbreak: // if here, been through dir, safe to delete
						//temp.Name[0] = 0xe5; // clear out the dir entry
						rootDir[(i*SECTOR)+(j*sizeof(direntry))] = 0xe5;
						
						// clear out the cluster chain
						index = temp.startCluster;
						while((index&0xFF8) != 0xFF8) {
							int temp = index;
							index = getFAT(index);
							setFAT(temp, 0);
						}
						return 1;
					}
					else return 0;
				}
			}
		}
	} else { // in normal dir
		//cout << "Normal dir" << endl;
		int index = cwdIndex;
		for(char *physpointer = cwd; (index&0xFF8) != 0xFF8; index = getFAT(index), physpointer = mountedFile+(index+clusterStart)*SECTOR) {
			for(unsigned int j = 0; j < SECTOR/sizeof(direntry); j++) {
				direntry temp;
				memcpy(&temp, physpointer+(j*sizeof(direntry)), sizeof(direntry));
				if(temp.Name[0] == 0xe5) continue; // pass this entry
				if(!temp.Name[0]) goto breakout; // done with directory
				
				if(!memcmp(temp.Name, locdir, 11)) { // found it
					if(temp.Attributes & DIRECTORY) { // and it really is a directory
						//cout << "Found a directory of that name" << endl;
						//cout << "Checking for files" << endl;
						int index = temp.startCluster;
						for(char *dirpointer = ((index)+clusterStart)*SECTOR+mountedFile; (index&0xFF8) != 0xFF8;
							index = getFAT(index), dirpointer = mountedFile+(index+clusterStart)*SECTOR) {
							for(unsigned int k = 0; k < SECTOR/sizeof(direntry); k++) {
								direntry temp1;
								memcpy(&temp1, dirpointer+(k*sizeof(direntry)), sizeof(direntry));
								//cout << "S" << index << " E" << k << ":" << endl;
								if(temp1.Name[0] == 0xe5) continue; // pass this entry
								if(!temp1.Name[0]) goto lowbreak; // done with directory
								
								if(!memcmp(reserved1, temp1.Name, 11) || !memcmp(reserved2, temp1.Name, 11))
									continue; //they don't count
								else {
									//cout << "Is a file!" << endl;
									return 0; // file in dir, can't delete
								}
							}
						}
						lowbreak: // if here, been through dir, safe to delete
						//cout << "Was the end of the DIR" << endl;
						//temp.Name[0] = 0xe5; // clear out the dir entry
						physpointer[(j*sizeof(direntry))] = 0xe5;
						
						// clear out the cluster chain
						index = temp.startCluster;
						while((index&0xFF8) != 0xFF8) {
							int temp = index;
							index = getFAT(index);
							setFAT(temp, 0);
						}
						return 1;
					}
					else return 0;
				}
			}
		}
	}
	breakout:
	return 0;
}

// returns the index in the fat of a 0
// perhaps make more complicated?
int getFree() {
	for(int i = 2; i < (((SECTOR*9)*2)/3); i++) // for each possible FAT entry
		if(!getFAT(i)) return i; // first 2 skipped, just in case
	return -1;
}

// This function will create an empty directory named dirname.
int fdMakeDir(char *dirname) {
	// Verify that dirname does not exist in the current directory.
	// Check for available space on the volume to create the directory.
	// Create an empty directory that contains the two standard directory pointer entries: 
	//		dot [.], a "this" directory entry pointer to the current directory; and double-dot [..],
	//		a directory entry pointer to the parent directory.
	// Write the entry for dirname in the current directory. 
	//		Make sure that your directories can "grow" in size, if the cluster that contains the directory is full.
	
	char locdir[11];
	memcpy(locdir, dirname, 11);
	if(locdir[0] == '.') return 0; //leading periods not allowed
		
	char *reserved1 = "..         ";
	char *reserved2 = ".          ";
	
	// gonna do time and date up here, a bit of a pain
	short Time;
	short Date; // not working right, month is
	{
		time_t timeret = time(0);
		struct tm *tempt = localtime(&timeret);
		char timebuf[100];
		strftime(timebuf, 100, "%H", tempt);
		short Hour = atoi(timebuf)+1;
		strftime(timebuf, 100, "%M", tempt);
		short Min = atoi(timebuf);
		strftime(timebuf, 100, "%S", tempt);
		short Sec = atoi(timebuf);
		Time = Hour*2048+Min*32+Sec/2;
		
		strftime(timebuf, 100, "%Y", tempt);
		short Year = atoi(timebuf);
		strftime(timebuf, 100, "%m", tempt);
		short Mon = atoi(timebuf);
		strftime(timebuf, 100, "%d", tempt);
		short Day = atoi(timebuf);
		
		//printf("%d/%d/%d\n", Year, Mon, Day);
		Date = (Year-1980)*512 + Mon*32 + Day;
	}
	
	//cout << "Make Dir" << endl;
	//search for file in directory, make sure not there
	if(cwd == rootDir && cwdIndex == -1) { // in root dir
		//cout << "In root dir" << endl;
		for(int i = 0; i < rootLen; i++) {
			for(unsigned int j = 0; j < SECTOR/sizeof(direntry); j++) {
				direntry temp;
				memcpy(&temp, rootDir+(i*SECTOR)+(j*sizeof(direntry)), sizeof(direntry));
				if(temp.Name[0] == 0xe5) continue; // pass this entry
				if(!temp.Name[0]) goto breakroot; // done with directory
				
				//cout << "S" << i << " E" << j << " taken" << endl;
				
				if(!memcmp(temp.Name, locdir, 11)) { // found it
					//cout << "Already there" << endl;
					return 0; // already exists
				}
			}
		}
		breakroot: // if got here, can make the file
		//cout << "Name not taken" << endl;
		for(int i = 0; i < rootLen; i++) {
			for(unsigned int j = 0; j < SECTOR/sizeof(direntry); j++) {
				direntry temp;
				memcpy(&temp, rootDir+(i*SECTOR)+(j*sizeof(direntry)), sizeof(direntry));
				if(temp.Name[0] == 0xe5 || !temp.Name[0]) {
					//cout << "Found an empty slot: S" << i << " E" << j << endl;
					int freespace = getFree();
					if(freespace == -1) return 0;
					temp.startCluster = freespace;
					setFAT(freespace, 0xff8); // Also forgot to EOC the cluster
					//cout << "Free cluster = " << freespace << endl;
					
					memcpy(temp.Name, locdir, 11); // name + extension
					temp.Attributes = DIRECTORY; // only one I will be setting here
					temp.Time = Time;
					temp.Date = Date;
					temp.fileSize = 0;
					
					// Must writeback now, since I did everything indirectly
					memcpy(rootDir+(i*SECTOR)+(j*sizeof(direntry)), &temp, sizeof(direntry));

					// Zero the whole directory first -- should work
					memset(mountedFile+(temp.startCluster+clusterStart)*SECTOR, 0, SECTOR);
					
					// Also, set up the two other directories
					direntry setup;
					memcpy(&setup, mountedFile+(temp.startCluster+clusterStart)*SECTOR, sizeof(direntry));
					memcpy(setup.Name, reserved2, 11);
					setup.fileSize = 0;
					setup.startCluster = temp.startCluster;
					setup.Time = Time;
					setup.Date = Date;
					setup.Attributes = DIRECTORY;
					memcpy(mountedFile+(temp.startCluster+clusterStart)*SECTOR, &setup, sizeof(direntry));
					
					memcpy(&setup, mountedFile+(temp.startCluster+clusterStart)*SECTOR+sizeof(direntry), sizeof(direntry));
					setup.fileSize = 0;
					setup.startCluster = 0;
					setup.Time = Time;
					setup.Date = Date;
					memcpy(setup.Name, reserved1, 11);
					setup.Attributes = DIRECTORY;
					memcpy(mountedFile+(temp.startCluster+clusterStart)*SECTOR+sizeof(direntry), &setup, sizeof(direntry));
					
					//cout << "Dir created" << endl;
					return 1;
				}
			}
		}
		// If we get to here, need to make the directory bigger, but is root...
		//cout << "Cannot resize root" << endl;
		return 0;
	} else { // in normal dir
		//cout << "In normal dir" << endl;
		int index = cwdIndex;
		for(char *physpointer = cwd; (index&0xFF8) != 0xFF8; index = getFAT(index), physpointer = mountedFile+(index+clusterStart)*SECTOR) {
			for(unsigned int j = 0; j < SECTOR/sizeof(direntry); j++) {
				direntry temp;
				memcpy(&temp, physpointer+(j*sizeof(direntry)), sizeof(direntry));
				if(temp.Name[0] == 0xe5) continue; // pass this entry
				if(!temp.Name[0]) goto breaknormal; // done with directory
				
				//cout << "S" << index << " E" << j << " taken" << endl;
				
				if(!memcmp(temp.Name, locdir, 11)) { // found it
					return 0; // already exists
				}
			}
		}
		breaknormal: // if got here, can make the file
		index = cwdIndex;
		for(char *physpointer = cwd; (index&0xFF8) != 0xFF8; index = getFAT(index), physpointer = mountedFile+(index+clusterStart)*SECTOR) {
			for(unsigned int j = 0; j < SECTOR/sizeof(direntry); j++) {
				direntry temp;
				memcpy(&temp, physpointer+(j*sizeof(direntry)), sizeof(direntry));
				if(temp.Name[0] == 0xe5 || !temp.Name[0]) {					
					int freespace = getFree();
					if(freespace == -1) return 0;
					temp.startCluster = freespace;
					setFAT(freespace, 0xff8); // Also forgot to EOC the cluster
					
					//cout << "Using S" << index << " E" << j << ", cluster " << freespace << endl;
					
					memcpy(temp.Name, locdir, 11); // name + extension
					temp.Attributes = DIRECTORY; // only one I will be setting here
					temp.Time = Time;
					temp.Date = Date;
					temp.fileSize = 0;
					
					// Must writeback now, since I did everything indirectly
					memcpy(physpointer+(j*sizeof(direntry)), &temp, sizeof(direntry));
					
					// Zero the whole directory first -- should work
					memset(mountedFile+(temp.startCluster+clusterStart)*SECTOR, 0, SECTOR);
					
					// Also, set up the two other directories
					direntry setup;
					
					memcpy(&setup, mountedFile+(temp.startCluster+clusterStart)*SECTOR, sizeof(direntry));
					memcpy(setup.Name, reserved2, 11);
					setup.fileSize = 0;
					setup.startCluster = temp.startCluster;
					setup.Time = Time;
					setup.Date = Date;
					setup.Attributes = DIRECTORY;
					memcpy(mountedFile+(temp.startCluster+clusterStart)*SECTOR, &setup, sizeof(direntry));
		
					memcpy(&setup, mountedFile+(temp.startCluster+clusterStart)*SECTOR+sizeof(direntry), sizeof(direntry));
					memcpy(setup.Name, reserved1, 11);
					setup.fileSize = 0;
					setup.startCluster = cwdIndex;
					setup.Time = Time;
					setup.Date = Date;
					setup.Attributes = DIRECTORY;
					memcpy(mountedFile+(temp.startCluster+clusterStart)*SECTOR+sizeof(direntry), &setup, sizeof(direntry));
					
					//cout << "Dir Created" << endl;
					return 1;
				}
			}
		}
		// If we get to here, need to make the directory bigger
		index = cwdIndex;
		int preindex;
		do {
			preindex = index;
			index = getFAT(index);
		} while((index&0xFF8) != 0xFF8);
		index = getFree();
		if(index == -1) return 0;
		setFAT(preindex, index);
		setFAT(index, 0xff8);
		
		// Zero the whole directory cluster
		memset(mountedFile+(index+clusterStart)*SECTOR, 0, SECTOR);
		
		direntry temp;
		
		int freespace = getFree();
		if(freespace == -1) return 0;
		temp.startCluster = freespace; // for the subdirectory now
		setFAT(freespace, 0xff8); // Also forgot to EOC the cluster

		memcpy(temp.Name, locdir, 11);
		temp.Attributes = DIRECTORY;
		temp.Time = Time;
		temp.Date = Date;
		temp.fileSize = 0;
		
		// Never even got this from memory, why didn't I see it???
		memcpy(mountedFile+(index+clusterStart)*SECTOR, &temp, sizeof(direntry));
		
		// Zero the whole directory first -- should work
		memset(mountedFile+(temp.startCluster+clusterStart)*SECTOR, 0, SECTOR);
		
    	// Also, set up the two other directories
		direntry setup;
		memcpy(&setup, mountedFile+(temp.startCluster+clusterStart)*SECTOR, sizeof(direntry));
		memcpy(setup.Name, reserved2, 11);
		setup.fileSize = 0;
		setup.startCluster = temp.startCluster;
		setup.Time = Time;
		setup.Date = Date;
		setup.Attributes = DIRECTORY;
		memcpy(mountedFile+(temp.startCluster+clusterStart)*SECTOR, &setup, sizeof(direntry));
		
		memcpy(&setup, mountedFile+(temp.startCluster+clusterStart)*SECTOR+sizeof(direntry), sizeof(direntry));
		setup.fileSize = 0;
		setup.startCluster = cwdIndex;
		setup.Time = Time;
		setup.Date = Date;
		memcpy(setup.Name, reserved1, 11);
		setup.Attributes = DIRECTORY;
		memcpy(mountedFile+(temp.startCluster+clusterStart)*SECTOR+sizeof(direntry), &setup, sizeof(direntry));
		
		return 1;
	}
	return 0;
}

int fdEraseFile(char *filename) {
	// Verify that filename exists in the current directory, and that it is a valid file.
	// Can't do directories
	// Zero out the file's FAT cluster chain.
	// Modify the file's directory entry to indicate that it has been deleted
	char locfile[11];
	memcpy(locfile, filename, 11);
	
	//cout << "In Del File" << endl;
	if(cwd == rootDir && cwdIndex == -1) { // in root dir
		//cout << "Root" << endl;
		for(int i = 0; i < rootLen; i++) {
			for(unsigned int j = 0; j < SECTOR/sizeof(direntry); j++) {
				direntry temp;
				memcpy(&temp, rootDir+(i*SECTOR)+(j*sizeof(direntry)), sizeof(direntry));
				if(temp.Name[0] == 0xe5) continue; // pass this entry
				if(!temp.Name[0]) goto breakout; // done with directory

				if(!memcmp(temp.Name, locfile, 11)) { // found it
					if(temp.Attributes & ARCHIVE) { // and it really is a file
						//cout << "Found a file of that name" << endl;
						
						//temp.Name[0] = 0xe5; // clear out the dir entry
						rootDir[(i*SECTOR)+(j*sizeof(direntry))] = 0xe5;
						
						// clear out the cluster chain
						int index = temp.startCluster;
						while((index&0xFF8) != 0xFF8) {
							int temp = index;
							index = getFAT(index);
							setFAT(temp, 0);
						}
						return 1;
					}
					else return 0;
				}
			}
		}
	} else { // in normal dir
		//cout << "Normal dir" << endl;
		int index = cwdIndex;
		for(char *physpointer = cwd; (index&0xFF8) != 0xFF8; index = getFAT(index), physpointer = mountedFile+(index+clusterStart)*SECTOR) {
			for(unsigned int j = 0; j < SECTOR/sizeof(direntry); j++) {
				direntry temp;
				memcpy(&temp, physpointer+(j*sizeof(direntry)), sizeof(direntry));
				if(temp.Name[0] == 0xe5) continue; // pass this entry
				if(!temp.Name[0]) goto breakout; // done with directory
				
				if(!memcmp(temp.Name, locfile, 11)) { // found it
					if(temp.Attributes & ARCHIVE) { // and it really is a file
						//cout << "Found a file of that name" << endl;
						
						//temp.Name[0] = 0xe5; // clear out the dir entry
						physpointer[(j*sizeof(direntry))] = 0xe5;
						
						// clear out the cluster chain
						index = temp.startCluster;
						while((index&0xFF8) != 0xFF8) {
							int temp = index;
							index = getFAT(index);
							setFAT(temp, 0);
						}
						return 1;
					}
					else return 0;
				}
			}
		}
	}
	breakout:
	return 0;
}

int fdCopyFile(char *sourcefile, char *destfile) {
	// sourcefile is assumed to be located in the current directory. destfile should be created in the current directory.
	// Verify that sourcefile exists in the current directory, and that it is a valid file.
	// Verify that destfile is a valid file name.
	// Certify that destfile does not exist in the current directory.
	// Check for available space on the volume to perform the copy. Fail if there is not enough space!
	// Copy the sectors from sourcefile to destfile.
	// Write the FAT cluster chain for destfile.
	// Write the directory entry for destfile in the current directory. 
	//		Make sure that your directories can "grow" in size, if the cluster that contains the directory is full.
	
	// gonna do time and date up here, a bit of a pain
	short Time;
	short Date;
	{
		time_t timeret = time(0);
		struct tm *tempt = localtime(&timeret);
		char timebuf[100];
		strftime(timebuf, 100, "%H", tempt);
		short Hour = atoi(timebuf)+1;
		strftime(timebuf, 100, "%M", tempt);
		short Min = atoi(timebuf);
		strftime(timebuf, 100, "%S", tempt);
		short Sec = atoi(timebuf);
		Time = Hour*2048+Min*32+Sec/2;
		
		strftime(timebuf, 100, "%Y", tempt);
		short Year = atoi(timebuf);
		strftime(timebuf, 100, "%m", tempt);
		short Mon = atoi(timebuf);
		strftime(timebuf, 100, "%d", tempt);
		short Day = atoi(timebuf);
		
		//printf("%d/%d/%d\n", Year, Mon, Day);
		Date = (Year-1980)*512 + Mon*32 + Day;
	}
	
	//cout << "Copy File" << endl;
	//search for file in directory, make sure not there
	if(cwd == rootDir && cwdIndex == -1) { // in root dir
		//cout << "In root dir" << endl;
		bool source = false;
		direntry sourceentry;
		for(int i = 0; i < rootLen; i++) {
			for(unsigned int j = 0; j < SECTOR/sizeof(direntry); j++) {
				direntry temp;
				memcpy(&temp, rootDir+(i*SECTOR)+(j*sizeof(direntry)), sizeof(direntry));
				if(temp.Name[0] == 0xe5) continue; // pass this entry
				if(!temp.Name[0]) goto breakroot; // done with directory
				
				//cout << "S" << i << " E" << j << " taken" << endl;
				
				if(!memcmp(temp.Name, destfile, 11)) { // found what shouldn't exist
					//cout << "Already there" << endl;
					return 0; // already exists
				}
				if(!memcmp(temp.Name, sourcefile, 11)) { // found what should exist
					source = true;
					memcpy(&sourceentry, &temp, sizeof(direntry));
				}
			}
		}
		breakroot: // if got here, can make the file
		if(!source) return 0; // source file !exists
		
		//cout << "Name not taken" << endl;
		
		// Find first free slot in dir
		for(int i = 0; i < rootLen; i++) {
			for(unsigned int j = 0; j < SECTOR/sizeof(direntry); j++) {
				direntry temp;
				memcpy(&temp, rootDir+(i*SECTOR)+(j*sizeof(direntry)), sizeof(direntry));
				if(temp.Name[0] == 0xe5 || !temp.Name[0]) { // got it
					//cout << "Found an empty slot: S" << i << " E" << j << endl;
					int freespace = getFree();
					if(freespace == -1) return 0;
					temp.startCluster = freespace;
					setFAT(freespace, 0xff8);
					//cout << "Free cluster = " << freespace << endl;
					
					memcpy(temp.Name, destfile, 11); // name + extension
					temp.Attributes = ARCHIVE; // only one I will be setting here
					temp.Time = Time;
					temp.Date = Date;
					temp.fileSize = sourceentry.fileSize;
					
					// Must writeback now, since I did everything indirectly
					memcpy(rootDir+(i*SECTOR)+(j*sizeof(direntry)), &temp, sizeof(direntry));
					
					// Now copy each cluster of the source file
					int sindex = sourceentry.startCluster;
					int dindex = freespace;
					memcpy(mountedFile+(dindex+clusterStart)*SECTOR, mountedFile+(sindex+clusterStart)*SECTOR, SECTOR); // copy a sector chunk
					
					while(((sindex = getFAT(sindex))&0xFF8) != 0xFF8) { // for each cluster in the initial file
						int freespace = getFree();
						if(freespace == -1) return 0;
						setFAT(freespace, 0xff8);
						setFAT(dindex, freespace);
						dindex = freespace;
						memcpy(mountedFile+(dindex+clusterStart)*SECTOR, mountedFile+(sindex+clusterStart)*SECTOR, SECTOR); // copy a sector chunk
					}
					
					return 1;
				}
			}
		}
		// If we get to here, need to make the directory bigger, but is root...
		//cout << "Cannot resize root" << endl;
		return 0;
	} else { // in normal dir
		//cout << "In normal dir" << endl;
		int index = cwdIndex;
		bool source = false;
		direntry sourceentry;
		for(char *physpointer = cwd; (index&0xFF8) != 0xFF8; index = getFAT(index), physpointer = mountedFile+(index+clusterStart)*SECTOR) {
			for(unsigned int j = 0; j < SECTOR/sizeof(direntry); j++) {
				direntry temp;
				memcpy(&temp, physpointer+(j*sizeof(direntry)), sizeof(direntry));
				if(temp.Name[0] == 0xe5) continue; // pass this entry
				if(!temp.Name[0]) goto breaknormal; // done with directory
				
				//cout << "S" << index << " E" << j << " taken" << endl;
				
				if(!memcmp(temp.Name, destfile, 11)) { // found what shouldn't exist
					//cout << "Already there" << endl;
					return 0; // already exists
				}
				if(!memcmp(temp.Name, sourcefile, 11)) { // found what should exist
					source = true;
					memcpy(&sourceentry, &temp, sizeof(direntry));
				}
			}
		}
		breaknormal: // if got here, can make the file
		if(!source) return 0;
			
		//cout << "Name not taken" << endl;
		
		// find first free slot in dir
		index = cwdIndex;
		for(char *physpointer = cwd; (index&0xFF8) != 0xFF8; index = getFAT(index), physpointer = mountedFile+(index+clusterStart)*SECTOR) {
			for(unsigned int j = 0; j < SECTOR/sizeof(direntry); j++) {
				direntry temp;
				memcpy(&temp, physpointer+(j*sizeof(direntry)), sizeof(direntry));
				if(temp.Name[0] == 0xe5 || !temp.Name[0]) {					
					int freespace = getFree();
					if(freespace == -1) return 0;
					temp.startCluster = freespace;
					setFAT(freespace, 0xff8);
					//cout << "Free cluster = " << freespace << endl;
					
					memcpy(temp.Name, destfile, 11); // name + extension
					temp.Attributes = ARCHIVE; // only one I will be setting here
					temp.Time = Time;
					temp.Date = Date;
					temp.fileSize = sourceentry.fileSize;
					
					// Must writeback now, since I did everything indirectly
					memcpy(physpointer+(j*sizeof(direntry)), &temp, sizeof(direntry));
					
					// Now copy each cluster of the source file
					int sindex = sourceentry.startCluster;
					int dindex = freespace;
					memcpy(mountedFile+(dindex+clusterStart)*SECTOR, mountedFile+(sindex+clusterStart)*SECTOR, SECTOR); // copy a sector chunk
					
					while(((sindex = getFAT(sindex))&0xFF8) != 0xFF8) { // for each cluster in the initial file
						int freespace = getFree();
						if(freespace == -1) return 0;
						setFAT(freespace, 0xff8);
						setFAT(dindex, freespace);
						dindex = freespace;
						memcpy(mountedFile+(dindex+clusterStart)*SECTOR, mountedFile+(sindex+clusterStart)*SECTOR, SECTOR); // copy a sector chunk
					}
					
					return 1;
				}
			}
		}
		// If we get to here, need to make the directory bigger
		index = cwdIndex;
		int preindex;
		do {
			preindex = index;
			index = getFAT(index);
		} while((index&0xFF8) != 0xFF8);
		index = getFree();
		if(index == -1) return 0;
		setFAT(preindex, index);
		setFAT(index, 0xff8);
		
		// Zero the whole directory cluster
		memset(mountedFile+(index+clusterStart)*SECTOR, 0, SECTOR);
		
		direntry temp;
		
		int freespace = getFree();
		if(freespace == -1) return 0;
		temp.startCluster = freespace; // for the file now
		setFAT(freespace, 0xff8);

		memcpy(temp.Name, destfile, 11);
		temp.Attributes = ARCHIVE;
		temp.Time = Time;
		temp.Date = Date;
		temp.fileSize = sourceentry.fileSize;
		memcpy(mountedFile+(index+clusterStart)*SECTOR, &temp, sizeof(direntry));
		
		// Now copy each cluster of the source file
		int sindex = sourceentry.startCluster;
		int dindex = freespace;
		memcpy(mountedFile+(dindex+clusterStart)*SECTOR, mountedFile+(sindex+clusterStart)*SECTOR, SECTOR); // copy a sector chunk
		
		while(((sindex = getFAT(sindex))&0xFF8) != 0xFF8) { // for each cluster in the initial file
			int freespace = getFree();
			if(freespace == -1) return 0;
			setFAT(freespace, 0xff8);
			setFAT(dindex, freespace);
			dindex = freespace;
			memcpy(mountedFile+(dindex+clusterStart)*SECTOR, mountedFile+(sindex+clusterStart)*SECTOR, SECTOR); // copy a sector chunk
		}
		return 1;
	}
	return 0;
}

// copied the TA .so behavior
char* fdGetFat() {
	// Copy your formatted FAT table into the character array.
	// Return the allocated string (using new) for success, NULL on failure
	string output;
	output.reserve(200);
	char linebuf[200]; // bigger, prolly need it
	sprintf(linebuf, "     %5d%5d%5d%5d%5d%5d%5d%5d%5d%5d\n", 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
	output += linebuf;
	sprintf(linebuf, "     +--------------------------------------------------");
	output += linebuf;
	
	for(int i = 0; i < (((SECTOR*9)*2)/3); i++) { // for each possible FAT entry
		if(!(i % 10)) { // a multiple of 10
			sprintf(linebuf, "\n%4d |", i);
			output += linebuf;
		}
		int fat = getFAT(i);
		if((fat & 0xFF8) == 0xFF8)
			output += "  EOC"; // pad to 5
		else if(fat == 0xFF7)
			output += "  BAD";
		else if((fat & 0xFF0) == 0xFF0)
			output += " RSRV";
		else if(!fat)
			output += "    0"; // why a 0 for unused?
		else {
			sprintf(linebuf, "%5u", fat);
			output += linebuf;
		}
	}
	
	char *retval = (char *) malloc(output.size()+1);
	memcpy(retval, output.c_str(), output.size()+1);
	return retval;
}

///* Special test function, main
//#define TEST 0
#ifdef TEST
int main() {
	char *filename = "./floppy.img";
	cout << "Mounting image." << endl;
	assert(fdLoadMntPt(filename));
	/* Testing DIR and CD
	cout << "Dir:" << endl << fdListDir() << endl;
	char *dirname1 = "DIR1       ";
	cout << "CD " << dirname1 << endl;
	assert(fdChangeDir(dirname1));
	cout << "Dir:" << endl << fdListDir() << endl;
	char *dirname2 = "..         ";
	cout << "CD " << dirname2 << endl;
	assert(fdChangeDir(dirname2));
	cout << "Dir:" << endl << fdListDir() << endl;
	char *dirname3 = "FILE1      ";
	assert(!fdChangeDir(dirname3));
	//*/
	/* Testing getFat
	cout << "Fat:" << endl << fdGetFat() << endl;
	//*/
	/* Testing make/del dir
	char *dirname4 = "GEDDIR     ";
	assert(fdMakeDir(dirname4)); // make dir in root
	cout << "Dir:" << endl << fdListDir() << endl;
	assert(fdChangeDir(dirname4));
	char *dirname5 = "GEDDIR2    ";
	assert(fdMakeDir(dirname5)); // make dir not in root
	cout << "Dir:" << endl << fdListDir() << endl;
	char *dirname6 = "..         ";	
	assert(fdChangeDir(dirname6));
	assert(!fdDelDir(dirname4)); // delete non-empty dir
	cout << "Dir:" << endl << fdListDir() << endl;
	assert(fdChangeDir(dirname4));
	assert(fdDelDir(dirname5)); // delete empty dir from non root
	cout << "Dir:" << endl << fdListDir() << endl;
	assert(fdChangeDir(dirname6));
	assert(fdDelDir(dirname4)); // delete empty dir from root
	cout << "Dir:" << endl << fdListDir() << endl;
	//*/
	/* Testing Copying/deleting files
	cout << "Dir:" << endl << fdListDir() << endl;
	char *filename1 = "GEDFILE    ";
	char *filename2 = "FILE1      ";
	char *filename3 = "NOTAFILE   ";
	char *dirname7 = "DIR2       ";
	
	assert(fdCopyFile(filename2, filename1)); // copy in root
	assert(!fdCopyFile(filename2, filename1)); // bad dest in root
	assert(!fdCopyFile(filename3, filename1)); // bad source in root
	cout << "Dir:" << endl << fdListDir() << endl;
	assert(fdEraseFile(filename1)); // delete in root
	assert(!fdEraseFile(filename1)); // better not delete same file twice
	assert(!fdEraseFile(filename3)); // bad erase in root
	assert(!fdEraseFile(dirname7)); // Is a directory
	cout << "Dir:" << endl << fdListDir() << endl;
	
	assert(fdChangeDir(dirname7)); // now for another dir
	cout << "Dir:" << endl << fdListDir() << endl;
	assert(fdCopyFile(filename2, filename1)); // copy in subdirectory
	assert(!fdCopyFile(filename2, filename1)); // bad dest
	assert(!fdCopyFile(filename3, filename1)); // bad source
	cout << "Dir:" << endl << fdListDir() << endl;
	assert(fdEraseFile(filename1)); // good erase
	assert(!fdEraseFile(filename1)); // bad erase (same file)
	assert(!fdEraseFile(filename3)); // bad erase (no file)
	cout << "Dir: " << endl << fdListDir() << endl;
	//*/
	
	///* TA test sequence!
	cout << "Dir:" << endl << fdListDir() << endl;
	getchar();
	
	char *TADir1 = "DIR2       ";
	assert(fdChangeDir(TADir1));
	getchar();
	
	char *TADir2 = "..         ";
	assert(fdChangeDir(TADir2));
	getchar();
	
	char *TADir3 = "FILE1   TXT";
	assert(!fdChangeDir(TADir3));
	getchar();
	
	
	char *TADir4 = "FILE1      ";
	assert(!fdChangeDir(TADir4));
	getchar();
	
	char *TADir5 = "FILE1   DIR";
	assert(fdChangeDir(TADir5));
	getchar();
	
	char *TAFile1 = "SHORT      ";
	assert(fdEraseFile(TAFile1));
	getchar();
	fdChangeDir(TADir2);
	
	char *TAFile2 = "DIR1       ";
	assert(!fdEraseFile(TAFile2));
	getchar();
	
	char *TADir6 = "FILE1   DIR";
	assert(fdDelDir(TADir6));
	getchar();
	
	char *TADir7 = "DIR1       ";
	assert(!fdDelDir(TADir7));
	getchar();
	fdChangeDir(TADir7);
	
	char *TADir8 = "TA      DIR";
	assert(fdMakeDir(TADir8));
	getchar();
	fdChangeDir(TADir2);
	fdChangeDir(TADir1);
	
	char *TADir9 = "TA_RULESDIR";
	assert(fdMakeDir(TADir9));
	getchar();
	
	getchar();
	cout << "Dir:" << endl << fdListDir() << endl;
	getchar();
	fdChangeDir(TADir9);
	
	getchar();
	cout << "Dir:" << endl << fdListDir() << endl;
	getchar();
	fdChangeDir(TADir2);
	fdChangeDir(TADir2);
	
	char *TAFile3 = "FILE1      ";
	char *TAFile4 = "TA_FILE TXT";
	assert(fdCopyFile(TAFile3, TAFile4));
	getchar();
	
	getchar();
	cout << "Dir:" << endl << fdListDir() << endl;
	getchar();
	fdChangeDir(TADir7);
	fdChangeDir(TADir1);
	
	char *TAFile5 = "FILE1   TXT";
	assert(fdCopyFile(TAFile5, TAFile4));
	getchar();
	
	getchar();
	cout << "Dir:" << endl << fdListDir() << endl;
	getchar();
	//*/
}
#endif
//*/

}
