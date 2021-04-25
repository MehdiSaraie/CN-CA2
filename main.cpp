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
	cout << program_id << endl;
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
				cin >> number_of_ports;
				cin >> switch_number;
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
					pid = fork();
					struct Switch new_switch;
					new_switch.pid = pid;
					new_switch.number_of_ports = number_of_ports;
					new_switch.number = switch_number;
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
				cin >> system_number;
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
					pid = fork();
					struct System new_system;
					new_system.pid = pid;
					new_system.number = system_number;
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
			char inbuf1[LENGTH];
			
			if(systems.size() == 1){
				cout << "hello1\n";
				write(systems[0].pipe[1], &("HELLP MY SWITCH")[0], LENGTH);	
				
			}
				
			else if(switches.size() == 1){
				cout << "hello2\n";
				read(switches[0].pipe[0][0], inbuf1, LENGTH);
				cout << inbuf1 << "  fsedf" << endl;
			}
		}
		//cout << getpid() << endl;
	}

	/*int id = fork();
	if (id > 0)
		cout << id << ", " << getpid() <<endl;
	else if (id == 0)
		cout << getpid() << ", " << getppid();*/
	return 0;
}
