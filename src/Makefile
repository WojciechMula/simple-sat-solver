CC=g++
CCFLAGS=-O2 -Wall -pedantic -std=c++11

VM=virtual_machine.h virtual_machine.cpp
SOLVER=solver.h solver.cpp

all: virtual_machine.o solver.o

solver.o: $(SOLVER) virtual_machine.h
	$(CC) $(CCFLAGS) solver.cpp -c

virtual_machine.o: $(VM)
	$(CC) $(CCFLAGS) virtual_machine.cpp -c
