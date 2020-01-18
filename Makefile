CFLAGS = -g -Wall -Wstrict-prototypes
PROGS = make-matrix print-matrix mm-serial mm-parallel
LDFLAGS = -lm
CC = gcc
MCC = mpicc

all: $(PROGS)

make-matrix: make-matrix.o functions.o
	$(MCC) $(LDFLAGS) -o make-matrix make-matrix.o functions.o

print-matrix: print-matrix.o functions.o
	$(MCC) $(LDFLAGS) -o print-matrix print-matrix.o functions.o

mm-serial: mm-serial.o functions.o timer.h
	$(MCC) $(LDFLAGS) -o mm-serial mm-serial.o functions.o

mm-parallel: mm-parallel.o functions.o MyMPI.o timer.h
	$(MCC) $(LDFLAGS) -o mm-parallel mm-parallel.o functions.o MyMPI.o

make-matrix.o: make-matrix.c
	$(MCC) $(CFLAGS) -c make-matrix.c

print-matrix.o: print-matrix.c
	$(MCC) $(CFLAGS) -c print-matrix.c

mm-serial.o: mm-serial.c
	$(MCC) $(CFLAGS) -c mm-serial.c

mm-parallel.o: mm-parallel.c
	$(MCC) $(CFLAGS) -c mm-parallel.c

functions.o: functions.c functions.h
	$(MCC) $(CFLAGS) -c functions.c

MyMPI.o: MyMPI.c MyMPI.h
	$(MCC) $(CFLAGS) -c MyMPI.c

clean:
	rm -f $(PROGS) *.o core*
