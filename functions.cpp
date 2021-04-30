#include "functions.h"

void split_frame(char buffer[LENGTH], int& src_system, int& dest_system, int& tag, char msg[LENGTH]){
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
	
	int l = k+1;
	while (buffer[l] != '-')
		l++;
	char tag_str[LENGTH];
	strncpy(tag_str, buffer+k+1, l-(k+1));
	tag = atoi(tag_str);

	// cout << buffer << endl;
	// cout << j << k << l << endl;
	// cout << buffer  << "$"<<tag << endl;
	strcpy(msg, buffer+l+1);
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


vector<string> read_file_chunk(string file_name){

	const int BUFFER_SIZE = 512;
	vector<char> buffer (BUFFER_SIZE + 1, 0);

	ifstream ifile(file_name, std::ifstream::binary);
	vector<string> chunks;
	while(1)
	{
		ifile.read(buffer.data(), BUFFER_SIZE);
		streamsize s = ((ifile) ? BUFFER_SIZE : ifile.gcount());
		buffer[s] = 0;
		chunks.push_back(buffer.data());
		if(!ifile) break;
	}
	ifile.close();
	return chunks;
}

void WriteInFile(string filename, string msg){
	ofstream outfile;
	outfile.open(filename, ios::app);
	outfile << msg;
	outfile.close();

	// ostream out(filename, ofstream::binary);
	// out.write(msg.c_str(),msg.size());
	// out.close();
}