#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
using namespace std;

//Defines
#define CRITICAL 5
#define BASE_PORT 31337

#define DEAD 0x80
#define REQ 0x01
#define REP 0x02

// Globals
typedef struct {
	unsigned char status;
	int reqtime;
	time_t lastmessage;
} process;
process *procinfo;

int procid;
int max_clients;
int my_time;

sem_t *gmutex;

// Functions
inline int max(int a, int b) {
	return ((a>b)?a:b);
}

void* listenThread(void *unused) {
	// bind a socket
	cout << "Listen thread running" << endl;
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  	if(sock == -1) {
		cerr << "Error creating socket." << endl;
		return 0;
	}
	int one = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
	
	short port = BASE_PORT + procid;
	struct sockaddr_in Address;
	Address.sin_addr.s_addr=INADDR_ANY;
    Address.sin_port=htons(port);
    Address.sin_family=AF_INET;
	
	if(bind(sock, (struct sockaddr*)&Address, sizeof(struct sockaddr_in))) {
		cerr << "Socket binding error." << endl;
		return 0;
	}
	
	if(listen(sock, max_clients)) {
		cerr << "Listen error." << endl;
		return 0;
	}
		
	while(1) {
		int temp = sizeof(struct sockaddr_in);
		int csock = accept(sock, (struct sockaddr*)&Address, (socklen_t *)&temp);
		char buffer[100];
		int lastbyte = recv(csock, buffer, 100, 0);
		buffer[lastbyte] = 0;
		int type, intime, pid;
		sscanf(buffer, "%d %d %d", &type, &intime, &pid);
		close(csock);
		
		//printf("Got a message: %d %d %d\n", type, intime, pid);
		printf("Recieved: ");
		if(type == 1) printf("Request ");
		else if(type == 2) printf("Reply ");
		else if(type == 3) printf("Keepalive from %d\n", pid);
		if(type != 3) {
			printf("Time=%d ", intime);
			printf("PID=%d\n", pid);
		}
		
		// handle the data now
		sem_wait(gmutex);
		procinfo[pid].status &= ~DEAD;
		switch(type) {
			case 1:
				procinfo[pid].reqtime = intime;
				procinfo[pid].status |= REQ;
				break;
			case 2:
				procinfo[pid].status |= REP;
				break;
			case 3:
				break;
		}
		//printf("Status %d: %02X\n", pid, procinfo[pid].status);
		procinfo[pid].lastmessage = time(0);
		my_time = 1 + max(my_time, intime); // updating time
		sem_post(gmutex);
	}
}

// if down, set to -1
// if is -1 and can connect, set to 0
// increment timestamp before send
void message(int pid, int state, int intime = -1) {
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock == -1) {
		cerr << "Error creating socket." << endl;
		return;
	}
	
	struct hostent* pHostInfo;
	struct sockaddr_in Address;
	
	char buf[50];
	gethostname(buf, 50);
	pHostInfo=gethostbyname(buf);
	
	long ipaddy;
	memcpy(&ipaddy,pHostInfo->h_addr,pHostInfo->h_length);
	Address.sin_addr.s_addr=ipaddy;
	
	short port = BASE_PORT+pid;
    Address.sin_port=htons(port);
    Address.sin_family=AF_INET;
	
	int ret = connect(sock, (struct sockaddr*)&Address, sizeof(struct sockaddr_in));
	
	sem_wait(gmutex);
	procinfo[pid].lastmessage = time(0);
	sem_post(gmutex);
	
	if(ret) {
		sem_wait(gmutex);
		procinfo[pid].status = DEAD;
		sem_post(gmutex);
		cerr << "Error connecting to client " << pid << endl;
		return;
	}
	
	sprintf(buf, "%d %d %d", state, (intime != -1)?intime:++my_time, procid);
	
	printf("Sending: ");
	if(state == 1) printf("Request ");
	else if(state == 2) printf("Reply ");
	else if(state == 3) printf("Keepalive to %d\n", pid);
	if(state != 3) {
		printf("Time=%d ", (intime != -1)?intime:my_time);
		printf("PID=%d\n", procid);
	}
		
	ret = send(sock, buf, strlen(buf), 0);
	if(ret == -1) {
		cerr << "Error sending data to client " << pid << endl;
		return;
	}
}

