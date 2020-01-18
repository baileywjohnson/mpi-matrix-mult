#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <mpi.h>

#include "MyMPI.h"
#include "timer.h"
#include "functions.h"

int main(int argc, char *argv[]){

	MPI_Init(NULL, NULL);

	double io_start_1 = 0;
	GET_TIME(io_start_1);

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

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

	int m_A, n_A;
	double **subs_A;
	double *storage_A;
	read_row_striped_matrix(a_in, (void***)&subs_A, (void**)&storage_A, MPI_DOUBLE, &m_A, &n_A, MPI_COMM_WORLD);

	int m_B, n_B;
	double **subs_B;
	double *storage_B;
	read_row_striped_matrix(b_in, (void***)&subs_B, (void**)&storage_B, MPI_DOUBLE, &m_B, &n_B, MPI_COMM_WORLD);

	if(n_A != m_B){
		if(rank == 0){
			printf("Matrices Incompatible for Multiplication\n");
		}
		MPI_Finalize();
		exit(1);
	}

	int m_C, n_C;
	double *C;
	m_C = m_A;
	n_C = n_B;
	C = calloc((n_C * (m_C/size)), sizeof(double));

	double io_finish_1 = 0;
	GET_TIME(io_finish_1);

	double flops = 0;

	int i, j, k;
	for(i=0; i<size; i++){

		for(j=0; j<((m_A/size)*(m_A/size)); j++){

			int a_index = 0;
			//not first row in block
			if(j >= m_A/size){
				//one processor
				if(size == 1){
					a_index = j;
				}
				//more than one processor
				else{
					a_index = j + ((j/(m_A/size)) * (m_A - (m_A/size))) + (rank * (m_A/size));
				}

				//adjust for phase, handle wraparound if necessary
				if(((a_index - (i*(m_A/size)))/m_A) != (a_index/m_A)){
					a_index -= (i*(m_A/size));
					a_index += m_A;
				}
				else{
					a_index -= (i*(m_A/size));
				}
			}
			//first row
			else{
				a_index = j+(rank*(m_A/size));

				//adjust for phase, handle wraparound if necessary
				if(a_index - (i*(m_A/size)) < 0){
					a_index -= (i*(m_A/size));
					a_index += m_A;
				}
				else{
					a_index -= (i*(m_A/size));
				}
			}

			for(k=0; k<n_B; k++){
				int b_index = 0;
				if(a_index % m_A < m_A/size){
					if(a_index >= m_A){
						b_index = k+(m_A * (a_index % m_A));
					}
					else{
						b_index = k+(m_A * a_index);
					}
				}
				else{
					if(a_index >= m_A){
						b_index = k+(m_A * ((a_index % (m_A/size)) % m_A));
					}
					else{
						b_index = k+(m_A * (a_index % (m_A/size)));
					}
				}
				double result = storage_A[a_index] * storage_B[b_index];
				C[(k+(m_A*(a_index/m_A)))] += result;
				flops += 1;
			}
		}
		MPI_Sendrecv(storage_B, n_B*(n_B/size), MPI_DOUBLE, ((rank + 1) % size), 1, storage_B, n_B*(n_B/size), MPI_DOUBLE,
		 (((rank - 1) + size) % size), 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	double io_start_2 = 0;
	GET_TIME(io_start_2);

	double *C_final = malloc(sizeof(double) * (m_C * n_C));

	MPI_Gather(C, n_C * (m_C/size), MPI_DOUBLE, C_final, n_C * (m_C/size), MPI_DOUBLE, 0, MPI_COMM_WORLD);

	if(rank == 0){
		write_matrix(c_out, m_C, n_C, C_final);
	}

	free(storage_A);
	free(storage_B);
	free(C);
	free(C_final);

	double io_finish_2 = 0;
	GET_TIME(io_finish_2);

	if(rank == 0){
		double io_time = (io_finish_2 - io_start_2) + (io_finish_1 - io_start_1);
		double mm_time = (io_start_2 - io_finish_1);
		double mflop_count = (flops * size)/1000000;
		printf("M: %i, N: %i\n", m_A, n_A);
		printf("Time Doing I/O: %f sec.\n", io_time);
		printf("Time Doing MM: %f sec.\n", mm_time);
		printf("Total Time: %f sec.\n", io_time + mm_time);
		printf("MFlops (w/o IO): %e, MFlops (w/ IO): %e\n", mflop_count/mm_time, mflop_count/(io_time + mm_time));
	}

	MPI_Finalize();

	return 0;
}
