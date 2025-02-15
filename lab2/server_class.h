#ifndef SERVER_H_
#define SERVER_H_


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "util.h"

//C++ includes
#include <string>
#include <vector>
#include <iostream>

class Server{
	private:
	char* port;
	int sockfd, new_fd;
	
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes; //remove
	char s[INET6_ADDRSTRLEN];
	int rv;
	
	static const int BACKLOG=10;
	
	public:
	static const int SUCCESS = 0;
	static const int ERROR = 1;
	
	Server(char* port);
	int init();
	int listenForConnections();
	void closeParentSock();
	void closeChildSock();
	int receive(char* buf, int &buflen);
	int mySend(char* buf, int &buflen);
	void stop();
};

#endif // SERVER
