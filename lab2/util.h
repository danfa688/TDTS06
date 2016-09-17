#ifndef UTIL_H_
#define UTIL_H_

//C++ includes
#include <string>
#include <vector>
#include <iostream>

void *get_in_addr(struct sockaddr *sa);
std::string get_host(std::vector<std::string> tokens);
std::vector<std::string> split(std::string stringToBeSplitted, std::string delimeter);
int find_header_data_index(std::string header, std::string str);
int get_all_buf(int socket, char *out_buf);
void send_all_buf(int socket, char *in_buf, int buflen);
void change_header_data_to(std::vector<std::string> &tokens, std::string header, std::string new_data);
int tokens_to_buf(std::vector<std::string> &tokens, char *buf);

#endif // UTIL_H_
