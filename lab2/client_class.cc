#include "client_class.h"
#include <algorithm>

using namespace std;

Client::Client(){}

int Client::init(const char* host){
	memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(host, CLIENT_PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    return SUCCESS;
}

int Client::sendAll(char* buf, int &buflen){
	int bytes_sent=0;
	int numbytes;
	while(bytes_sent < buflen){
		if((numbytes = send(sockfd, buf+bytes_sent, buflen-bytes_sent, 0)) == -1){
			perror("send");
			return 1;
		}
		bytes_sent+=numbytes;
    }
    buflen = 0;
    return SUCCESS;
}

int Client::receiveHeader(char* buf, int &buflen){
	if ((buflen = recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
		perror("recv");
		return ERROR;
	}

	string s_buf(buf);
	vector<string> tokens = split(s_buf, "\r\n");
	int index = 0;
	string contentType = "";
	for(unsigned int i=0; i < tokens.size(); i++){
		index = find_header_data_index("Content-Type: ", tokens[i]);
		if(index > -1){
			contentType = tokens[i].substr(index);
		}
	}

	if (contentType.size() > 0){
		transform(contentType.begin(), contentType.end(), contentType.begin(),::tolower);
		if (contentType.find("text") != -1){
			return FILTER;
		}
	}

	return PASS_THROUGH;
}

int Client::receive(char* buf, int &buflen){
	if ((buflen = recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
		perror("recv");
		return ERROR;
	}
	return buflen != 0;
}

int Client::receiveRest(char* buf, int &buflen){
	int numbytes = -1;
	while (numbytes != 0){
		if ((numbytes = recv(sockfd, buf+buflen, MAXDATASIZE-buflen, 0)) == -1) {
			perror("recv");
			return ERROR;
		}
		buflen += numbytes;
	}
	return SUCCESS;

}

void Client::stop(){
	close(sockfd);
	printf("client: closed connection from %s\n", s);
}
