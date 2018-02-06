#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <pthread.h>

using namespace std::chrono;

struct Thread_data {
	double **A_t;
	double **B_t;
	double **C_t;
	int col_t;
	int width_t;
	int n_t;
};

void *threadFunc(void *threadArg) {
	struct Thread_data * t_data;
	t_data = (struct Thread_data*) threadArg;

	int col = t_data->col_t;
	int width = t_data->width_t;
	int n = t_data->n_t;
	double **A = t_data->A_t;
	double **B = t_data->B_t;
	double **C = t_data->C_t;

	for (int i=0; i<n; i++) {
		for (int j=col; j<col+width; j++) {
			for (int k=0; k<n; k++) {
				C[i][j] = C[i][j] + (A[i][k] * B[k][j]);
			}
		}
	}

	pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
		int i, j, k;
		struct timespec start, stop; 
		double time;
		int n = 1024; // matrix size is n*n


		// getting cmd line input for num threads

		if (argc < 2) {
			std::cout << "Please retry with numThreads as a command line parameter.\n";
			return 0;
		} 
		int numThreads = atoi(argv[1]);
		if (numThreads != 1 && numThreads != 4 && numThreads != 16 &&
			numThreads != 64 && numThreads != 256) {
			std::cout << "Please retry with a value of 1, 4, 16, 64, or 256.\n";
			return 0;
		}
		
		//
				
		double **A = (double**) malloc (sizeof(double*)*n);
		double **B = (double**) malloc (sizeof(double*)*n);
		double **C = (double**) malloc (sizeof(double*)*n);
		for (i=0; i<n; i++) {
			A[i] = (double*) malloc(sizeof(double)*n);
			B[i] = (double*) malloc(sizeof(double)*n);
			C[i] = (double*) malloc(sizeof(double)*n);
		}
		
		for (i=0; i<n; i++){
			for(j=0; j< n; j++){
				A[i][j]=i;
				B[i][j]=i+j;
				C[i][j]=0;			
			}
		}


		// initialize thread array

		pthread_t *threads = new pthread_t[numThreads];

		//
		// initialize thread_data_array

		Thread_data *thread_data_array = new Thread_data[numThreads];
		for (int t=0; t<numThreads; t++) {
			thread_data_array[t].A_t = A; // questionable
			thread_data_array[t].B_t = B; // questionable
			thread_data_array[t].C_t = C; // questionable

			thread_data_array[t].width_t = n/numThreads;
			thread_data_array[t].col_t = t*(n/numThreads);
			thread_data_array[t].n_t = n;
		}

		//


				
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		
		// Your code goes here //
		// Matrix C = Matrix A * Matrix B //	
		//*******************************//

		int rc;
		for (int t=0; t<numThreads; t++) {
			rc = pthread_create(&threads[t], NULL, threadFunc, (void*) &thread_data_array[t]); 
			if (rc) {
				std::cout << "creating error" <<"\n";
				exit(-1);
			}
		}
		for (int t=0; t<numThreads; t++) {
			pthread_join(threads[t], NULL);
		}
				
		//*******************************//

		high_resolution_clock::time_point t2 = high_resolution_clock::now();

	    auto duration = duration_cast<nanoseconds>( t2 - t1 ).count();
	    double timeSec = (double) duration/1000000000;
		
		std::cout << "Number of FLOPs = " << (double) 2*n*n*n << "; Execution time = " << duration << " nanoseconds; MFLOPs per sec = " << (double) 1/timeSec/1e6*2*n*n*n << "\n";
		std::cout << "C[100][100]=" << C[100][100] << "\n";

		// release memory
		for (i=0; i<n; i++) {
			free(A[i]);
			free(B[i]);
			free(C[i]);
		}
		free(A);
		free(B);
		free(C);
		delete[] threads;
		delete[] thread_data_array;
		pthread_exit(NULL);
		return 0;
}