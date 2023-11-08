.PHONY: clean all

all: pparser

pparser: pparser.o lexer.o
	clang++ -o pparser pparser.o lexer.o

pparser.o: pparser.hpp pparser.cpp tokens.h
	clang++ -c pparser.cpp

lexer.o: lexer.cpp tokens.h
	clang++ -c lexer.cpp

lexer.cpp: lexer.l
	flex++ -o lexer.cpp lexer.l

pparser.hpp: ${GRAMMAR}.ll1
	python3 first_and_follow.py -o ${GRAMMAR} ${GRAMMAR}.ll1
	python3 makeparser.py ${GRAMMAR}.pickle

clean:
	rm -f *~ *.o pparser lexer.cpp tokens.h pparser.hpp *.pickle
