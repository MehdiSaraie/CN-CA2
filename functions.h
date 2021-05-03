#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <fstream>
#include <unordered_map>

#define LENGTH 1024

using namespace std;

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

void update_lookup(int lookup[][2], int& lookup_size, int system_number, int port_number);
void add_connection(int connection[][3], int& connection_size, int port_number, int read_fd, int write_fd);
void split_frame(char buffer[LENGTH], int& src_system, int& dest_system, int& label, char msg[LENGTH]);
void make_pipe(string switch_number, string port_number, int flag, int& in_fd, int& out_fd);
vector<string> read_file_chunk(string file_name, string status);
void WriteInFile(int system_number, int src_system, unordered_map<pair<int,int>, string, hash_pair> messages);
