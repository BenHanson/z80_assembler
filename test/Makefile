CXX = g++
CXXFLAGS = -O -std=c++17

LDFLAGS = -O

LIBS = 

all: test

test: data.o disassem.o main.o parsers.o
	$(CXX) $(LDFLAGS) -o test data.o disassem.o main.o parsers.o $(LIBS)

data.o: ../data.cpp
	$(CXX) $(CXXFLAGS) -o data.o -c ../data.cpp

disassem.o: ../disassem.cpp
	$(CXX) $(CXXFLAGS) -o disassem.o -c ../disassem.cpp

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -o main.o -c main.cpp

parsers.o: ../parsers.cpp
	$(CXX) $(CXXFLAGS) -o parsers.o -c ../parsers.cpp

library:

binary:

clean:
	- rm *.o
	- rm test

