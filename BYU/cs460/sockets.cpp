#include <iostream>
#include <sys/socket.h>
#include <signal.h>
#include <pthread.h>

typedef struct {
	int socket;
	struct sockaddr_in addy;
	int addySize;
} singleArg;

void shutdown();
void cHandler(singleArg *arg);

pthread_t *active = 0;
int activeNo = 0;

int main() {
	short port = 31337;
	int maxWaiting = 2;
	
	signal(SIGINT, shutdown);
	
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		cerr << "Error creating socket." << endl, exit(0);
	
	// Create an IP addy, using a constant for now
	struct in_addr IP;
	inet_aton("127.0.0.1", &IP);
	// Instead of IP, can prolly have INADDR_{LOOPBACK,ANY}
	
	// Internet socket, required
	// Port number, needs to be in network ordering
	// Address to bind to.  For listening, using any listens on all the comps IPs
	struct sockaddr_in bindLoc = {AF_INET, htons(port), INADDR_LOOPBACK}
	// For now, w/the loopback, only local programs can access it
	
	if(bind(sock, bindLoc, sizeof(sockaddr_in)))
		cerr << "Error binding socket." << endl, exit(0);
	
	if(listen(sock, maxWaiting))
		cerr << "Error trying to listen" << endl, exit(0);
	
	cout << "Server listening on port " << port << "." << endl;
	
	while(1) {
		singleArg *data = malloc(sizeof(singleArg));
		//struct sockaddr_in *client = malloc(sizeof(sockaddr_in));
		//int *cStructSize = malloc(sizeof(int));
		int cSock = accept(sock, &(singleArg->addy), &(singleArg->addySize));
		// create a thread to handle the new socket
		pthread_t threadId;
		pthread_create(&threadId, 0, cHandler, data);
		pthread_t *temp = malloc(++activeNo * sizeof(pthread_t *)));
		for(int i = 0; i < activeNo-1; i++)
			temp[i] = active[i];
		temp[activeNo-1] = threadId;
		free(active);
		active = temp;
	}
}

void shutdown() {
	//tell all threads to close, then exit
	cout << "Closing down the server." << endl;
	
	for(int i = 0; i < activeNo; i++) {
		cout << "Thread " << active[i] << " terminated." << endl;
	}
	
	exit(0);
}

void cHandler(singleArg *arg) {
	char welcome[] = "Welcome to the happy fun server.\n\nThis server actually does nothing, so whatever. \
						Feel free to enter commands at your heart's content.\n$";
	char buf[80];
	send(arg->socket, welcome, strlen(welcome), 0);
	while(1) {
		recv(arg->socket, buf, 80, 0);
		if(!strcmp(buf, "quit")) {
			send(arg->socket, "Quitting...", 12, 0);
			free(arg);
			pthread_exit(0);
		}
		else
			send(arg->socket, "You said: " + buf, 10+strlen(buf), 0);
	}
}

