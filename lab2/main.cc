
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




//C++ includes
#include <string>
#include <vector>
#include <iostream>

#include "server_class.h"
#include "client_class.h"
#include "util.h"

using namespace std;

int main(int argc, char* argv[]){
	if(argc < 2){
		cout << "error, no port given!" << endl;
		return 1;
	}

	//create a server object
	Server server(argv[1]);
	server.init();

	printf("server: waiting for connections...\n");

	while(1) {
		if(server.listenForConnections()){
			continue;
		}
		if (!fork()) {
			// this is a child process,
			// one is created for each get request fom the browser

			//used to keep track of data
			char buf[MAXDATASIZE];
			int buflen=0;


			server.closeParentSock();

			// recive get request form browser
			if (server.receive(buf, buflen) == server.ERROR){
				server.stop();
				return 1;
			};

			//copy buf to a string to make it easier to work with
			string s_buf(buf);
			//split string into several string with the divisor \r\n
			vector<string> tokens = split(s_buf, "\r\n");
			//extract url from split strings
			string url = get_url(tokens);
			//extract host from split strings
			string host = get_host(tokens);

			//filter url + host an look for keywords that are not allowed
			if (filter_msgs(host + url)){
				redirectBuff(buf, buflen);
				printf("Resirect: %s\n", buf);
				server.mySend(buf, buflen);
				server.stop();
				return 0;
			}

			// creat a clinet object
			Client client;
			if(client.init(host.c_str()) != client.SUCCESS){
				server.stop();
				return 1;
			}

			//change header in get request to say "Connection: close"
			change_header_data_to(tokens, "Connection: ", "close");
			buflen = tokens_to_buf(tokens, buf);
		
			// send get request to web server
			if (client.sendAll(buf, buflen) != client.SUCCESS){
				server.stop();
				client.stop();
				return 1;
			}

			// recive header from web server
			int clientState = client.receiveHeader(buf, buflen);
			// decide based on header if the contsns i text or other
			if(clientState == client.PASS_THROUGH){
				// if other then pass message through to browser
				server.mySend(buf, buflen);
				while(client.receive(buf, buflen)){
					server.mySend(buf, buflen);
				}
			}else if (clientState == client.FILTER){
				// if text, buffer all text and filter the text and look for keywords
				client.receiveRest(buf, buflen);
				//filter
				string tmp(buf);
				if (filter_msgs(tmp)){
					// if keywords exist, redderect to error site
					redirectBuff(buf,buflen);
				}
				// if no keyword then send along to browser
				server.mySend(buf, buflen);
			}

			client.stop();
			server.stop();
			return 0;
		}
		server.closeChildSock();  // parent doesn't need this
	}


	return 0;
}
