all: main.out switch.out system.out
CXXFLAGS = -std=c++11

main.out: main.cpp
	g++ main.cpp -o main.out

switch.out: switch.cpp functions.cpp functions.h
	g++ switch.cpp functions.cpp -o switch.out

system.out: system.cpp functions.cpp functions.h
	g++ system.cpp functions.cpp -o system.out

.PHONY: clean
clean:
	rm *.out
	rm -r pipes

