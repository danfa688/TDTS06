#ifndef CLIENT_H_
#define CLIENT_H_


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

#define CLIENT_PORT "80"

class Client{
	private:
	int sockfd, numbytes;  
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

	public:
	const static int SUCCESS = 0;
	const static int ERROR = 1;
	const static int FILTER = 2;
	const static int PASS_THROUGH = 3;
	
	Client();
	int init(const char* host);
	int sendAll(char* buf, int &buflen);
	int receiveHeader(char* buf, int &buflen);
	int receive(char* buf, int &buflen);
	int receiveRest(char* buf, int &buflen);
	void stop();
	
};

#endif // CLIENT
