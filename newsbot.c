#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

// Yes, mere security through obscurity.  Can lock it down
// with a password before printing if abuse becomes a problem
#define NEWS_PORT 50127
#define BUFFER_SIZE 256

int IRCsock = -1;

void die(const char *msg) {
	printf("Critical Error: %s", msg);
	exit(1);
}

void say(const char *msg) {
	char buf[BUFFER_SIZE];

	if(IRCsock < 0) return;

	sprintf(buf, "PRIVMSG #news :%s\n\r", msg);
	write(IRCsock, buf, strlen(buf));

	return;
}

void newsreader() {
	int ssock; // Server socket
	char buffer[BUFFER_SIZE];
	struct sockaddr_in Address; // Internet socket address stuct
	socklen_t sizeref = sizeof(struct sockaddr_in);

	ssock = socket(AF_INET, SOCK_STREAM, 0);
	if(ssock < 0) die("Could not create a socket\n");

	// fill address struct
	Address.sin_addr.s_addr = INADDR_ANY;
	Address.sin_port = htons(NEWS_PORT);
	Address.sin_family = AF_INET;

	// bind to a port
	if(bind(ssock, (struct sockaddr*)&Address, sizeof(Address)) < 0)
		die("Could not bind to port\n");
		//die("Port already in use -- prolly no need to start\n");

	// establish listen queue
	if(listen(ssock, 5) < 0) die("Listen failed\n");

#ifdef DEBUG
	printf("NewsBot listening for connections on port %d\n", NEWS_PORT);
#endif

	// Waiting for connections
	while(1) {
		// accept the connection
		int sock = accept(ssock,(struct sockaddr*)&Address, &sizeref);
		if(sock < 0) die("Accept failed\n");

		// Get the news info
		if(read(sock, buffer, BUFFER_SIZE) < 0) die("Read failed\n");

#ifdef DEBUG
		printf("News: %s\n", buffer);
#endif

		say(buffer);
		bzero(buffer, BUFFER_SIZE);

		// done, close socket
		if(close(sock) < 0)
			die("Could not close socket\n");
	}
}

void tidyquit() {
  	if(IRCsock < 0) return;
  	write(IRCsock, "QUIT", 4);
	exit(0);
}

int main(int argc, char* argv[]) {
	char buffer[BUFFER_SIZE];
	struct sockaddr_in Address; // Internet socket address stuct
	pthread_t listen;
	
	// Will this daemonize it?
	if(fork()) return;

#ifdef DEBUG
	printf("Starting SL NewsBot\n");
#endif
	signal(SIGINT, tidyquit);
	pthread_create(&listen, 0, (void *)newsreader, 0);

	IRCsock = socket(AF_INET, SOCK_STREAM, 0);
	if(IRCsock < 0) die("Could not create a socket\n");

	//Address.sin_addr.s_addr = inet_addr("127.0.0.1");
	//Address.sin_port = htons(8888);
	Address.sin_addr.s_addr = inet_addr("109.74.195.116");
	Address.sin_port = htons(6667);
	Address.sin_family = AF_INET;

	if(connect(IRCsock, (struct sockaddr*)&Address, sizeof(Address)) < 0) {
		perror(0);
		die("Failed to connect to IRC server\n");
	}

	// We're in -- authenticate
	sleep(5);
	write(IRCsock, "NICK Skylords\r\n", 15);
	write(IRCsock, "USER Skylords 8 * : SkyLords News\r\n", 31);
	write(IRCsock, "JOIN #skylords\r\n", 16);
	write(IRCsock, "PRIVMSG AuthServ@X3.AfterNET.Services :AUTH Skylords password\r\n", 49);
	//write(IRCsock, "NICK News\r\n", 11);
	//write(IRCsock, "USER News 8 * : SkyLords News\r\n", 31);
	//write(IRCsock, "PART #SkyLords :Join me in #news\r\n", 34);
	//write(IRCsock, "PRIVMSG NickServ :IDENTIFY password\r\n", 49);
	//write(IRCsock, "JOIN #news\r\n", 12);
	//write(IRCsock, "MODE #News +v News\r\n", 22);

	// Now just have to loop and respond to PING
	while(read(IRCsock, buffer, BUFFER_SIZE) >= 0) {
		if(buffer[0] == 0) exit(0);
	        char pong[BUFFER_SIZE] = "PONG: ";
		//if(strcmp(buffer, "PING :skylords.com\r\n") == 0) {
		//	write(IRCsock, "PONG :skylords.com\r\n", 20);
		printf("%s\n", sscanf(buffer, "PING :%s", &pong + 6));
		printf("%s\n", pong);
		if(sscanf(buffer, "PING :%s", &pong + 6)) {
			write(IRCsock, pong, strlen(pong));
			write(IRCsock, "\r\n", 2);
#ifdef DEBUG
			printf("PING? PONG!\n");
#endif
		}
#ifdef DEBUG
		else {
			printf("%s\n", buffer);
		}
#endif
		bzero(buffer, BUFFER_SIZE);
	}
}
