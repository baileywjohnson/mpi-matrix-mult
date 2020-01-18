# mpi-matrix-mult
This project implements matrix multiplication, serial and parallel, using C and MPI.

The MyMPI.c/MyMPI.h and timer files were written by Michael J. Quinn, author of *Parallel Programming in C with MPI and OpenMP*.  The MyMPI files are a library of matrix/vector input/output/redistribution functions.

After verifying the MPI kernel extension is loaded, running the Makefile yields several executables:
- make-matrix
- print-matrix
- mm-serial
- mm-parallel

Generate matrix files with "make-matrix" and use "print-matrix" to display the contents of a given data file.  "mm-serial" and "mm-parallel" both take two matrix data files as input and compute the multiplication of the matrices, directing output to the parameter specified location.
