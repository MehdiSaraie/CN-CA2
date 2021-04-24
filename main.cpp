#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <string>
#include <unistd.h>
#include <bits/stdc++.h>

using namespace std;

struct System
{
	int pid;
	int number;
};

struct Switch
{
	int pid;
	int number_of_ports;
	int number;
	int lookup_table[1024][2];
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
		}
	}

	/*int id = fork();
	if (id > 0)
		cout << id << ", " << getpid() <<endl;
	else if (id == 0)
		cout << getpid() << ", " << getppid();*/
	return 0;
}
