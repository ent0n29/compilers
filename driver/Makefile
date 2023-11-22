.PHONY: clean all

all: kcomp

kcomp:   driver.o parser.o scanner.o kcomp.o
	g++ -o kcomp driver.o parser.o scanner.o kcomp.o

kcomp.o:	kcomp.cpp
	g++ -c -std=c++17 kcomp.cpp
	
parser.o: parser.cpp
	g++ -c -std=c++17 parser.cpp
	
scanner.o: scanner.cpp parser.hpp
	g++ -c -std=c++17 scanner.cpp
	
driver.o: driver.cpp parser.hpp driver.hpp
	g++ -c -std=c++17 driver.cpp

parser.cpp parser.hpp: 	parser.yy 
	bison -o parser.cpp parser.yy

scanner.cpp: scanner.ll
	flex -o scanner.cpp scanner.ll

clean:
	rm -f *~ *.o kcomp scanner.cpp parser.cpp parser.hpp
