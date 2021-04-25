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
	vector<struct System> systems;
	vector<struct Switch> switches;
	string command;
	while(true){
	    cin >> command;
		if (command == "MySwitch"){
        	int p[2];
            pipe(p);
			int switch_number, number_of_ports;
			cin >> number_of_ports;
			cin >> switch_number;
            char inbuf1[LENGTH], inbuf2[LENGTH];
			int n1 = fork();
            
            if (n1>0){ //parent
                write(p[1], &(to_string(switch_number))[0], LENGTH);
                write(p[1], &(to_string(number_of_ports))[0], LENGTH);
            }

            if (n1 == 0){ //child
                read(p[0], inbuf1, LENGTH);
                read(p[0], inbuf2, LENGTH);
                char* args[]={"./switch.out", NULL}; 
                string named_pipe = "aliii";
                execlp(args[0],&inbuf1[0], &inbuf2[0], &named_pipe[0]); 
                exit(0);
            }
        }
        if (command == "MySystem"){
        	int p[2];
            pipe(p);
			int system_number;
			cin >> system_number;
            char inbuf1[LENGTH], inbuf2[LENGTH];
			int n1 = fork();
            
            if (n1>0){ //parent
                write(p[1], &(to_string(system_number))[0], LENGTH);
            }

            if (n1 == 0){ //child
                read(p[0], inbuf1, LENGTH);
                char* args[]={"./system.out", NULL}; 
                string named_pipe = "aliii";
                execlp(args[0],&inbuf1[0], &named_pipe[0] ); 
                exit(0);
            }
        }
	}
	return 0;
}
