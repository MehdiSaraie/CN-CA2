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
	int system_number = atoi(argv[0]);
	int main_pipe_read_end = atoi(argv[1]);
    char buffer[LENGTH];
    const char* myfifo1;
    const char* myfifo2;
    int switch_out_fd = 0, switch_in_fd = 0;
    int max_fd, activity;
    fd_set readfds;
	unordered_map<pair<int,int>, string, hash_pair> messages;

	while(true){
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
			string token;
			vector<string> tokens;
			while(line >> token)
				tokens.push_back(token);
		    if(command == "Connect"){
		        string switch_number = tokens[1];
		        string port_number = tokens[2];
				make_pipe(switch_number, port_number, 2, switch_in_fd, switch_out_fd);
		    }
		    else if (command == "Send"){
				string sender = tokens[0];
				string receiver = tokens[1];
				string file_name = tokens[2];
				vector<string> chunks = read_file_chunk(file_name, "send");
				for(int h=0; h<chunks.size(); h++){
					string msg = sender + "-" + receiver + "-" + to_string(h) + "-" + chunks[h];
					if (switch_in_fd == 0){
						cout << "System " << sender << " isn't connected to any switch\n";
						continue;
					}
					write(switch_in_fd, &msg[0], LENGTH);
				}
		    }
			else if (command == "Receive"){
		        string sender = tokens[0];
		        string receiver = tokens[1];
				string file_name = tokens[2];
				string msg = sender + "-" + receiver + "-0-request-" + file_name;
				if (switch_in_fd == 0){
					cout << "System " << sender << " isn't connected to any switch.\n";
					continue;
				}
				write(switch_in_fd, &msg[0], LENGTH);
		    }
		}
		
        if (FD_ISSET(switch_out_fd, &readfds)){ //msg from switch
        	memset(&buffer, 0, LENGTH);
			read(switch_out_fd, buffer, LENGTH);
			int src_system, dest_system, label;
			char msg[LENGTH];
			split_frame(buffer, src_system, dest_system, label, msg);
			if (dest_system == system_number){
				char *output = NULL;
				output = strstr(msg,"request-");
				if(!output){    //msg from switch
					cout << "System " << system_number << " accepted received frame.\n" << endl;
					pair<int, int> temp(src_system, label);
					messages[temp] = msg;
					WriteInFile(system_number, src_system, messages);
					messages.clear();
				}
				else{ //msg for request a file
					cout << "System " << system_number << " accepted request.\n" << endl;
					int j = 0;
					while (msg[j] != '-')
						j++;
					char file_name[LENGTH];
					strcpy(file_name, msg+j+1);
					vector<string> chunks = read_file_chunk(file_name, "request");  //send requested file to switch
					for(int h=0; h<chunks.size(); h++){
						string msg = to_string(system_number) + "-" + to_string(src_system) + "-" + to_string(h) + "-" + chunks[h];
						if (switch_in_fd == 0){
							cout << "System " << system_number << " isn't connected to any switch.\n";
							continue;
						}
						write(switch_in_fd, &msg[0], LENGTH);
					}
				}
			}
			else
				cout << "System " << system_number << " discarded received frame\n" << endl;
        }
    }
    return 0;
}
