#include <bits/stdc++.h> 
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <fcntl.h> 
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <string.h> 
#include <sys/types.h>
#include <errno.h>
#include "functions.h"

#define LENGTH 1024
#define SPANNINGTREE 0

using namespace std;


int main(int argc, char* argv[]){
    string file_name;
    cin >> file_name;
	const int BUFFER_SIZE = 512;
	vector<char> buffer (BUFFER_SIZE + 1, 0);

	ifstream ifile(file_name, std::ifstream::binary);
    if (ifile.fail()) cout << "TEF\n";
	vector<string> chunks;
	while(1)
	{
		ifile.read(buffer.data(), BUFFER_SIZE);
		streamsize s = ((ifile) ? BUFFER_SIZE : ifile.gcount());
		buffer[s] = 0;
		chunks.push_back(buffer.data());
		if(!ifile) break;
	}
	ifile.close();
    return 0;
}
