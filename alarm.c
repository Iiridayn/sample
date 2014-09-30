#include <stdio.h>
#include <stdlib.h>
#include <termios.h> // for raw reading
#include <time.h>
#include <signal.h>

#define SNOOZE 5

void usage() {
	printf("alarm [time or # mins]\n");
	exit(0);
}

void readTime(int *hours, int *minutes) {
	char read;

	scanf("%d", minutes);
	read=getchar();
	if(!isspace(read))
	  if(read == ':') {
			*hours = *minutes;
	  	scanf("%d", minutes);
		}
		else usage();

	if(*hours > 23) {
		*minutes += 60 * *hours;
		*hours = -1;
	}
}

int readCommand(int *hours, int *minutes, char* input) {
	int i, hh=-1, mm=-1, white = 0;

	for(i = 0; input[i] && !white; i++) {
		switch(input[i]) {
			case ' ':
			case '\n':
				//return 1; // done, true
				white = 1;
				//input[i+1] = '\0'; // force quit, don't care about the rest anyway
				break;
			case ':':
				if(mm == -1) {
					fprintf(stderr, "Bad format string: %s\n", input);
					exit(1);
				}
				hh = mm;
				mm = -1;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if(mm == -1) mm = 0;
				mm *= 10;
				mm += input[i]-'0';
				break;
			default:
				fprintf(stderr, "Bad format string: %s\n", input);
				exit(1);
		} // switch
	} // for

	if(!white) return 0; // no space, format string not ready to be parsed

	if(hh == -1 && mm == -1) // we got whitespace first
		mm = SNOOZE;
	if(hh > 23) {
		mm += hh*60;
		hh = -1;
	}

	*hours = hh;
	*minutes = mm;
	return 1; //done, good parse
}

time_t alarmTime = 0;

void doneFun() {
	int active = time(NULL)-alarmTime+1;

	if(alarmTime == 0) exit(0);

	printf("\n\nLast alarm active for ");
	if(active > 3599) {
		printf("%d hours ", (int)active/3600);
		active %= 3600;
	}
	if(active > 59) {
		printf("%d minutes ", (int)active/60);
		active %= 60;
	}
	printf("%d seconds.\n", active);
	exit(0);
}

struct termios stored;

void restoreTerm() {
	tcsetattr (0, TCSANOW, &stored);
}

int timeUntil(int hh, int mm) { // all times are assumed to be in 24 hr format
	struct tm *Time;
	time_t secTime;
	int till = 0;

	secTime = time(NULL);
	Time = localtime(&secTime);

	if(hh < Time->tm_hour)
		till += 60 * (hh + 24-Time->tm_hour);
	else
		till += 60 * (hh - Time->tm_hour);

	if(mm < Time->tm_min) {
		if(!till)
			till = 60 * 24;
		till = (till - Time->tm_min) + mm;
	}
	else
		till += mm - Time->tm_min;

	/*
	// quick debug
	printf("The time is %02d:%02d.\nWe want %02d:%02d.\nWe sleep for %d hours, %d minutes.\n",
					Time->tm_hour, Time->tm_min, hh, mm, (int)till/60, till%60);
	*/
	return till;
}

int main(int argc, char* argv[]) {
	// vars section
	int hh=-1, mm=-1/*, ss*/;
	struct termios tattr;
	char inchar;//[10];
	char buf[80];
	int i;

	signal(SIGINT, doneFun);
	tcgetattr (0, &stored);
	atexit(restoreTerm); // restore normal terminal behavior when done

	// read input
	switch(argc) {
		case 1: //get from terminal
			printf("%c", 12); //blank terminal

			// Set so reads chars 1 at a time
			tcgetattr (0, &tattr); // 0 is the file # of stdin
			tattr.c_cc[VMIN] = 1;
			tattr.c_cc[VTIME] = 0;//3000; // 5 mins timeout
			tattr.c_lflag &= ~(ICANON);
  		tcsetattr (0, TCSAFLUSH, &tattr);

  		if (!isatty(0)) {
				fprintf (stderr, "Not a terminal.\n");
			  exit (1);
			}

			do {
		 		printf("What type of alarm would you like?\n");
		  	printf("  (1) Standard\n  (2) Timer\n\n");
		  	printf("Enter your choice: ");
		  	fflush(stdout);

		  	read(0, &inchar, 1);
		  	putchar('\n'); // because \n not required for input anymore
		  	switch(inchar-'0') {
					case 1:
						printf("When should it go off? ");
						readTime(&hh, &mm);
						if(hh == -1) usage();
						printf("Alarm will go off at %02d:%02d.\n", hh, mm);
						break;
					case 2:
						printf("How long until it goes off? ");
						readTime(&hh, &mm);
						printf("Alarm will go off in %d hours, %d minutes.\n", (int)mm/60, mm%60);
						break;
					default:
						printf("%c", 12); //blank terminal
						// try blank line, overwrite only?  maybe backspace?  CR?
						// CR, reprint: leaves the old answer there, cursor over it, also works for other
						//		term types.
						//printf("Currently only two modes are supported.  Please pick one of them.\n\n");
						continue; //will this drop me out of switch to while, or try the switch?
				}
			} while(mm == -1); // need some time first
			break;
		case 2: // got our arg.  number is counted as timer (mins), time is next time (12 or 24)
			//readTime(&hh, &mm, argv[1]);
			printf("Alarm will be %s %d:%d.\n", (1)?"in":"at", hh, mm);
			break;
		default:
			usage();
	}

	if(hh == -1)
		sleep(60*mm); // sleep the indicated # of minutes
		//sleep(mm);
	else
		sleep(60*timeUntil(hh, mm));
		//printf("That feature is not yet supported.\n");
	time(&alarmTime);

	tcgetattr (0, &tattr); // 0 is the file # of stdin
	tattr.c_cc[VMIN] = 0;
	tattr.c_cc[VTIME] = 0;//10; // 1 second
	tattr.c_lflag &= ~(ICANON);
  tcsetattr (0, TCSAFLUSH, &tattr);

  //make an alarm sound
  i = 0;
	while(1) { //temp only
		//printf("Alarm!%c\n", 7);
		//putchar('\007'); // alarm simulator
		putchar((char)7);
		fflush(stdout);
		sleep(1);
		i += read(0, &(buf[i]), 80-i); // basically, stores the stuff before while round the loop
		buf[i] = '\0'; // make it string compatible
		if(readCommand(&hh, &mm, buf)) {
			i = 0; // reset buffer
			if(hh == -1)
				sleep(60*mm); // sleep the indicated # of minutes
				//sleep(mm);
			else
				sleep(60*timeUntil(hh, mm));
			time(&alarmTime);
		}
	}
}
