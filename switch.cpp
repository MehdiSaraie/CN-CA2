#include <sys/stat.h>
#include <bits/stdc++.h> 
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include "functions.h"

#define LENGTH 1024

using namespace std;

void update_lookup(int lookup[][2], int& lookup_size, int system_number, int port_number){
	lookup[lookup_size][0] = system_number;
	lookup[lookup_size][1] = port_number;
	lookup_size++;
}

void add_connection(int connection[][3], int& connection_size, int port_number, int read_fd, int write_fd){
	connection[connection_size][0] = port_number;
	connection[connection_size][1] = read_fd;
	connection[connection_size][2] = write_fd;
	connection_size++;
}


int main(int argc, char* argv[]){
	const int number_of_ports = atoi(argv[0]);
	string switch_number = argv[1];
	int main_pipe_read_end = atoi(argv[2]);
	int lookup[1024][2];//dest system + port
	int lookup_size = 0;
	int connection[number_of_ports][3];//port + read_fd + write_fd (for all connected switches or systems)
	int connection_size = 0;
	int i, j, sd, max_sd, activity;
	fd_set readfds;
	char buffer[LENGTH];

	while(true) {
		 FD_ZERO(&readfds);

		 FD_SET(main_pipe_read_end, &readfds);
		 max_sd = main_pipe_read_end;
		 for (i = 0; i < connection_size; i++){
		 	sd = connection[i][1];
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
	 			if (tokens.size() == 3){ //connect system to switch
					int system_number = tokens[0];
					int port_number = tokens[2];	
					int in_fd, out_fd;
					make_pipe(switch_number, to_string(port_number), 1, in_fd, out_fd);
					update_lookup(lookup, lookup_size, system_number, port_number);
					add_connection(connection, connection_size, port_number, in_fd, out_fd);
					cout << "System " << system_number << " connected to switch " << switch_number << " on port " << port_number << endl;
				}
				else if(tokens.size() == 4){ //connect two switches
					string switch1_number = to_string(tokens[0]) , switch2_number = to_string(tokens[1]);
					int switch1_port = tokens[2], switch2_port = tokens[3];
					
					
					if (switch_number == switch1_number){
						int in_fd, out_fd;
						make_pipe(switch1_number, to_string(switch1_port), 1, in_fd, out_fd);
						add_connection(connection, connection_size, switch1_port, in_fd, out_fd);
						cout << "Switch " << switch1_number << " on port " << switch1_port << " connected to switch " << switch2_number << " on port " << switch2_port << endl;
					}
					else if (switch_number == switch2_number){
						int in_fd, out_fd;
						make_pipe(switch1_number, to_string(switch1_port), 2, in_fd, out_fd);
						add_connection(connection, connection_size, switch2_port, out_fd, in_fd);
					}
				}
			}
		 }

		 for (i = 0; i < connection_size; i++){ //msg from a system
		 	int src_fd = connection[i][1];
		 	int src_port = connection[i][0];
		 	if (FD_ISSET(src_fd, &readfds)){
		 		memset(&buffer, 0, LENGTH);
		 		int valread = read(src_fd, buffer, LENGTH);
		 		
		 		int src_system, dest_system,tag;
		 		char msg[LENGTH];
		 		split_frame(buffer, src_system, dest_system, tag, msg);

		 		int dest_port, dest_fd;
		 		bool src_found = false, port_found = false;
		 		for (j = 0; j < lookup_size; j++){
		 			if (lookup[j][0] == src_system){ //check if src exists in lookup
		 				src_found = true;
		 			}
		 			if (lookup[j][0] == dest_system){ //check if dest exists in lookup
		 				dest_port = lookup[j][1];
		 				port_found = true;
		 			}
		 		}
		 		if (!src_found){ //add src to lookup
		 			update_lookup(lookup, lookup_size, src_system, src_port);
		 			cout << "Switch " << switch_number << " updated its lookup:\n";
		 			cout << "system = " << src_system << ", port = " << src_port << endl;
		 		}
		 		if (!port_found){
		 			cout << "Switch " << switch_number << " broadcasted frame on ports:\n";
					cout << connection_size << endl;
		 			for (j = 0; j < connection_size; j++){ //broadcast
			 			if (connection[j][1] != src_fd){
			 				dest_fd = connection[j][2];
			 				cout << connection[j][0] << endl;
			 				write(dest_fd, buffer, valread);
			 			}
			 		}
		 		}
		 		else{
		 			for (j = 0; j < connection_size; j++){ //find dest pipe
			 			if (connection[j][0] == dest_port){
			 				dest_fd = connection[j][2];
			 				break;
			 			}
			 		}
			 		cout << "Switch " << switch_number << " sent frame on port " << dest_port << endl;
			 		write(dest_fd, buffer, valread);
		 		}

		 	}
		 }
	}
    return 0;
}


