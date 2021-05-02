#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <fstream>
#define LENGTH 1024

using namespace std;

void split_frame(char buffer[LENGTH], int& src_system, int& dest_system, int& label, char msg[LENGTH]);
void make_pipe(string switch_number, string port_number, int flag, int& in_fd, int& out_fd);
vector<string> read_file_chunk(string file_name, string status);
void WriteInFile(string filename, string msg);
