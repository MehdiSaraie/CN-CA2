#include <sys/stat.h>
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

#define LENGTH 1024


using namespace std;


int main(int argc, char* argv[]){
	int system_number = argv[0];
	main_pipe_read_end = argv[1];
    string result = "HELLO MY SWITCH\n";
	const char* myfifo
    mkfifo(myfifo, 0666);
    int fd1 = open(myfifo, O_WRONLY);
    write(fd1, result.c_str(), LENGTH); 
    
    fd1 = open(myfifo, O_WRONLY);
    write(fd1, "result", LENGTH); 
    close(fd1);

    return 0;

}
