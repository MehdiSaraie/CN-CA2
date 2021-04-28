#include "functions.h"

void split_frame(char buffer[LENGTH], int& src_system, int& dest_system, char msg[LENGTH]){
	int j = 0;
	while (buffer[j] != '-')
		j++;
	char src_system_str[LENGTH];
	strncpy(src_system_str, buffer, j);
	src_system = atoi(src_system_str);
	
	int k = j+1;
	while (buffer[k] != '-')
		k++;
	char dest_system_str[LENGTH];
	strncpy(dest_system_str, buffer+j+1, k-(j+1));
	dest_system = atoi(dest_system_str);
	strcpy(msg, buffer+k+1);
}

void make_pipe(string switch_number, string port_number, int flag, int& in_fd, int& out_fd){
	const char* myfifo1 = &("pipes/" + switch_number + "-" + port_number + "-in")[0];
	mkfifo(myfifo1, 0666);
	if (flag == 1)
		in_fd = open(myfifo1, O_RDONLY);
	else if (flag == 2)
		in_fd = open(myfifo1, O_WRONLY);
	const char* myfifo2 = &("pipes/" + switch_number + "-" + port_number + "-out")[0];
	mkfifo(myfifo2, 0666);
	if (flag == 1)
		out_fd = open(myfifo2, O_WRONLY);
	else if (flag == 2)
		out_fd = open(myfifo2, O_RDONLY);
}
