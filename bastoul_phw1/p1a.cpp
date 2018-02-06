#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <chrono>

using namespace std::chrono;


int main(void){
		int i, j, k;
		struct timespec start, stop; 
		double time;
		int n = 1024; // matrix size is n*n
		
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
				
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		
		// Your code goes here //
		// Matrix C = Matrix A * Matrix B //	
		//*******************************//

		for (int i=0; i<n; i++) {
			for (int j=0; j<n; j++) {
				for (int k=0; k<n; k++) {
					C[i][j] = C[i][j] + (A[i][k] * B[k][j]);
				}
			}
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
		return 0;
}