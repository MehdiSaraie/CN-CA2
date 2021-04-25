#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <string>
#include <unistd.h>
#include <bits/stdc++.h>
#include <sys/wait.h>
#include <vector>
#include <fstream>

using namespace std;
#define LENGTH 1024
struct System
{
	int pid;
	int number;
	int main_pipe[2]; //to recv msg from main proc
	vector<int> pipe;
};

struct Switch
{
	int pid;
	int number_of_ports;
	int number;
	int lookup_table[1024][2];
	int main_pipe[2]; //to recv msg from main proc
	vector<vector<int>> pipe;
};

int main()
{
	int program_id = getpid();
	cout << program_id << endl;
	vector<struct System> systems;
	vector<struct Switch> switches;
	int pid;
	while(true)
	{	
		if (getpid() == program_id)
		{
			string input_str;
			getline(cin,input_str);
			char* input = &input_str[0];
			istringstream line(input);

			string command;
			line >> command;

			if (command == "MySwitch")
			{
				int switch_number, number_of_ports;
				line >> number_of_ports;
				line >> switch_number;
				bool switch_exists = false;
				for(int i=0;i<switches.size();i++)
				{
					if(switches[i].number == switch_number)
					{
						switch_exists = true;
						break;
					}
				}
				if(!switch_exists)
				{
					cout << "Switch " << switch_number << " created\n";
					struct Switch new_switch;
					new_switch.number_of_ports = number_of_ports;
					new_switch.number = switch_number;
					if (pipe(new_switch.main_pipe)<0)
						cout<< "pipe error\n";
					pid = fork();
					new_switch.pid = pid;
					if (pid > 0) //main process
					{
						//
						
					}
					else if (pid == 0){
						systems.clear();
						switches.clear();
					}
					switches.push_back(new_switch);
				}
				else
					cout << "Switch " << switch_number << " already exists\n";
			}

			else if (command == "MySystem")
			{
				int system_number;
				line >> system_number;
				bool system_exists = false;
				for(int i=0;i<systems.size();i++)
				{
					if(systems[i].number == system_number)
					{
						system_exists = true;
						break;
					}
				}
				if(!system_exists)
				{
					cout << "System " << system_number << " created\n";
					struct System new_system;
					new_system.number = system_number;
					if (pipe(new_system.main_pipe)<0)
						cout<< "pipe error\n";
					pid = fork();
					new_system.pid = pid;
					if (pid > 0) //main process
					{
						//
						
					}
					else if (pid == 0){
						systems.clear();
						switches.clear();
					}
					systems.push_back(new_system);
				}
				else
					cout << "System " << system_number << " already exists\n";
			}

			else if(command == "Connect")
			{	
				int system_number, switch_number, port_number;
				line >> system_number >> switch_number >> port_number;
				int id = getpid();
				if (pid > 0){
					vector<int> p;
					for(int i=0; i<switches.size(); i++){
						if(switches[i].number == switch_number){
							write(switches[i].main_pipe[1], input, strlen(input)); //send input to switch for proccessing
							//p = switches[i].pipe[port_number];
						}
					}
					for(int i=0; i<systems.size(); i++){
						if(systems[i].number == system_number){
							//systems[i].pipe = p;
						}
					}
				}
				
			}
			else if (command == "Send")
			{

			}
		}
		else{
			char buffer[LENGTH];
			
			if(systems.size() == 1){
				//recv resonse from switch
				//make pipe if successfully connect


				//write(systems[0].pipe[1], &("HELLP MY SWITCH")[0], LENGTH);
				
			}
				
			else if(switches.size() == 1){
				struct Switch this_switch = switches[0];
				memset(&buffer, 0, LENGTH);
				read(this_switch.main_pipe[0], buffer, LENGTH);
				istringstream line(buffer);
				string command;
				line >> command;
				if (command == "Connect"){
					//check constraints
					//send response to system
					//make pipe if needed

				}
				puts(buffer);

				//read(switches[0].pipe[0][0], inbuf1, LENGTH);
				//cout << inbuf1 << "  fsedf" << endl;
			}
		}
		//cout << getpid() << endl;
	}

	return 0;
}
