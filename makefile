all: main.out switch.out system.out

main.out: main.o
	g++ main.o -o main.out

switch.out: switch.o
	g++ switch.o -o switch.out

system.out: system.o
	g++ system.o -o system.out

main.o: main.cpp
	g++ -std=c++11 -c main.cpp -o main.o

switch.o: switch.cpp
	g++ -std=c++11 -c switch.cpp -o switch.o

system.o: system.cpp
	g++ -std=c++11 -c system.cpp -o system.o

.phony: clean
clean:
	rm*.o
	rm main.out
