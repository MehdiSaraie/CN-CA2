#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <string>
#include <unistd.h>
#include <bits/stdc++.h>
#include <sys/wait.h>
#include <vector>

using namespace std;
#define LENGTH 1024
struct System
{
	int pid;
	int number;
	vector<int> pipe;
};

struct Switch
{
	int pid;
	int number_of_ports;
	int number;
	int lookup_table[1024][2];
	vector<vector<int>> pipe;
};

int main()
{
	int program_id = getpid();
	vector<struct System> systems;
	vector<struct Switch> switches;
	string command;
	int pid;
	while(true)
	{	
		if (getpid() == program_id)
		{
			cin >> command;
			if (command == "MySwitch")
			{
				int switch_number, number_of_ports;
				cin >> switch_number;
				cin >> number_of_ports;
				pid = fork();
				if (pid > 0) //main process
				{
					struct Switch new_switch;
					new_switch.pid = pid;
					new_switch.number_of_ports = number_of_ports;
					new_switch.number = switch_number;
					for(int i=0; i<number_of_ports; i++)
						new_switch.pipe.push_back(vector<int>(2));
					switches.push_back(new_switch);
				}
			}
			else if (command == "MySystem")
			{
				int system_number;
				cin >> system_number;
				pid = fork();
				if (pid > 0) //main process
				{
					struct System new_system;
					new_system.pid = pid;
					new_system.number = system_number;
					systems.push_back(new_system);
				}
			}
			else if(command == "Connect")
			{	
				int system_number, switch_number, port_number;
				cin >> system_number >> switch_number >> port_number;
				int id = getpid();
				if (pid > 0){
					vector<int> p;
					for(int i=0; i<switches.size(); i++){
						if(switches[i].number == switch_number){
							p = switches[i].pipe[port_number];
						}
					}
					for(int i=0; i<systems.size(); i++){
						if(systems[i].number == system_number){
							systems[i].pipe = p;
						}
					}
				}
				
			}
			else if (command == "send")
			{

			}
		}
		else{
			for (int i=0; i<systems.size();i++){
				cout << "hello\n";
				if(getpid() == systems[i].pid){
					write(systems[i].pipe[1], &("HELLP MY SWITCH")[0], LENGTH);
				}
				
			}
			for(int i=0; i<switches.size(); i++){
				char inbuf1[LENGTH];
				if(getpid() == switches[i].pid){
					read(switches[i].pipe[0][0], inbuf1, LENGTH);
					cout << inbuf1 << "  fsedf" << endl;
				}
			}
		}
	}

	/*int id = fork();
	if (id > 0)
		cout << id << ", " << getpid() <<endl;
	else if (id == 0)
		cout << getpid() << ", " << getppid();*/
	return 0;
}
