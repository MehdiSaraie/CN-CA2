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
    int switch_pipe_fd = 0;
    int max_fd, activity;
    fd_set readfds;

	while(true) {
		FD_ZERO(&readfds);

		FD_SET(main_pipe_read_end, &readfds);
		max_fd = main_pipe_read_end;
		if (switch_pipe_fd > 0)
			FD_SET(switch_pipe_fd, &readfds);
		if (switch_pipe_fd > max_fd)
			max_fd = switch_pipe_fd;
		 
		activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
		if ((activity < 0) && (errno!=EINTR)){
			cerr << ("select error\n");
		}

		if (FD_ISSET(main_pipe_read_end, &readfds)){ //msg from main
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
		        myfifo = &("pipes/" + switch_number + "-" + port_number)[0];
		        mkfifo(myfifo, 0666);
		        switch_pipe_fd = open(myfifo, O_RDWR);
		    }
		    else if (command == "Send"){
		        string sender = to_string(tokens[0]);
		        string receiver = to_string(tokens[1]);
		        string msg = sender + "-" + receiver + "-" + "MY MSG";
		        if (switch_pipe_fd == 0){
		        	cout << "System " << sender << " isn't connected to any switch\n";
		        	continue;
		        }
		        write(switch_pipe_fd, &msg[0], strlen(&msg[0]));
		    }
		}
		
        if (FD_ISSET(switch_pipe_fd, &readfds)){ //msg from switch
        	//cout << "OK\n";
        	memset(&buffer, 0, LENGTH);
			read(switch_pipe_fd, buffer, LENGTH);
			cout << buffer << endl;
        }
    }
	
    return 0;
}
