all: main.out

main.out: main.o
	g++ main.o -o main.out

main.o: main.cpp
	g++ -std=c++11 -c main.cpp -o main.o

.phony: clean
clean:
	rm*.o
	rm main.out
