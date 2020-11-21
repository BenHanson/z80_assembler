CXX = g++
CXXFLAGS = -O -std=c++17

LDFLAGS = -O

LIBS = 

all: z80_assembler

z80_assembler: data.o disassem.o main.o
	$(CXX) $(LDFLAGS) -o z80_assembler data.o disassem.o main.o $(LIBS)

data.o: data.cpp
	$(CXX) $(CXXFLAGS) -o data.o -c data.cpp

disassem.o: disassem.cpp
	$(CXX) $(CXXFLAGS) -o disassem.o -c disassem.cpp

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -o main.o -c main.cpp

library:

binary:

clean:
	- rm *.o
	- rm z80_assembler

