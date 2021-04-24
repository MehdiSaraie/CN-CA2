#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <string>
#include <unistd.h>
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char* argv[])
{
	cout << "hello1" << endl;
    cout << argc << endl;
    for (int i=0; i<argc; i++)
        cout << argv[i] << endl;
	return 0;
}
