#include "player.h"
#include "globals.h"

void *player_init(void *in_socket) {
	int socket = (int) insocket;
	char inbuffer[50];
	int i, j, k;
	
	// Send some pretty ascii first
	
	// rewrite this all, the scan is a security risk, and I'm not sure it will work
	fprintf(socket, "Please enter your player name: ");
	fscanf(socket, "%s\n", &inbuffer);
	
	k = -3;
	while(/*filename !exists &&*/ k) {
		fprintf(socket, "I can't seem to find that player.  Are you new? ");
		fscanf(socket, "%s\n", &inbuffer);
		
		i = 0;
		j = -3;
		while(i > 0 && j) {
			switch(inbuffer[i]) {
				case 'y':
				case 'Y':
					i = -1;
					break;
				case 'n':
				case 'N':
					i = -2;
					break;
				case ' ':
					i++;
					break;
				default:
					fprintf(socket, "Please enter yes or no (Y/N): ");
					fscanf(socket, "%s\n", &inbuffer);
					i = 0;
					j++;
			}
		}
		if(!j) {
			fprintf(socket, "I can see that you do not wish to communicate.  Goodbye.\n");
			close(socket);
			return 0;
		}
		if(i == -1) {
			// create player file
			printf("Player file created for: %s.\n", playername);
			fprintf(socket, "New player created.\n");
		}
		if(i == -2)
			k++;
	}
	if(!k) {
		fprintf(socket, "Too many retries.  Goodbye.\n");
		close(socket);
		return 0;
	}
	
	// open player file, get data, including password.  Use encrypted passwords
	// get only password, close again?  don't want to have to do any freeing on
	//		fail cases
	
	
	
	// PASSWORDS: hidden text?
	
	//fprintf(socket, "All passwords on this server are stored encrypted.\n");
	fprintf(socket, "What is your password? ");
	
	k = -3;
	while(k) {
		fscanf(socket, "%s\n", &inbuffer);
		// hash the input
	
		if(/*null file password*/) {
			// file password = hash... problem: first entry is the password, not changable
			fprintf(socket, "Please enter it again for verification: ");
		}
		if(/*!password match*/) {
			k++;
			fprintf(socket, "Password doesn't match.  You have %d more attempts.\n", -k);
			fprintf(socket, "What is your password? ");
		}
	}
	if(!k) { 
		fprintf(socket, "Too many password attempts.  Goodbye.\n");
		close(socket);
		return 0;
	}
	
	// We are here.  They have a password; they are into their account
	
	// motd
	// message listing?
	// give them their room: default is newbie help
	
	// and now take commands...  newbs will have to pick it up fast enough
	
}

