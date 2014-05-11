
CC=g++
CCFLAGS=-O2 -Wall -pedantic -std=c++11
LEXER=lexer.h lexer.cpp
PARSER=parser.h parser.cpp
AST=ast.h ast.cpp

SSAT=../src/solver.o ../src/virtual_machine.o

all: src test demo

test: test.cpp ast.o lexer.o parser.o $(SSAT)
	$(CC) $(CCFLAGS) test.cpp ast.o lexer.o parser.o $(SSAT) -o test

demo: demo.cpp ast.o lexer.o parser.o $(SSAT)
	$(CC) $(CCFLAGS) demo.cpp ast.o lexer.o parser.o $(SSAT) -o demo

ast.o: $(AST)
	$(CC) $(CCFLAGS) $(AST) -c

lexer.o: $(LEXER)
	$(CC) $(CCFLAGS) $(LEXER) -c

parser.o: $(PARSER)
	$(CC) $(CCFLAGS) $(PARSER) -c

src:
	make -C ../src

../src/solver.o:
	make -C ../src solver.o

../src/virtual_machine.o:
	make -C ../src virtual_machine.o

clean:
	rm -f test demo *.o

#.PHONY: src
