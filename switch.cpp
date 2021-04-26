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
	int pp[number_of_ports][2];//port + pipe_fd
	int i, sd, max_sd, activity;
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
		cout << activity << endl;		
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
			line >> command;
			while(line >> token)
				tokens.push_back(token);
			if (command == "Connect"){
				//add row to lookup
				string port_number = to_string(tokens[2]);
				const char* myfifo = &(switch_number + "-" + port_number)[0]; //(e.g. 1-40);
    			mkfifo(myfifo, 0666);
				int fd = open(myfifo ,O_RDONLY); //must store fd in pp beside its port
				close(fd);
			}
		}

		for (i = 0; i < number_of_ports; i++){ //msg from a system
			sd = pp[i][1];
			if (FD_ISSET(sd, &readfds)){
				//forward frame
				const char* myfifo = &(switch_number + "-" + to_string(i))[0];
				cout << myfifo << endl;
				int fd1 = open(myfifo ,O_RDONLY);
				memset(&buffer, 0, LENGTH);
				read(fd1, buffer, LENGTH); 
				cout << buffer << endl;
				close(fd1);
			}
		}
    }
    return 0;
}

