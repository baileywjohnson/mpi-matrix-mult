#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "timer.h"
#include "functions.h"

int main(int argc, char *argv[]){

	double io_start_1 = 0;
	GET_TIME(io_start_1);

	double *A;
	int a_rows, a_columns;
	double *B;
	int b_rows, b_columns;
	double *C;
	int c_rows, c_columns;

	char *a_in;
	char *b_in;
	char *c_out;

	int c;
	if(argc != 7){
		printf("USAGE: mm-serial -A <matrix 1 filename> -B <matrix 2 filename> -C <output filename>\n");
		exit(1);
	}
	while((c = getopt(argc, argv, "A:B:C:")) != -1){
		switch(c){
			case 'A':
				a_in = optarg;
				break;
			case 'B':
				b_in = optarg;
				break;
			case 'C':
				c_out = optarg;
				break;
			case '?':
				exit(1);
				break;
			default:
				break;
		}
	}

	read_matrix(a_in, &a_rows, &a_columns, &A);
	read_matrix(b_in, &b_rows, &b_columns, &B);

	if(a_columns != b_rows){
		printf("Matrices provided not compatible for multiplication.\n");
		exit(1);
	}

	c_rows = a_rows;
	c_columns = b_columns;
	C = malloc(sizeof(double) * (c_rows * c_columns));

	double io_finish_1 = 0;
	GET_TIME(io_finish_1);

	double flops = 0;

	int i, j, count;
	for(i = 0; i < a_rows; i++){
		count = 0;
		while(count < b_columns){
			double result = 0;
			for(j = 0; j < b_rows; j++){
				result += A[(i * a_columns) + j] * B[(j * b_columns) + count];
				flops += 1;
			}
			C[(c_columns * i) + count] = result;
			count += 1;
		}
	}	
	
	double io_start_2 = 0;
	GET_TIME(io_start_2);

	write_matrix(c_out, c_rows, c_columns, C);	

	free(A);
	free(B);
	free(C);

	double io_finish_2 = 0;
	GET_TIME(io_finish_2);

	double io_time = (io_finish_2 - io_start_2) + (io_finish_1 - io_start_1);
	double mm_time = (io_start_2 - io_finish_1);
	double mflop_count = flops/1000000;
	printf("M: %i, N: %i\n", a_rows, b_columns);
	printf("Time Doing I/O: %f sec.\n", io_time);
	printf("Time Doing MM: %f sec.\n", mm_time);
	printf("Total Time: %f sec.\n", io_time + mm_time);
	printf("MFlops (without I/O): %e, MFlops (with I/O): %e\n", mflop_count/mm_time, mflop_count/(io_time + mm_time));

	return 0;
}
