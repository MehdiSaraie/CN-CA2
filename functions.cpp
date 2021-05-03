#include "functions.h"

void update_lookup(int lookup[][2], int& lookup_size, int system_number, int port_number){
	lookup[lookup_size][0] = system_number;
	lookup[lookup_size][1] = port_number;
	lookup_size++;
}

void add_connection(int connection[][3], int& connection_size, int port_number, int read_fd, int write_fd){
	connection[connection_size][0] = port_number;
	connection[connection_size][1] = read_fd;
	connection[connection_size][2] = write_fd;
	connection_size++;
}

void split_frame(char buffer[LENGTH], int& src_system, int& dest_system, int& label, char msg[LENGTH]){
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
	char label_str[LENGTH];
	strncpy(label_str, buffer+k+1, l-(k+1));
	label = atoi(label_str);

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


vector<string> read_file_chunk(string file_name, string status){

	const int BUFFER_SIZE = 512;
	vector<char> buffer (BUFFER_SIZE + 1, 0);

	ifstream ifile(file_name, std::ifstream::binary);
	vector<string> chunks;
	if (ifile.fail()){
		cout << "No such file.\n";
		if (status == "request"){
			chunks.push_back("No such file.\n");
		}
		return chunks;
	}
	
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

void WriteInFile(int system_number, int src_system, unordered_map<pair<int,int>, string, hash_pair> messages){
	string filename = "files/#"+to_string(system_number) + "<-" + to_string(src_system);
	ofstream outfile;
	outfile.open(filename, ios::app);
	for(auto p : messages){
		if((p.first).first == src_system){
			outfile << p.second;
		}
	}
	outfile.close();
}
