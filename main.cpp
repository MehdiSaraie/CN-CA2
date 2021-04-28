#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <string.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>


using namespace std;
#define LENGTH 1024

struct System
{
	int number;
	int main_pipe_write_end; //to send msg from main to proc
};

struct Switch
{
	int number;
	int main_pipe_write_end; //to recv msg from main proc
};

int main()
{
	vector<Switch> switches;
	vector<System> systems;
	mkdir("pipes", 0777);
	mkdir("files", 0777);
	while(true){
	    string input_str;
		getline(cin,input_str);
		char* input = &input_str[0];
		istringstream line(input);
		string command;
		int token;
		vector<int> tokens;
		line >> command;
		while(line >> token)
			tokens.push_back(token);

		if (command == "MySwitch"){
			if (tokens.size() != 2){
				cout << "Bad arguments\n";
				continue;
			}
			int number_of_ports = tokens[0], switch_number = tokens[1];
			int switch_exists = false;
			for(int i=0;i<switches.size();i++)
			{
				if(switches[i].number == switch_number)
				{
					switch_exists = true;
					break;
				}
			}
			if(switch_exists){
				cout << "Switch " << switch_number << " already exists\n";
				continue;
			}
			cout << "Switch " << switch_number << " created\n";

			struct Switch new_switch;
			new_switch.number = switch_number;
			int main_pipe[2];
			pipe(main_pipe);
			new_switch.main_pipe_write_end = main_pipe[1];
			switches.push_back(new_switch);

			int temp_pipe[2];
			pipe(temp_pipe);

            char inbuf1[LENGTH], inbuf2[LENGTH];
			int n1 = fork();
            
            if (n1>0){ //parent
                write(temp_pipe[1], &(to_string(number_of_ports))[0], LENGTH);
                write(temp_pipe[1], &(to_string(switch_number))[0], LENGTH);
            }

            if (n1 == 0){ //child
                read(temp_pipe[0], inbuf1, LENGTH);
                read(temp_pipe[0], inbuf2, LENGTH);
                char* args[]={"./switch.out", NULL}; 
                execlp(args[0],&inbuf1[0], &inbuf2[0], &(to_string(main_pipe[0]))[0]); 
                exit(0);
            }
        }
        if (command == "MySystem"){
			if (tokens.size() != 1){
				cout << "Bad arguments\n";
				continue;
			}
			int system_number = tokens[0];
			int system_exists = false;
			for(int i=0;i<systems.size();i++)
			{
				if(systems[i].number == system_number)
				{
					system_exists = true;
					break;
				}
			}
			if(system_exists){
				cout << "System " << system_number << " already exists\n";
				continue;
			}
			cout << "System " << system_number << " created\n";

			struct System new_system;
			new_system.number = system_number;
			int main_pipe[2];
            pipe(main_pipe);
			new_system.main_pipe_write_end = main_pipe[1];
			systems.push_back(new_system);

			int temp_pipe[2];
			pipe(temp_pipe);
			
            char inbuf1[LENGTH], inbuf2[LENGTH];
			int n1 = fork();
            
            if (n1>0){ //parent
                write(temp_pipe[1], &(to_string(system_number))[0], LENGTH);
            }

            if (n1 == 0){ //child
                read(temp_pipe[0], inbuf1, LENGTH);
                char* args[]={"./system.out", NULL}; 
                execlp(args[0],&inbuf1[0], &(to_string(main_pipe[0]))[0]); 
                exit(0);
            }
        }
		else if(command == "Connect")
		{
			if (tokens.size() == 3){ //connect system to switch
			
				int system_number = tokens[0] , switch_number = tokens[1], port_number = tokens[2];
				int i, j;
				bool switch_exists = false, system_exists = false;
				for(i=0; i<switches.size(); i++){
					if(switches[i].number == switch_number){
						switch_exists = true;
						break;
					}
				}
				for(j=0; j<systems.size(); j++){
					if(systems[j].number == system_number){
						system_exists = true;
						break;
					}
				}
				if (switch_exists && system_exists)
				{
					write(switches[i].main_pipe_write_end, input, strlen(input)); //send input to switch
					write(systems[j].main_pipe_write_end, input, strlen(input)); //send input to system
				}
				else
					cout << "Switch or system doesn't exists\n";
			}
			
			else if (tokens.size() == 4){ //connect two switches
				int switch1_number = tokens[0] , switch2_number = tokens[1];
				int switch1_port = tokens[2], switch2_port = tokens[3];
				int s, r;
				bool switch1_exists = false, switch2_exists = false;
				for(int i=0; i<switches.size(); i++){
					if(switches[i].number == switch1_number){
						switch1_exists = true;
						s = i;
					}
					if(switches[i].number == switch2_number){
						switch2_exists = true;
						r = i;
					}
				}

				if (switch1_exists && switch2_exists)
				{
					write(switches[s].main_pipe_write_end, input, strlen(input)); //send input to switch1
					write(switches[r].main_pipe_write_end, input, strlen(input)); //send input to switch2
				}
				else
					cout << "Switch1 or switch2 doesn't exists\n";
			}
			
			else
				cout << "Bad arguments\n";
			
		}
		else if (command == "Send")
		{
			int sender = tokens[0] , receiver = tokens[1];
			int s , r;
			bool sender_exists = false, receiver_exists = false;
			for(int j=0; j<systems.size(); j++){
				if(systems[j].number == sender){
					sender_exists = true;
					s = j;
				}
				if(systems[j].number == receiver){
					receiver_exists = true;
					r = j;
				}
			}
			if (sender_exists && receiver_exists)
			{
				write(systems[s].main_pipe_write_end, input, strlen(input)); //send input to system
			}
			else
				cout << "Sender or Receiver doesn't exists\n";
		}
	}
	return 0;
}



/*
MySwitch 100 1

MySwitch 200 2

MySystem 1

MySystem 2

MySystem 3

Connect 1 1 0

Connect 3 2 0

Connect 1 2 10 20

Connect 2 1 1

*/
