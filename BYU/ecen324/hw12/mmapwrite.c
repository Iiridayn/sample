#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

void mmapwrite(int fd, int len) {
  /* use mmap to modify the disk file */
  char* file = (char *) mmap(0, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	file[0] = 'J'; // change first char to J
  munmap(file, len); // unmap it
}

int main(int argc, char **argv) {
  /* add required variable declarations here */
	int infile;
	struct stat data;

  /* check for required command line argument */
	if (argc != 2) {
		printf("usage: %s <filename>\n", argv[0]);
		exit(0);
  }

  /* (1) open the input file using open() */
	infile = open(argv[1], O_RDWR);
	  if(!infile) {
	    fprintf(stderr, "File didn't open right: %s\n", argv[1]);
		exit(0);
  }

  /* (2) get its size using fstat() */
	fstat(infile, &data);
	//data.st_size has the size

  /* (3) call mmapwrite() */
	mmapwrite(infile, data.st_size);


	/* clean up */
	close(infile);
  exit(0);
}
