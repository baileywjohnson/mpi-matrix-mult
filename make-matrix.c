#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "functions.h"

int main(int argc, char *argv[]){

	double *A;
	char *in;

	int rows, columns, lower, upper;
	
	if(argc != 11){
		printf("USAGE: make-matrix -m <number of rows> -n <number of columns> -l <lower bound> -u <upper bound> -o <output file>\n");
		exit(1);
	}

	int c;
	while((c = getopt(argc, argv, "m:n:l:u:o:")) != -1){
		switch(c){
			case 'm':
				rows = atoi(optarg);
				break;
			case 'n':
				columns = atoi(optarg);
				break;
			case 'l':
				lower = atoi(optarg);
				break;
			case 'u':
				upper = atoi(optarg);
				break;
			case 'o':
				in = optarg;
				break;
			case '?':
				exit(1);
				break;
			default:
				break;
		}
	}	

	make_matrix(rows, columns, lower, upper, &A);
	write_matrix(in, rows, columns, A);
	free(A);

	return 0;
}
