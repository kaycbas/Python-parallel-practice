#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cublas.h>
#include <time.h>

#define n 1024
#define BLOCK_SIZE 8
#define GRID_SIZE 1024/BLOCK_SIZE

typedef struct {
	int width;
	int height;
	int* elements;
} Matrix;

__global__ void matrix_mult(Matrix a, Matrix b, Matrix c) {

	int id_x = blockIdx.x * blockDim.x + threadIdx.x;
	int id_y = blockIdx.y * blockDim.y + threadIdx.y;
	
	//if(row > a.height || col > b.width) return;

	int local_c = 0;
	for (int i=0; i<n; i++) {
		local_c += (a.elements[id_x*n+i]) * (b.elements[i*n+id_y]);
	}
	c.elements[id_x*n+id_y] = local_c;
}

int main () {
	int i;

	Matrix a;
	Matrix b;
	Matrix c;

	a.height = n;
	a.width = n;
	b.height = n;
	b.width = n;
	c.height = n;
	c.width = n;

	a.elements = (int*) malloc (a.width * a.height * sizeof(int));
	b.elements = (int*) malloc (b.width * b.height * sizeof(int));
	c.elements = (int*) malloc (c.width * c.height * sizeof(int));

	for (i=0; i<n*n; i++) {
		a.elements[i] = 1;
		b.elements[i] = 2;
		c.elements[i] = 0;
	}


	Matrix gpu_a;
	Matrix gpu_b;
	Matrix gpu_c;

	cudaError_t err = cudaMalloc(&gpu_a.elements, sizeof(int)*n*n);
	printf("CUDA malloc A: %s\n",cudaGetErrorString(err));
	err = cudaMalloc(&gpu_b.elements, sizeof(int)*n*n);
	printf("CUDA malloc B: %s\n",cudaGetErrorString(err));
	err = cudaMalloc(&gpu_c.elements, sizeof(int)*n*n);
	printf("CUDA malloc C: %s\n",cudaGetErrorString(err));


	err = cudaMemcpy(gpu_a.elements, a.elements, sizeof(int)*n*n, cudaMemcpyHostToDevice);
	printf("Copy A onto device: %s\n",cudaGetErrorString(err));
	err = cudaMemcpy(gpu_b.elements, b.elements, sizeof(int)*n*n, cudaMemcpyHostToDevice);
	printf("Copy B onto device: %s\n",cudaGetErrorString(err));
	err = cudaMemcpy(gpu_c.elements, c.elements, sizeof(int)*n*n, cudaMemcpyHostToDevice);
	printf("Copy C onto device: %s\n",cudaGetErrorString(err));


	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid(GRID_SIZE, GRID_SIZE);

	struct timespec start, stop; 
    double time;

    if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) { perror( "clock gettime" );}
	matrix_mult<<<dimGrid, dimBlock>>>(gpu_a, gpu_b, gpu_c);
	cudaThreadSynchronize();

	err = cudaMemcpy(c.elements, gpu_c.elements, sizeof(int)*n*n, cudaMemcpyDeviceToHost);
	printf("Copy C off of device: %s\n",cudaGetErrorString(err));

	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror( "clock gettime" );}	  
	time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
	printf("time is %f ns\n", time*1e9);

	//for (i=0; i<n*n; ++i) {
	std::cout << "c[451][451] = " << c.elements[451*n+451] << std::endl;
	//}

	free(a.elements);
	free(b.elements);
	free(c.elements);
	cudaFree(gpu_a.elements);
	cudaFree(gpu_b.elements);
	cudaFree(gpu_c.elements);

	return 0;
}

