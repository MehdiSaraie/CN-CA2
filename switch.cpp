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
#define SPANNINGTREE 0

using namespace std;


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
	bool spanConnect = false; //is added to spanning tree?

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
					
					bool flag = true;	
					if (connection_size == number_of_ports){
						cout << "All ports in use. please connect to other switch\n";
						flag = false;
					}	
					else{	
						for (j = 0; j < connection_size; j++){
							if (connection[j][0] == port_number){
								cout << "Port " << port_number << " in use\n";
								flag = false;
								break;
							}
						}
					}					
					if (flag){	
						add_connection(connection, connection_size, port_number, in_fd, out_fd);
						cout << "System " << system_number << " connected to switch " << switch_number << " on port " << port_number << endl;
					}
					else
						write(out_fd, &"connect error"[0], strlen(&"connect error"[0]));
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
			else if(command == "SpanningTree"){
				spanConnect = true;
				string msg = switch_number + "-" + to_string(SPANNINGTREE) + "-0-0";
				for (int j = 0; j < connection_size; j++){ //broadcast
					int dest_fd = connection[j][2];
					write(dest_fd, &msg[0], LENGTH);
				}
			}
		}

		for (i = 0; i < connection_size; i++){ //msg from a system
			int src_fd = connection[i][1];
			int src_port = connection[i][0];
			if (FD_ISSET(src_fd, &readfds)){
				memset(&buffer, 0, LENGTH);
				int valread = read(src_fd, buffer, LENGTH);
				
				int src_system, dest_system,label;
				char msg[LENGTH];
				split_frame(buffer, src_system, dest_system, label, msg);

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
				for (j = 0; j < connection_size; j++){
					if (connection[j][1] != src_fd )
						port_found = false;
				}
				if (!port_found && dest_system){
					cout << "Switch " << switch_number << " broadcasted frame on ports:\n";
					// cout << connection_size << endl;
					for (j = 0; j < connection_size; j++){ //broadcast
						if (connection[j][1] != src_fd && connection[j][0]!= -1){
							dest_fd = connection[j][2];
							cout << connection[j][0] << endl;
							write(dest_fd, buffer, valread);
						}
					}
				}
				else if(port_found && dest_system){
					for (j = 0; j < connection_size; j++){ //find dest pipe
						if (connection[j][0] == dest_port){
							dest_fd = connection[j][2];
							break;
						}
					}
					cout << "Switch " << switch_number << " sent frame on port " << dest_port << endl;
					write(dest_fd, buffer, valread);
				}
				else if(dest_system == 0){ //spannig tree
					if(!spanConnect){ //node has not connected to tree
						spanConnect = true;
						for (j = 0; j < connection_size; j++){ //broadcast
							if (connection[j][1] != src_fd && connection[j][0]!= -1){
								int dest_fd = connection[j][2];
								string msg = switch_number + "-" + to_string(SPANNINGTREE) + "-0-0";
								write(dest_fd, &msg[0], LENGTH);
							}
						}
					}
					else{ //node has connected to tree
						for (j = 0; j < connection_size; j++){
							if(connection[j][0] == src_port){
								connection[j][0] = -1;
								connection[j][1] = -1;
								connection[j][2] = -1;
								// connection_size--;
								break;
							}
						}
						cout << "Connection " << switch_number << " to " << src_system << " port " << src_port << " " << dest_port << " distroyed!\n";
					}
				}
			}
		}
	}
	return 0;
}


