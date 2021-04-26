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
	int system_number = atoi(argv[0]);
	int main_pipe_read_end = atoi(argv[1]);
    char buffer[LENGTH];
    const char* myfifo;
    while(1){
        memset(&buffer, 0, LENGTH);
		read(main_pipe_read_end, buffer, LENGTH);
        istringstream line(buffer);
		string command;
		line >> command;
		int token;
		vector<int> tokens;
		while(line >> token)
			tokens.push_back(token);
        if(command == "Connect"){
            string switch_number = to_string(tokens[1]);
            string port_number = to_string(tokens[2]);
            myfifo = &(switch_number + "-" + port_number)[0];
            mkfifo(myfifo, 0666);
            int fd = open(myfifo, O_WRONLY); //must store fd in pp beside its port
			// close(fd);
            // fd = open(myfifo, O_RDONLY); //must store fd in pp beside its port
			// close(fd);
        }
        else if (command == "Send"){
            string sender = to_string(tokens[0]);
            string receiver = to_string(tokens[1]);
            int fd1 = open(myfifo, O_WRONLY);
            string msg = "MY MSG" + sender + "-" + receiver;
            write(fd1, &msg[0], LENGTH);
            close(fd1);
        }
        
        
        // fd1 = open(myfifo, O_WRONLY);
        // write(fd1, "result", LENGTH); 
        // close(fd1);
    }
	

    return 0;

}
