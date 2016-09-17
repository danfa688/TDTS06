
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
//#include <string>
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

using namespace std;

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

std::string get_host(std::vector<std::string> tokens){
	int index = 0;
	for(unsigned int i=0; i < tokens.size(); i++){
		index = find_header_data_index("Host: ", tokens[i]);
		if(index > -1){
			return tokens[i].substr(index);
		}
	}
	return "";
}

std::vector<std::string> split(std::string stringToBeSplitted, std::string delimeter)
{
     std::vector<std::string> splittedString;
     int startIndex = 0;
     int  endIndex = 0;
     while( (endIndex = stringToBeSplitted.find(delimeter, startIndex)) < stringToBeSplitted.size() )
    {
       if(startIndex == endIndex){
            std::string val = stringToBeSplitted.substr(startIndex);
       		splittedString.push_back(val);
       		return splittedString;
       }
       std::string val = stringToBeSplitted.substr(startIndex, endIndex - startIndex);
       splittedString.push_back(val);
       startIndex = endIndex + delimeter.size();
     }
     if(startIndex < stringToBeSplitted.size())
     {
       std::string val = stringToBeSplitted.substr(startIndex);
       splittedString.push_back(val);
     }
     return splittedString;
}

int find_header_data_index(std::string header, std::string str){
	int index;
	index = str.find(header);
	if(index > -1){
		return index + header.size();
	}
	return -1;
}

int get_all_buf(int socket, char *out_buf){
	char buf[4096];
	int bufptr=0;
	int numbytes=-1;
	
	while(numbytes != 0){
    	if ((numbytes = recv(socket, buf, 4095, 0)) == -1) {
        	perror("recv");
        	exit(1);
    	}
    	cout << "Numbytes: " << numbytes << endl;
    	for(int i = 0; i<numbytes; i++){
    		out_buf[bufptr++]=buf[i];
    	}
    }
    
    cout << "bufptr" << bufptr << endl;
    
    return bufptr;
}

void send_all_buf(int socket, char *in_buf, int buflen){
	int bytes_sent=0;
	int numbytes;		
	while(bytes_sent < buflen){
		if((numbytes = send(socket, in_buf+bytes_sent, buflen-bytes_sent, 0)) == -1){
			perror("send");
			exit(1);
		}
		bytes_sent+=numbytes;
    }
}

void change_header_data_to(std::vector<std::string> &tokens, std::string header, std::string new_data){
	int index;
	for(int i = 0; i < tokens.size(); i++){
		index = find_header_data_index(header,tokens[i]);
		if (index != -1){
			tokens[i] = tokens[i].substr(0,index) + new_data;l;
		}
	}
}


int tokens_to_buf(std::vector<std::string> &tokens, char *buf){
	string s_buf;
	for(int i=0; i < tokens.size()-1; i++){
		s_buf += tokens[i]+"\r\n";
	}
	s_buf += tokens[tokens.size()-1];
	const char* c_s_buf=s_buf.c_str();
	for(int i=0; i < s_buf.size(); i++){
		buf[i]=c_s_buf[i];
	}
	return s_buf.size();
}
