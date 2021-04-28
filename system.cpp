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


using namespace std;


int main(int argc, char* argv[]){
	int system_number = atoi(argv[0]);
	int main_pipe_read_end = atoi(argv[1]);
    char buffer[LENGTH];
    const char* myfifo1;
    const char* myfifo2;
    int switch_out_fd = 0, switch_in_fd = 0;
    int max_fd, activity;
    fd_set readfds;

	while(true) {
		FD_ZERO(&readfds);

		FD_SET(main_pipe_read_end, &readfds);
		max_fd = main_pipe_read_end;
		if (switch_out_fd > 0)
			FD_SET(switch_out_fd, &readfds);
		if (switch_out_fd > max_fd)
			max_fd = switch_out_fd;
		 
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
				make_pipe(switch_number, port_number, 2, switch_in_fd, switch_out_fd);
		    }
		    else if (command == "Send"){
		        string sender = to_string(tokens[0]);
		        string receiver = to_string(tokens[1]);
		        string msg = sender + "-" + receiver + "-" + "MY MSG";
		        if (switch_in_fd == 0){
		        	cout << "System " << sender << " isn't connected to any switch\n";
		        	continue;
		        }
		        write(switch_in_fd, &msg[0], strlen(&msg[0]));
		    }
		}
		
        if (FD_ISSET(switch_out_fd, &readfds)){ //msg from switch
        	memset(&buffer, 0, LENGTH);
			read(switch_out_fd, buffer, LENGTH);
			int src_system, dest_system;
			char msg[LENGTH];
			split_frame(buffer, src_system, dest_system, msg);
			if (dest_system == system_number)
				cout << "System " << system_number << " accepted received frame\n" << msg << endl;
			else
				cout << "System " << system_number << " discarded received frame" << endl;
        }
    }
	
    return 0;
}
