#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <string>
#include <unistd.h>
#include <bits/stdc++.h>
#include <sys/wait.h>

using namespace std;

struct System
{
	int pid;
	int number;
	int switch_number;
};

struct Switch
{
	int pid;
	int number_of_ports;
	int number;
	int lookup_table[1024][2];
	int* system;
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
					new_switch.system = new int[number_of_ports];
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
					for(int i=0; i<switches.size(); i++){
						if(switches[i].number == switch_number){
							switches[i].system[port_number] = system_number;
						}
					}
					for(int i=0; i<systems.size(); i++){
						if(systems[i].number == system_number){
							systems[i].switch_number = switch_number;
						}
					}
				}
				
			}
		}
		else{
			for(int i=0; i<switches.size(); i++){
				if(getpid() == switches[i].pid){
					
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
