#ifndef CLIENT_H_
#define CLIENT_H_

#define CLIENT_PORT "80" // the port client will be connecting to 

#define MAXDATASIZE 10000 // max number of bytes we can get at once 

int run_client(char *buf);

#endif // CLIENT_H_
