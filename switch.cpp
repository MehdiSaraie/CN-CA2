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

int find_pipe(int lookup[1024][2], int lookup_size, int pp[][2], int pp_size, int dest_system){
	int dest_port, dest_fd, j;
	bool port_found = false;
	for (j = 0; j < lookup_size; j++){ //find port
		if (lookup[j][0] == dest_system){
			dest_port = lookup[j][1];
			port_found = true;
			break;
		}
	}
	if (!port_found)
		return -1;
	for (j = 0; j < pp_size; j++){ //find pipe
		if (pp[j][0] == dest_port){
			dest_fd = pp[j][1];
			return dest_fd;
		}
	}
}


int main(int argc, char* argv[]){
	const int number_of_ports = atoi(argv[0]);
	string switch_number = argv[1];
	int main_pipe_read_end = atoi(argv[2]);
	int lookup[1024][2];//dest system + port
	int lookup_size = 0;
	int pp[number_of_ports][2];//port + pipe_fd
	int pp_size = 0;
	int i, j, sd, max_sd, activity;
	fd_set readfds;
	char buffer[LENGTH];
	for (i = 0; i < number_of_ports; i++)
		pp[i][1] = 0;

	while(true) {
		 FD_ZERO(&readfds);

		 FD_SET(main_pipe_read_end, &readfds);
		 max_sd = main_pipe_read_end;
		 for (i = 0; i < number_of_ports; i++){
		 	sd = pp[i][1];
		 	if (sd > 0)
		 		FD_SET(sd, &readfds);
		 	if (sd > max_sd)
		 		max_sd = sd;
		 }
		 
		 activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
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
				int system_number = tokens[0];
			    int port_number = tokens[2];
				const char* myfifo = &("pipes/" + switch_number + "-" + to_string(port_number))[0];
				mkfifo(myfifo, 0666);
				int fd = open(myfifo, O_RDWR);
				cout << "swi_fd=" << fd << endl;
				lookup[lookup_size][0] = system_number;
				lookup[lookup_size][1] = port_number;
				lookup_size++;
				pp[pp_size][0] = port_number;
				pp[pp_size][1] = fd;
				pp_size++;
				cout << "System " << system_number << " connected to switch " << switch_number << " on port " << port_number << endl;
			}
		 }

		for (i = 0; i < number_of_ports; i++){ //msg from a system
			int src_fd = pp[i][1];
			if (FD_ISSET(src_fd, &readfds)){
		 		memset(&buffer, 0, LENGTH);
		 		cout << "ok\n";
		 		read(src_fd, buffer, LENGTH);
		 		cout << buffer << endl;
		 		
				int dest_system = 2; //**must be replaced by frame destination
				int dest_fd = find_pipe(lookup, lookup_size, pp, pp_size, dest_system);
		 		if (dest_fd < 0){
		 			cout << "broadcast\n";
		 			//broadcast
		 		}
		 		else{
		 			cout << "sending...\n";
		 			//forward frame through pipe
		 		}
		 	}
		}
	}
    return 0;
}
