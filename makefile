all: main.out switch.out

main.out: main.o
	g++ main.o -o main.out

switch.out: switch.o
	g++ switch.o -o switch.out

main.o: main.cpp
	g++ -std=c++11 -c main.cpp -o main.o

switch.o: switch.cpp
	g++ -std=c++11 -c switch.cpp -o switch.o

.phony: clean
clean:
	rm*.o
	rm main.out
