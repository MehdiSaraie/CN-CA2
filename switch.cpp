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
		 	cout << ("select error\n");
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
		 	int src_port = pp[i][0];
		 	if (FD_ISSET(src_fd, &readfds)){
		 		//cout << "ok\n";
		 		memset(&buffer, 0, LENGTH);
		 		int valread = read(src_fd, buffer, LENGTH);
		 		//cout << buffer << endl;
		 		
		 		j = 0;
	 			while (buffer[j] != '-')
	 				j++;
	 			char src_system_str[LENGTH];
 				strncpy(src_system_str, buffer, j);
 				int src_system = atoi(src_system_str);
		 		
		 		int k = j+1;
				while (buffer[k] != '-')
					k++;
				char dest_system_str[LENGTH];
 				strncpy(dest_system_str, buffer+j+1, k-(j+1));
 				int dest_system = atoi(dest_system_str);

		 		int dest_port, dest_fd;
		 		bool src_found = false, port_found = false;
		 		for (j = 0; j < lookup_size; j++){
		 			if (lookup[j][0] == src_system){ //check if dest exists in lookup
		 				src_found = true;
		 			}
		 			if (lookup[j][0] == dest_system){ //check if src exists in lookup
		 				dest_port = lookup[j][1];
		 				port_found = true;
		 			}
		 		}
		 		if (!src_found){ //add src to lookup
		 			lookup[lookup_size][0] = src_system;
					lookup[lookup_size][1] = src_port;
					lookup_size++;
					pp[pp_size][0] = src_port;
					pp[pp_size][1] = src_fd;
					pp_size++;
		 		}
		 		if (!port_found){
		 			cout << "broadcast\n";
		 			//broadcast
		 		}
		 		else{
		 			for (j = 0; j < pp_size; j++){ //find pipe
			 			if (pp[j][0] == dest_port){
			 				dest_fd = pp[j][1];
			 				break;
			 			}
			 		}
			 		//cout << "sending...\n";
			 		write(dest_fd, buffer, valread);
		 		}

		 	}
		 }
	}
    return 0;
}


