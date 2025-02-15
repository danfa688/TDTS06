#include "server_class.h"

using namespace std;

void sigchld_handler(int s)
{
	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}

Server::Server(char* port){
	this->port=port;
	yes=1; //remove
	printf("Server port: %s\n",port);
}

int Server::init(){
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
			sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

}

int Server::listenForConnections(){
	sin_size = sizeof their_addr;
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
	if (new_fd == -1) {
		perror("accept");
		return 1;
	}

	inet_ntop(their_addr.ss_family,
		get_in_addr((struct sockaddr *)&their_addr),
		s, sizeof s);
		printf("server: got connection from %s\n", s);

	return 0;
}


void Server::closeParentSock(){
	close(sockfd); // child doesn't need the listener
}

void Server::closeChildSock(){
	close(new_fd);
}
void Server::stop(){
	close(new_fd);
	printf("server: closed connection from %s\n", s);
}

int Server::receive(char* buf, int &buflen){
	if ((buflen = recv(new_fd, buf, MAXDATASIZE, 0)) == -1) {
		perror("recv");
		return ERROR;
	}
	return SUCCESS;
}

int Server::mySend(char* buf, int &buflen){
	int bytes_sent=0;
	int numbytes;
	while(bytes_sent < buflen){
		if((numbytes = send(new_fd, buf+bytes_sent, buflen-bytes_sent, 0)) == -1){
			perror("send");
			return ERROR;
		}
		bytes_sent+=numbytes;
    }
    buflen = 0;
    return SUCCESS;
}
