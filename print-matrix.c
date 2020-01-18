#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "functions.h"

int main(int argc, char *argv[]){

	int rows, columns;
	double *A;
	char *in;

	int c;
	if(argc != 3){
		printf("USAGE: print-matrix -i <your filename>\n");
		exit(1);
	}
	while((c = getopt(argc, argv, "i:")) != -1){
		switch(c){
			case 'i':
				in = optarg;	
				break;
			case '?':
				exit(1);
				break;
			default:
				break;
		}
	}

	read_matrix(in, &rows, &columns, &A);
	print_matrix(rows, columns, A);
	free(A);

	return 0;
}
