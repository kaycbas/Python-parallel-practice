#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <chrono>

using namespace std::chrono;


int main(int argc, char *argv[]){
	int i, j, k;
	int w, x, y;
	int n = 1024; // matrix size is n*n
	int m;  // number or blocks m*m
	int b;   // dimensions of blocks b*b

	if (argc < 2) {
		std::cout << "Please enter a 'b' value as a command line parameter.\n";
		return 0;
	} 
	int b_val = atoi(argv[1]);
	if (b_val != 4 && b_val != 8 && b_val != 16) {
		std::cout << "'b' value must be 4, 8, or 16.\n";
		return 0;
	} else {
		b = b_val;
		m = n/b;	
	}
	
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


	//iterate through in a block by block fashion
	for (i=0; i<n; i+=b) {
		for (j=0; j<n; j+=b) {
			for (k=0; k<n; k+=b) {
			//now perform the multiply/add operations within these blocks
				for (w=0; w<b; w++) {
					for (x=0; x<b; x++) {
						for (y=0; y<b; y++) {	
							C[i+w][j+x] = C[i+w][j+x] + (A[i+w][k+y] * B[k+y][j+x]);				
						}
					}
				}
			//finish inner block operations

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

