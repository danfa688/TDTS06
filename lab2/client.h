#ifndef CLIENT_H_
#define CLIENT_H_

#define CLIENT_PORT "80" // the port client will be connecting to 

#define MAXDATASIZE 100000 // max number of bytes we can get at once 

int run_client(char *buf, int * buflen,const char *adress);

#endif // CLIENT_H_
