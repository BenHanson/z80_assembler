CXX = g++
CXXFLAGS = -O -std=c++20 -Wall -I ../lexertl17/include -I ../parsertl17/include

LDFLAGS = -O

LIBS = 

all: z80_assembler

z80_assembler: data.o disassem.o dump.o main.o parsers.o skool.o sna.o
	$(CXX) $(LDFLAGS) -o z80_assembler data.o disassem.o dump.o \
main.o parsers.o sna.o skool.o $(LIBS)

data.o: data.cpp
	$(CXX) $(CXXFLAGS) -o data.o -c data.cpp

disassem.o: disassem.cpp
	$(CXX) $(CXXFLAGS) -o disassem.o -c disassem.cpp

dump.o: dump.cpp
	$(CXX) $(CXXFLAGS) -o dump.o -c dump.cpp

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -o main.o -c main.cpp

parsers.o: parsers.cpp
	$(CXX) $(CXXFLAGS) -o parsers.o -c parsers.cpp

sna.o: sna.cpp
	$(CXX) $(CXXFLAGS) -o sna.o -c sna.cpp

skool.o: skool.cpp
	$(CXX) $(CXXFLAGS) -o skool.o -c skool.cpp

library:

binary:

clean:
	- rm *.o
	- rm z80_assembler

