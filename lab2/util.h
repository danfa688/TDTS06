#ifndef UTIL_H_
#define UTIL_H_

//C++ includes
#include <string>
#include <vector>
#include <iostream>

#define MAXDATASIZE 131072

void *get_in_addr(struct sockaddr *sa);
std::string get_host(std::vector<std::string> tokens);
std::vector<std::string> split(std::string stringToBeSplitted, std::string delimeter);
int find_header_data_index(std::string header, std::string str);
std::string get_url(std::vector<std::string> tokens);
void change_header_data_to(std::vector<std::string> &tokens, std::string header, std::string new_data);
int tokens_to_buf(std::vector<std::string> &tokens, char *buf);
bool filter_msgs(std::string str);
void redirectBuff(char* buf, int &buflen);

#endif // UTIL_H_