void* liveThread(void *unused) {
	while(1) {
		sleep(4);
		for(int i = 0; i < max_clients; i++) {
			if(i == procid) continue;
			if(procinfo[i].status == DEAD) continue;
			message(i, 3, 0);
		}
	}
}

bool missingACK() {
	bool ret = false;
	int i;
	for(i = 0; i < max_clients; i++) {
		if(i == procid) continue;
		if(procinfo[i].status == DEAD) continue;
		if(!(procinfo[i].status & REP)) {
			ret = true;
			break;
		}
	}
	//printf("%d sent no ack!\n", i);
	//getchar();
	return ret;
}

void usage() {
	cout << "Usage: dmutex processNo totalProcesses" << endl;
	cout << "Can restart a process, if the PID is right, and if all the"
			<< " other processes recognize it as dead FIRST." << endl;
	exit(1);
}

int main(int argc, char *argv[]) {
	if(argc != 3) usage();
	procid = atoi(argv[1]);
	max_clients = atoi(argv[2]);
	
	cout << "Starting up, PID " << procid << endl;
	
	srand(time(0)); // seed the randomness.  Muhahahaha
	procinfo = (process *) calloc(max_clients, sizeof(process));
	my_time = 0;
	
	gmutex = (sem_t *) malloc(sizeof(sem_t));
	sem_init(gmutex, 0, 1);
	pthread_t temp;
	pthread_create(&temp, 0, listenThread, 0);
	//pthread_create(&temp, 0, liveThread, 0);
	
	cout << "Main thread runnning" << endl;
	
	for(int i = 0; i < CRITICAL; i++) {
		int sleeptime = rand()%900000+100000; // between 0.1 and 1 second
		clock_t sleepStart = clock();
		printf("Sleeping for %dus\n", sleeptime);
		
		//loop through the requests and reply to them
		for(int i = 0; (clock()-sleepStart)<sleeptime; i++, i %= max_clients) {
			if(i == procid) continue;
			if(procinfo[i].status == DEAD) continue; // If host is down
			else if(procinfo[i].status&REQ) {
				message(i, 2); // sending reply
				sem_wait(gmutex);
				procinfo[i].status &= ~REQ; // sent, not again
				sem_post(gmutex);
			}
			else if(procinfo[i].status&REP) procinfo[i].status &= ~REP;
			// no replies to me while out, and clear old ones
		}
		
		cout << "Requesting the file" << endl;
		
		int reqtime = ++my_time;
		for(int i = 0; i < max_clients; i++) {
			// send out a request
			if(i == procid) continue; // don't do yourself
			message(i, 1, reqtime);
		}
		
		for(int i = 0; missingACK(); i++, i %= max_clients) {
			if(i == procid) continue;
			if(difftime(time(0), procinfo[i].lastmessage) >= 2) {
				//message(i, 1, reqtime); // try again every 5 seconds
				message(i, 3, 0); // send a keepalive
			}
			if(procinfo[i].status == DEAD) continue; // If host is down
			/*else if(procinfo[i]->status == 2) {
				;
			}*/
			else if(procinfo[i].status&REQ) { // request
				if(procinfo[i].reqtime < reqtime) {
					// issue a reply, it won't have one for me, so will spin till got one from it
					message(i, 2);
					sem_wait(gmutex);
					procinfo[i].status &= ~REQ; // Sent reply, not again
					sem_post(gmutex);
				} // "put in queue" if >
				else if(procinfo[i].reqtime == reqtime) {
					if(i < procid) {
						// again issue a reply
						message(i, 2);
						sem_wait(gmutex);
						procinfo[i].status &= ~REQ; // Sent my reply, only once
						sem_post(gmutex);
					}
				}
			}
		}
		
		// critical section
		printf("Doing my thing in the file%c\n", 0x07);
		int file = open("dmutex.dat", O_RDONLY | O_SYNC);
		char buf[10];
		read(file, buf, 10);
		close(file);
		
		int value;
		sscanf(buf, "%d", &value);
		value++;
		sprintf(buf, "%d\n", value);
		
		file = open("dmutex.dat", O_WRONLY | O_SYNC);
		write(file, buf, strlen(buf));
		close(file);
	}
	cout << "Done" << endl;
	//sleep(1);
}
