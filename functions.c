#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

//make_matrix() - creates a matrix using integer array dynamically allocated inside this function, returns in A
void make_matrix(int m, int n, int l, int u, double **A){
	*A = malloc(sizeof(double) * (n * m));
	int i;
	srand(time(0));
	for(i = 0; i < (n * m); i++){
		double myRand = rand()/(1.0 + RAND_MAX);
		int range = u - l + 1;
		double myRand_scaled = (myRand * range) + l;
		(*A)[i] = myRand_scaled;
	}
}


//print_matrix() - prints to stdout the contents of matrix stored in array A
void print_matrix(int m, int n, double* A){
	int i, j;
	int index = 0;
	for(i=0; i<m; i++){
		for(j=0; j<n; j++){
			printf("%f\t", A[index]);
			index += 1;
		}
		printf("\n");
	}
}


//write_matrix() - opens file with name out, writes m to first location, n to second location, and then writes all integers from A (row-major)
void write_matrix(char* out, int m, int n, double* A){
	FILE *fp = fopen(out, "wb");
	//write m to first location in file
	fwrite(&m, sizeof(int), 1, fp);
	//write n to second location in file
	fwrite(&n, sizeof(int), 1, fp);
	//write n integers from A into it ALL IN ONE fwrite()
	fwrite(A, sizeof(double), (n * m), fp);
	fclose(fp);
}


//read_matrix() - opens a file, reads the contents out, returns rows in m, columns in n, and contents in A
void read_matrix(char* out, int *m, int *n, double** A){
	int rows, columns;
	FILE *fp = fopen(out, "rb");
	//fread() once to get the rows out of the file
	fread(&rows, sizeof(int), 1, fp);
	*m = rows;
	//fread() again to get the columns out of the file
	fread(&columns, sizeof(int), 1, fp);
	*n = columns;
	//malloc a buffer of the right size to hold the rest of the remaining data
	*A = malloc(sizeof(double) * (rows * columns));
	//fread() the remaining data into buffer
	fread(*A, sizeof(double), (rows * columns), fp);
	fclose(fp);
}
