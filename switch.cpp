#include <sys/stat.h> 
#include <fcntl.h> 
#include <bits/stdc++.h> 
#include <sys/wait.h>


#include <iostream>
#include<vector>
#include<string>
#include<fstream>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <dirent.h>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <sstream>
#include <stdio.h> 
#include <string.h> 
#include <sys/types.h> 

#define LENGTH 1024

using namespace std;


int main(int argc, char* argv[]){

    const char* myfifo = argv[2];
    mkfifo(myfifo, 0666);
    while(1){
        char str1[LENGTH];
        int fd1 = open(myfifo ,O_RDONLY); 
        read(fd1, str1, LENGTH); 
        cout << str1 << endl;
        close(fd1);
    }
    return 0;
}

